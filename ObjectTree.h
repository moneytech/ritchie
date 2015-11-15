#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "errors.h"

typedef enum {
  Undefined,
  Variable,
  Type,
  Constructor,
  Function,
  AssignmentFunction,
  CodeBlock,
  Expression,
} OBJ_TYPE;

typedef struct _Object      Object;
typedef struct _ListObject  ListObject;
typedef struct _ListString  ListString;

struct _ListString {
  char*  value;
  ListString*  next;
};
struct _ListObject{
  Object*      value;
  ListObject*  next;
};

struct _Object{
  char*        name;           //symbol name     ("myInteger", "calcTotalArea ", "Rectangle")
  char*        fullname;       //symbol fullname ("myInteger", "Integer_calcTotalArea_Rectangle_Rectangle", "BaseType_Rectangle")
  Object*      parentScope;    //parent scope    (global scope, global scope, BaseType)
  OBJ_TYPE     type;           //What is this?   (Variable, Function, Class)
  char*        returnType;     //What value type?(Integer,  Integer,  NULL)
  ListString*  paramTypes;     //parameters?     (NULL,     [Integer, Integer], NULL)
  ListObject*  definedSymbols; //Things inside?  (NULL, [Rectangle "r1", Rectangle "r2", Integer "a1", Integer "a2"], [Integer "w", Integer "h", Constructor "Rectangle", Function "Area"])
  ListString*  code;           //CodeBlock       (NULL, "Integer ...calcTotalArea...(...) {...", "typedef struct...")
};

//mallocs memory and returns a pointer to a new Object
Object * CreateObject(char* name, char* fullname, Object* parent, OBJ_TYPE type, char* returnType);

//append item to end of linked list
int addParam(Object* tree, char* type);
int addSymbol(Object* tree, Object* leaf);
int addCode(Object* tree, char* line);
int listlen(ListString* head);

//writes the code of root first, then children in order
void writeTree(FILE* outc, FILE* outh, Object* tree);
void writeTreeHelper(FILE* outc, FILE* outh, Object* tree, int indent);
void printTree(Object* tree, int indent);

//searches for identifier in current, and parent scope.
//returns Undefined if identifier isn't found.
OBJ_TYPE getIdentType(Object* scope, char* identifier);
//return null if name not found
Object* findByNameInScope(Object* scope, char* name);
Object* findByFullNameInScope(Object* scope, char* name);

Object* findFunctionMatch(Object* scope, char* name, int paramc, char** params);

#endif
