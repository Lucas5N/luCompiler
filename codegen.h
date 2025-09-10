#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h" // Per accedere alla struttura dei nodi dell'AST

// Struttura per un singolo elemento della tabella dei simboli
typedef struct Symbol {
    char* name;
    int offset;
    struct Symbol* next;
} Symbol;

// Funzioni per la tabella dei simboli
void add_symbol(char* name, int offset);
int get_symbol_offset(char* name);
void free_symbol_table();

// Prototipo della funzione principale di generazione del codice
void generate_code(Node* ast_root);
void generate_assembly(Node* ast, const char* filename);
#endif // CODEGEN_H
