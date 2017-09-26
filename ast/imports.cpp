#include "imports.h"

#include <list>

#include "ast_builder.h"
#include "class.h"
#include "element.h"

AST_Imports::AST_Imports()
{
  _imports = newAST_StringList();
}

AST_Imports::~AST_Imports()
{
  delete _imports;
}

void
AST_Imports::visit(AST_Class x)
{
}

void
AST_Imports::leave(AST_Class x)
{
}

void
AST_Imports::visit(AST_Composition x)
{
}

void
AST_Imports::leave(AST_Composition x)
{
}

void
AST_Imports::visit(AST_CompositionElement x)
{
}

void
AST_Imports::leave(AST_CompositionElement x)
{
}

void
AST_Imports::visit(AST_CompositionEqsAlgs x)
{
}

void
AST_Imports::leave(AST_CompositionEqsAlgs x)
{
}

void
AST_Imports::visit(AST_External_Function_Call)
{
}

void
AST_Imports::visit(AST_Element x)
{
  ElementType e = x->elementType();
  if(e == IMPORT)
  {
    AST_Element_ImportClause i = x->getAsImportClause();
    AST_ListAppend(_imports, newAST_String(i->name()));
  }
  else if(e == ELCLASS)
  {
    AST_Class c = x->getAsClassWrapper()->getClass();
    c->accept(this);
  }
}

void
AST_Imports::visit(AST_Modification x)
{
}

void
AST_Imports::leave(AST_Modification x)
{
}

void
AST_Imports::visit(AST_Comment x)
{
}

void
AST_Imports::visit(AST_Equation x)
{
}

void
AST_Imports::visit(AST_ForIndex x)
{
}

void
AST_Imports::visit(AST_Equation_Else x)
{
}

void
AST_Imports::visit(AST_Expression x)
{
}

void
AST_Imports::visit(AST_Argument x)
{
}

void
AST_Imports::visit(AST_Statement x)
{
}

void
AST_Imports::leave(AST_Statement x)
{
}

void
AST_Imports::visit(AST_Statement_Else x)
{
}

void
AST_Imports::visit(AST_StoredDefinition x)
{
}

void
AST_Imports::leave(AST_StoredDefinition x)
{
}

int
AST_Imports::apply(AST_Node x)
{
  x->accept(this);
  return 0;
}

AST_StringList
AST_Imports::imports()
{
  return _imports;
}
