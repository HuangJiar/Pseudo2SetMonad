#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lang.h"

struct expr * new_expr_ptr() {
  struct expr * res = (struct expr *) malloc(sizeof(struct expr));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct cmd * new_cmd_ptr() {
  struct cmd * res = (struct cmd *) malloc(sizeof(struct cmd));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct expr_list * new_expr_list_ptr() {
  struct expr_list* res = (struct expr_list *) malloc(sizeof(struct expr_list));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct expr * TConst(unsigned int value) {
  struct expr * res = new_expr_ptr();
  res -> t = T_CONST;
  res -> d.CONST.value = value;
  return res;
}

struct expr * TVar(char * name) {
  struct expr * res = new_expr_ptr();
  res -> t = T_VAR;
  res -> d.VAR.name = name;
  return res;
}

struct expr * TBinOp(enum BinOpType op, struct expr * left, struct expr * right) {
  struct expr * res = new_expr_ptr();
  res -> t = T_BINOP;
  res -> d.BINOP.op = op;
  res -> d.BINOP.left = left;
  res -> d.BINOP.right = right;
  return res;
}

struct expr * TUnOp(enum UnOpType op, struct expr * arg) {
  struct expr * res = new_expr_ptr();
  res -> t = T_UNOP;
  res -> d.UNOP.op = op;
  res -> d.UNOP.arg = arg;
  return res;
}

struct expr * TFun(char* name, struct expr_list * arg) {
  struct expr * res = new_expr_ptr();
  res -> t = T_FUN;
  res -> d.FUN.name = name;
  res -> d.FUN.arg = arg;
  return res;
}

struct cmd * TDecl(char * name) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_DECL;
  res -> d.DECL.name = name;
  return res;
}

struct cmd * TAsgn(struct expr * left, struct expr * right) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_ASGN;
  res -> d.ASGN.left = left;
  res -> d.ASGN.right = right;
  return res;
}

struct cmd * TSeq(struct cmd * left, struct cmd * right) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_SEQ;
  res -> d.SEQ.left = left;
  res -> d.SEQ.right = right;
  return res;
}

struct cmd * TIf(struct expr * cond, struct cmd * left, struct cmd * right) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_IF;
  res -> d.IF.cond = cond;
  res -> d.IF.left = left;
  res -> d.IF.right = right;
  return res;
}

struct cmd * TContinue() {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_CONTINUE;
  return res;
}

struct cmd * TBreak() {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_BREAK;
  return res;
}

struct cmd * TSkip() {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_SKIP;
  return res;
}

struct cmd * TLoop(struct cmd * init, struct cmd * body) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_LOOP;
  res -> d.LOOP.init = init;
  res -> d.LOOP.body = body;
  return res;
}

struct cmd * TExpr(struct expr * exp) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_EXPR;
  res -> d.EXPR.exp = exp;
  return res;

}

struct expr_list * TExprList(struct expr * expr, struct expr_list * nxt) {
  struct expr_list * res = new_expr_list_ptr();
  res->exp = expr;
  res->nxt = nxt;
  return res;
}


#define PP_INDENT_UNIT 2

struct var_list;

struct loop_map {
  const struct cmd *loop;
  int id;
  struct var_list *vars;
  struct loop_map *next;
};

struct var_list {
  char *name;
  struct var_list *next;
};

static struct loop_map *g_loops = NULL;
static int g_loop_counter = 0;

static void pp_spaces(int n) {
  for (int i = 0; i < n; i++) {
    putchar(' ');
  }
}

static void pp_newline(int indent) {
  putchar('\n');
  pp_spaces(indent);
}

static int loop_id_of(const struct cmd *loop) {
  for (struct loop_map *p = g_loops; p != NULL; p = p->next) {
    if (p->loop == loop) {
      return p->id;
    }
  }
  return -1;
}

static struct loop_map *loop_map_of(const struct cmd *loop) {
  for (struct loop_map *p = g_loops; p != NULL; p = p->next) {
    if (p->loop == loop) {
      return p;
    }
  }
  return NULL;
}

static int var_list_contains(const struct var_list *vars, const char *name) {
  for (const struct var_list *p = vars; p != NULL; p = p->next) {
    if (p->name != NULL && name != NULL && strcmp(p->name, name) == 0) {
      return 1;
    }
  }
  return 0;
}

static void var_list_append(struct var_list **vars, char *name) {
  if (name == NULL) {
    return;
  }
  if (var_list_contains(*vars, name)) {
    return;
  }

  struct var_list *node = (struct var_list *)malloc(sizeof(struct var_list));
  if (node == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  node->name = name;
  node->next = NULL;

  if (*vars == NULL) {
    *vars = node;
    return;
  }
  struct var_list *tail = *vars;
  while (tail->next != NULL) {
    tail = tail->next;
  }
  tail->next = node;
}

static void collect_vars_in_cmd(struct cmd *c, struct var_list **vars) {
  if (c == NULL) {
    return;
  }
  switch (c->t) {
  case T_ASGN:
    if (c->d.ASGN.left != NULL && c->d.ASGN.left->t == T_VAR) {
      var_list_append(vars, c->d.ASGN.left->d.VAR.name);
    }
    break;
  case T_SEQ:
    collect_vars_in_cmd(c->d.SEQ.left, vars);
    collect_vars_in_cmd(c->d.SEQ.right, vars);
    break;
  case T_IF:
    collect_vars_in_cmd(c->d.IF.left, vars);
    collect_vars_in_cmd(c->d.IF.right, vars);
    break;
  case T_LOOP:
    collect_vars_in_cmd(c->d.LOOP.init, vars);
    collect_vars_in_cmd(c->d.LOOP.body, vars);
    break;
  default:
    break;
  }
}

static void collect_loops(const struct cmd *c) {
  if (c == NULL) {
    return;
  }
  switch (c->t) {
  case T_SEQ:
    collect_loops(c->d.SEQ.left);
    collect_loops(c->d.SEQ.right);
    break;
  case T_IF:
    collect_loops(c->d.IF.left);
    collect_loops(c->d.IF.right);
    break;
  case T_LOOP: {
    if (loop_id_of(c) == -1) {
      struct loop_map *node = (struct loop_map *)malloc(sizeof(struct loop_map));
      if (node == NULL) {
        printf("Failure in malloc.\n");
        exit(0);
      }
      node->loop = c;
      node->id = g_loop_counter++;
      node->vars = NULL;
      node->next = g_loops;
      g_loops = node;

      /* Variables threaded through repeat_break: those assigned in init/body. */
      collect_vars_in_cmd((struct cmd *)c->d.LOOP.init, &node->vars);
      collect_vars_in_cmd((struct cmd *)c->d.LOOP.body, &node->vars);
    }
    collect_loops(c->d.LOOP.init);
    collect_loops(c->d.LOOP.body);
    break;
  }
  default:
    break;
  }
}

static void free_loops(void) {
  while (g_loops != NULL) {
    struct loop_map *next = g_loops->next;
    while (g_loops->vars != NULL) {
      struct var_list *vnext = g_loops->vars->next;
      free(g_loops->vars);
      g_loops->vars = vnext;
    }
    free(g_loops);
    g_loops = next;
  }
  g_loop_counter = 0;
}

static void pp_expr(struct expr *e);
static void pp_cmd_expr_inner(struct cmd *c, int indent, int is_tail);
static void pp_cmd_expr(struct cmd *c, int indent);

static int var_list_length(const struct var_list *vars);
static void pp_state_type(const struct var_list *vars);
static void pp_state_pattern(const struct var_list *vars);
static void pp_state_tuple(const struct var_list *vars);
static void pp_cmd_loop_body(struct cmd *c, int indent, const struct var_list *vars);

static int var_list_length(const struct var_list *vars) {
  int n = 0;
  for (const struct var_list *p = vars; p != NULL; p = p->next) {
    n++;
  }
  return n;
}

static void pp_state_type(const struct var_list *vars) {
  int n = var_list_length(vars);
  if (n <= 0) {
    printf("unit");
    return;
  }
  for (int i = 0; i < n; i++) {
    if (i == 0) {
      printf("Z");
    } else {
      printf(" * Z");
    }
  }
}

static void pp_state_pattern_rec(const struct var_list *vars) {
  if (vars == NULL) {
    printf("tt");
    return;
  }
  if (vars->next == NULL) {
    printf("%s", vars->name);
    return;
  }
  putchar('(');
  printf("%s", vars->name);
  printf(", ");
  if (vars->next->next == NULL) {
    printf("%s", vars->next->name);
  } else {
    pp_state_pattern_rec(vars->next);
  }
  putchar(')');
}

static void pp_state_pattern(const struct var_list *vars) {
  int n = var_list_length(vars);
  if (n <= 0) {
    printf("_");
    return;
  }
  if (n == 1) {
    printf("%s", vars->name);
    return;
  }
  printf("'");
  pp_state_pattern_rec(vars);
}

static void pp_state_tuple_rec(const struct var_list *vars) {
  if (vars == NULL) {
    printf("tt");
    return;
  }
  if (vars->next == NULL) {
    printf("%s", vars->name);
    return;
  }
  putchar('(');
  printf("%s", vars->name);
  printf(", ");
  if (vars->next->next == NULL) {
    printf("%s", vars->next->name);
  } else {
    pp_state_tuple_rec(vars->next);
  }
  putchar(')');
}

static void pp_state_tuple(const struct var_list *vars) {
  int n = var_list_length(vars);
  if (n <= 0) {
    printf("tt");
    return;
  }
  if (n == 1) {
    printf("%s", vars->name);
    return;
  }
  pp_state_tuple_rec(vars);
}

static void pp_expr(struct expr *e) {
  if (e == NULL) {
    return;
  }
  switch (e->t) {
  case T_CONST:
    printf("%u", e->d.CONST.value);
    break;
  case T_VAR:
    printf("%s", e->d.VAR.name);
    break;
  case T_BINOP:
    putchar('(');
    pp_expr(e->d.BINOP.left);
    switch (e->d.BINOP.op) {
    case T_PLUS:
      printf(" + ");
      break;
    case T_MINUS:
      printf(" - ");
      break;
    case T_MUL:
      printf(" * ");
      break;
    case T_DIV:
      printf(" / ");
      break;
    case T_MOD:
      printf(" mod ");
      break;
    case T_LT:
      printf(" < ");
      break;
    case T_GT:
      printf(" > ");
      break;
    case T_LE:
      printf(" <= ");
      break;
    case T_GE:
      printf(" >= ");
      break;
    case T_EQ:
      printf(" = ");
      break;
    case T_NE:
      printf(" <> ");
      break;
    case T_AND:
      printf(" /\\ ");
      break;
    case T_OR:
      putchar(' ');
      putchar('\\');
      putchar('/');
      putchar(' ');
      break;
    }
    pp_expr(e->d.BINOP.right);
    putchar(')');
    break;
  case T_UNOP:
    if (e->d.UNOP.op == T_UMINUS) {
      printf("(-");
      pp_expr(e->d.UNOP.arg);
      putchar(')');
    } else {
      printf("(~ ");
      pp_expr(e->d.UNOP.arg);
      putchar(')');
    }
    break;
  case T_FUN:
    printf("%s", e->d.FUN.name);
    for (struct expr_list *it = e->d.FUN.arg; it != NULL; it = it->nxt) {
      putchar(' ');
      pp_expr(it->exp);
    }
    break;
  }
}

void print_expr(struct expr *e) {
  pp_expr(e);
}

static void pp_cmd_expr_inner(struct cmd *c, int indent, int is_tail) {
  if (c == NULL) {
    printf("ret tt");
    return;
  }
  switch (c->t) {
  case T_DECL:
    printf("ret tt");
    if (!is_tail) {
      printf(";;");
    }
    break;
  case T_ASGN:
    if (c->d.ASGN.left != NULL && c->d.ASGN.left->t == T_VAR) {
      printf("%s <- ", c->d.ASGN.left->d.VAR.name);
      pp_expr(c->d.ASGN.right);
      printf(";;");
    } else {
      pp_expr(c->d.ASGN.left);
      printf(" <- ");
      pp_expr(c->d.ASGN.right);
      printf(";;");
    }
    if (is_tail) {
      pp_newline(indent);
      printf("ret tt");
    }
    break;
  case T_SEQ:
    pp_cmd_expr_inner(c->d.SEQ.left, indent, 0);
    pp_newline(indent);
    pp_cmd_expr_inner(c->d.SEQ.right, indent, is_tail);
    break;
  case T_IF:
    printf("choice");
    pp_newline(indent + PP_INDENT_UNIT);
    putchar('(');
    printf("assume (");
    pp_expr(c->d.IF.cond);
    printf(");;");
    pp_newline(indent + 2 * PP_INDENT_UNIT);
    pp_cmd_expr_inner(c->d.IF.left, indent + 2 * PP_INDENT_UNIT, 1);
    putchar(')');
    pp_newline(indent + PP_INDENT_UNIT);
    putchar('(');
    printf("assume (~ ");
    pp_expr(c->d.IF.cond);
    printf(");;");
    pp_newline(indent + 2 * PP_INDENT_UNIT);
    pp_cmd_expr_inner(c->d.IF.right, indent + 2 * PP_INDENT_UNIT, 1);
    putchar(')');
    if (!is_tail) {
      printf(";;");
    }
    break;
  case T_CONTINUE:
    printf("continue tt");
    if (!is_tail) {
      printf(";;");
    }
    break;
  case T_BREAK:
    printf("break tt");
    if (!is_tail) {
      printf(";;");
    }
    break;
  case T_SKIP:
    printf("ret tt");
    if (!is_tail) {
      printf(";;");
    }
    break;
  case T_LOOP: {
    int id = loop_id_of(c);
    if (id < 0) {
      id = 0;
    }
    struct loop_map *lm = loop_map_of(c);
    const struct var_list *vars = (lm == NULL ? NULL : lm->vars);

    /* First run loop_init to bind initial state variables. */
    if (c->d.LOOP.init != NULL && c->d.LOOP.init->t != T_SKIP) {
      pp_cmd_expr_inner(c->d.LOOP.init, indent, 0);
      pp_newline(indent);
    }

    /* Then call repeat_break with initial state and bind the returned state. */
    pp_state_pattern(vars);
    printf(" <- repeat_break body_%d ", id);
    pp_state_tuple(vars);
    printf(";;");
    if (is_tail) {
      pp_newline(indent);
      printf("ret tt");
    }
    break;
  }
  case T_EXPR:
    /* Discard unused result: bind to '_' */
    printf("_ <- ");
    if (c->d.EXPR.exp != NULL && c->d.EXPR.exp->t == T_FUN) {
      pp_expr(c->d.EXPR.exp);
    } else {
      printf("ret (");
      pp_expr(c->d.EXPR.exp);
      putchar(')');
    }
    printf(";;");
    if (is_tail) {
      pp_newline(indent);
      printf("ret tt");
    }
    break;
  }
}

static void pp_cmd_expr(struct cmd *c, int indent) {
  pp_cmd_expr_inner(c, indent, 1);
}

static int cmd_always_terminal_in_loop_body(const struct cmd *c) {
  if (c == NULL) {
    return 0;
  }
  switch (c->t) {
  case T_CONTINUE:
  case T_BREAK:
    return 1;
  case T_SEQ:
    return cmd_always_terminal_in_loop_body(c->d.SEQ.right);
  case T_IF:
    return cmd_always_terminal_in_loop_body(c->d.IF.left) &&
           cmd_always_terminal_in_loop_body(c->d.IF.right);
  default:
    return 0;
  }
}

static void pp_cmd_loop_body_inner(struct cmd *c, int indent, const struct var_list *vars, int is_tail);

static void pp_cmd_loop_body_inner(struct cmd *c, int indent, const struct var_list *vars, int is_tail) {
  if (c == NULL) {
    if (is_tail) {
      printf("continue ");
      pp_state_tuple(vars);
    } else {
      printf("ret tt");
    }
    return;
  }

  switch (c->t) {
  case T_DECL:
    printf("ret tt");
    break;
  case T_ASGN:
    if (c->d.ASGN.left != NULL && c->d.ASGN.left->t == T_VAR) {
      printf("%s <- ", c->d.ASGN.left->d.VAR.name);
      pp_expr(c->d.ASGN.right);
      printf(";;");
    } else {
      pp_expr(c->d.ASGN.left);
      printf(" <- ");
      pp_expr(c->d.ASGN.right);
      printf(";;");
    }
    break;
  case T_SEQ:
    pp_cmd_loop_body_inner(c->d.SEQ.left, indent, vars, 0);
    pp_newline(indent);
    pp_cmd_loop_body_inner(c->d.SEQ.right, indent, vars, is_tail);
    return;
  case T_IF:
    printf("choice");
    pp_newline(indent + PP_INDENT_UNIT);
    putchar('(');
    printf("assume (");
    pp_expr(c->d.IF.cond);
    printf(");;");
    pp_newline(indent + 2 * PP_INDENT_UNIT);
    pp_cmd_loop_body_inner(c->d.IF.left, indent + 2 * PP_INDENT_UNIT, vars, is_tail);
    putchar(')');
    pp_newline(indent + PP_INDENT_UNIT);
    putchar('(');
    printf("assume (~ ");
    pp_expr(c->d.IF.cond);
    printf(");;");
    pp_newline(indent + 2 * PP_INDENT_UNIT);
    pp_cmd_loop_body_inner(c->d.IF.right, indent + 2 * PP_INDENT_UNIT, vars, is_tail);
    putchar(')');
    return;
  case T_CONTINUE:
    printf("continue ");
    pp_state_tuple(vars);
    return;
  case T_BREAK:
    printf("break ");
    pp_state_tuple(vars);
    return;
  case T_SKIP:
    printf("ret tt");
    break;
  case T_LOOP:
    /* Nested loop inside loop body is a statement; sequence it. */
    pp_cmd_expr_inner(c, indent, 0);
    break;
  case T_EXPR:
    /* Discard unused result: bind to '_' */
    printf("_ <- ");
    if (c->d.EXPR.exp != NULL && c->d.EXPR.exp->t == T_FUN) {
      pp_expr(c->d.EXPR.exp);
    } else {
      printf("ret (");
      pp_expr(c->d.EXPR.exp);
      putchar(')');
    }
    printf(";;");
    break;
  }

  if (is_tail && !cmd_always_terminal_in_loop_body(c)) {
    pp_newline(indent);
    printf("continue ");
    pp_state_tuple(vars);
  }
}

static void pp_cmd_loop_body(struct cmd *c, int indent, const struct var_list *vars) {
  pp_cmd_loop_body_inner(c, indent, vars, 1);
}

static void pp_emit_loop_definitions(void) {
  /* g_loops is a stack (LIFO). Emit in reverse id order to get stable numbering. */
  for (int id = g_loop_counter - 1; id >= 0; id--) {
    const struct cmd *loop = NULL;
    const struct var_list *vars = NULL;
    for (struct loop_map *p = g_loops; p != NULL; p = p->next) {
      if (p->id == id) {
        loop = p->loop;
        vars = p->vars;
        break;
      }
    }
    if (loop == NULL) {
      continue;
    }

    printf("Definition body_%d : ", id);
    pp_state_type(vars);
    printf(" -> SetMonad.M (ContinueOrBreak (");
    pp_state_type(vars);
    printf(") (");
    pp_state_type(vars);
    printf(")) :=\n");
    printf("  fun ");
    pp_state_pattern(vars);
    printf(" =>\n");
    printf("  ");
    pp_cmd_loop_body(((struct cmd *)loop)->d.LOOP.body, 2, vars);
    printf(".\n\n");
  }
}

static void pp_program(struct cmd *root) {
  free_loops();
  collect_loops(root);

  pp_emit_loop_definitions();

  printf("Definition main : SetMonad.M unit :=\n");
  printf("  ");
  pp_cmd_expr(root, 2);
  printf(".\n");

  free_loops();
}

void print_cmd(struct cmd *c) {
  pp_program(c);
}

unsigned int build_nat(char * c, int len) {
  int s = 0, i = 0;
  for (i = 0; i < len; ++i) {
    if (s > 429496729) {
      printf("We cannot handle natural numbers greater than 4294967295.\n");
      exit(0);
    }
    if (s == 429496729 && c[i] > '5') {
      printf("We cannot handle natural numbers greater than 4294967295.\n");
      exit(0);
    }
    s = s * 10 + (c[i] - '0');
  }
  return s;
}

char * new_str(char * str, int len) {
  char * res = (char *) malloc(sizeof(char) * (len + 1));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  strcpy(res, str);
  return res;
}
