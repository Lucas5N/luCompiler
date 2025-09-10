#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "codegen.h"

// Dichiarazione delle funzioni esterne del parser
extern int yyparse();
extern FILE *yyin;
extern Node* ast; // Questa è la variabile definita nel parser

// Definizione della variabile globale per la radice dell'AST
Node* ast_root = NULL;

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <file_di_input.mc>\n", argv[0]);
        return 1;
    }

    // Apri il file di input
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Errore nell'apertura del file di input");
        return 1;
    }

    // Analisi sintattica e costruzione dell'AST
    printf("Parsing in corso...\n");
    int result = yyparse();
    
    // Chiudi il file di input
    fclose(yyin);

    // Se l'analisi sintattica ha avuto successo, genera l'output
    if (result == 0 && ast) {
        ast_root = ast; // Copia il riferimento
        printf("AST generato con successo. Stampa dell'AST:\n");
        print_ast(ast_root, 0);
        
        printf("\nGenerazione del codice assembly...\n");
        generate_assembly(ast_root, "output.s");
        printf("Codice assembly salvato in 'output.s'.\n");
        
        free_ast(ast_root);
    } else {
        fprintf(stderr, "Errore di parsing. Impossibile generare l'AST.\n");
        return 1;
    }

    return 0;
}
