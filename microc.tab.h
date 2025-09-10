/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_MICROC_TAB_H_INCLUDED
# define YY_YY_MICROC_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT = 258,                     /* INT  */
    VOID = 259,                    /* VOID  */
    RETURN = 260,                  /* RETURN  */
    IF = 261,                      /* IF  */
    ELSE = 262,                    /* ELSE  */
    WHILE = 263,                   /* WHILE  */
    FOR = 264,                     /* FOR  */
    SCOLON = 265,                  /* SCOLON  */
    LBRACE = 266,                  /* LBRACE  */
    RBRACE = 267,                  /* RBRACE  */
    LPAR = 268,                    /* LPAR  */
    RPAR = 269,                    /* RPAR  */
    COMMA = 270,                   /* COMMA  */
    ASG_OP = 271,                  /* ASG_OP  */
    PLUS = 272,                    /* PLUS  */
    MINUS = 273,                   /* MINUS  */
    MULT = 274,                    /* MULT  */
    DIVIDE = 275,                  /* DIVIDE  */
    EQ_OP = 276,                   /* EQ_OP  */
    NOT_EQ_OP = 277,               /* NOT_EQ_OP  */
    LESS_THAN_OP = 278,            /* LESS_THAN_OP  */
    GREATER_THAN_OP = 279,         /* GREATER_THAN_OP  */
    LESS_EQ_OP = 280,              /* LESS_EQ_OP  */
    GREATER_EQ_OP = 281,           /* GREATER_EQ_OP  */
    AND_OP = 282,                  /* AND_OP  */
    OR_OP = 283,                   /* OR_OP  */
    NOT_OP = 284,                  /* NOT_OP  */
    NUMBER = 285,                  /* NUMBER  */
    IDENTIFIER = 286               /* IDENTIFIER  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 12 "microc.y"

    int number;
    char* identifier;
    Node* node;
    List* list;

#line 102 "microc.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_MICROC_TAB_H_INCLUDED  */
