#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "codegen.h"
//ao
// Tabella dei simboli per tenere traccia delle variabili locali.
// Questa è una lista concatenata semplice che associa il nome della variabile all'offset dello stack.
// Sta cosa serve al compilatore per evitare anche di ridefinire variabili, controllare la semantica del codice e viene gestita dal stm symbol table manager che lavora con l'error handler circa.
typedef struct Symbol {
    char* name;
    int offset;
    struct Symbol* next;
} Symbol;

static Symbol* symbol_table = NULL;
static int offset_counter = -4; 
static int label_count = 0;

// Aggiunge un nuovo simbolo alla tabella dei simboli.
void add_symbol(char* name, int offset) {
    Symbol* new_symbol = (Symbol*)malloc(sizeof(Symbol));
    if (!new_symbol) {
        perror("Errore di allocazione del simbolo");
        exit(EXIT_FAILURE);
    }
    new_symbol->name = strdup(name);
    new_symbol->offset = offset;
    new_symbol->next = symbol_table;
    symbol_table = new_symbol;
}

// Restituisce l'offset di una variabile dalla tabella dei simboli.
int get_symbol_offset(char* name) {
    Symbol* current = symbol_table;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current->offset;
        }
        current = current->next;
    }
    fprintf(stderr, "Errore: variabile '%s' non dichiarata.\n", name);
    return 0; 
}

// Libera la memoria allocata per la tabella dei simboli.
void free_symbol_table() {
    Symbol* current = symbol_table;
    while (current) {
        Symbol* temp = current;
        current = current->next;
        free(temp->name);
        free(temp);
    }
    symbol_table = NULL;
}

// Prototipi delle funzioni di generazione del codice.
static void generate_statement(Node* node, FILE* output_file);
static void generate_statements(List* list, FILE* output_file);
static void generate_expression(Node* node, FILE* output_file);
static char* generate_label();

// Funzione principale per la generazione del codice assembly.
void generate_assembly(Node* ast, const char* filename) {
    FILE* output_file = fopen(filename, "w");
    if (!output_file) {
        perror("Impossibile aprire il file di output");
        return;
    }
    
    // Genera il codice a partire dalla radice dell'AST.
    generate_statement(ast, output_file);
    free_symbol_table();
    fclose(output_file);
}

// Genera il codice per una singola espressione.
static void generate_expression(Node* node, FILE* output_file) {
    if (!node) return;

    switch (node->type) {
        case NODE_NUMBER:
            // Sposta il valore numerico nel registro EAX.
            fprintf(output_file, "  movl $%d, %%eax\n", node->number_val);
            break;
        case NODE_IDENTIFIER:
            // Sposta il valore della variabile dal suo offset nello stack a EAX.
            fprintf(output_file, "  movl %d(%%ebp), %%eax\n", get_symbol_offset(node->identifier_name));
            break;
        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_MULT:
        case NODE_DIVIDE:
            // Le espressioni binare vengono risolte valutando il lato sinistro,
            // mettendo il risultato sullo stack, valutando il lato destro,
            // e poi eseguendo l'operazione.
            generate_expression(node->binary_op.left, output_file);  // CORREZIONE: prima sinistra
            fprintf(output_file, "  pushl %%eax\n");
            generate_expression(node->binary_op.right, output_file); // poi destra
            fprintf(output_file, "  popl %%ebx\n");
            
            if (node->type == NODE_PLUS) {
                fprintf(output_file, "  addl %%ebx, %%eax\n");
            } else if (node->type == NODE_MINUS) {
                fprintf(output_file, "  subl %%eax, %%ebx\n"); 
                fprintf(output_file, "  movl %%ebx, %%eax\n");
            } else if (node->type == NODE_MULT) {
                fprintf(output_file, "  imull %%ebx, %%eax\n");
            } else if (node->type == NODE_DIVIDE) {
                fprintf(output_file, "  movl %%ebx, %%eax\n");  // CORREZIONE: metti il dividendo in EAX
                fprintf(output_file, "  cdq\n"); 
                fprintf(output_file, "  popl %%ebx\n");  // divisore in EBX
                fprintf(output_file, "  idivl %%ebx\n");
            }
            break;
        case NODE_ASSIGN_OP:
            // Valuta l'espressione a destra e assegna il risultato alla variabile.
            generate_expression(node->assign_op.expression, output_file);
            fprintf(output_file, "  movl %%eax, %d(%%ebp)\n", get_symbol_offset(node->assign_op.identifier));
            break;
        case NODE_EQUAL_OP:
        case NODE_NOT_EQUAL_OP:
        case NODE_LESS_THAN_OP:
        case NODE_GREATER_THAN_OP:
            // Genera codice per le operazioni di confronto.
            generate_expression(node->binary_op.left, output_file);   // CORREZIONE: prima sinistra
            fprintf(output_file, "  pushl %%eax\n");
            generate_expression(node->binary_op.right, output_file);  // poi destra
            fprintf(output_file, "  popl %%ebx\n");
            fprintf(output_file, "  cmpl %%eax, %%ebx\n");  // CORREZIONE: confronta ebx con eax
            
            if (node->type == NODE_EQUAL_OP) {
                fprintf(output_file, "  sete %%al\n");
            } else if (node->type == NODE_NOT_EQUAL_OP) {
                fprintf(output_file, "  setne %%al\n");
            } else if (node->type == NODE_LESS_THAN_OP) {
                fprintf(output_file, "  setl %%al\n");
            } else if (node->type == NODE_GREATER_THAN_OP) {
                fprintf(output_file, "  setg %%al\n");
            }
            // Mette il risultato (0 o 1) nel registro EAX.
            fprintf(output_file, "  movzbl %%al, %%eax\n");
            break;
    }
}

// Genera il codice per una singola istruzione.
static void generate_statement(Node* node, FILE* output_file) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            generate_statement(node->program_node.function, output_file);
            break;
        case NODE_FUNCTION:
            fprintf(output_file, ".globl main\n");
            fprintf(output_file, "main:\n");
            fprintf(output_file, "  pushl %%ebp\n");
            fprintf(output_file, "  movl %%esp, %%ebp\n");

            // Conta le variabili dichiarate per allocare spazio sullo stack
            List* decl_list = node->function_def.declarations;
            int var_space = 0;
            while(decl_list) {
                var_space += 4;
                decl_list = decl_list->next;
            }
            if (var_space > 0) {
                fprintf(output_file, "  subl $%d, %%esp\n", var_space);
            }

            // Aggiungi le dichiarazioni alla tabella dei simboli
            decl_list = node->function_def.declarations;
            offset_counter = -4; 
            while(decl_list) {
                add_symbol(decl_list->node->declaration_stmt.identifier, offset_counter);
                offset_counter -= 4; 
                decl_list = decl_list->next;
            }
            
            // CORREZIONE PRINCIPALE: Genera PRIMA le istruzioni, poi l'epilogo
            generate_statements(node->function_def.statements, output_file);

            // Epilogo della funzione (solo se non c'è già un return esplicito)
            fprintf(output_file, "  movl $0, %%eax\n");
            fprintf(output_file, "  movl %%ebp, %%esp\n");
            fprintf(output_file, "  popl %%ebp\n");
            fprintf(output_file, "  ret\n");
            break;
        case NODE_RETURN_STMT:
            generate_expression(node->return_stmt.expression, output_file);
            fprintf(output_file, "  movl %%ebp, %%esp\n");
            fprintf(output_file, "  popl %%ebp\n");
            fprintf(output_file, "  ret\n");
            break;
        case NODE_EXPR_STMT:
            generate_expression(node->expr_stmt.expression, output_file);
            break;
        case NODE_IF_STMT: {
            char* end_if_label = generate_label();
            generate_expression(node->if_stmt.condition, output_file);
            fprintf(output_file, "  cmpl $0, %%eax\n");
            fprintf(output_file, "  je %s\n", end_if_label);
            generate_statements(node->if_stmt.if_body, output_file);
            fprintf(output_file, "%s:\n", end_if_label);
            free(end_if_label);
            break;
        }
        case NODE_IF_ELSE_STMT: {
            char* else_label = generate_label();
            char* end_if_else_label = generate_label();
            generate_expression(node->if_stmt.condition, output_file);
            fprintf(output_file, "  cmpl $0, %%eax\n");
            fprintf(output_file, "  je %s\n", else_label);
            generate_statements(node->if_stmt.if_body, output_file);
            fprintf(output_file, "  jmp %s\n", end_if_else_label);
            fprintf(output_file, "%s:\n", else_label);
            generate_statements(node->if_stmt.else_body, output_file);
            fprintf(output_file, "%s:\n", end_if_else_label);
            free(else_label);
            free(end_if_else_label);
            break;
        }
        case NODE_WHILE_STMT: {
            char* start_while_label = generate_label();
            char* end_while_label = generate_label();
            fprintf(output_file, "%s:\n", start_while_label);
            generate_expression(node->while_stmt.condition, output_file);
            fprintf(output_file, "  cmpl $0, %%eax\n");
            fprintf(output_file, "  je %s\n", end_while_label);
            generate_statements(node->while_stmt.while_body, output_file);
            fprintf(output_file, "  jmp %s\n", start_while_label);
            fprintf(output_file, "%s:\n", end_while_label);
            free(start_while_label);
            free(end_while_label);
            break;
        }
    }
}

// Genera il codice per una lista di istruzioni.
static void generate_statements(List* list, FILE* output_file) {
    if (!list) return;
    List* current = list;
    while(current) {
        generate_statement(current->node, output_file);
        current = current->next;
    }
}

// Genera un'etichetta unica.
static char* generate_label() {
    char* label_name = (char*)malloc(16);
    if (!label_name) {
        perror("Errore di allocazione");
        exit(EXIT_FAILURE);
    }
    sprintf(label_name, "L%d", label_count++);
    return label_name;
}
