%option noyywrap yylineno
%option outfile="lexer.c" header-file="lexer.h"
%{
#include "lang.h"
#include "parser.h"
%}

%%

0|[1-9][0-9]* {
    yylval.n = build_nat(yytext, yyleng);
    return TM_NAT;
}

"if" {
    return TM_IF;
}

"else" {
    return TM_ELSE;
}

"loop_init" {
    return TM_LOOP_INIT;
}

"loop_body" {
    return TM_LOOP_BODY;
}

"continue" {
    return TM_CONTINUE;
}

"break" {
    return TM_BREAK;
}

"skip" {
    return TM_SKIP;
}

"true" {
    return TM_TRUE;
}

"false" {
    return TM_FALSE;
}

[_A-Za-z][_A-Za-z0-9]* {
    yylval.i = new_str(yytext, yyleng);
    if (yylval.i[0] == '_' && yylval.i[1] == '\0') {
        fprintf(stderr, "Illigal identity \'_\'\n");
        exit(0);
    }
    return TM_IDENT;
}

";;" {
    return TM_SEMICOL;
    }

"{" {
    return TM_LEFT_BRACE;
}

"}" {
    return TM_RIGHT_BRACE;
}

"(" {
    return TM_LEFT_PAREN;
    }

")" {
    return TM_RIGHT_PAREN;
    }

":" {
    return TM_COLON;    
}

"," {
    return TM_COMMA;
}

"+" {
    return TM_PLUS;
    }

"-" {
    return TM_MINUS;
    }

"*" {
    return TM_MUL;
    }

"/" {
    return TM_DIV;
    }

"%" {
    return TM_MOD;
    }

"<" {
    return TM_LT;
    }

">" {
    return TM_GT;
    }

"<=" {
    return TM_LE;
    }

">=" {
    return TM_GE;
    }

"==" {
    return TM_EQ;
    }

"!=" {
    return TM_NE;
    }

"<-" {
    return TM_ASGNOP;
    }

"&&" {
    return TM_AND;
    }

"||" {
    return TM_OR;
    }

"!" {
    return TM_NOT;
    }

[ \t\n\r]    { };

.   {printf("%s",yytext);
     return -1; }
%%
