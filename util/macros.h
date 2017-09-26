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

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

using namespace std;

/* Types */
/**
 *
 */
#define DEFINE_TYPE(X) class X##_; typedef X##_ *X;
/**
 *
 */
#define DEFINE_LIST(X) typedef std::list<X>  *X##List; typedef std::list<X>::iterator X##ListIterator; typedef std::list<X>::reverse_iterator X##ListReverseIterator;
/**
 *
 */
#define _MMOC_S(s) newAST_String(s)
/**
 *
 */
#define GET_AS(X,Y) AST_##X##_##Y getAs##Y ();
/**
 *
 */
#define GET_AS_IMP(X,Y) AST_##X##_##Y AST_##X##_::getAs##Y () { return dynamic_cast<AST_##X##_##Y > (this); }
/* Printers */
/**
 *
 */
#define DEFINE_PRINTER(X) ostream & operator<<(ostream &os , const X &x );
/**
 *
 */
#define DEFINE_CLASS_PRINTER(X) friend ostream & operator<<(ostream &os , const X##_ &x );
/**
 *
 */
#define CLASSP_PRINTER_IMP(X) ostream & operator<<(ostream &os , const X &e ){ \
                                os << *e; return os;  }
/**
 *
 */
#define CLASS_PRINTER_IMP(X) ostream & operator<<(ostream &os, \
                                    const X##_ &e ){ os << e.print(); return os; } \
                             CLASSP_PRINTER_IMP(X)

#endif  /*  DEFINITIONS_H_ */
