/************************* plone.c ************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/*
** �ۭq���Y��
*/
#include "scanner.h"
#include "resword.h"
#include "err.h"
#include "followsym.h"
#include "idobj.h"
#include "procobj.h"
#include "startsym.h"
/*
** �ۭq�`��
*/
#define IDLEN 36
#define BUFSIZE 512
#define PROCDEPTH 36
/*
** �ۭq�禡�쫬
*/
void Identifier();
void Number();
void IdentifierList();
void Factor();
void Term();
void Expression();
void Condition();
void WriteStatement();
void ReadStatement();
void WhileStatement();
void IfStatement();
void CompoundStatement();
void CallStatement();
void AssignmentStatement();
void Statement();
void ProcDeclaration();
void VarDeclaration();
void ConstDeclaration();
void Block();
void ProgramHead();
void Program();
/*
** �����ܼ�
*/

struct symbolTag *goahead;
struct symbolTag *token;
int errorCount = 0;
struct idobjTag *idobj;
char outname[IDLEN];
FILE *outfile;
int labelCount = 0;
int level = -1;
char progname[IDLEN];
char procname[IDLEN];
struct procobjTag *procStack[BUFSIZE];
int procTop = 0;
struct procobjTag *procobj = NULL;
char id[IDLEN];
char buf[BUFSIZE];

int brace_balance = 0;
bool multiple_statement = false;
/*
** Error()
*/
void Error(int n)
{
  int j;
  printf("****");
  for (j = 0; j <= token->right; j++)
    printf(" ");
  printf("^%d  %s\n", n, errmsgs[n]);
  errorCount++;
}
/*
** checkexist()
*/
bool Check_ID_Exist()
{
  idobj = getIdobj(procStack[procTop - 1], token->value);
  if (idobj == NULL)
    return false;
  else
    return true;
}

/*
** skip()
*/
void skip(char follows[], int n)
{
  if (follows[token->sym] == 0)
  {
    Error(n);
    while (follows[token->sym] == 0)
    {
      token = nextToken();
      // printf("%s\n", token->value);
    }
  }
}

bool expected_value(char *expect, char *current, int error, char *skip_name, int skip_err_num)
{
  if (strcmp(expect, current) == 0)
  {
    token = nextToken();
    return true;
  }
  else
  {
    Error(error);
    skip(skip_name, skip_err_num);
    return false;
  }
}

bool ID_IDtypecheck(struct idobjTag *id, char *skip_name, int err_num)
{
  struct idobjTag *temp = getIdobj(procStack[procTop - 1], token->value);
  if (temp->attr == id->attr)
  {
    return true;
  }

  Error(err_num);
  skip(skip_name, 23);

  return false;
}

bool ID_EXPRtypecheck(int attr, char *skip_name, int err_num)
{
  if (attr == symINT && token->sym == symNUMBER)
  {
    return true;
  }
  else if (attr == symSTRING && token->sym == symSTRING)
  {
    return true;
  }

  Error(err_num);
  skip(skip_name, 23);

  return false;
}

/*
** �y�k�W�h#1 <Program>
*/
void Program()
{
  struct procobjTag *p;
  if (strcmp(token->value, "int") == 0)
  {
    token = nextToken();
    if (strcmp(token->value, "main") == 0)
    {
      strcpy(procname, token->value);
      p = newProcobj(procname);
      procpush(p);
      procStack[procTop++] = p;
      strcpy(progname, token->value);
      strcpy(outname, token->value);
      strcat(outname, ".asm");
      outfile = fopen(outname, "w");
      ++labelCount;
      sprintf(buf,
              ";************** %s ****************\n"
              ";\n"
              "\tORG\t100H\n"
              "\tJMP\t_start%d\n"
              "_intstr\tDB\t'     ','$'\n"
              "_buf\tTIMES 256 DB ' '\n"
              "\tDB 13,10,'$'\n",
              outname, labelCount);
      fprintf(outfile, buf);
      strcpy(buf, "%include\t\"dispstr.mac\"\n");
      strcat(buf, "%include\t\"itostr.mac\"\n");
      strcat(buf, "%include\t\"readstr.mac\"\n");
      strcat(buf, "%include\t\"strtoi.mac\"\n");
      strcat(buf, "%include\t\"newline.mac\"\n");
      fputs(buf, outfile);
      token = nextToken();

      sprintf(buf, "_start%d:\n", labelCount);
      fprintf(outfile, buf);

      if (token->sym == symLPAREN)
      {
        token = nextToken();
        if (token->sym == symRPAREN)
        {
          token = nextToken();
        }
        else
        {
          Error(18);
        }
        CompoundStatement();

        if (token != NULL)
        {
          Error(29);
        }
      }
      else
      {
        Error(17);
      }
    }
    else
      Error(1);
  }
}
// Programhead�N�Oint main
/*
** �y�k�W�h#2 <ProgramHead>
*/
// void ProgramHead()
// {
//   struct procobjTag *p;
//   if (strcmp(token->value,"PROGRAM")==0)
//   {
//     token = nextToken();
//     if (token->sym == symIDENTIFIER)
//     {
//       strcpy(procname, token->value);
//       p = newProcobj(procname);
//       p->next = NULL;
//       p->head = NULL;
//       p->tail = NULL;
//       procpush(p);
//       procStack[procTop++] = p;
//       strcpy(progname, token->value);
//       strcpy(outname, token->value);
//       strcat(outname, ".asm");
//       outfile = fopen(outname, "w");
//       ++labelCount;
//       sprintf(buf,
//         ";************** %s ****************\n"
//         ";\n"
//         "\tORG\t100H\n"
//         "\tJMP\t_start%d\n"
//         "_intstr\tDB\t'     ','$'\n"
//         "_buf\tTIMES 256 DB ' '\n"
//         "\tDB 13,10,'$'\n",
//         outname, labelCount);
//       fprintf(outfile, buf);
//       strcpy(buf, "%include\t\"dispstr.mac\"\n");
//       strcat(buf, "%include\t\"itostr.mac\"\n");
//       strcat(buf, "%include\t\"readstr.mac\"\n");
//       strcat(buf, "%include\t\"strtoi.mac\"\n");
//       strcat(buf, "%include\t\"newline.mac\"\n");
//       fputs(buf, outfile);
//       token = nextToken();
//       if (token->sym == symSEMI)
//         token = nextToken();
//       else
//         Error(3);
//     }
//     else
//       Error(2);
//   }
//   else
//     Error(1);
// }
/*
** �y�k�W�h#3 <Block>
*/

void Block()
{
  ++level;
  sprintf(buf, "_start%d:\n", labelCount);
  fprintf(outfile, buf);
  if (strcmp(token->value, "PROCEDURE") == 0)
    ProcDeclaration();
  strcpy(procname, procStack[procTop - 1]->name);
  CompoundStatement();
  --level;
}
/*
** �y�k�W�h#4 <ConstDeclaration>
*/
// void ConstDeclaration()
// {
//   if (strcmp(token->value, "CONST")==0)
//   {
//     token = nextToken();
//     if (token->sym == symIDENTIFIER)
//     {
//       varlistadd(procStack[procTop-1],
//         newIdobj(token->value, token->sym, symCONST,
//         level, procStack[procTop-1]->name));
//       strcpy(id, procStack[procTop-1]->name);
//       strcat(id, "_");
//       strcat(id, token->value);
//       sprintf(buf, "%s\tDB\t", id);
//       fprintf(outfile, buf);
//       token = nextToken();
//     }
//     if (token->sym == symEQ)
//     {
//       token = nextToken();
//       sprintf(buf, "'%s','$'\n", token->value);
//       fprintf(outfile, buf);
//       token = nextToken();
//       while (token->sym == symCOMMA)
//       {
//         token = nextToken();
//         if (token->sym == symIDENTIFIER)
//         {
//           varlistadd(procStack[procTop-1],
//             newIdobj(token->value, token->sym, symCONST,
//             level, procStack[procTop-1]->name));
//           sprintf(buf, "%s_%s\tDB\t",
//             procStack[procTop-1]->name, token->value);
//           fprintf(outfile, buf);
//           token = nextToken();
//         }
//         if (token->sym == symEQ)
//         {
//           token = nextToken();
//           sprintf(buf, "'%s','$'\n", token->value);
//           fprintf(outfile, buf);
//           token = nextToken();
//         }
//         else
//           Error(5);
//       }
//       if (token->sym == symSEMI)
//         token = nextToken();
//       else
//       {
//         Error(6);
//         skip(statement, 23);
//         if (token->sym == symSEMI)
//           token = nextToken();
//       }
//     }
//     else
//     {
//       Error(5);
//       skip(statement, 23);
//       if (token->sym == symSEMI)
//         token = nextToken();
//     }
//   }
//   else
//   {
//     Error(4);
//   }
// }
/*
** �y�k�W�h#5 <VarDeclaration>
*/
void create_int_id()
{
  idobj = newIdobj(token->value, token->sym,
                   symINT, level, procStack[procTop - 1]->name);

  varlistadd(procobjTop, idobj);
  sprintf(buf, "%s_%s\tDW\t0\n",
          procStack[procTop - 1]->name, token->value);
  fprintf(outfile, buf);
}

void INTDeclaration()
{
  if (strcmp(token->value, "int") == 0)
  {
    token = nextToken();
    if (token->sym == symIDENTIFIER)
    {
      create_int_id();
      goahead = nextToken();

      if (strcmp(goahead->value, "=") == 0) // ��assign
      {
        char result[BUFSIZE];
        if (idobj != NULL)
        {
          sprintf(result, "%s_%s",
                  idobj->procname, token->value);
        }
        else
        {
          Error(26);
          skip(statement, 23);
        }

        token = nextToken();

        if (token->sym == symIDENTIFIER)
        {
          if (!ID_IDtypecheck(idobj, statement, 32))
          {
            token = nextToken();
            return;
          }
        }
        else
        {
          if (!ID_EXPRtypecheck(idobj->attr, statement, 32))
          {
            token = nextToken();
            return;
          }
        }
        Expression();
        sprintf(buf, "\tPOP\tAX\n"
                     "\tMOV\t[%s], AX\n",
                result);
        fprintf(outfile, buf);
      }
      else
      {
        token = goahead;
      }

      while (token->sym == symCOMMA)
      {
        token = nextToken();
        if (token->sym == symIDENTIFIER)
        {
          create_int_id();
        }
        else
        {
          Error(21);
          skip(statement, 23);
        }

        goahead = nextToken();

        if (strcmp(goahead->value, "=") == 0) // ��assign
        {
          char result[BUFSIZE];

          if (idobj != NULL)
          {
            sprintf(result, "%s_%s",
                    idobj->procname, token->value);
          }
          else
          {
            Error(21);
            skip(statement, 23);
          }

          token = nextToken();
          if (token->sym == symIDENTIFIER)
          {
            if (!ID_IDtypecheck(idobj, statement, 32))
            {
              token = nextToken();
              return;
            }
          }
          else
          {
            if (!ID_EXPRtypecheck(idobj->attr, statement, 32))
            {
              token = nextToken();
              return;
            }
          }
          Expression();
          sprintf(buf, "\tPOP\tAX\n"
                       "\tMOV\t[%s], AX\n",
                  result);
          fprintf(outfile, buf);
        }
        else
        {
          token = goahead;
        }
      }      
    }
    else
    {
      Error(21);
      skip(statement, 23);
      return;
    }
  }
  else
  {
    Error(7);
  }
}

void Const_String()
{
  if (token->sym == symSTRING)
  {
    token = nextToken();
  }
  else
  {
    Error(34);
    skip(statement, 23);
  }
}

void create_string_id()
{
  idobj = newIdobj(token->value, token->sym,
                   symSTRING, level, procStack[procTop - 1]->name);

  varlistadd(procobjTop, idobj);
  sprintf(buf, "%s_%s\tDW\n",
          procStack[procTop - 1]->name, token->value);
  fprintf(outfile, buf);
  token = nextToken();
}

void StringDeclaration()
{
  if (token->sym == symSTRINGDeclare)
  {
    token = nextToken();

    if (token->sym == symIDENTIFIER)
    {
      create_string_id();

      if (token->sym == symLSquareBracket)
      {
        token = nextToken();

        if (token->sym != symNUMBER)
        {
          Error(22);
          skip(statement, 23);
          return;
        }
        else
        {
          token = nextToken();

          if (token->sym != symRSquareBracket)
          {
            Error(31);
            skip(statement, 23);
            return;
          }
        }
      }
      else
      {
        Error(30);
        skip(statement, 23);
        return;
      }

      goahead = nextToken();

      if (strcmp(goahead->value, "=") == 0) // ��assign
      {
        char result[BUFSIZE];
        if (idobj != NULL)
        {
          sprintf(result, "%s_%s",
                  idobj->procname, token->value);
        }
        else
        {
          Error(26);
          skip(statement, 23);
          return;
        }

        token = nextToken();       
        if (token->sym == symIDENTIFIER)
        {
          if (ID_IDtypecheck(idobj, statement, 32))
          {
            Expression();
          }
        }
        else
        {
          if (ID_EXPRtypecheck(idobj->attr, statement, 32))
          {
            Const_String();
          }
        }
      }
      else
      {
        token = goahead;
      }
      printf("%s %d\n" , token->value , token->sym);
      while (token->sym == symCOMMA)
      {
        token = nextToken();
        if (token->sym == symIDENTIFIER)
        {
          create_string_id();

          if (token->sym == symLSquareBracket)
          {
            token = nextToken();
            if (token->sym != symNUMBER)
            {
              Error(22);
              skip(statement, 23);
            }
            else
            {
              token = nextToken();
              if (token->sym == symRSquareBracket)
              {
                token = nextToken();
                if (token->sym != symSTRING)
                {
                  Error(34);
                  skip(statement, 23);
                }
              }
              else
              {
                Error(31);
                skip(statement, 23);
              }
            }
          }
          else
          {
            Error(31);
            skip(statement, 23);
          }

          goahead = nextToken();
          if (strcmp(goahead->value, "=") == 0) // ��assign
          {
            char result[BUFSIZE];
            if (idobj != NULL)
            {
              sprintf(result, "%s_%s",
                      idobj->procname, token->value);
            }
            else
            {
              Error(26);
              skip(statement, 23);
            }

            token = nextToken();
            if (token->sym == symIDENTIFIER)
            {
              if (ID_IDtypecheck(idobj, statement, 32))
              {
                Expression();
              }
            }
            else
            {
              if (ID_EXPRtypecheck(idobj->attr, statement, 32))
              {
                Const_String();
              }
            }
          }
          else
          {
            token = goahead;
          }
        }
        else
        {
          Error(26);
          skip(statement , 23);
          return;
        }
      }      
    }
    else
    {
      Error(21);
      skip(statement, 23);
      return;
    }
  }
  else
  {
    Error(30);
    skip(statement , 23);
    return;
  }
}

void VarDeclaration()
{
  if (token->sym == symINT)
  {
    printf("�iint\n");
    INTDeclaration();    
  }
  else if (token->sym == symSTRINGDeclare)
  {
    printf("�istring\n");
    StringDeclaration();
  }
}
/*
** �y�k�W�h#6 <ProcDeclaration>
*/
void ProcDeclaration()
{
  int tail;
  char *procid;
  struct procobjTag *p;
  while (strcmp(token->value, "PROCEDURE") == 0)
  {
    token = nextToken();
    tail = 0;
    if (token->sym == symIDENTIFIER)
    {
      strcpy(procname, token->value);
      p = newProcobj(procname);
      p->head = NULL;
      p->tail = NULL;
      varlistadd(
          procStack[procTop - 1],
          newIdobj(procname, token->sym, symPROCEDURE,
                   level, procStack[procTop - 1]->name));
      procpush(p);
      procStack[procTop++] = p;
      tail = ++labelCount;
      sprintf(buf, "\tJMP\t_go%d\n%s:\n"
                   "\tJMP\t_start%d\n",
              tail, token->value, labelCount);
      fprintf(outfile, buf);
      token = nextToken();
    }
    if (token->sym == symSEMI)
    {
      token = nextToken();
      Block();
      if (token->sym == symSEMI)
      {
        sprintf(buf, "\tRET\n_go%d:\n", tail);
        fprintf(outfile, buf);
        token = nextToken();
      }
      else
      {
        Error(6);
        skip(statement, 23);
        if (token->sym == symSEMI)
          token = nextToken();
      }
    }
    else
    {
      Error(6);
    }
    --procTop;
  }
}
/*
** �y�k�W�h#7 <Statement>
*/
void Statement()
{  
  if (isResword(token->value) != -1)
  {
    if (strcmp(token->value, "if") == 0)
    {
      printf("�iif\n");
      IfStatement();
    }
    else if (strcmp(token->value, "{") == 0)
    {
      printf("�icompound\n");
      CompoundStatement();
    }
    else if (strcmp(token->value, "while") == 0)
    {
      printf("�iwhile\n");
      WhileStatement();
    }
    else if (strcmp(token->value, "scanf") == 0)
    {
      printf("�iscanf\n");
      ReadStatement();
      if (token->sym != symSEMI)
      {
        Error(6);
        skip(statement, 23);
      }
      token = nextToken();
    }
    else if (strcmp(token->value, "printf") == 0)
    {
      printf("�iprintf\n");
      WriteStatement();
      if(token->sym != symSEMI){
        Error(6);
        skip(statement , 23);
      }
      token = nextToken();
    }
    else if (strcmp(token->value, "CALL") == 0)
    {
      printf("�icall\n");
      CallStatement();
    }
  }
  else if (token->sym == symIDENTIFIER)
  {
    printf("�iassign\n");
    AssignmentStatement();
    if (token->sym != symSEMI)
    {
      Error(6);
      skip(statement, 23);
    }
    token = nextToken();
  }
  else if (token->sym == symINT || token->sym == symSTRINGDeclare)
  {
    printf("�ivar\n");
    VarDeclaration();
    if (token->sym != symSEMI)
    {
      Error(6);
      skip(statement, 23);
    }
    token = nextToken();
  }
  else
  {
    // �o�̦n�������?
    Error(35);
    skip(statement, 23);
  }
}
/*
** �y�k�W�h#8 <AssignmentStatement>
*/
void AssignmentStatement()
{
  char result[BUFSIZE];
  idobj = getIdobj(procStack[procTop - 1], token->value);

  if (idobj != NULL)
  {
    sprintf(result, "%s_%s",
            idobj->procname, token->value);
  }

  Identifier();

  if (token->sym == symBECOMES)
  {
    token = nextToken();
    // �n�סA�h�Fstring

    if (token->sym == symIDENTIFIER)
    {
      if (!ID_IDtypecheck(idobj, statement, 32))
      {
        token = nextToken();
        return;
      }
    }
    else
    {
      if (!ID_EXPRtypecheck(idobj->attr, statement, 32))
      {
        token = nextToken();
        return;
      }
    }
    Expression();
    sprintf(buf, "\tPOP\tAX\n"
                 "\tMOV\t[%s], AX\n",
            result);
    fprintf(outfile, buf);
  }
  else
  {
    Error(8);
    skip(statement, 23);
  }  
}
/*
** �y�k�W�h#9 <CallStatement>
*/
void CallStatement()
{
  if (strcmp(token->value, "CALL") == 0)
  {
    token = nextToken();
    idobj = getIdobj(procStack[procTop - 1], token->value);
    if (idobj != NULL)
    {
      sprintf(buf, "\tCALL\t%s\n", token->value);
      fprintf(outfile, buf);
    }
    Identifier();
  }
  else
  {
    Error(9);
    skip(statement, 23);
  }
}
/*
** �y�k�W�h#10 <CompoundStatement>
*/
void CompoundStatement()
{
  level++;
  if (strcmp(token->value, "{") == 0)
  {
    token = nextToken();    
    while (token->sym == symINT || token->sym == symSTRINGDeclare)
    {
      printf("�ivar\n");
      VarDeclaration();
      if (token->sym != symSEMI)
      {
        Error(6);
        skip(statement, 23);
      }
      token = nextToken();
    }

    while (statement_start[token->sym])
    {
      printf("�istatement\n");
      Statement();
    }

    if (strcmp(token->value, "}") != 0)
    {
      Error(28);
      skip(statement, 23);
    }
    else
    {
      token = nextToken();
    }
  }
  else
  {
    Error(27);
    skip(statement, 23);
  }
  level--;
}
/*
** �y�k�W�h#11 <IfStatement>
*/
void IfStatement() // �ͦ��ջy�Pı�n��
{
  int head, tail, ending;
  if (strcmp(token->value, "if") == 0)
  {
    token = nextToken();
    if (strcmp(token->value, "(") == 0)
    {
      token = nextToken();
      Condition();
      head = labelCount;
      tail = ++labelCount;
      ending = tail;

      if (strcmp(token->value, ")") == 0)
      {
        token = nextToken();

        sprintf(buf, "\tJMP\t_go%d\n"
                     "_go%d:\n",
                tail, head);
        fprintf(outfile, buf);

        Statement();
        // if ��statement ��label�Ohead�Aelse��statementl��label�Otail
        // �p�G�S��else ->ending���ӬOhead+1�A�p�G��else -> label���ӬOtail+1
        // �ݧ��
        // �{�b���Y��label�O�Ū�
        // �ȧ�n
        if (strcmp(token->value, "else") == 0)
        {
          ending = ++labelCount;
          sprintf(buf, "\tJMP\t_go%d\n"
                       "_go%d:\n",
                  ending, tail);
          fprintf(outfile, buf);

          token = nextToken();
          Statement();

          sprintf(buf, "_go%d:\n", ending);
          fprintf(outfile, buf);
        }
        else
        {
          sprintf(buf, "_go%d:\n", tail);
          fprintf(outfile, buf);
        }
      }
      else
      {
        Error(18);
        skip(statement, 23);
        return;
      }
    }
    else
    {
      Error(17);
      skip(statement, 23);
      return;
    }
  }
  else
  {
    Error(12);
    skip(statement, 23);
    return;
  }
}
/*
** �y�k�W�h#12 <WhileStatement>
*/
void WhileStatement()
{
  int home, head, tail;
  if (strcmp(token->value, "while") == 0)
  {
    home = ++labelCount;
    sprintf(buf, "_go%d:\n", home);
    fprintf(outfile, buf);

    token = nextToken();
    if (strcmp(token->value, "(") == 0)
    {
      token = nextToken();
      
      Condition();

      head = labelCount;
      tail = ++labelCount;

      sprintf(buf, "\tJMP\t_go%d\n"
                   "_go%d:\n",
              tail, head);
      fprintf(outfile, buf);

      
      if (strcmp(token->value, ")") == 0)
      {
        token = nextToken();        
        Statement();
        sprintf(buf, "\tJMP\t_go%d\n", home);
        fprintf(outfile, buf);
      }
      else
      {
        Error(18);
        skip(statement, 23);
      }
    }
    else
    {
      Error(17);
      skip(statement, 23);
    }    

    sprintf(buf, "_go%d:\n", tail);
    fprintf(outfile, buf);
  }
  else
  {
    Error(14);
    skip(statement, 23);
  }
}
/*
** �y�k�W�h#13 <ReadStatement>
*/
void ReadStatement()
{
  if (strcmp(token->value, "scanf") == 0)
  {
    token = nextToken();
    int num_int = 0, num_specifier = 0, num_id = 0;
    

    if (token->sym == symLPAREN)
    {
      token = nextToken();

      if (token->sym == symSTRING)
      {
        // �P�_�榡�O�_���~
        int length = strlen(token->value);

        for (int i = 0; i < length; i++)
        {
          if (token->value[i] == '%')
          {
            if (num_specifier != num_int)
            {
              Error(33);
              skip(statement, 23);
              return;
            }
            num_specifier++;
          }
          else if (token->value[i] == 'd')
          {
            if (num_specifier != (num_int + 1))
            {
              Error(33);
              skip(statement, 23);
              return;
            }
            num_int++;          
          }
          // else if (token->value[i] == 's')
          // {
          //   if (num_specifier != (num_int + num_str + 1))
          //   {
          //     Error(33);
          //     skip(statement, 23);
          //     return;
          //   }
          //   num_str++;
          //   matching[num_id] = symSTRING;
          //   num_id++;
          // }
        }
        token = nextToken();
      }
      else
      {
        Error(33);
        skip(statement, 23);
        return;
      }
      
      printf("%s %d\n", token->value, token->sym);
      while (token->sym == symCOMMA)
      {
        token = nextToken();
        // �P�_�ѧO�r���ݩʬO�_�������T
        if(token->sym == symGetAddress)
        {
          token = nextToken();

          if (token->sym == symIDENTIFIER)
          {
            if (Check_ID_Exist())
            {
              sprintf(id, "%s_%s",
                      idobj->procname, token->value);
              sprintf(buf, "\treadstr\t_buf\n"
                            "\tstrtoi\t_buf, '$', %s\n"
                            "\tnewline\n",id);
              fprintf(outfile, buf);
              num_id++;
            }
            else{
              Error(26);
              skip(statement , 23);
              return;
            }
            Identifier();
          }
          else{
            Error(21);
            skip(statement , 23);
            return;
          }          
        }
        else{
          Error(37);
          skip(statement , 23);
          return;
        }        
      }

      if(num_id != num_int){
        Error(33);
        skip(statement , 23);
        return;
      }

      if (token->sym == symRPAREN)
      {
        token = nextToken();
      }
      else
      {
        Error(18);
        skip(statement, 23);
        return;
      }
    }
    else
    {
      Error(17);
      skip(statement, 23);
      return;
    }
  }
  else
  {
    Error(19);
    skip(statement, 23);
    return;
  }  
}

/*
** �y�k�W�h#14 <WriteStatement>
*/
void WriteStatement()
{
  if (strcmp(token->value, "printf") == 0)
  {
    token = nextToken();
    int num_int = 0, num_str = 0, num_specifier = 0 , num_id = 0;
    int matching[BUFSIZE];

    if (token->sym == symLPAREN)
    {
      token = nextToken();

      if (token->sym == symSTRING)
      {
        // �P�_�榡�O�_���~
        int length = strlen(token->value);

        for (int i = 0; i < length; i++)
        {
          if (token->value[i] == '%')
          {
            if (num_specifier != (num_int + num_str))
            {
              Error(33);
              skip(statement, 23);
              return;
            }
            num_specifier++;
          }
          else if (token->value[i] == 'd')
          {
            if (num_specifier != (num_int + num_str + 1))
            {
              Error(33);
              skip(statement, 23);
              return;
            }
            num_int++;
            matching[num_id] = symINT;
            num_id++;
          }
          else if (token->value[i] == 's')
          {
            if (num_specifier != (num_int + num_str + 1))
            {
              Error(33);
              skip(statement, 23);
              return;
            }
            num_str++;
            matching[num_id] = symSTRING;
            num_id++;            
          }
        }
        token = nextToken();       
      }
      else
      {
        Error(33);
        skip(statement, 23);
        return;
      }

      num_id = 0;
      
      while (token->sym == symCOMMA)
      {
        token = nextToken();
        // �P�_�ѧO�r���ݩʬO�_�������T
        if (token->sym == symIDENTIFIER)
        {
          if(Check_ID_Exist()){
            sprintf(id, "%s_%s",
                    idobj->procname, token->value);
            if(idobj->attr == symINT){
              sprintf(buf, "\titostr\t%s, _intstr, '$'\n"
                           "\tMOV\tDX, _intstr\n"
                           "\tMOV\tAH, 09H\n"
                           "\tINT\t21H\n"
                           "\tnewline\n",
                      id);
              fprintf(outfile, buf);
            }
            else{
              sprintf(buf, "\tdispstr\t%s\n", id);
              fprintf(outfile, buf);
            }
          }
          
          Identifier();
        }

        if (num_id >= num_specifier)
        {
          Error(33);
          skip(statement, 23);
          return;
        }

        if(idobj->attr != matching[num_id]){
          Error(32);
          skip(statement , 23);
          return;
        }
        else{
          num_id++;
        }
      } 

      if(num_id != num_specifier){
        Error(33);
        skip(statement , 23);
        return;
      }

      if (token->sym == symRPAREN){
        token = nextToken();
      }       
      else
      {
        Error(18);
        skip(statement, 23);
        return;
      }     
    }
    else
    {
      Error(17);
      skip(statement, 23);
      return;
    }
  }
  else
  {
    Error(19);
    skip(statement, 23);
    return;
  }
}
/*
** �y�k�W�h#15 <IdentifierList>
*/
void IdentifierList()
{
  Identifier();
  while (token->sym == symCOMMA)
  {
    token = nextToken();
    Identifier();
  }
}
/*
** �y�k�W�h#16 <Condition>
*/
void Condition()
{
  Expression();

  if (token->sym == symLESS ||
      token->sym == symLEQ ||
      token->sym == symEQ ||
      token->sym == symNEQ ||
      token->sym == symGREATER ||
      token->sym == symGEQ)
  {
    int operator= token->sym;
    token = nextToken();
    Expression();
    sprintf(buf, "\tPOP\tBX\n"
                 "\tPOP\tAX\n"
                 "\tCMP\tAX, BX\n");
    fprintf(outfile, buf);
    switch (operator)
    {
    case symEQ:
      sprintf(buf, "\tJE\t_go%d\n", (++labelCount));
      fprintf(outfile, buf);
      break;
    case symNEQ:
      sprintf(buf, "\tJNE\t_go%d\n", (++labelCount));
      fprintf(outfile, buf);
      break;
    case symLESS:
      sprintf(buf, "\tJL\t_go%d\n", (++labelCount));
      fprintf(outfile, buf);
      break;
    case symLEQ:
      sprintf(buf, "\tJLE\t_go%d\n", (++labelCount));
      fprintf(outfile, buf);
      break;
    case symGREATER:
      sprintf(buf, "\tJG\t_go%d\n", (++labelCount));
      fprintf(outfile, buf);
      break;
    case symGEQ:
      sprintf(buf, "\tJGE\t_go%d\n", (++labelCount));
      fprintf(outfile, buf);
      break;
    }
  }
  else
  {
    Error(20);
    skip(condition, 23);
  }
}
/*
** �y�k�W�h#17 <Expression>
*/
void Expression()
{
  if (token->sym == symPLUS ||
      token->sym == symMINUS)
  {
    token = nextToken();
  }
  Term();
  while (token->sym == symPLUS ||
         token->sym == symMINUS)
  {
    int operator= token->sym;
    token = nextToken();
    Term();
    if (operator== symPLUS)
    {
      fprintf(outfile, "\tPOP\tBX\n"
                       "\tPOP\tAX\n"
                       "\tADD\tAX, BX\n"
                       "\tPUSH\tAX\n");
    }
    else
    {
      fprintf(outfile, "\tPOP\tBX\n"
                       "\tPOP\tAX\n"
                       "\tSUB\tAX, BX\n"
                       "\tPUSH\tAX\n");
    }
  }
}
/*
** �y�k�W�h#18 <Term>
*/
void Term()
{
  Factor();
  while (token->sym == symMUL ||
         token->sym == symDIV)
  {
    int operator= token->sym;
    token = nextToken();
    Factor();
    if (operator== symMUL)
    {
      fprintf(outfile, "\tPOP\tBX\n"
                       "\tPOP\tAX\n"
                       "\tMUL\tBX\n"
                       "\tPUSH\tAX\n");
    }
    else
    {
      fprintf(outfile, "\tPOP\tBX\n"
                       "\tMOV\tDX, 0\n"
                       "\tPOP\tAX\n"
                       "\tDIV\tBX\n"
                       "\tPUSH\tAX\n");
    }
  }
}
/*
** �y�k�W�h#19 <Factor>
*/
void Factor()
{
  if (token->sym == symIDENTIFIER)
  {
    idobj = getIdobj(procStack[procTop - 1], token->value);
    if (idobj != NULL)
    {
      sprintf(id, "%s_%s", idobj->procname, token->value);
      sprintf(buf, "\tPUSH\tWORD [%s]\n", id);
      fprintf(outfile, buf);
    }
    Identifier();
  }
  else if (token->sym == symNUMBER)
  {
    sprintf(buf, "\tPUSH\t%s\n", token->value);
    fprintf(outfile, buf);
    Number();
  }
  else if (token->sym == symLPAREN)
  {
    token = nextToken();
    Expression();
    if (token->sym == symRPAREN)
      token = nextToken();
    else
    {
      Error(18);
      skip(factor_start, 23);
    }
  }
}
/*
** �ѧO�r�ŰO�B�z
*/
void Identifier()
{
  if (token->sym == symIDENTIFIER)
  {
    idobj = getIdobj(procStack[procTop - 1], token->value);
    if (idobj == NULL || idobj->level > level)
    {
      Error(26);
    }

    token = nextToken();
  }
  else
    Error(21);
}
/*
** �Ʀr�ŰO�B�z
*/
void Number()
{
  if (token->sym == symNUMBER)
    token = nextToken();
  else
    Error(22);
}
/*
****************************** �D�{�� **********************
*/
int main(int argc, char *argv[])
{
  FILE *f = fopen(argv[1], "r");
  scanner(f);
  followsyminit();
  startsyminit();
  token = nextToken();
  Program();
  fprintf(outfile, "\tMOV\tAX, 4C00H\n"
                   "\tINT\t21H\n");
  printf("\n  Plone compile completed. "
         "\n  Error count : %d\n",
         errorCount);
  if (argc == 3)
  {
    printf("\n�{�ǵ��c���|���e�p�U�G\n%s\n",
           procobjToString());
  }
  fclose(outfile);
  fclose(f);
  if (errorCount == 0)
  {
    FILE *batchfile;
    sprintf(buf, "%s.bat", progname);
    batchfile = fopen(buf, "w");
    fprintf(batchfile, "nasmw %s.asm -o %s.com\n",
            progname, progname);
    fprintf(batchfile, "%s.com\n", progname);
    fclose(batchfile);
  }
  return 0;
}