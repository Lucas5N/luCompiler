%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "microc.tab.h"

Node* ast;
void yyerror(const char* s);
extern int yylex();
%}

%union {
    int number;
    char* identifier;
    Node* node;
    List* list;
}

%token INT
%token VOID
%token RETURN
%token IF
%token ELSE
%token WHILE
%token FOR
%token SCOLON
%token LBRACE
%token RBRACE
%token LPAR
%token RPAR
%token COMMA
%token ASG_OP
%token PLUS
%token MINUS
%token MULT
%token DIVIDE
%token EQ_OP
%token NOT_EQ_OP
%token LESS_THAN_OP
%token GREATER_THAN_OP
%token LESS_EQ_OP
%token GREATER_EQ_OP
%token AND_OP
%token OR_OP
%token NOT_OP

%token<number> NUMBER
%token<identifier> IDENTIFIER

%type<node> program
%type<node> function_declaration
%type<node> declaration_statement
%type<node> statement
%type<node> expression_statement
%type<node> expression
%type<node> assignment_expression
%type<node> relational_expression
%type<node> additive_expression
%type<node> multiplicative_expression
%type<node> primary_expression
%type<node> if_statement
%type<node> while_statement
%type<node> return_statement
%type<list> declarations
%type<list> statements

%% //regole grammaticali

program: function_declaration { ast = $1; };

function_declaration: INT IDENTIFIER LPAR RPAR LBRACE declarations statements RBRACE {
    $$ = create_function_node($2, $6, $7);
};

declarations: declarations declaration_statement { $$ = create_list_node($2, $1); }
            | /* empty */ { $$ = NULL; };

statements: statements statement { $$ = create_list_node($2, $1); }
          | /* empty */ { $$ = NULL; };

statement: expression_statement { $$ = $1; }
         | if_statement { $$ = $1; }
         | while_statement { $$ = $1; }
         | return_statement { $$ = $1; };

declaration_statement: INT IDENTIFIER SCOLON { $$ = create_declaration_node($2); };

return_statement: RETURN expression SCOLON { $$ = create_return_node($2); };

expression_statement: expression SCOLON { $$ = create_expr_stmt_node($1); };

expression: assignment_expression { $$ = $1; };

assignment_expression: IDENTIFIER ASG_OP expression { $$ = create_assign_node($1, $3); }
                     | relational_expression { $$ = $1; };

relational_expression: additive_expression EQ_OP additive_expression { $$ = create_binary_op_node(NODE_EQUAL_OP, $1, $3); }
                     | additive_expression NOT_EQ_OP additive_expression { $$ = create_binary_op_node(NODE_NOT_EQUAL_OP, $1, $3); }
                     | additive_expression LESS_THAN_OP additive_expression { $$ = create_binary_op_node(NODE_LESS_THAN_OP, $1, $3); }
                     | additive_expression GREATER_THAN_OP additive_expression { $$ = create_binary_op_node(NODE_GREATER_THAN_OP, $1, $3); }
                     | additive_expression { $$ = $1; };

additive_expression: additive_expression PLUS multiplicative_expression { $$ = create_binary_op_node(NODE_PLUS, $1, $3); }
                   | additive_expression MINUS multiplicative_expression { $$ = create_binary_op_node(NODE_MINUS, $1, $3); }
                   | multiplicative_expression { $$ = $1; };

multiplicative_expression: multiplicative_expression MULT primary_expression { $$ = create_binary_op_node(NODE_MULT, $1, $3); }
                         | multiplicative_expression DIVIDE primary_expression { $$ = create_binary_op_node(NODE_DIVIDE, $1, $3); }
                         | primary_expression { $$ = $1; };

primary_expression: NUMBER { $$ = create_number_node($1); }
                  | IDENTIFIER { $$ = create_identifier_node($1); }
                  | LPAR expression RPAR { $$ = $2; };

if_statement: IF LPAR expression RPAR LBRACE statements RBRACE { $$ = create_if_node($3, $6, NULL); }
            | IF LPAR expression RPAR LBRACE statements RBRACE ELSE LBRACE statements RBRACE { $$ = create_if_node($3, $6, $10); };

while_statement: WHILE LPAR expression RPAR LBRACE statements RBRACE { $$ = create_while_node($3, $6); };

%%

void yyerror(const char *s) {
    fprintf(stderr, "Errore di parsing: %s\n", s);
}
