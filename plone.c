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
bool Identifier();
void Number();
void Factor();
void Term();
void Expression();
void Condition(bool is_do_while, int label);
void WriteStatement();
void ReadStatement();
void DO_WHILE_Statement();
void WhileStatement();
void IfStatement();
void CompoundStatement();
void CallStatement();
void AssignmentStatement();
void Statement();
void FuncDeclaration();
void VarDeclaration();
void Program();

bool Check_ID_Exist();
bool Check_Func_Exist();
bool expected_value(char *expect, char *current, int error, char *skip_name, int skip_err_num);
bool expected_sym(int expect, int current, int error, char *skip_name);
bool ID_IDtypecheck(struct idobjTag *id, char *skip_name, int err_num);
bool ID_EXPRtypecheck(int attr, char *skip_name, int err_num);
void Program_Start(char *file);

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

/*
** Error()
*/
void Error(int n)
{
  int j;
  if(n == 39){
    printf("^%d  %s\n", n, errmsgs[n]);
    return;
  }
  printf("****");
  
  for (j = 0; j <= token->right; j++)
    printf(" ");
  printf("^%d  %s\n", n, errmsgs[n]);
  if(n != 23){
    errorCount++;
  }
  
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

bool Check_Func_Exist(){
  procobj = findProcobj(token->value);
  if (procobj == NULL)
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

bool expected_sym(int expect, int current, int error, char *skip_name)
{
  if (expect == current)
  {
    token = nextToken();
    return true;
  }
  
  Error(error);
  skip(skip_name, 23);
  return false;
  
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

void Program_Start(char* file){  
  char filename[IDLEN];
  //filename�̫��Ӧr�O.c
  int name_len = strlen(file) - 2;

  struct procobjTag *p;
  strncpy(filename, file , name_len);
  //�̫�@�Ӧr�O. =>���������Ÿ� \0
  filename[name_len] = '\0';

  p = newProcobj(filename);
  procpush(p);
  procStack[procTop++] = p;

  strcpy(progname, filename);
  strcpy(outname, filename);
  strcat(filename, ".asm");
  outfile = fopen(filename, "w");
  ++labelCount;
  sprintf(buf,
          ";************** %s ****************\n"
          ";\n"
          "\tORG\t100H\n"
          "\tJMP\t_start%d\n"
          "_intstr\tDB\t'     ','$'\n"
          "_buf\tTIMES 256 DB ' '\n"
          "\tDB 13,10,'$'\n",
          filename, labelCount);
  fprintf(outfile, buf);
  strcpy(buf, "%include\t\"dispstr.mac\"\n");
  strcat(buf, "%include\t\"itostr.mac\"\n");
  strcat(buf, "%include\t\"readstr.mac\"\n");
  strcat(buf, "%include\t\"strtoi.mac\"\n");
  strcat(buf, "%include\t\"newline.mac\"\n");
  fputs(buf, outfile);
}

void MainFunction(){
  if(!expected_sym(symMAIN , token->sym , 39 , statement)){
    return;
  }

  struct procobjTag *p;
  p = newProcobj("main");
  procpush(p);
  procStack[procTop++] = p;

  sprintf(buf, "\tJMP\t_main\n"
               "_main:\n");
  fprintf(outfile, buf);
  
  if (!expected_sym(symLPAREN , token->sym , 17 , statement)){
    return;
  }
  if (!expected_sym(symRPAREN, token->sym, 18, statement))
  {
    return;
  }
    
  CompoundStatement(); 
}
/*
** �y�k�W�h#1 <Program>
*/
void GlbVarDeclaration(){
  while(token->sym == symINT || token->sym == symSTRINGDeclare){
    VarDeclaration();    
  }  
}
//Rule 1
void Program()
{
  GlbVarDeclaration();  
  bool No_main = true;
  sprintf(buf, "_start%d:\n", labelCount);
  fprintf(outfile, buf);

  while(strcmp(token->value, "void") == 0)
  {
    token = nextToken();    
    if(token->sym == symIDENTIFIER){
      // printf("�ifunction\n");
      FuncDeclaration();
    }
    else if(token->sym == symMAIN){
      No_main = false;
      // printf("�imain\n");
      MainFunction();
      break;
    }        
    else{
      Error(38);
      skip(function , 23);
      return;
    }
    
    // printf("#procedure : %d\n", procTop);

    if(token == NULL){      
      if(No_main){
        Error(39);
      }
      break;
    }
  }  
}


void create_int_id()
{
  idobj = newIdobj(token->value, token->sym,
                   symINT, level, procStack[procTop - 1]->name);

  varlistadd(procobjTop, idobj);
  sprintf(buf, "%s_%s\tDW\t0\n",
          procStack[procTop - 1]->name, token->value);
  fprintf(outfile, buf);
}
//Rule 4
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
          // �p�GExpression��()�A�̭��S��id���� �ڧR��typecheck�F...
          
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

void Const_String(int limit_len)
{    
  if(strlen(token->value) > limit_len){
    Error(41);
    skip(statement , 23);
    return;
  }
  
  if (token->sym == symSTRING)
  {
    sprintf(buf, "'%s','$'\n", token->value);
    fprintf(outfile, buf);
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
  sprintf(buf, "%s_%s\tDW\t",
          procStack[procTop - 1]->name, token->value);
  fprintf(outfile, buf);
  token = nextToken();
}

int string_to_int(){
  char temp[IDLEN];
  strcpy(temp , token->value);
  int ans = 0 , index = 0;

  while(index < strlen(temp)){
    ans *= 10;
    ans += temp[index] - '0';
    index++;
  }
  return ans;
}
//Rule 5
void StringDeclaration()
{
  int length = 0;
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
          length = string_to_int();          
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
        if (idobj == NULL)
        {
          Error(26);
          skip(statement, 23);
          return;
        }        

        token = nextToken();        
        Const_String(length);
      }
      else
      {
        sprintf(buf, "\n");
        fprintf(outfile, buf);
        token = goahead;
      }
      
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
              length = string_to_int(token->value);
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
            Error(31);
            skip(statement, 23);
            return;
          }

          goahead = nextToken();
          if (strcmp(goahead->value, "=") == 0) // ��assign
          {            
            char result[BUFSIZE];
            if (idobj == NULL)
            {
              Error(26);
              skip(statement, 23);
              return;
            }
            
            token = nextToken();
            Const_String(length);            
          }
          else
          {
            sprintf(buf, "\n");
            fprintf(outfile, buf);
            token = goahead;
          }
        }
        else
        {
          Error(26);
          skip(statement, 23);
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
    skip(statement, 23);
    return;
  }
}
//Rule 3
void VarDeclaration()
{
  if (token->sym == symINT)
  {
    // printf("�iint\n");
    INTDeclaration();
  }
  else if (token->sym == symSTRINGDeclare)
  {
    // printf("�istring\n");
    StringDeclaration();
  }
  else{
    Error(40);
    return;
  }
  
  if(token->sym != symSEMI){
    Error(6);
    skip(statement , 23);
  }
  token = nextToken();
}
/*
** �y�k�W�h#6 <ProcDeclaration>
*/
void create_func(){
  struct procobjTag *p;
  strcpy(procname, token->value);
  p = newProcobj(procname);
  varlistadd(
      procStack[procTop - 1],
      newIdobj(procname, token->sym, symPROCEDURE,
               level, procStack[procTop - 1]->name));
  procpush(p);
  procStack[procTop++] = p;
}

void FuncDeclaration()
{
  int tail = 0;   
    
  if (token->sym == symIDENTIFIER)
  {
    create_func();    
    tail = ++labelCount;
 
    sprintf(buf, "\tJMP\t_go%d\n%s:\n",                 
            tail, token->value);
    fprintf(outfile, buf);

    token = nextToken();

    if(!expected_sym(symLPAREN , token->sym , 17 , statement)){
      return;
    }
    if (!expected_sym(symRPAREN, token->sym, 18, statement))
    {
      return;
      
    }
    CompoundStatement();    
  }
  else{
    Error(38);
    skip(statement , 23);
    return;
  }

  sprintf(buf, "\tRET\n"
               "_go%d:\n", tail);
  fprintf(outfile, buf);
}

//Rule 6
void Statement()
{
  if (isResword(token->value) != -1)
  {
    if (token->sym == symIF)
    {
      // printf("�iif\n");
      IfStatement();
    }
    else if (token->sym == symLBRACE)
    {
      // printf("�icompound\n");
      CompoundStatement();
    }
    else if (token->sym == symWHILE)
    {
      // printf("�iwhile\n");
      WhileStatement();
    }
    else if(token->sym == symDO){
      // printf("�ido_while\n");
      DO_WHILE_Statement();
      if (token->sym != symSEMI)
      {
        Error(6);
        skip(statement, 23);
      }
      token = nextToken();
    }
    else if (token->sym == symREAD)
    {
      // printf("�iscanf\n");
      ReadStatement();
      if (token->sym != symSEMI)
      {
        Error(6);
        skip(statement, 23);
      }
      token = nextToken();
    }
    else if (token->sym == symWRITE)
    {
      // printf("�iprintf\n");
      WriteStatement();
      if (token->sym != symSEMI)
      {
        Error(6);
        skip(statement, 23);
      }
      token = nextToken();
    }    
  }
  else if (token->sym == symIDENTIFIER)
  {
    if(Check_Func_Exist()){
      // printf("����function\n");
      CallStatement();
    }
    else{
      // printf("�iassign\n");
      AssignmentStatement();
      
    }
    if (token->sym != symSEMI){
      Error(6);
      skip(statement, 23);
    }
    token = nextToken();
    
  }
  else if (token->sym == symINT || token->sym == symSTRINGDeclare)
  {
    // printf("�ivar\n");
    VarDeclaration();   
  }
  else if (token->sym == symRBRACE)
  {
    Error(27);
    token = nextToken();
  }
  else
  {
    // �o�̦n�������?
    Error(35);
    skip(statement, 23);
  }
}
//Rule 7
void AssignmentStatement()
{
  char result[BUFSIZE];
  idobj = getIdobj(procStack[procTop - 1], token->value);

  if (idobj != NULL)
  {
    sprintf(result, "%s_%s",
            idobj->procname, token->value);
  }

  if(Identifier()){
    if (token->sym == symBECOMES)
    {
      token = nextToken();    
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
}
//Rule 8
void CallStatement()
{
  sprintf(buf, "\tCALL\t%s\n", token->value);
  fprintf(outfile, buf);
  token = nextToken();
  
  if(!expected_sym(symLPAREN , token->sym , 17 , statement)){
    return;
  }
  if(!expected_sym(symRPAREN , token->sym , 18 , statement)){
    return;
  }   
}
// Rule 2
void CompoundStatement()
{
  level++;
  if (strcmp(token->value, "{") == 0)
  {
    token = nextToken();
    while (token->sym == symINT || token->sym == symSTRINGDeclare)
    {
      // printf("�ivar\n");
      VarDeclaration();      
    }

    while (statement_start[token->sym])
    {
      // printf("�istatement\n");
      Statement();
    }

    if (strcmp(token->value, "}") != 0)
    {
      Error(28);
      skip(compound, 23);
    }
    else
    {
      token = nextToken();
    }
  }
  else
  {
    Error(27);
    skip(compound, 23);
  }
  level--;
}
// Rule 9
void IfStatement() 
{
  int head, tail, ending;
  if (strcmp(token->value, "if") == 0)
  {
    token = nextToken();
    if (strcmp(token->value, "(") == 0)
    {
      token = nextToken();
      Condition(false, 0);
      
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
      }
    }
    else
    {
      Error(17);
      skip(statement, 23);      
    }
  }
  else
  {
    Error(12);
    skip(statement, 23);    
  }  
}
// Rule 10
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

      Condition(false, 0);

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
// Rule 11
void DO_WHILE_Statement()
{
  int head, tail;

  if (strcmp(token->value, "do") == 0)
  {
    head = ++labelCount;
    sprintf(buf, "_go%d:\n", head);
    fprintf(outfile, buf);

    token = nextToken();
    Statement();
    

    if (token->sym == symWHILE)
    {
      token = nextToken();
      if (strcmp(token->value, "(") == 0)
      {
        token = nextToken();

        Condition(true, head);

        
        tail = ++labelCount;     

        if (strcmp(token->value, ")") == 0)
        {
          token = nextToken();          
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
  else
  {
    Error(15);
    skip(statement, 23);
  }
}
// Rule 12
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
              Error(36);
              skip(statement, 23);
              return;
            }
            num_int++;
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

      while (token->sym == symCOMMA)
      {
        token = nextToken();
        
        if (token->sym == symGetAddress)
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
                           "\tnewline\n",
                      id);
              fprintf(outfile, buf);
              num_id++;
            }
            else
            {
              Error(26);
              skip(statement, 23);
              return;
            }
            Identifier();
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
          Error(37);
          skip(statement, 23);
          return;
        }
      }

      if (num_id != num_int)
      {
        Error(36);
        skip(statement, 23);
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

// Rule 13
void WriteStatement()
{
  if (strcmp(token->value, "printf") == 0)
  {
    token = nextToken();
    int num_int = 0, num_str = 0, num_specifier = 0, num_id = 0;
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
        
        if (token->sym == symIDENTIFIER)
        {
          if (Check_ID_Exist())
          {
            sprintf(id, "%s_%s",
                    idobj->procname, token->value);
            if (idobj->attr == symINT)
            {
              sprintf(buf, "\titostr\t%s, _intstr, '$'\n"
                           "\tMOV\tDX, _intstr\n"
                           "\tMOV\tAH, 09H\n"
                           "\tINT\t21H\n"
                           "\tnewline\n",
                      id);
              fprintf(outfile, buf);
            }
            else
            {
              sprintf(buf, "\tdispstr\t%s\n", id);
              fprintf(outfile, buf);
            }
          }

          if(!Identifier()){
            return;
          }
        }

        if (num_id >= num_specifier)
        {
          Error(33);
          skip(statement, 23);
          return;
        }

        if (idobj->attr != matching[num_id])
        {
          Error(32);
          skip(statement, 23);
          return;
        }
        else
        {
          num_id++;
        }
      }

      if (num_id != num_specifier)
      {
        Error(33);
        skip(statement, 23);
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

// Rule 14
void Condition(bool is_do_while, int label)
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
    if (is_do_while)
    {
      switch (operator)
      {
      case symEQ:
        sprintf(buf, "\tJE\t_go%d\n", (label));
        fprintf(outfile, buf);
        break;
      case symNEQ:
        sprintf(buf, "\tJNE\t_go%d\n", (label));
        fprintf(outfile, buf);
        break;
      case symLESS:
        sprintf(buf, "\tJL\t_go%d\n", (label));
        fprintf(outfile, buf);
        break;
      case symLEQ:
        sprintf(buf, "\tJLE\t_go%d\n", (label));
        fprintf(outfile, buf);
        break;
      case symGREATER:
        sprintf(buf, "\tJG\t_go%d\n", (label));
        fprintf(outfile, buf);
        break;
      case symGEQ:
        sprintf(buf, "\tJGE\t_go%d\n", (label));
        fprintf(outfile, buf);
        break;
      }
    }
    else
    {
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
  else{
    Error(42);
    skip(statement , 23);
  }
}
/*
** �ѧO�r�ŰO�B�z
*/
bool Identifier()
{
  if (token->sym == symIDENTIFIER)
  {
    idobj = getIdobj(procStack[procTop - 1], token->value);
    if (idobj == NULL || idobj->level > level)
    {
      Error(26);
      skip(statement , 23);
      return false;
    }

    token = nextToken();
  }
  else{
    Error(21);
    skip(statement , 23);
    return false;
  }
  return true; 
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
  Program_Start(argv[1]);
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
  // if (errorCount == 0)
  // {
  //   FILE *batchfile;
  //   sprintf(buf, "%s.bat", progname);
  //   batchfile = fopen(buf, "w");
  //   fprintf(batchfile, "nasmw %s.asm -o %s.com\n",
  //           progname, progname);
  //   fprintf(batchfile, "%s.com\n", progname);
  //   fclose(batchfile);
  // }
  return 0;
}