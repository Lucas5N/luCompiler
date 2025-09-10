#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
typedef struct List List;

// Dichiarazione della variabile globale ast_root
extern Node* ast_root;

// Definizione dei tipi di nodo
typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_DECLARATION,
    NODE_NUMBER,
    NODE_IDENTIFIER,
    NODE_PLUS,
    NODE_MINUS,
    NODE_MULT,
    NODE_DIVIDE,
    NODE_ASSIGN_OP,
    NODE_EQUAL_OP,
    NODE_NOT_EQUAL_OP,
    NODE_LESS_THAN_OP,
    NODE_GREATER_THAN_OP,
    NODE_IF_STMT,
    NODE_IF_ELSE_STMT,
    NODE_WHILE_STMT,
    NODE_RETURN_STMT,
    NODE_EXPR_STMT
} NodeType;

// Struttura di un nodo dell'albero sintattico
struct Node {
    NodeType type;
    union {
        // NODES BINARI
        struct {
            Node* left;
            Node* right;
        } binary_op;
        
        // NODE DI ASSEGNAZIONE
        struct {
            char* identifier;
            Node* expression;
        } assign_op;
        
        // NODES DI ISTITUZIONE
        struct {
            Node* expression;
        } expr_stmt;
        
        struct {
            Node* expression;
        } return_stmt;
        
        // NODES DI DICHIARAZIONE
        struct {
            char* identifier;
        } declaration_stmt;
        
        // NODE IF / IF-ELSE
        struct {
            Node* condition;
            List* if_body;
            List* else_body;
        } if_stmt;
        
        // NODE WHILE
        struct {
            Node* condition;
            List* while_body;
        } while_stmt;
        
        // NODES FOGLIA
        int number_val;
        char* identifier_name;
        
        // NODES DI PROGRAMMA E FUNZIONE
        struct {
            Node* function;
        } program_node;
        
        struct {
            char* name;
            List* declarations;
            List* statements;
        } function_def;
    };
};

// Struttura di una lista per le istruzioni e le dichiarazioni
struct List {
    Node* node;
    List* next;
};

// Funzioni per la creazione dei nodi 
Node* new_node(NodeType type, ...);
List* new_list(Node* node, List* next);

// Funzioni helper per la creazione dei nodi specifici
Node* create_function_node(char* name, List* declarations, List* statements);
Node* create_declaration_node(char* identifier);
Node* create_return_node(Node* expression);
Node* create_expr_stmt_node(Node* expression);
Node* create_assign_node(char* identifier, Node* expression);
Node* create_binary_op_node(NodeType type, Node* left, Node* right);
Node* create_number_node(int value);
Node* create_identifier_node(char* name);
Node* create_if_node(Node* condition, List* if_body, List* else_body);
Node* create_while_node(Node* condition, List* while_body);
List* create_list_node(Node* node, List* next);

// Funzioni per la stampa
void print_ast(Node* node, int indent);
void print_list(List* list, int indent);

// Funzione per la pulizia della memoria
void free_ast(Node* node);
void free_list(List* list);

#endif
