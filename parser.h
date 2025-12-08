/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TM_NAT = 258,
    TM_IDENT = 259,
    TM_LEFT_BRACE = 260,
    TM_RIGHT_BRACE = 261,
    TM_LEFT_PAREN = 262,
    TM_RIGHT_PAREN = 263,
    TM_TRUE = 264,
    TM_FALSE = 265,
    TM_COLON = 266,
    TM_COMMA = 267,
    TM_CONTINUE = 268,
    TM_BREAK = 269,
    TM_SKIP = 270,
    TM_SEMICOL = 271,
    TM_LOOP_INIT = 272,
    TM_LOOP_BODY = 273,
    TM_MALLOC = 274,
    TM_RI = 275,
    TM_RC = 276,
    TM_WI = 277,
    TM_WC = 278,
    TM_VAR = 279,
    TM_IF = 280,
    TM_THEN = 281,
    TM_ELSE = 282,
    TM_WHILE = 283,
    TM_DO = 284,
    TM_ASGNOP = 285,
    TM_OR = 286,
    TM_AND = 287,
    TM_NOT = 288,
    TM_LT = 289,
    TM_LE = 290,
    TM_GT = 291,
    TM_GE = 292,
    TM_EQ = 293,
    TM_NE = 294,
    TM_PLUS = 295,
    TM_MINUS = 296,
    TM_MUL = 297,
    TM_DIV = 298,
    TM_MOD = 299
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 11 "lang.y"

unsigned int n;
char * i;
struct expr_list * e_l;
struct expr * e;
struct cmd * c;
void * none;

#line 111 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
