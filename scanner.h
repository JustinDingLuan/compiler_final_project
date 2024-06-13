/********************** scanner.h ********************/
#include <stdio.h>
#include <stdlib.h>
#include "sym.h"
#include "symbol.h"
int nextChar;
int cp = 0;
int linelen = 0;
int linenum = 0;
char line[512];
FILE *sourcefile;
void advance()
{
  cp++;
  if (cp < linelen)
    nextChar = line[cp];
  else
  {
    if (feof(sourcefile))
    {
      linelen = 0;
      nextChar = -1;
    }
    else
    {
      if (fgets(line, 512, sourcefile) != NULL)
      {
        linelen = strlen(line);
        linenum++;
        cp = 0;
        nextChar = line[cp];
        printf("%4d %s", linenum, line);
      }
    }
  }
}
struct symbolTag *nextToken()
{
  static char s[128] = "";
  int n = 0;
  while (1)
  {
    if (nextChar >= 'a' && nextChar <= 'z' ||
        nextChar >= 'A' && nextChar <= 'Z')
    {
      do
      {
        s[n++] = nextChar;
        advance();
      } while (nextChar >= 'a' && nextChar <= 'z' ||
               nextChar >= 'A' && nextChar <= 'Z' ||
               nextChar >= '0' && nextChar <= '9');
      s[n] = '\0';

      if (strcmp(s, "if") == 0)
      {
        return newSymbol(symIF, linenum, cp, s);
      }
      else if (strcmp(s, "else") == 0)
      {
        return newSymbol(symELSE, linenum, cp, s);
      }
      else if (strcmp(s, "int") == 0)
      {
        return newSymbol(symINT, linenum, cp, s);
      }
      else if (strcmp(s, "main") == 0)
      {
        return newSymbol(symMAIN, linenum, cp, s);
      }
      else if (strcmp(s, "while") == 0)
      {
        return newSymbol(symWHILE, linenum, cp, s);
      }
      else if (strcmp(s, "printf") == 0)
      {
        return newSymbol(symWRITE, linenum, cp, s);
      }
      else if (strcmp(s, "scanf") == 0)
      {
        return newSymbol(symREAD, linenum, cp, s);
      }
      else if (strcmp(s, "char") == 0)
      {
        return newSymbol(symSTRINGDeclare, linenum, cp, s);
      }

      return newSymbol(symIDENTIFIER, linenum, cp, s);
    }
    else if (nextChar >= '0' && nextChar <= '9')
    {
      do
      {
        s[n++] = nextChar;
        advance();
      } while (nextChar >= '0' && nextChar <= '9');
      s[n] = '\0';
      return newSymbol(symNUMBER, linenum, cp, s);
    }
    else
    {
      switch (nextChar)
      {
      case ';':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symSEMI, linenum, cp, s);
      case '.':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symPERIOD, linenum, cp, s);
      case ',':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symCOMMA, linenum, cp, s);
      case '(':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symLPAREN, linenum, cp, s);
      case ')':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symRPAREN, linenum, cp, s);
      case '[':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symLSquareBracket, linenum, cp, s);
      case ']':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symRSquareBracket, linenum, cp, s);
      case '{':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symLBRACE, linenum, cp, s);
      case '}':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symRBRACE, linenum, cp, s);
      case '+':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symPLUS, linenum, cp, s);
      case '-':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symMINUS, linenum, cp, s);
      case '*':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symMUL, linenum, cp, s);
      case '/':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symDIV, linenum, cp, s);
      case '%':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symMOD, linenum, cp, s);
      case '&':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        if (nextChar == '&')
        {
          s[n++] = nextChar;
          advance();
          s[n] = '\0';
          return newSymbol(symAnd, linenum, cp, s);
        }
        return newSymbol(symGetAddress, linenum, cp, s);
      case '|':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        if (nextChar == '|')
        {
          s[n++] = nextChar;
          advance();
          s[n] = '\0';
          return newSymbol(symOr, linenum, cp, s);
        }
        return newSymbol(symerror, linenum, cp, s);
      case '!':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        if(nextChar == '='){
          s[n++] = nextChar;
          advance();
          s[n] = '\0';
          return newSymbol(symNEQ, linenum, cp, s);
        }
        return newSymbol(symerror, linenum, cp, s);
      case '=':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        if (nextChar == '=')
        {
          s[n++] = nextChar;
          advance();
          s[n] = '\0';
          return newSymbol(symEQ, linenum, cp, s);
        }
        return newSymbol(symBECOMES, linenum, cp, s);
      case '<':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        if (nextChar == '=')
        {
          s[n++] = nextChar;
          advance();
          s[n] = '\0';
          return newSymbol(symLEQ, linenum, cp, s);
        }        
        else
        {
          return newSymbol(symLESS, linenum, cp, s);
        }
      case '>':
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        if (nextChar == '=')
        {
          s[n++] = nextChar;
          advance();
          s[n] = '\0';
          return newSymbol(symGEQ, linenum, cp, s);
        }
        else
        {
          return newSymbol(symGREATER, linenum, cp, s);
        }
      // case ':':
      //   s[n++] = nextChar;
      //   s[n] = '\0';
      //   advance();
      //   if (nextChar == '=')
      //   {
      //     s[n++] = nextChar;
      //     advance();
      //     s[n] = '\0';
      //     return newSymbol(symBECOMES, linenum, cp, s);
      //   }
      //   else
      //   {
      //     return newSymbol(symerror, linenum, cp, s);
      //   }
      case '"':
        advance();
        while (nextChar != '"')
        {
          s[n++] = nextChar;
          advance();
        }
        advance();
        s[n] = '\0';
        return newSymbol(symSTRING, linenum, cp, s);
      case '\0':
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        advance(); // ©¿²¤ªÅ¥Õ¦r¤¸
        break;
      case -1:
        return NULL;
      default:
        s[n++] = nextChar;
        s[n] = '\0';
        advance();
        return newSymbol(symerror, linenum, cp, s);
      }
    }
  }
}
void scanner(FILE *f)
{
  sourcefile = f;
}
