#ifndef LANG_H_INCLUDED
#define LANG_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

enum BinOpType {
  T_PLUS,
  T_MINUS,
  T_MUL,
  T_DIV,
  T_MOD,
  T_LT,
  T_GT,
  T_LE,
  T_GE,
  T_EQ,
  T_NE,
  T_AND,
  T_OR
};

enum UnOpType {
  T_UMINUS,
  T_NOT
};

enum ExprType {
  T_CONST = 0,
  T_VAR,
  T_BINOP,
  T_UNOP,
  T_FUN
  // ,
  // T_DEREF,
  // T_MALLOC,
  // T_RI,
  // T_RC
};

enum CmdType {
  T_DECL = 0,
  T_ASGN,
  T_SEQ,
  T_IF,
  T_CONTINUE,
  T_BREAK,
  T_SKIP,
  T_LOOP,
  T_EXPR
  // ,
  // T_WHILE,
  // T_WI,
  // T_WC
};

struct expr_list;
struct expr;

struct expr {
  enum ExprType t;
  union {
    struct {unsigned int value; } CONST;
    struct {char * name; } VAR;
    struct {enum BinOpType op; struct expr * left; struct expr * right; } BINOP;
    struct {enum UnOpType op; struct expr * arg; } UNOP;
    struct {char* name; struct expr_list * arg;} FUN;
  } d;
};

struct expr_list {
  struct expr * exp;
  struct expr_list * nxt;
};

struct cmd {
  enum CmdType t;
  union {
    struct {char * name; } DECL;
    struct {struct expr * left; struct expr * right; } ASGN;
    struct {struct cmd * left; struct cmd * right; } SEQ;
    struct {struct expr * cond; struct cmd * left; struct cmd * right; } IF;
    struct {} CONTINUE;
    struct {} BREAK;
    struct {} SKIP;
    struct {struct cmd * init; struct cmd * body;} LOOP;
    struct {struct expr * exp;} EXPR;
    // struct {struct expr * cond; struct cmd * body; } WHILE;
    // struct {struct expr * arg; } WI;
    // struct {struct expr * arg; } WC;
  } d;
};

struct expr * TConst(unsigned int value);
struct expr * TVar(char * name);
struct expr * TBinOp(enum BinOpType op, struct expr * left, struct expr * right);
struct expr * TUnOp(enum UnOpType op, struct expr * arg);
struct expr * TFun(char * name, struct expr_list * arg);
// struct expr * TDeref(struct expr * arg);
// struct expr * TMalloc(struct expr * arg);
// struct expr * TReadInt();
// struct expr * TReadChar();
struct cmd * TDecl(char * name);
struct cmd * TAsgn(struct expr * left, struct expr * right);
struct cmd * TSeq(struct cmd * left, struct cmd * right);
struct cmd * TIf(struct expr * cond, struct cmd * left, struct cmd * right);
struct cmd * TContinue();
struct cmd * TBreak();
struct cmd * TSkip();
struct cmd * TLoop(struct cmd * init, struct cmd * body);
struct cmd * TExpr(struct expr * exp);
// struct cmd * TWhile(struct expr * cond, struct cmd * body);
// struct cmd * TWriteInt(struct expr * arg);
// struct cmd * TWriteChar(struct expr * arg);

struct expr_list * TExprList(struct expr * expr, struct expr_list * nxt);

void print_binop(enum BinOpType op);
void print_unop(enum UnOpType op);
void print_expr(struct expr * e);
void print_cmd(struct cmd * c);
void print_expr_list(struct expr_list * lst);

unsigned int build_nat(char * c, int len);
char * new_str(char * str, int len);

#endif // LANG_H_INCLUDED
