/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#ifndef UTIL_TYPES_H_
#define UTIL_TYPES_H_

#include <string>

#include "macros.h"

/**
 *
 */
DEFINE_TYPE(VarSymbolTable);
/**
 *
 */
DEFINE_TYPE(TypeSymbolTable);
/**
 *
 */
DEFINE_TYPE(VarInfo);
/**
 *
 */
DEFINE_TYPE(MMO_EquationTable);
/**
 *
 */
DEFINE_TYPE(MMO_EventTable);
/**
 *
 */
DEFINE_TYPE(MMO_FunctionTable);
/**
 *
 */
DEFINE_TYPE(MMO_PackageTable);
/**
 *
 */
DEFINE_TYPE(MMO_DependenciesTable);
/**
 *
 */
DEFINE_TYPE(MMO_ArgumentsTable);
/**
 *
 */
DEFINE_TYPE(MMO_StatementTable);
/**
 *
 */
DEFINE_TYPE(MMO_IndexTable);
/**
 *
 */
DEFINE_TYPE(MMO_ImportTable);
/**
 *
 */
DEFINE_TYPE(MMO_SymbolRefTable);
/**
 *
 */
DEFINE_TYPE(MMO_FunctionDefinitionTable);
/**
 *
 */
typedef std::string VarName;
/**
 *
 */
typedef std::string TypeName;
/**
 *
 */
DEFINE_TYPE(AST_Visitor);
/**
 *
 */
DEFINE_TYPE(AST_MicroModelica);
/**
 *
 */
typedef class Dependencies_ *Dependencies;
/**
 *
 */
typedef class GenerateDeps_ *GenerateDeps;
/**
 *
 */
typedef class MDIndex_ Index;
/**
 *
 */
typedef class Range_ Range;
/**
 *
 */
typedef class MDVariableInterval_ VariableInterval;
/**
 *
 */
typedef class ExpressionIndex_ *ExpressionIndex;
/**
 *
 */
typedef class ExpIndexes_ *ExpIndexes;
/**
 *
 */
typedef class ReplaceDer_ *ReplaceDer;
/**
 *
 */
typedef class ControlVars_ *ControlVars;
/**
 *
 */
typedef class MMO_CompileFlags_ *MMO_CompileFlags;

#endif  /*  UTIL_TYPES_H_ */
