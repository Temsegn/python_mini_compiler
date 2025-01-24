/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "parser.y"

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdarg.h>

	#define MAXRECST 200
	#define MAXST 100
	#define MAXCHILDREN 100
	#define MAXLEVELS 20
	#define MAXQUADS 1000
	
	extern int yylineno;
	extern int depth;
	extern int top();
	extern int pop();
        extern FILE* yyin;
	int currentScope = 1, previousScope = 1;
	
	int *arrayScope = NULL;
	
	typedef struct record
	{
		char *type;
		char *name;
		int decLineNo;
		int lastUseLine;
	} record;

	typedef struct STable
	{
		int no;
		int noOfElems;
		int scope;
		record *Elements;
		int Parent;
		
	} STable;
	
	typedef struct ASTNode
	{
		int nodeNo;
    /*Operator*/
    char *NType;
    int noOps;
    struct ASTNode** NextLevel;
    
    /*Identifier or Const*/
    record *id;
	
	} node;
  
	typedef struct Quad
	{
		char *R;
		char *A1;
		char *A2;
		char *Op;
		int I;
	} Quad;
	
	STable *symbolTables = NULL;
	int sIndex = -1, aIndex = -1, tabCount = 0, tIndex = 0 , lIndex = 0, qIndex = 0, nodeCount = 0;
	node *rootNode;
	char *argsList = NULL;
	char *tString = NULL, *lString = NULL;
	Quad *allQ = NULL;
	node ***Tree = NULL;
	int *levelIndices = NULL;
	
	/*-----------------------------Declarations----------------------------------*/
	
	record* findRecord(const char *name, const char *type, int scope);
  node *createID_Const(char *value, char *type, int scope);
  int power(int base, int exp);
  void updateCScope(int scope);
  void resetDepth();
	int scopeBasedTableSearch(int scope);
	void initNewTable(int scope);
	void init();
	int searchRecordInScope(const char* type, const char *name, int index);
	void insertRecord(const char* type, const char *name, int lineNo, int scope);
	int searchRecordInScope(const char* type, const char *name, int index);
	void checkList(const char *name, int lineNo, int scope);
	void printSTable();
	void freeAll();
	void addToList(char *newVal, int flag);
	void clearArgsList();
	int checkIfBinOperator(char *Op);
	/*------------------------------------------------------------------------------*/
	
	void Xitoa(int num, char *str)
	{
		if(str == NULL)
		{
			 printf("Allocate Memory\n");
		   return;
		}
		sprintf(str, "%d", num);
	}

	
	char *makeStr(int no, int flag)
	{
		char A[10];
		Xitoa(no, A);
		
		if(flag==1)
		{
				strcpy(tString, "T");
				strcat(tString, A);
				insertRecord("ICGTempVar", tString, -1, 0);
				return tString;
		}
		else
		{
				strcpy(lString, "L");
				strcat(lString, A);
				insertRecord("ICGTempLabel", lString, -1, 0);
				return lString;
		}

	}
	
	void makeQ(char *R, char *A1, char *A2, char *Op)
	{
		
		allQ[qIndex].R = (char*)malloc(strlen(R)+1);
		allQ[qIndex].Op = (char*)malloc(strlen(Op)+1);
		allQ[qIndex].A1 = (char*)malloc(strlen(A1)+1);
		allQ[qIndex].A2 = (char*)malloc(strlen(A2)+1);
		
		strcpy(allQ[qIndex].R, R);
		strcpy(allQ[qIndex].A1, A1);
		strcpy(allQ[qIndex].A2, A2);
		strcpy(allQ[qIndex].Op, Op);
		allQ[qIndex].I = qIndex;
 
		qIndex++;
		
		return;
	}
	
	int checkIfBinOperator(char *Op)
	{
		if((!strcmp(Op, "+")) || (!strcmp(Op, "*")) || (!strcmp(Op, "/")) || (!strcmp(Op, ">=")) || (!strcmp(Op, "<=")) || (!strcmp(Op, "<")) || (!strcmp(Op, ">")) || 
			 (!strcmp(Op, "in")) || (!strcmp(Op, "==")) || (!strcmp(Op, "and")) || (!strcmp(Op, "or")))
			{
				return 1;
			}
			
			else 
			{
				return 0;
			}
	}
	
	void codeGenOp(node *opNode)
	{
		if(opNode == NULL)
		{
			return;
		}
		
		if(opNode->NType == NULL)
		{
			if((!strcmp(opNode->id->type, "Identifier")) || (!strcmp(opNode->id->type, "Constant")))
			{
				printf("T%d = %s\n", opNode->nodeNo, opNode->id->name);
				makeQ(makeStr(opNode->nodeNo, 1), opNode->id->name, "-", "=");
			}
			return;
		}
		
		if((!strcmp(opNode->NType, "If")) || (!strcmp(opNode->NType, "Elif")))
		{			
			switch(opNode->noOps)
			{
				case 2 : 
				{
					int temp = lIndex;
					codeGenOp(opNode->NextLevel[0]);
					printf("If False T%d goto L%d\n", opNode->NextLevel[0]->nodeNo, lIndex);
					makeQ(makeStr(temp, 0), makeStr(opNode->NextLevel[0]->nodeNo, 1), "-", "If False");
					lIndex++;
					codeGenOp(opNode->NextLevel[1]);
					lIndex--;
					printf("L%d: ", temp);
					makeQ(makeStr(temp, 0), "-", "-", "Label");
					break;
				}
				case 3 : 
				{
					int temp = lIndex;
					codeGenOp(opNode->NextLevel[0]);
					printf("If False T%d goto L%d\n", opNode->NextLevel[0]->nodeNo, lIndex);
					makeQ(makeStr(temp, 0), makeStr(opNode->NextLevel[0]->nodeNo, 1), "-", "If False");					
					codeGenOp(opNode->NextLevel[1]);
					printf("goto L%d\n", temp+1);
					makeQ(makeStr(temp+1, 0), "-", "-", "goto");
					printf("L%d: ", temp);
					makeQ(makeStr(temp, 0), "-", "-", "Label");
					codeGenOp(opNode->NextLevel[2]);
					printf("L%d: ", temp+1);
					makeQ(makeStr(temp+1, 0), "-", "-", "Label");
					lIndex+=2;
					break;
				}
			}
			return;
		}
		
		if(!strcmp(opNode->NType, "Else"))
		{
			codeGenOp(opNode->NextLevel[0]);
			return;
		}
		
		if(!strcmp(opNode->NType, "While"))
		{
			int temp = lIndex;
			codeGenOp(opNode->NextLevel[0]);
			printf("L%d: If False T%d goto L%d\n", lIndex, opNode->NextLevel[0]->nodeNo, lIndex+1);
			makeQ(makeStr(temp, 0), "-", "-", "Label");		
			makeQ(makeStr(temp+1, 0), makeStr(opNode->NextLevel[0]->nodeNo, 1), "-", "If False");								
			lIndex+=2;			
			codeGenOp(opNode->NextLevel[1]);
			printf("goto L%d\n", temp);
			makeQ(makeStr(temp, 0), "-", "-", "goto");
			printf("L%d: ", temp+1);
			makeQ(makeStr(temp+1, 0), "-", "-", "Label"); 
			lIndex = lIndex+2;
			return;
		}
		if(!strcmp(opNode->NType, "For"))
		{
			int temp = lIndex;
			codeGenOp(opNode->NextLevel[0]);
			//codeGenOp(opNode->NextLevel[1]);
			printf("L%d: If False T%d goto L%d\n", lIndex, opNode->NextLevel[0]->nodeNo, lIndex+1);
			makeQ(makeStr(temp, 0), "-", "-", "Label");
			makeQ(makeStr(temp+1, 0), makeStr(opNode->NextLevel[0]->nodeNo, 1), "-", "If False");
			lIndex+=2;
			codeGenOp(opNode->NextLevel[1]);
			printf("goto L%d\n", temp);
			makeQ(makeStr(temp, 0), "-", "-", "goto");
			printf("L%d: ", temp+1);
			makeQ(makeStr(temp+1, 0), "-", "-", "Label"); 
			lIndex = lIndex+2;
			return;
		}
		if(!strcmp(opNode->NType, "Next"))
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);
			return;
		}
		
		if(!strcmp(opNode->NType, "BeginBlock"))
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);		
			return;	
		}
		
		if(!strcmp(opNode->NType, "EndBlock"))
		{
			switch(opNode->noOps)
			{
				case 0 : 
				{
					break;
				}
				case 1 : 
				{
					codeGenOp(opNode->NextLevel[0]);
					break;
				}
			}
			return;
		}
		
		if(!strcmp(opNode->NType, "ListIndex"))
		{
			printf("T%d = %s[%s]\n", opNode->nodeNo, opNode->NextLevel[0]->id->name, opNode->NextLevel[1]->id->name);
			makeQ(makeStr(opNode->nodeNo, 1), opNode->NextLevel[0]->id->name, opNode->NextLevel[1]->id->name, "=[]");
			return;
		}
		
		if(checkIfBinOperator(opNode->NType)==1)
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);
			char *X1 = (char*)malloc(10);
			char *X2 = (char*)malloc(10);
			char *X3 = (char*)malloc(10);
			
			strcpy(X1, makeStr(opNode->nodeNo, 1));
			strcpy(X2, makeStr(opNode->NextLevel[0]->nodeNo, 1));
			strcpy(X3, makeStr(opNode->NextLevel[1]->nodeNo, 1));

			printf("T%d = T%d %s T%d\n", opNode->nodeNo, opNode->NextLevel[0]->nodeNo, opNode->NType, opNode->NextLevel[1]->nodeNo);
			makeQ(X1, X2, X3, opNode->NType);
			free(X1);
			free(X2);
			free(X3);
			return;
		}
		
		if(!strcmp(opNode->NType, "-"))
		{
			if(opNode->noOps == 1)
			{
				codeGenOp(opNode->NextLevel[0]);
				char *X1 = (char*)malloc(10);
				char *X2 = (char*)malloc(10);
				strcpy(X1, makeStr(opNode->nodeNo, 1));
				strcpy(X2, makeStr(opNode->NextLevel[0]->nodeNo, 1));
				printf("T%d = %s T%d\n", opNode->nodeNo, opNode->NType, opNode->NextLevel[0]->nodeNo);
				makeQ(X1, X2, "-", opNode->NType);	
			}
			
			else
			{
				codeGenOp(opNode->NextLevel[0]);
				codeGenOp(opNode->NextLevel[1]);
				char *X1 = (char*)malloc(10);
				char *X2 = (char*)malloc(10);
				char *X3 = (char*)malloc(10);
			
				strcpy(X1, makeStr(opNode->nodeNo, 1));
				strcpy(X2, makeStr(opNode->NextLevel[0]->nodeNo, 1));
				strcpy(X3, makeStr(opNode->NextLevel[1]->nodeNo, 1));

				printf("T%d = T%d %s T%d\n", opNode->nodeNo, opNode->NextLevel[0]->nodeNo, opNode->NType, opNode->NextLevel[1]->nodeNo);
				makeQ(X1, X2, X3, opNode->NType);
				free(X1);
				free(X2);
				free(X3);
				return;
			
			}
		}
		
		if(!strcmp(opNode->NType, "import"))
		{
			printf("import %s\n", opNode->NextLevel[0]->id->name);
			makeQ("-", opNode->NextLevel[0]->id->name, "-", "import");
			return;
		}
		
		if(!strcmp(opNode->NType, "NewLine"))
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);
			return;
		}
		
		if(!strcmp(opNode->NType, "Range"))
		{
			codeGenOp(opNode->NextLevel[0]);
			codeGenOp(opNode->NextLevel[1]);
			char *X1 = (char*)malloc(10);
			char *X2 = (char*)malloc(10);
			char *X3 = (char*)malloc(10);
			strcpy(X1, makeStr(opNode->nodeNo, 1));
			strcpy(X2, makeStr(opNode->NextLevel[0]->nodeNo, 1));
			strcpy(X3, makeStr(opNode->NextLevel[1]->nodeNo, 1));
			printf("T%d = Range(T%d,T%d)\n",opNode->nodeNo, opNode->NextLevel[0]->nodeNo,opNode->NextLevel[1]->nodeNo);
			makeQ(X1,X2,X3,"Range");
			return;
		}
		
		if(!strcmp(opNode->NType, "="))
		{
			codeGenOp(opNode->NextLevel[1]);
			printf("%s = T%d\n", opNode->NextLevel[0]->id->name, opNode->NextLevel[1]->nodeNo);
			makeQ(opNode->NextLevel[0]->id->name, makeStr(opNode->NextLevel[1]->nodeNo, 1), "-", opNode->NType);
			return;
		}
		
		if(!strcmp(opNode->NType, "Func_Name"))
		{
			printf("Begin Function %s\n", opNode->NextLevel[0]->id->name);
			makeQ("-", opNode->NextLevel[0]->id->name, "-", "BeginF");
			codeGenOp(opNode->NextLevel[2]);
			printf("End Function %s\n", opNode->NextLevel[0]->id->name);
			makeQ("-", opNode->NextLevel[0]->id->name, "-", "EndF");
			return;
		}
		
		if(!strcmp(opNode->NType, "Func_Call"))
		{
			if(!strcmp(opNode->NextLevel[1]->NType, "Void"))
			{
				printf("(T%d)Call Function %s\n", opNode->nodeNo, opNode->NextLevel[0]->id->name);
				makeQ(makeStr(opNode->nodeNo, 1), opNode->NextLevel[0]->id->name, "-", "Call");
			}
			else
			{
				char A[10];
				char* token = strtok(opNode->NextLevel[1]->NType, ","); 
  			int i = 0;
				while (token != NULL) 
				{
						i++; 
				    printf("Push Param %s\n", token);
				    makeQ("-", token, "-", "Param"); 
				    token = strtok(NULL, ","); 
				}
				
				printf("(T%d)Call Function %s, %d\n", opNode->nodeNo, opNode->NextLevel[0]->id->name, i);
				sprintf(A, "%d", i);
				makeQ(makeStr(opNode->nodeNo, 1), opNode->NextLevel[0]->id->name, A, "Call");
				printf("Pop Params for Function %s, %d\n", opNode->NextLevel[0]->id->name, i);
								
				return;
			}
		}		
		
		if(!(strcmp(opNode->NType, "Print")))
		{
			codeGenOp(opNode->NextLevel[0]);
			printf("Print T%d\n", opNode->NextLevel[0]->nodeNo);
			makeQ("-", makeStr(opNode->nodeNo, 1), "-", "Print");
		}
		if(opNode->noOps == 0)
		{
			if(!strcmp(opNode->NType, "break"))
			{
				printf("goto L%d\n", lIndex);
				makeQ(makeStr(lIndex, 0), "-", "-", "goto");
			}

			if(!strcmp(opNode->NType, "pass"))
			{
				makeQ("-", "-", "-", "pass");
			}

			if(!strcmp(opNode->NType, "return"))
			{
				printf("return\n");
				makeQ("-", "-", "-", "return");
			}
		}
		
		
	}
	
  node *createID_Const(char *type, char *value, int scope)
  {
    node *newNode;
    newNode = (node*)calloc(1, sizeof(node));
    newNode->NType = NULL;
    newNode->noOps = -1;
    newNode->id = findRecord(value, type, scope);
    newNode->nodeNo = nodeCount++;
    return newNode;
  }

  node *createOp(char *oper, int noOps, ...)
  {
  
    va_list params;
    node *newNode;
    int i;
    newNode = (node*)calloc(1, sizeof(node));
    
    newNode->NextLevel = (node**)calloc(noOps, sizeof(node*));
    
    newNode->NType = (char*)malloc(strlen(oper)+1);
    strcpy(newNode->NType, oper);
    newNode->noOps = noOps;
    va_start(params, noOps);
    
    for (i = 0; i < noOps; i++)
      newNode->NextLevel[i] = va_arg(params, node*);
    
    va_end(params);
    newNode->nodeNo = nodeCount++;
    return newNode;
  }
  
  void addToList(char *newVal, int flag)
  {
  	if(flag==0)
  	{
		  strcat(argsList, ", ");
		  strcat(argsList, newVal);
		}
		else
		{
			strcat(argsList, newVal);
		}
    //printf("\n\t%s\n", newVal);
  }
  
  void clearArgsList()
  {
    strcpy(argsList, "");
  }
  
	int power(int base, int exp)
	{
		int i =0, res = 1;
		for(i; i<exp; i++)
		{
			res *= base;
		}
		return res;
	}
	
	void updateCScope(int scope)
	{
		currentScope = scope;
	}
	
	void resetDepth()
	{
		while(top()) pop();
		depth = 10;
	}
	
	int scopeBasedTableSearch(int scope)
	{
		int i = sIndex;
		for(i; i > -1; i--)
		{
			if(symbolTables[i].scope == scope)
			{
				return i;
			}
		}
		return -1;
	}
	
	void initNewTable(int scope)
	{
		arrayScope[scope]++;
		sIndex++;
		symbolTables[sIndex].no = sIndex;
		symbolTables[sIndex].scope = power(scope, arrayScope[scope]);
		symbolTables[sIndex].noOfElems = 0;		
		symbolTables[sIndex].Elements = (record*)calloc(MAXRECST, sizeof(record));
		
		symbolTables[sIndex].Parent = scopeBasedTableSearch(currentScope); 
	}
	
	void init()
	{
		int i = 0;
		symbolTables = (STable*)calloc(MAXST, sizeof(STable));
		arrayScope = (int*)calloc(10, sizeof(int));
		initNewTable(1);
		argsList = (char *)malloc(100);
		strcpy(argsList, "");
		tString = (char*)calloc(10, sizeof(char));
		lString = (char*)calloc(10, sizeof(char));
		allQ = (Quad*)calloc(MAXQUADS, sizeof(Quad));
		
		levelIndices = (int*)calloc(MAXLEVELS, sizeof(int));
		Tree = (node***)calloc(MAXLEVELS, sizeof(node**));
		for(i = 0; i<MAXLEVELS; i++)
		{
			Tree[i] = (node**)calloc(MAXCHILDREN, sizeof(node*));
		}
	}

	int searchRecordInScope(const char* type, const char *name, int index)
	{
		int i =0;
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if((strcmp(symbolTables[index].Elements[i].type, type)==0) && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				return i;
			}	
		}
		return -1;
	}
		
	void modifyRecordID(const char *type, const char *name, int lineNo, int scope)
	{
		int i =0;
		int index = scopeBasedTableSearch(scope);
		if(index==0)
		{
			for(i=0; i<symbolTables[index].noOfElems; i++)
			{
				
				if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
				{
					symbolTables[index].Elements[i].lastUseLine = lineNo;
					return;
				}	
			}
			printf("%s '%s' at line %d Not Declared\n", type, name, yylineno);
			exit(1);
		}
		
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				symbolTables[index].Elements[i].lastUseLine = lineNo;
				return;
			}	
		}
		return modifyRecordID(type, name, lineNo, symbolTables[symbolTables[index].Parent].scope);
	}
	
	void insertRecord(const char* type, const char *name, int lineNo, int scope)
	{ 
		int FScope = power(scope, arrayScope[scope]);
		int index = scopeBasedTableSearch(FScope);
		int recordIndex = searchRecordInScope(type, name, index);
		if(recordIndex==-1)
		{
			
			symbolTables[index].Elements[symbolTables[index].noOfElems].type = (char*)calloc(30, sizeof(char));
			symbolTables[index].Elements[symbolTables[index].noOfElems].name = (char*)calloc(20, sizeof(char));
		
			strcpy(symbolTables[index].Elements[symbolTables[index].noOfElems].type, type);	
			strcpy(symbolTables[index].Elements[symbolTables[index].noOfElems].name, name);
			symbolTables[index].Elements[symbolTables[index].noOfElems].decLineNo = lineNo;
			symbolTables[index].Elements[symbolTables[index].noOfElems].lastUseLine = lineNo;
			symbolTables[index].noOfElems++;

		}
		else
		{
			symbolTables[index].Elements[recordIndex].lastUseLine = lineNo;
		}
	}
	
	void checkList(const char *name, int lineNo, int scope)
	{
		int index = scopeBasedTableSearch(scope);
		int i;
		if(index==0)
		{
			
			for(i=0; i<symbolTables[index].noOfElems; i++)
			{
				
				if(strcmp(symbolTables[index].Elements[i].type, "ListTypeID")==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
				{
					symbolTables[index].Elements[i].lastUseLine = lineNo;
					return;
				}	

				else if(strcmp(symbolTables[index].Elements[i].name, name)==0)
				{
					printf("Identifier '%s' at line %d Not Indexable\n", name, yylineno);
					exit(1);

				}

			}
			printf("Identifier '%s' at line %d Not Indexable\n", name, yylineno);
			exit(1);
		}
		
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if(strcmp(symbolTables[index].Elements[i].type, "ListTypeID")==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				symbolTables[index].Elements[i].lastUseLine = lineNo;
				return;
			}
			
			else if(strcmp(symbolTables[index].Elements[i].name, name)==0)
			{
				printf("Identifier '%s' at line %d Not Indexable\n", name, yylineno);
				exit(1);

			}
		}
		
		return checkList(name, lineNo, symbolTables[symbolTables[index].Parent].scope);

	}
	
	record* findRecord(const char *name, const char *type, int scope)
	{
		int i =0;
		int index = scopeBasedTableSearch(scope);
		//printf("FR: %d, %s\n", scope, name);
		if(index==0)
		{
			for(i=0; i<symbolTables[index].noOfElems; i++)
			{
				
				if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
				{
					return &(symbolTables[index].Elements[i]);
				}	
			}
			printf("\n%s '%s' at line %d Not Found in Symbol Table\n", type, name, yylineno);
			exit(1);
		}
		
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				return &(symbolTables[index].Elements[i]);
			}	
		}
		return findRecord(name, type, symbolTables[symbolTables[index].Parent].scope);
	}

	void printSTable()
	{
		int i = 0, j = 0;
		
		printf("\n----------------------------All Symbol Tables----------------------------");
		printf("\nScope\tName\tType\t\tDeclaration\tLast Used Line\n");
		for(i=0; i<=sIndex; i++)
		{
			for(j=0; j<symbolTables[i].noOfElems; j++)
			{
				if(!(strcmp(symbolTables[i].Elements[j].type,"Constant")==0))
				{
				printf("(%d)\t%s\t%s\t%d\t\t%d\n", symbolTables[i].scope, symbolTables[i].Elements[j].name, symbolTables[i].Elements[j].type, symbolTables[i].Elements[j].decLineNo,  symbolTables[i].Elements[j].lastUseLine);
			}
			}
		}
		
		printf("-------------------------------------------------------------------------\n");
		
	}
	
	void ASTToArray(node *root, int level)
	{
	  if(root == NULL )
	  {
	    return;
	  }
	  
	  if(root->noOps <= 0)
	  {
	  	Tree[level][levelIndices[level]] = root;
	  	levelIndices[level]++;
	  }
	  
	  if(root->noOps > 0)
	  {
	 		int j;
	 		Tree[level][levelIndices[level]] = root;
	 		levelIndices[level]++; 
	    for(j=0; j<root->noOps; j++)
	    {
	    	ASTToArray(root->NextLevel[j], level+1);
	    }
	  }
	}
	
	void printAST(node *root)
	{
		printf("\n-------------------------Abstract Syntax Tree--------------------------\n");
		ASTToArray(root, 0);
		int j = 0, p, q, maxLevel = 0, lCount = 0;
		
		while(levelIndices[maxLevel] > 0) maxLevel++;
		
		while(levelIndices[j] > 0)
		{
			for(q=0; q<lCount; q++)
			{
				printf(" ");
			
			}
			for(p=0; p<levelIndices[j] ; p++)
			{
				if(Tree[j][p]->noOps == -1)
				{
					printf("%s  ", Tree[j][p]->id->name);
					lCount+=strlen(Tree[j][p]->id->name);
				}
				else if(Tree[j][p]->noOps == 0)
				{
					printf("%s  ", Tree[j][p]->NType);
					lCount+=strlen(Tree[j][p]->NType);
				}
				else
				{
					printf("%s(%d) ", Tree[j][p]->NType, Tree[j][p]->noOps);
				}
			}
			j++;
			printf("\n");
		}
	}
	
	int IsValidNumber(char * string)
	{
   for(int i = 0; i < strlen( string ); i ++)
   {
      //ASCII value of 0 = 48, 9 = 57. So if value is outside of numeric range then fail
      //Checking for negative sign "-" could be added: ASCII value 45.
      if (string[i] < 48 || string[i] > 57)
         return 0;
   }
 
   return 1;
	}
	
	int deadCodeElimination()
	{
		int i = 0, j = 0, flag = 1, XF=0;
		while(flag==1)
		{
			
			flag=0;
			for(i=0; i<qIndex; i++)
			{
				XF=0;
				if(!((strcmp(allQ[i].Op,"Label")==0)|(strcmp(allQ[i].Op,"goto")==0)|(strcmp(allQ[i].Op,"If False")==0)))
				{
					for(j=i+1; j<qIndex; j++)
					{
							if(((strcmp(allQ[i].R, allQ[j].A1)==0) && (allQ[j].I!=-1)) | ((strcmp(allQ[i].R, allQ[j].A2)==0) && (allQ[j].I!=-1)))
							{
								XF=1;
							}
					}
				
					if((XF==0) & (allQ[i].I != -1))
					{
						allQ[i].I = -1;
						flag=1;
					}
				}

			}
		}
		return flag;
	}
	
	void constant_folding()
	{
		int i = 0, j = 0, flag = 1, ans;
		while(flag==1)
		{
			
			flag=0;
			for(i=0; i<qIndex; i++)
			{
				if(isdigit(*allQ[i].A1) && isdigit(*allQ[i].A2))
				{
					
					switch(*allQ[i].Op)
					{
						case '+': ans=(*allQ[i].A1)+(*allQ[i].A2);
						break;
						case '-': ans=(*allQ[i].A1)-(*allQ[i].A2);
						break;
					}
								
							
				}
			}
		}
	}
		
	
	 
	void freeAll()
	{
		deadCodeElimination();
 		printf("\n");
		int i = 0, j = 0;
		for(i=0; i<=sIndex; i++)
		{
			for(j=0; j<symbolTables[i].noOfElems; j++)
			{
				free(symbolTables[i].Elements[j].name);
				free(symbolTables[i].Elements[j].type);	
			}
			free(symbolTables[i].Elements);
		}
		free(symbolTables);
		free(allQ);
	}

/* Line 371 of yacc.c  */
#line 955 "parser.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_EndOfFile = 258,
     T_Return = 259,
     T_Number = 260,
     T_True = 261,
     T_False = 262,
     T_ID = 263,
     T_Print = 264,
     T_Cln = 265,
     T_NL = 266,
     T_EQL = 267,
     T_NEQ = 268,
     T_EQ = 269,
     T_GT = 270,
     T_LT = 271,
     T_EGT = 272,
     T_ELT = 273,
     T_Or = 274,
     T_And = 275,
     T_Not = 276,
     T_In = 277,
     ID = 278,
     ND = 279,
     DD = 280,
     T_String = 281,
     T_If = 282,
     T_Elif = 283,
     T_While = 284,
     T_Else = 285,
     T_Import = 286,
     T_Break = 287,
     T_Pass = 288,
     T_MN = 289,
     T_PL = 290,
     T_DV = 291,
     T_ML = 292,
     T_OP = 293,
     T_CP = 294,
     T_OB = 295,
     T_CB = 296,
     T_Def = 297,
     T_Comma = 298,
     T_List = 299,
     T_For = 300,
     T_Range = 301
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 888 "parser.y"
 char *text; int depth; struct ASTNode* node;

/* Line 387 of yacc.c  */
#line 1047 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;
extern YYLTYPE yylloc;
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 1088 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   245

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  96
/* YYNRULES -- Number of states.  */
#define YYNSTATES  174

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     8,    10,    12,    14,    16,    18,
      23,    26,    27,    32,    35,    37,    39,    41,    43,    45,
      47,    49,    51,    53,    57,    61,    65,    69,    72,    76,
      80,    84,    88,    92,    96,   100,   104,   106,   108,   112,
     114,   116,   119,   123,   126,   128,   130,   132,   136,   140,
     144,   149,   154,   156,   158,   160,   162,   165,   167,   169,
     171,   176,   182,   184,   190,   194,   199,   206,   213,   215,
     216,   222,   227,   230,   231,   235,   236,   239,   240,   241,
     245,   246,   247,   252,   253,   254,   259,   260,   261,   265,
     266,   270,   271,   275,   276,   277,   286
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      48,     0,    -1,    -1,    49,    53,     3,    -1,     5,    -1,
      26,    -1,     8,    -1,    50,    -1,    52,    -1,     8,    40,
      50,    41,    -1,    11,    53,    -1,    -1,    66,    11,    54,
      53,    -1,    66,    11,    -1,    61,    -1,    62,    -1,    60,
      -1,    64,    -1,    56,    -1,    57,    -1,    65,    -1,    63,
      -1,    51,    -1,    56,    35,    56,    -1,    56,    34,    56,
      -1,    56,    37,    56,    -1,    56,    36,    56,    -1,    34,
      56,    -1,    38,    56,    39,    -1,    58,    19,    58,    -1,
      56,    16,    56,    -1,    58,    20,    58,    -1,    56,    15,
      56,    -1,    56,    18,    56,    -1,    56,    17,    56,    -1,
      56,    22,     8,    -1,    58,    -1,    59,    -1,    56,    14,
      56,    -1,     6,    -1,     7,    -1,    21,    59,    -1,    38,
      57,    39,    -1,    31,     8,    -1,    33,    -1,    32,    -1,
       4,    -1,     8,    12,    56,    -1,     8,    12,    57,    -1,
       8,    12,    92,    -1,     8,    12,    40,    41,    -1,     9,
      38,    51,    39,    -1,    55,    -1,    67,    -1,    90,    -1,
      92,    -1,     1,    11,    -1,    68,    -1,    71,    -1,    72,
      -1,    27,    57,    10,    74,    -1,    27,    57,    10,    74,
      69,    -1,    70,    -1,    28,    57,    10,    74,    69,    -1,
      30,    10,    74,    -1,    29,    57,    10,    74,    -1,    45,
       8,    22,    73,    10,    74,    -1,    46,    38,    50,    43,
      50,    39,    -1,    55,    -1,    -1,    11,    23,    75,    66,
      76,    -1,    11,    24,    66,    76,    -1,    11,    77,    -1,
      -1,    25,    78,    66,    -1,    -1,    25,    79,    -1,    -1,
      -1,     8,    81,    82,    -1,    -1,    -1,    43,     8,    83,
      82,    -1,    -1,    -1,    43,    51,    85,    84,    -1,    -1,
      -1,     8,    87,    84,    -1,    -1,     5,    88,    84,    -1,
      -1,    26,    89,    84,    -1,    -1,    -1,    42,     8,    91,
      38,    80,    39,    10,    74,    -1,     8,    38,    86,    39,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   904,   904,   904,   906,   907,   909,   910,   911,   914,
     916,   916,   916,   916,   918,   919,   920,   921,   922,   923,
     924,   925,   927,   928,   929,   930,   931,   932,   933,   936,
     937,   938,   939,   940,   941,   942,   943,   945,   946,   947,
     948,   950,   951,   953,   954,   955,   956,   958,   959,   960,
     961,   963,   965,   966,   967,   968,   969,   971,   972,   973,
     976,   977,   979,   980,   982,   984,   986,   988,   991,   992,
     992,   994,   995,   997,   997,   998,   998,   999,  1001,  1001,
    1002,  1004,  1004,  1004,  1007,  1007,  1007,  1009,  1009,  1010,
    1010,  1011,  1011,  1012,  1014,  1014,  1016
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_EndOfFile", "T_Return", "T_Number",
  "T_True", "T_False", "T_ID", "T_Print", "T_Cln", "T_NL", "T_EQL",
  "T_NEQ", "T_EQ", "T_GT", "T_LT", "T_EGT", "T_ELT", "T_Or", "T_And",
  "T_Not", "T_In", "ID", "ND", "DD", "T_String", "T_If", "T_Elif",
  "T_While", "T_Else", "T_Import", "T_Break", "T_Pass", "T_MN", "T_PL",
  "T_DV", "T_ML", "T_OP", "T_CP", "T_OB", "T_CB", "T_Def", "T_Comma",
  "T_List", "T_For", "T_Range", "$accept", "StartDebugger", "$@1",
  "constant", "term", "list_index", "StartParse", "$@2", "basic_stmt",
  "arith_exp", "bool_exp", "bool_term", "bool_factor", "import_stmt",
  "pass_stmt", "break_stmt", "return_stmt", "assign_stmt", "print_stmt",
  "finalStatements", "cmpd_stmt", "if_stmt", "elif_stmts", "else_stmt",
  "while_stmt", "for_stmt", "iterable", "start_suite", "$@3", "suite",
  "end_suite", "$@4", "$@5", "args", "$@6", "args_list", "$@7",
  "call_list", "$@8", "call_args", "$@9", "$@10", "$@11", "func_def",
  "$@12", "func_call", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    49,    48,    50,    50,    51,    51,    51,    52,
      53,    54,    53,    53,    55,    55,    55,    55,    55,    55,
      55,    55,    56,    56,    56,    56,    56,    56,    56,    57,
      57,    57,    57,    57,    57,    57,    57,    58,    58,    58,
      58,    59,    59,    60,    61,    62,    63,    64,    64,    64,
      64,    65,    66,    66,    66,    66,    66,    67,    67,    67,
      68,    68,    69,    69,    70,    71,    72,    73,    74,    75,
      74,    76,    76,    78,    77,    79,    77,    77,    81,    80,
      80,    83,    82,    82,    85,    84,    84,    87,    86,    88,
      86,    89,    86,    86,    91,    90,    92
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     1,     1,     1,     1,     1,     4,
       2,     0,     4,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     3,     1,
       1,     2,     3,     2,     1,     1,     1,     3,     3,     3,
       4,     4,     1,     1,     1,     1,     2,     1,     1,     1,
       4,     5,     1,     5,     3,     4,     6,     6,     1,     0,
       5,     4,     2,     0,     3,     0,     2,     0,     0,     3,
       0,     0,     4,     0,     0,     4,     0,     0,     3,     0,
       3,     0,     3,     0,     0,     8,     4
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,    46,     4,    39,    40,     6,
       0,     0,     0,     5,     0,     0,     0,    45,    44,     0,
       0,     0,     0,     7,    22,     8,     0,    52,    18,    19,
      36,    37,    16,    14,    15,    21,    17,    20,     0,    53,
      57,    58,    59,    54,    55,    56,     0,    93,     0,     0,
      10,     0,    41,     6,     0,     0,     0,    43,     0,    27,
       0,     0,    94,     0,     3,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    11,     6,     0,
      47,    48,    49,    89,    87,    91,     0,     0,     0,     0,
       0,     0,    28,    42,     0,     0,    38,    32,    30,    34,
      33,    35,    24,    23,    26,    25,     0,    29,    31,     0,
      50,    86,    86,    86,    96,     9,    51,     6,     0,    68,
      60,    65,    80,     0,     0,    12,     0,    90,    88,    92,
      69,     0,     0,    61,    62,    78,     0,     0,     0,    84,
       0,     0,     0,    83,     0,     0,    66,    86,     0,     0,
      64,     0,    79,     0,     0,    85,    77,    70,     0,    81,
      95,     0,     0,    73,    72,    63,    83,    67,     0,     0,
      76,    82,    71,    74
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    23,    24,    25,    26,   109,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,   133,   134,    41,    42,   124,   120,   140,   157,
     164,   169,   170,   136,   143,   152,   166,   127,   147,    86,
     112,   111,   113,    43,    94,    44
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -128
static const yytype_int16 yypact[] =
{
    -128,    23,    95,  -128,    25,  -128,  -128,  -128,  -128,     9,
      18,    95,     3,  -128,    12,    12,    68,  -128,  -128,   169,
      12,    75,    80,  -128,  -128,  -128,    82,  -128,   208,  -128,
      78,  -128,  -128,  -128,  -128,  -128,  -128,  -128,    84,  -128,
    -128,  -128,  -128,  -128,  -128,  -128,   146,    17,    61,    22,
    -128,    12,  -128,    69,   208,   105,   108,  -128,   169,    44,
      55,    81,  -128,   101,  -128,   169,   169,   169,   169,   169,
     117,   169,   169,   169,   169,    12,    12,   127,    -9,    93,
     208,  -128,  -128,  -128,  -128,  -128,    96,   106,    97,   183,
     183,   162,  -128,  -128,   110,   103,    77,    77,    77,    77,
      77,  -128,    44,    44,  -128,  -128,   125,  -128,  -128,    95,
    -128,   107,   107,   107,  -128,  -128,  -128,     0,   132,  -128,
      -2,  -128,   148,   119,   155,  -128,    22,  -128,  -128,  -128,
    -128,    12,   163,  -128,  -128,  -128,   139,    61,   183,  -128,
     137,   166,   183,   138,   173,   142,  -128,   107,   182,   183,
    -128,   192,  -128,   183,    61,  -128,    83,  -128,    -2,  -128,
    -128,   167,   137,    54,  -128,  -128,   138,  -128,   182,   137,
    -128,  -128,  -128,  -128
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -128,  -128,  -128,   -44,   -40,  -128,    -4,  -128,   -75,   -14,
     -12,    56,   190,  -128,  -128,  -128,  -128,  -128,  -128,  -127,
    -128,  -128,    47,  -128,  -128,  -128,  -128,   -74,  -128,    40,
    -128,  -128,  -128,  -128,  -128,    45,  -128,  -102,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,   164
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -76
static const yytype_int16 yytable[] =
{
      54,    54,    55,    56,    87,    59,    60,    50,    61,    88,
     128,   129,    46,   148,   119,   119,   121,     6,     7,     8,
      53,    46,    83,     3,    12,    84,   131,     6,   132,    47,
      53,    48,    80,    12,    81,   168,    45,    54,    13,    61,
      48,    51,   173,    85,    91,   155,    19,    47,    13,    48,
      20,    96,    97,    98,    99,   100,    49,   102,   103,   104,
     105,   106,   106,   119,   146,   -75,     6,   119,   150,    65,
      66,    67,    68,    69,   119,   158,    57,    70,   119,   160,
      73,    74,   -75,    62,   -75,    64,   139,    13,    63,    71,
      72,    73,    74,   145,    92,    77,     4,    75,    76,     5,
       6,     7,     8,     9,    10,   125,    11,   162,   163,    48,
     161,    71,    72,    73,    74,    89,    12,    54,    90,   141,
      93,    13,    14,    95,    15,   101,    16,    17,    18,    19,
     -13,   107,   108,    20,   110,   114,   116,    21,     4,    65,
      22,     5,     6,     7,     8,     9,    10,   115,   122,   123,
     126,     6,     7,     8,    78,   130,   135,   137,    12,    71,
      72,    73,    74,    13,    14,   138,    15,    12,    16,    17,
      18,    19,    13,   142,     6,    20,   149,    53,   144,    21,
      19,   151,    22,   153,    20,   154,    79,     5,     6,     7,
       8,   117,    10,   156,   118,    13,    71,    72,    73,    74,
     159,    92,    52,    19,    12,   165,   167,    58,   172,    13,
      82,   171,     0,     0,    16,    17,    18,    19,     0,     0,
       0,    20,    65,    66,    67,    68,    69,     0,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-128)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      14,    15,    14,    15,    48,    19,    20,    11,    20,    49,
     112,   113,    12,   140,    89,    90,    90,     5,     6,     7,
       8,    12,     5,     0,    21,     8,    28,     5,    30,    38,
       8,    40,    46,    21,    46,   162,    11,    51,    26,    51,
      40,    38,   169,    26,    58,   147,    34,    38,    26,    40,
      38,    65,    66,    67,    68,    69,    38,    71,    72,    73,
      74,    75,    76,   138,   138,    11,     5,   142,   142,    14,
      15,    16,    17,    18,   149,   149,     8,    22,   153,   153,
      36,    37,    28,     8,    30,     3,   126,    26,     8,    34,
      35,    36,    37,   137,    39,    11,     1,    19,    20,     4,
       5,     6,     7,     8,     9,   109,    11,    24,    25,    40,
     154,    34,    35,    36,    37,    10,    21,   131,    10,   131,
      39,    26,    27,    22,    29,     8,    31,    32,    33,    34,
       3,    75,    76,    38,    41,    39,    39,    42,     1,    14,
      45,     4,     5,     6,     7,     8,     9,    41,    38,    46,
      43,     5,     6,     7,     8,    23,     8,    38,    21,    34,
      35,    36,    37,    26,    27,    10,    29,    21,    31,    32,
      33,    34,    26,    10,     5,    38,    10,     8,    39,    42,
      34,    43,    45,    10,    38,    43,    40,     4,     5,     6,
       7,     8,     9,    11,    11,    26,    34,    35,    36,    37,
       8,    39,    12,    34,    21,   158,    39,    38,   168,    26,
      46,   166,    -1,    -1,    31,    32,    33,    34,    -1,    -1,
      -1,    38,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    35,    36,    37
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    48,    49,     0,     1,     4,     5,     6,     7,     8,
       9,    11,    21,    26,    27,    29,    31,    32,    33,    34,
      38,    42,    45,    50,    51,    52,    53,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    71,    72,    90,    92,    11,    12,    38,    40,    38,
      53,    38,    59,     8,    56,    57,    57,     8,    38,    56,
      56,    57,     8,     8,     3,    14,    15,    16,    17,    18,
      22,    34,    35,    36,    37,    19,    20,    11,     8,    40,
      56,    57,    92,     5,     8,    26,    86,    50,    51,    10,
      10,    56,    39,    39,    91,    22,    56,    56,    56,    56,
      56,     8,    56,    56,    56,    56,    56,    58,    58,    54,
      41,    88,    87,    89,    39,    41,    39,     8,    11,    55,
      74,    74,    38,    46,    73,    53,    43,    84,    84,    84,
      23,    28,    30,    69,    70,     8,    80,    38,    10,    51,
      75,    57,    10,    81,    39,    50,    74,    85,    66,    10,
      74,    43,    82,    10,    43,    84,    11,    76,    74,     8,
      74,    50,    24,    25,    77,    69,    83,    39,    66,    78,
      79,    82,    76,    66
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (YYID (N))                                                     \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (YYID (0))
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

__attribute__((__unused__))
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
#else
static unsigned
yy_location_print_ (yyo, yylocp)
    FILE *yyo;
    YYLTYPE const * const yylocp;
#endif
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += fprintf (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += fprintf (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += fprintf (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += fprintf (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += fprintf (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;


/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 904 "parser.y"
    {init();}
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 904 "parser.y"
    {printf("\nValid Python Syntax\n");  printAST((yyvsp[(2) - (3)].node)); codeGenOp((yyvsp[(2) - (3)].node));  printSTable(); freeAll(); exit(0);}
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 906 "parser.y"
    {insertRecord("Constant", (yyvsp[(1) - (1)].text), (yylsp[(1) - (1)]).first_line, currentScope); (yyval.node) = createID_Const("Constant", (yyvsp[(1) - (1)].text), currentScope);}
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 907 "parser.y"
    {insertRecord("Constant", (yyvsp[(1) - (1)].text), (yylsp[(1) - (1)]).first_line, currentScope); (yyval.node) = createID_Const("Constant", (yyvsp[(1) - (1)].text), currentScope);}
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 909 "parser.y"
    {modifyRecordID("Identifier", (yyvsp[(1) - (1)].text), (yylsp[(1) - (1)]).first_line, currentScope); (yyval.node) = createID_Const("Identifier", (yyvsp[(1) - (1)].text), currentScope);}
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 910 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 911 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 914 "parser.y"
    {checkList((yyvsp[(1) - (4)].text), (yylsp[(1) - (4)]).first_line, currentScope); (yyval.node) = createOp("ListIndex", 2, createID_Const("ListTypeID", (yyvsp[(1) - (4)].text), currentScope), (yyvsp[(3) - (4)].node));}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 916 "parser.y"
    {(yyval.node)=(yyvsp[(2) - (2)].node);}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 916 "parser.y"
    {resetDepth();}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 916 "parser.y"
    {(yyval.node) = createOp("NewLine", 2, (yyvsp[(1) - (4)].node), (yyvsp[(4) - (4)].node));}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 916 "parser.y"
    {(yyval.node)=(yyvsp[(1) - (2)].node);}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 918 "parser.y"
    {(yyval.node)=(yyvsp[(1) - (1)].node);}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 919 "parser.y"
    {(yyval.node)=(yyvsp[(1) - (1)].node);}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 920 "parser.y"
    {(yyval.node)=(yyvsp[(1) - (1)].node);}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 921 "parser.y"
    {(yyval.node)=(yyvsp[(1) - (1)].node);}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 922 "parser.y"
    {(yyval.node)=(yyvsp[(1) - (1)].node);}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 923 "parser.y"
    {(yyval.node)=(yyvsp[(1) - (1)].node);}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 924 "parser.y"
    {(yyval.node)=(yyvsp[(1) - (1)].node);}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 925 "parser.y"
    {(yyval.node)=(yyvsp[(1) - (1)].node);}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 927 "parser.y"
    {(yyval.node)=(yyvsp[(1) - (1)].node);}
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 928 "parser.y"
    {(yyval.node) = createOp("+", 2, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 929 "parser.y"
    {(yyval.node) = createOp("-", 2, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 930 "parser.y"
    {(yyval.node) = createOp("*", 2, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 931 "parser.y"
    {(yyval.node) = createOp("/", 2, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 932 "parser.y"
    {(yyval.node) = createOp("-", 1, (yyvsp[(2) - (2)].node));}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 933 "parser.y"
    {(yyval.node) = (yyvsp[(2) - (3)].node);}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 936 "parser.y"
    {(yyval.node) = createOp("or", 2, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 937 "parser.y"
    {(yyval.node) = createOp("<", 2, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 938 "parser.y"
    {(yyval.node) = createOp("and", 2, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 939 "parser.y"
    {(yyval.node) = createOp(">", 2, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 940 "parser.y"
    {(yyval.node) = createOp("<=", 2, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 941 "parser.y"
    {(yyval.node) = createOp(">=", 2, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 942 "parser.y"
    {checkList((yyvsp[(3) - (3)].text), (yylsp[(3) - (3)]).first_line, currentScope); (yyval.node) = createOp("in", 2, (yyvsp[(1) - (3)].node), createID_Const("Constant", (yyvsp[(3) - (3)].text), currentScope));}
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 943 "parser.y"
    {(yyval.node)=(yyvsp[(1) - (1)].node);}
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 945 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 946 "parser.y"
    {(yyval.node) = createOp("==", 2, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 947 "parser.y"
    {insertRecord("Constant", "True", (yylsp[(1) - (1)]).first_line, currentScope); (yyval.node) = createID_Const("Constant", "True", currentScope);}
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 948 "parser.y"
    {insertRecord("Constant", "False", (yylsp[(1) - (1)]).first_line, currentScope); (yyval.node) = createID_Const("Constant", "False", currentScope);}
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 950 "parser.y"
    {(yyval.node) = createOp("!", 1, (yyvsp[(2) - (2)].node));}
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 951 "parser.y"
    {(yyval.node) = (yyvsp[(2) - (3)].node);}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 953 "parser.y"
    {insertRecord("PackageName", (yyvsp[(2) - (2)].text), (yylsp[(2) - (2)]).first_line, currentScope); (yyval.node) = createOp("import", 1, createID_Const("PackageName", (yyvsp[(2) - (2)].text), currentScope));}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 954 "parser.y"
    {(yyval.node) = createOp("pass", 0);}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 955 "parser.y"
    {(yyval.node) = createOp("break", 0);}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 956 "parser.y"
    {(yyval.node) = createOp("return", 0);}
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 958 "parser.y"
    {insertRecord("Identifier", (yyvsp[(1) - (3)].text), (yylsp[(1) - (3)]).first_line, currentScope); (yyval.node) = createOp("=", 2, createID_Const("Identifier", (yyvsp[(1) - (3)].text), currentScope), (yyvsp[(3) - (3)].node));}
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 959 "parser.y"
    {insertRecord("Identifier", (yyvsp[(1) - (3)].text), (yylsp[(1) - (3)]).first_line, currentScope);(yyval.node) = createOp("=", 2, createID_Const("Identifier", (yyvsp[(1) - (3)].text), currentScope), (yyvsp[(3) - (3)].node));}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 960 "parser.y"
    {insertRecord("Identifier", (yyvsp[(1) - (3)].text), (yylsp[(1) - (3)]).first_line, currentScope); (yyval.node) = createOp("=", 2, createID_Const("Identifier", (yyvsp[(1) - (3)].text), currentScope), (yyvsp[(3) - (3)].node));}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 961 "parser.y"
    {insertRecord("ListTypeID", (yyvsp[(1) - (4)].text), (yylsp[(1) - (4)]).first_line, currentScope); (yyval.node) = createID_Const("ListTypeID", (yyvsp[(1) - (4)].text), currentScope);}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 963 "parser.y"
    {(yyval.node) = createOp("Print", 1, (yyvsp[(3) - (4)].node));}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 965 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 966 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 967 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 968 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 969 "parser.y"
    {yyerrok; yyclearin; (yyval.node)=createOp("SyntaxError", 0);}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 971 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 972 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 973 "parser.y"
    {{(yyval.node) = (yyvsp[(1) - (1)].node);}}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 976 "parser.y"
    {(yyval.node) = createOp("If", 2, (yyvsp[(2) - (4)].node), (yyvsp[(4) - (4)].node));}
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 977 "parser.y"
    {(yyval.node) = createOp("If", 3, (yyvsp[(2) - (5)].node), (yyvsp[(4) - (5)].node), (yyvsp[(5) - (5)].node));}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 979 "parser.y"
    {(yyval.node)= (yyvsp[(1) - (1)].node);}
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 980 "parser.y"
    {(yyval.node)= createOp("Elif", 3, (yyvsp[(2) - (5)].node), (yyvsp[(4) - (5)].node), (yyvsp[(5) - (5)].node));}
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 982 "parser.y"
    {(yyval.node) = createOp("Else", 1, (yyvsp[(3) - (3)].node));}
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 984 "parser.y"
    {(yyval.node) = createOp("While", 2, (yyvsp[(2) - (4)].node), (yyvsp[(4) - (4)].node));}
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 986 "parser.y"
    {(yyval.node) = createOp("For", 2, (yyvsp[(4) - (6)].node), (yyvsp[(6) - (6)].node));}
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 988 "parser.y"
    {(yyval.node) = createOp("Range",2,(yyvsp[(3) - (6)].node) ,(yyvsp[(5) - (6)].node));}
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 991 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 992 "parser.y"
    {initNewTable((yyvsp[(2) - (2)].depth)); updateCScope((yyvsp[(2) - (2)].depth));}
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 992 "parser.y"
    {(yyval.node) = createOp("BeginBlock", 2, (yyvsp[(4) - (5)].node), (yyvsp[(5) - (5)].node));}
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 994 "parser.y"
    {(yyval.node) = createOp("Next", 2, (yyvsp[(3) - (4)].node), (yyvsp[(4) - (4)].node));}
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 995 "parser.y"
    {(yyval.node) = (yyvsp[(2) - (2)].node);}
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 997 "parser.y"
    {updateCScope((yyvsp[(1) - (1)].depth));}
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 997 "parser.y"
    {(yyval.node) = createOp("EndBlock", 1, (yyvsp[(3) - (3)].node));}
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 998 "parser.y"
    {updateCScope((yyvsp[(1) - (1)].depth));}
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 998 "parser.y"
    {(yyval.node) = createOp("EndBlock", 0);}
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 999 "parser.y"
    {(yyval.node) = createOp("EndBlock", 0); resetDepth();}
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 1001 "parser.y"
    {addToList((yyvsp[(1) - (1)].text), 1);}
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 1001 "parser.y"
    {(yyval.node) = createOp(argsList, 0); clearArgsList();}
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 1002 "parser.y"
    {(yyval.node) = createOp("Void", 0);}
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 1004 "parser.y"
    {addToList((yyvsp[(2) - (2)].text), 0);}
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 1007 "parser.y"
    {addToList((yyvsp[(1) - (2)].text), 0);}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 1009 "parser.y"
    {addToList((yyvsp[(1) - (1)].text), 1);}
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 1009 "parser.y"
    {(yyval.node) = createOp(argsList, 0); clearArgsList();}
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 1010 "parser.y"
    {addToList((yyvsp[(1) - (1)].text), 1);}
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 1010 "parser.y"
    {(yyval.node) = createOp(argsList, 0); clearArgsList();}
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 1011 "parser.y"
    {addToList((yyvsp[(1) - (1)].text), 1);}
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 1011 "parser.y"
    {(yyval.node) = createOp(argsList, 0); clearArgsList();}
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 1012 "parser.y"
    {(yyval.node) = createOp("Void", 0);}
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 1014 "parser.y"
    {insertRecord("Func_Name", (yyvsp[(2) - (2)].text), (yylsp[(2) - (2)]).first_line, currentScope);}
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 1014 "parser.y"
    {(yyval.node) = createOp("Func_Name", 3, createID_Const("Func_Name", (yyvsp[(2) - (8)].text), currentScope), (yyvsp[(5) - (8)].node), (yyvsp[(8) - (8)].node));}
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 1016 "parser.y"
    {(yyval.node) = createOp("Func_Call", 2, createID_Const("Func_Name", (yyvsp[(1) - (4)].text), currentScope), (yyvsp[(3) - (4)].node));}
    break;


/* Line 1792 of yacc.c  */
#line 3120 "parser.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 1019 "parser.y"


void yyerror(const char *msg)
{
	printf("\nSyntax Error at Line %d, Column : %d\n",  yylineno, yylloc.last_column);
	exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open the file
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return 1;
    }

    printf("Analyzing file: %s\n", argv[1]);

    // Start parsing
    if (yyparse() == 0) { 
        printf("Parsing completed successfully.\n");
    } else {
        printf("Parsing failed.\n");
    }

    fclose(yyin); // Close the file
    return 0;
}

 
