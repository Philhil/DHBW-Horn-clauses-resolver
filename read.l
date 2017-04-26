%option noyywrap

%{
  #include "data.h"
  #include "grammar.tab.h"
%}


INT      [0-9]
VARLEX [n-z]{INT}*
FUNCLEX [a-m]{INT}*
PRAEDIKATLEX [A-Z]{INT}*
%%

{VARLEX} {
  yylval.text = strdup(yytext);
  return VAR;
}

{FUNCLEX} {
  yylval.text = strdup(yytext);
  return FUNC;
}

{PRAEDIKATLEX} {
  yylval.text = strdup(yytext);
  return PRAEDIKAT;
}

"->" {
  return ARROW;
}

"&" {
  return UND;
} 

"," {
  return KOMMA;
}

true {
  return TRUE;
}

false {
  return FALSE;
}

"(" {
return KLAMMERO;
}

")" {
  return KLAMMERC;
}

"\n"|"\r\n" {
  return NEWLINE;
}

" " {
  /* Skip */
}

. { 
  printf("Do not know: %s\n", yytext);
}

%%