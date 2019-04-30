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

#include "graph.h"

#include <boost/variant/get.hpp>

#include "../../ast/ast_builder.h"
#include "../../ir/expression.h"
#include "../../util/visitors/occurs.h"
#include "../../util/visitors/partial_eval_exp.h"

namespace MicroModelica {
  using namespace IR;
  using namespace Util;
  namespace Deps {
    	  /*****************************************************************************
     ****                              LABEL                                  ****
     *****************************************************************************/
    Label::Label(IndexPairSet ips): ips(ips), _ip() {
     this->RemoveDuplicates();
    }

    Label::Label(IndexPair ip): ips(), _ip(ip) {
    }


    void Label::RemovePairs(IndexPairSet ipsToRemove) {
      IndexPairSet newIps;
      foreach_(IndexPair ipRemove, ipsToRemove) {
        foreach_(IndexPair ip, this->ips) {
  //        newIps.erase(ip);
          foreach_(IndexPair ipRemaining, (ip-ipRemove)) {
            newIps.insert(ipRemaining);
          }
        }
      }
      this->ips = newIps;
    }

    void Label::RemoveUnknowns(MDI const mdi) {
      IndexPairSet newIps;
      for(IndexPair ip: this->ips) {
        IndexPairSet afterRemove = ip.RemoveUnknowns(mdi);
        newIps.insert(afterRemove.begin(), afterRemove.end());
      }
  //    std::cout << "\nLabel::RemoveUnknowns result:\n" << newIps << "\n";
      this->ips=newIps;
    }

    void Label::RemoveEquations(MDI const mdi) {
      IndexPairSet newIps;
      for(IndexPair ipOld: this->ips) {
        for(IndexPair ipNew: ipOld.RemoveEquations(mdi)) {
          newIps.insert(ipNew);
        }
      }
      this->ips=newIps;
    }

    std::ostream& operator<<(std::ostream &os, const Label &label) {
      os << label._ip;
      return os;
    }

    void Label::RemoveDuplicates() {
      bool removeSomething = true;
      IndexPairSet newIPS = ips;
      while (removeSomething) {
        for (IndexPairSet::iterator checkingIP=ips.begin(); checkingIP!=ips.end(); checkingIP++) {
          //Ignore pairs 1-1 => should not be equal pairs in a set
          if (checkingIP->Dom().Size()==1 && checkingIP->Ran().Size()==1)
            continue;
          for (IndexPairSet::iterator otherIP=ips.begin(); otherIP!=ips.end(); otherIP++) {
            //Ignore the same pair
            if (checkingIP == otherIP)
              continue;
            switch (checkingIP->Type()) {
            case INDEX::RN_N:
              switch (otherIP->Type()) {
              case INDEX::RN_N:
                if (checkingIP->GetUsage()==otherIP->GetUsage()) {
                  if (checkingIP->GetOffset()==otherIP->GetOffset()) { // Same usage same offset => are equals: SHOULD NOT OCCUR
                    //ERROR("This case should not occur since should not be equal pairs in a set");
                    abort();
                  } else { // Same usage different offset => there is no intersection, nothing to remove
                    removeSomething = false;
                    continue;
                  }
                } else { //Different usage => ERROR: Not supported yet
                  //ERROR("Multiple usages of a same vector with different index usages in a same for equation not supported");
                  abort();
                }
              case INDEX::RN_1:
                if (checkingIP->Ran().Contains(otherIP->Ran())) { //There is intersection => remove it from the N-1 Index Pair
                  newIPS.erase(*otherIP);
                  MDI domToRemove = otherIP->Ran().RevertUsage(checkingIP->GetUsage(), checkingIP->Dom()).ApplyOffset(checkingIP->GetOffset());
                  for (MDI remainingDom: otherIP->Dom()-domToRemove) {
                    newIPS.insert(IndexPair(remainingDom, otherIP->Ran(), otherIP->GetOffset(), otherIP->GetUsage()));
                  }
                  removeSomething = true;
                  continue;
                }
                else { //No intersection => nothing to remove
                  removeSomething = false;
                  continue;
                }
              case INDEX::R1_N:
                if (checkingIP->Dom().Contains(otherIP->Dom())) { //There is intersection => remove the N-N Index Pair, since it must be a 1-1 pair.
                  newIPS.erase(*checkingIP);
                  removeSomething = true;
                  continue;
                }
              }
            case INDEX::RN_1:
              switch (otherIP->Type()) {
              case INDEX::RN_N:
                if (otherIP->Ran().Contains(checkingIP->Ran())) { //There is intersection => remove it from the N-1 Index Pair
                  newIPS.erase(*checkingIP);
                  MDI domToRemove = checkingIP->Ran().RevertUsage(otherIP->GetUsage(), otherIP->Dom()).ApplyOffset(otherIP->GetOffset());
                  for (MDI remainingDom: checkingIP->Dom()-domToRemove) {
                    newIPS.insert(IndexPair(remainingDom, checkingIP->Ran(), checkingIP->GetOffset(), checkingIP->GetUsage()));
                  }
                  removeSomething = true;
                  continue;
                }
                else { //No intersection => nothing to remove
                  removeSomething = false;
                  continue;
                }
              case INDEX::RN_1:
                if (checkingIP->Ran()==otherIP->Ran()) { // Same range => are equals: SHOULD NOT OCCUR
                  //ERROR("This case should not occur since should not be equal pairs in a set");
                  abort();
                }
                else { //No intersection => nothing to remove
                  removeSomething = false;
                  continue;
                }
              case INDEX::R1_N:
                //This case should not occur
                //ERROR("This case should not occur since should could not be N-1 and 1-N pairs in a same label");
                abort();
              }
            case INDEX::R1_N:
              switch (otherIP->Type()) {
              case INDEX::RN_N:
                if (otherIP->Dom().Contains(checkingIP->Dom())) { //There is intersection => remove the N-N Index Pair, since it must be a 1-1 pair.
                  newIPS.erase(*otherIP);
                  removeSomething = true;
                  continue;
                }
              case INDEX::RN_1:
                //This case should not occur
                //ERROR("This case should not occur since should could not be N-1 and 1-N pairs in a same label");
                abort();
              case INDEX::R1_N:
                if (checkingIP->Dom()==otherIP->Dom()) { // Same range => are equals: SHOULD NOT OCCUR
                  //ERROR("This case should not occur since should not be equal pairs in a set");
                  abort();
                }
                else { //No intersection => nothing to remove
                  removeSomething = false;
                  continue;
                }
              }
            }
          }
        }
        removeSomething = false;
      }
      ips = newIPS;
    }
    
    Occur::Occur(Expression exp, VarSymbolTable symbols, Option<Range> range) : 
      _exp(exp),
      _cr(nullptr),
      _symbols(symbols),
      _range(range)
    {
      if(_exp.expression()->expressionType() == EXPCOMPREF) {
        _cr = _exp.expression()->getAsComponentReference();
      } else {
        _cr = newAST_Expression_ComponentReference();
      }
      initialize();
    }

    string
    Occur::reference(AST_Expression exp) 
    {
      if (exp->expressionType() == EXPBINOP) {
        if (exp->getAsBinOp()->left()->expressionType() == EXPCOMPREF) {
          return exp->getAsBinOp()->left()->getAsComponentReference()->name();
        }  
        if (exp->getAsBinOp()->right()->expressionType() == EXPCOMPREF) {
          return exp->getAsBinOp()->right()->getAsComponentReference()->name();
        }
      }
      return "";
    }

    int 
    Occur::constant(AST_Expression exp) 
    {
      if (exp->expressionType() == EXPBINOP) {
        int umin = 1;
        if (exp->getAsBinOp()->binopType() == BINOPSUB) {
          umin = -1;
        }
        if (exp->getAsBinOp()->left()->expressionType() == EXPINTEGER) {
          return umin * exp->getAsBinOp()->left()->getAsInteger()->val();
        }  
        if (exp->getAsBinOp()->right()->expressionType() == EXPINTEGER) {
          return umin * exp->getAsBinOp()->right()->getAsInteger()->val();
        }
      }
      return 0;
    }

    void 
    Occur::initialize()
    {
      if (_cr->hasIndexes()) {
        std::vector<int> offset_vector;
        AST_ExpressionList indexes = _cr->firstIndex();
        AST_ExpressionListIterator it;
        foreach(it, indexes)
        {
          PartialEvalExp pexp(_symbols);
          AST_Expression cur = pexp.apply(current_element(it));
          if (cur->expressionType() == EXPINTEGER) {
            int v = cur->getAsInteger()->val();
            _intervals.push_back(Interval::closed(v,v)); 
            _usages.push_back(-1);
            _offsets.push_back(0);
          } else if (cur->expressionType() == EXPCOMPREF) {
            AST_Expression_ComponentReference cr = cur->getAsComponentReference();
            Option<Variable> var = _symbols[cr->name()];
            if(var && _range) {
              RangeDefinitionTable rdt = _range->definition();
              Option<RangeDefinition> rd = rdt[var->name()];
              int begin = 0, end = 0;
              if (rd) { 
                begin = rd->begin();
                end = rd->end();
              } 
              _intervals.push_back(Interval::closed(begin,end));
              _usages.push_back(_range->pos(var->name()));
              _offsets.push_back(0);
            } else {
              _intervals.push_back(Interval::closed(0,0));
              _usages.push_back(-1);
              _offsets.push_back(0);
            }
          } else if (cur->expressionType() == EXPBINOP) {
            int offset = constant(cur);
            string name = reference(cur);
            Option<Variable> var = _symbols[name];
            if(var && _range) {
              RangeDefinitionTable rdt = _range->definition();
              Option<RangeDefinition> rd = rdt[name];
              int begin = 0, end = 0;
              if (rd) { 
                begin = rd->begin();
                end = rd->end();
              } 
              _intervals.push_back(Interval::closed(begin+offset,end+offset));
              _usages.push_back(_range->pos(name));
              _offsets.push_back(offset);
            }
          }
        }
      }
    }

    bool 
    Occur::hasIndex()
    {
      return _cr->hasIndexes();
    }
        
    IntervalList
    Occur::intervals()
    {
      return _intervals;
    }

    Usage 
    Occur::usages()
    {
      return _usages;
    }

    Offset 
    Occur::offsets()
    {
      return Offset(_offsets);
    }

    GenerateEdge::GenerateEdge(struct VertexProperty eq, struct VertexProperty inf, 
                               VarSymbolTable symbols, VERTEX::Mode mode) : 
      _eq(eq), 
      _inf(inf), 
      _exist(false),
      _symbols(symbols),
      _ips(),
      _mode(mode) 
    {
      initialize();
    }

    void
    GenerateEdge::initialize() 
    {
      if (_inf.type == VERTEX::Influencer ||
          (_inf.type == VERTEX::Algebraic &&
           _mode == VERTEX::Output)) {
        string infName = _inf.var.name();
        Occurs oc(_inf.var.name(), _symbols);
        _exist = oc.apply(_eq.eq.equation());
        if(_exist) {
          build(oc.occurrences());
        }
      } else if (_inf.type == VERTEX::Algebraic && 
                 _mode == VERTEX::Input) {
        Occurs oc(_inf.var.name(), _symbols);
        _exist = oc.apply(_eq.eq.lhs().expression());
        if(_exist) {
          build(oc.occurrences());
        }
      } else {
        list<Expression> exps;
        exps.push_back(_eq.eq.lhs());
        _exist = true;
        build(exps);
      }
    }

    void 
    GenerateEdge::build(list<Expression> exps) 
    {
      Equation eq = _eq.eq;
      Option<Range> range = eq.range();
      IntervalList eqInterval;
      if (range) {
        for(auto rd : range->definition()) {
          eqInterval.push_back(Interval::closed(rd.second.begin(), rd.second.end()));
        }
      } 
      cout << "Expressions: " << exps.size() << endl;
      for (Expression exp : exps) {
        if (!exp.isReference()) {
          cout << "Error: expression must be a reference." << endl;
          break;
        }
        cout << "Expressions: " << exp << endl;
        Occur oc(exp, _symbols, range);
        if (range) { 
          if (oc.hasIndex()) { // N N
            cout << "Intenta agregar: " << exp << endl;
            _ips.insert(IndexPair(MDI(eqInterval), MDI(oc.intervals()), oc.offsets(), oc.usages(), exp));  
          } else { // 1 N
             cout << "Intenta agregar caso 1: " << exp << endl;
            _ips.insert(IndexPair(MDI(eqInterval), MDI(0), Offset(), Usage(), exp));
          }
        } else {
          if (oc.hasIndex()) { // 1 1
             cout << "Intenta agregar: caso 2" << exp << endl;
            _ips.insert(IndexPair(MDI(0), MDI(0), oc.offsets(), oc.usages(), exp));
          } else { // 1 1
             cout << "Intenta agregar: caso 3" << exp << endl;
            _ips.insert(IndexPair(MDI(0), MDI(0), Offset(), Usage(), exp));
          }
        } 
        cout << "Indexes: " << _ips.size() << endl;
      }   
    }

  }
}