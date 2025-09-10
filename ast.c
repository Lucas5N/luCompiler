#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void print_list(List *list, int indent);
List* reverse_list(List* list);

Node* new_node(NodeType type, ...) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        perror("Errore di allocazione della memoria");
        exit(1);
    }
    node->type = type;

    va_list args;
    va_start(args, type);

    switch (type) {
        case NODE_PROGRAM:
            node->program_node.function = va_arg(args, Node*);
            break;
        case NODE_FUNCTION:
            node->function_def.name = va_arg(args, char*);
            node->function_def.declarations = va_arg(args, List*);
            node->function_def.statements = va_arg(args, List*);
            break;
        case NODE_DECLARATION:
            node->declaration_stmt.identifier = va_arg(args, char*);
            break;
        case NODE_NUMBER:
            node->number_val = va_arg(args, int);
            break;
        case NODE_IDENTIFIER:
            node->identifier_name = va_arg(args, char*);
            break;
        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_MULT:
        case NODE_DIVIDE:
        case NODE_EQUAL_OP:
        case NODE_NOT_EQUAL_OP:
        case NODE_LESS_THAN_OP:
        case NODE_GREATER_THAN_OP:
            node->binary_op.left = va_arg(args, Node*);
            node->binary_op.right = va_arg(args, Node*);
            break;
        case NODE_ASSIGN_OP:
            node->assign_op.identifier = va_arg(args, char*);
            node->assign_op.expression = va_arg(args, Node*);
            break;
        case NODE_IF_STMT:
        case NODE_IF_ELSE_STMT:
            node->if_stmt.condition = va_arg(args, Node*);
            node->if_stmt.if_body = va_arg(args, List*);
            node->if_stmt.else_body = va_arg(args, List*);
            break;
        case NODE_WHILE_STMT:
            node->while_stmt.condition = va_arg(args, Node*);
            node->while_stmt.while_body = va_arg(args, List*);
            break;
        case NODE_RETURN_STMT:
            node->return_stmt.expression = va_arg(args, Node*);
            break;
        case NODE_EXPR_STMT:
            node->expr_stmt.expression = va_arg(args, Node*);
            break;
    }
    va_end(args);
    return node;
}

List* reverse_list(List* list) {
    List* prev = NULL;
    List* current = list;
    List* next = NULL;
    
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    
    return prev;
}

List* new_list(Node *node, List *next) {
    List* list = (List*)malloc(sizeof(List));
    if (!list) {
        perror("Errore di allocazione della memoria");
        exit(1);
    }
    list->node = node;
    list->next = next;
    return list;
}


Node* create_function_node(char* name, List* declarations, List* statements) {
    // Inverti le liste per ottenere l'ordine corretto
    List* reversed_declarations = reverse_list(declarations);
    List* reversed_statements = reverse_list(statements);
    return new_node(NODE_FUNCTION, name, reversed_declarations, reversed_statements);
}

Node* create_declaration_node(char* identifier) {
    return new_node(NODE_DECLARATION, identifier);
}

Node* create_return_node(Node* expression) {
    return new_node(NODE_RETURN_STMT, expression);
}

Node* create_expr_stmt_node(Node* expression) {
    return new_node(NODE_EXPR_STMT, expression);
}

Node* create_assign_node(char* identifier, Node* expression) {
    return new_node(NODE_ASSIGN_OP, identifier, expression);
}

Node* create_binary_op_node(NodeType type, Node* left, Node* right) {
    return new_node(type, left, right);
}

Node* create_number_node(int value) {
    return new_node(NODE_NUMBER, value);
}

Node* create_identifier_node(char* name) {
    return new_node(NODE_IDENTIFIER, name);
}

Node* create_if_node(Node* condition, List* if_body, List* else_body) {
    List* reversed_if_body = reverse_list(if_body);
    List* reversed_else_body = else_body ? reverse_list(else_body) : NULL;
    
    if (else_body) {
        return new_node(NODE_IF_ELSE_STMT, condition, reversed_if_body, reversed_else_body);
    } else {
        return new_node(NODE_IF_STMT, condition, reversed_if_body, reversed_else_body);
    }
}

Node* create_while_node(Node* condition, List* while_body) {
    List* reversed_while_body = reverse_list(while_body);
    return new_node(NODE_WHILE_STMT, condition, reversed_while_body);
}

List* create_list_node(Node* node, List* next) {
    return new_list(node, next);
}

void print_ast(Node *node, int indent) {
    if (!node) return;
    
    for (int i = 0; i < indent; i++) printf("  ");

    switch (node->type) {
        case NODE_PROGRAM:
            printf("Program\n");
            print_ast(node->program_node.function, indent + 1);
            break;
        case NODE_FUNCTION:
            printf("Function: %s\n", node->function_def.name);
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Declarations:\n");
            print_list(node->function_def.declarations, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Statements:\n");
            print_list(node->function_def.statements, indent + 2);
            break;
        case NODE_DECLARATION:
            printf("Declaration: %s\n", node->declaration_stmt.identifier);
            break;
        case NODE_NUMBER:
            printf("Number: %d\n", node->number_val);
            break;
        case NODE_IDENTIFIER:
            printf("Identifier: %s\n", node->identifier_name);
            break;
        case NODE_PLUS:
            printf("+\n");
            print_ast(node->binary_op.left, indent + 1);
            print_ast(node->binary_op.right, indent + 1);
            break;
        case NODE_MINUS:
            printf("-\n");
            print_ast(node->binary_op.left, indent + 1);
            print_ast(node->binary_op.right, indent + 1);
            break;
        case NODE_MULT:
            printf("*\n");
            print_ast(node->binary_op.left, indent + 1);
            print_ast(node->binary_op.right, indent + 1);
            break;
        case NODE_DIVIDE:
            printf("/\n");
            print_ast(node->binary_op.left, indent + 1);
            print_ast(node->binary_op.right, indent + 1);
            break;
        case NODE_ASSIGN_OP:
            printf("=\n");
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Identifier: %s\n", node->assign_op.identifier);
            print_ast(node->assign_op.expression, indent + 1);
            break;
        case NODE_EQUAL_OP:
            printf("==\n");
            print_ast(node->binary_op.left, indent + 1);
            print_ast(node->binary_op.right, indent + 1);
            break;
        case NODE_NOT_EQUAL_OP:
            printf("!=\n");
            print_ast(node->binary_op.left, indent + 1);
            print_ast(node->binary_op.right, indent + 1);
            break;
        case NODE_LESS_THAN_OP:
            printf("<\n");
            print_ast(node->binary_op.left, indent + 1);
            print_ast(node->binary_op.right, indent + 1);
            break;
        case NODE_GREATER_THAN_OP:
            printf(">\n");
            print_ast(node->binary_op.left, indent + 1);
            print_ast(node->binary_op.right, indent + 1);
            break;
        case NODE_IF_STMT:
            printf("If Statement\n");
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Condition:\n");
            print_ast(node->if_stmt.condition, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("If Body:\n");
            print_list(node->if_stmt.if_body, indent + 2);
            if (node->if_stmt.else_body) {
                for (int i = 0; i < indent + 1; i++) printf("  ");
                printf("Else Body:\n");
                print_list(node->if_stmt.else_body, indent + 2);
            }
            break;
        case NODE_IF_ELSE_STMT:
            printf("If-Else Statement\n");
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Condition:\n");
            print_ast(node->if_stmt.condition, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("If Body:\n");
            print_list(node->if_stmt.if_body, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Else Body:\n");
            print_list(node->if_stmt.else_body, indent + 2);
            break;
        case NODE_WHILE_STMT:
            printf("While Statement\n");
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Condition:\n");
            print_ast(node->while_stmt.condition, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("While Body:\n");
            print_list(node->while_stmt.while_body, indent + 2);
            break;
        case NODE_RETURN_STMT:
            printf("Return Statement\n");
            print_ast(node->return_stmt.expression, indent + 1);
            break;
        case NODE_EXPR_STMT:
            printf("Expression Statement\n");
            print_ast(node->expr_stmt.expression, indent + 1);
            break;
    }
}

void print_list(List *list, int indent) {
    List *current = list;
    while (current != NULL) {
        print_ast(current->node, indent);
        current = current->next;
    }
}

void free_ast(Node* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            free_ast(node->program_node.function);
            break;
        case NODE_FUNCTION:
            free(node->function_def.name);
            free_list(node->function_def.declarations);
            free_list(node->function_def.statements);
            break;
        case NODE_DECLARATION:
            free(node->declaration_stmt.identifier);
            break;
        case NODE_IDENTIFIER:
            free(node->identifier_name);
            break;
        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_MULT:
        case NODE_DIVIDE:
        case NODE_EQUAL_OP:
        case NODE_NOT_EQUAL_OP:
        case NODE_LESS_THAN_OP:
        case NODE_GREATER_THAN_OP:
            free_ast(node->binary_op.left);
            free_ast(node->binary_op.right);
            break;
        case NODE_ASSIGN_OP:
            free(node->assign_op.identifier);
            free_ast(node->assign_op.expression);
            break;
        case NODE_IF_STMT:
        case NODE_IF_ELSE_STMT:
            free_ast(node->if_stmt.condition);
            free_list(node->if_stmt.if_body);
            free_list(node->if_stmt.else_body);
            break;
        case NODE_WHILE_STMT:
            free_ast(node->while_stmt.condition);
            free_list(node->while_stmt.while_body);
            break;
        case NODE_RETURN_STMT:
            free_ast(node->return_stmt.expression);
            break;
        case NODE_EXPR_STMT:
            free_ast(node->expr_stmt.expression);
            break;
        default:
            break;
    }
    free(node);
}

void free_list(List* list) {
    List* current = list;
    while (current != NULL) {
        List* next = current->next;
        free_ast(current->node);
        free(current);
        current = next;
    }
}
