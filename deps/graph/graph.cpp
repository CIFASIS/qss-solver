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

#include "../../util/visitors/occurs.h"

namespace MicroModelica {
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
  //    std::cout << "\nLabel::RemoveEquations result:\n" << newIps << "\n";
      this->ips=newIps;
    }

    std::ostream& operator<<(std::ostream &os, const Label &label) {
      os << label.ips;
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
    
    GenerateEdge::GenerateEdge(struct VertexProperty eq, struct VertexProperty ifr, VarSymbolTable symbols) : 
      _eq(eq), 
      _ifr(ifr), 
      _exist(false),
      _symbols(symbols),
      _ips() 
    {
      initialize();
    }

    void
    GenerateEdge::initialize() 
    {
      string ifrName = _ifr.var.name();
      Occurs oc(_ifr.var.name(), _symbols);
      _exist = oc.apply(_eq.eq.equation());
      if(_exist) {
        build(oc.occurrences());
      }
    }

    void 
    GenerateEdge::build(list<Expression> exps) 
    {
      for (Expression ep : exps) {

      }   

    }

  }
}
