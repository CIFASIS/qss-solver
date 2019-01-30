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

#include "index_pair.h"

/*#include <ast/expression.h>
#include <boost/variant/get.hpp>
#include <util/debug.h>
#include <stdarg.h>*/ 


namespace MicroModelica {
  namespace Deps {


  std::list<IndexPair> IndexPair::operator-(const IndexPair& other) const {
    ERROR_UNLESS((this->Dom().Dimension()==other.Dom().Dimension()), "Domain dimension error in IndexPair subtraction");
    ERROR_UNLESS((this->Ran().Dimension()==other.Ran().Dimension()), "Range dimension error in IndexPair subtraction");
    std::list<IndexPair> ret;
    switch (this->Type()) {
    case _N_N:
      switch (other.Type()) {
      case _N_N:
        if (this->offset != other.offset) {
          //Nothing to subtract
          return std::list<IndexPair>{*this};
        } else {
          std::list<MDI> remainingDom = this->Dom()-other.Dom();
          std::list<MDI> remainingRan = this->Ran()-other.Ran();
          ERROR_UNLESS(remainingDom.size()==remainingRan.size(), "Size error in remaining domains and ranges");
          std::list<MDI>::iterator domIter = remainingDom.begin();
          std::list<MDI>::iterator ranIter = remainingRan.begin();
          std::list<IndexPair> ret;
          while (domIter!=remainingDom.end()) {
            ret.push_back(IndexPair(*domIter,*ranIter,this->offset, this->usage));
            domIter++;
            ranIter++;
          }
          return ret;
        }
      case _N_1:
        if (!this->Ran().Contains(other.Ran())) {
          //Nothing to subtract
          return std::list<IndexPair>{*this};
        } else {
          MDI domToRemove = (other.Ran().RevertUsage(usage, this->Dom())).ApplyOffset(-offset);
          ERROR_UNLESS(domToRemove.Size()==1, "Domain of removing a pair N-1 from a N-N must have size 1");
          if(!this->Dom().Contains(domToRemove)) {
            //Nothing to subtract
            return std::list<IndexPair>{*this};
          } else {
            std::list<MDI> remainingDom = this->Dom()-domToRemove;
            std::list<MDI> remainingRan = this->Ran()-other.Ran();
            ERROR_UNLESS(remainingDom.size()==remainingRan.size(), "Size error in remaining domains and ranges");
            std::list<MDI>::iterator domIter = remainingDom.begin();
            std::list<MDI>::iterator ranIter = remainingRan.begin();
            std::list<IndexPair> ret;
            while (domIter!=remainingDom.end()) {
              ret.push_back(IndexPair(*domIter,*ranIter,this->offset, this->usage));
              domIter++;
              ranIter++;
            }
            return ret;
          }
        }
      case _1_N:
        if (!this->Dom().Contains(other.Dom())) {
          //Nothing to subtract
          return std::list<IndexPair>{*this};
        } else {
          MDI ranToRemove = (other.Dom().ApplyUsage(usage, this->Ran())).ApplyOffset(offset);
          ERROR_UNLESS(ranToRemove.Size()==1, "Range of removing a pair N-1 from a N-N pair must have size 1");
          std::list<MDI> remainingDom = this->Dom()-other.Dom();
          std::list<MDI> remainingRan = this->Ran()-ranToRemove;
          ERROR_UNLESS(remainingDom.size()==remainingRan.size(), "Size error in remaining domains and ranges");
          std::list<MDI>::iterator domIter = remainingDom.begin();
          std::list<MDI>::iterator ranIter = remainingRan.begin();
          std::list<IndexPair> ret;
          while (domIter!=remainingDom.end()) {
            ret.push_back(IndexPair(*domIter,*ranIter,this->offset, this->usage));
            domIter++;
            ranIter++;
          }
          return ret;
        }
      }
    case _N_1:
      switch (other.Type()) {
      case _N_N:
        if (!other.Ran().Contains(this->Ran())) {
          //Nothing to subtract
          return std::list<IndexPair>{*this};
        } else {
          MDI domToRemove = (other.Ran().RevertUsage(other.usage, other.Dom())).ApplyOffset(-other.offset);
          ERROR_UNLESS(domToRemove.Size()==1, "Domain of removing a pair N-N from a N-1 pair must have size 1");
          if(!this->Dom().Contains(domToRemove)) {
            //Nothing to subtract
            return std::list<IndexPair>{*this};
          } else {
            std::list<MDI> remainingDom = this->Dom()-domToRemove;
            std::list<IndexPair> ret;
            for (MDI dom: remainingDom) {
              ret.push_back(IndexPair(dom, this->Ran(), this->offset, this->usage));
            }
            return ret;
          }
        }
      case _N_1:
        if (this->Ran()!=other.Ran()) {
          //Nothing to subtract
          return std::list<IndexPair>{*this};
        } else {
          std::list<MDI> remainingDom = this->Dom()-other.Dom();
          std::list<IndexPair> ret;
          for (MDI dom: remainingDom) {
            ret.push_back(IndexPair(dom, this->Ran(), this->offset, this->usage));
          }
          return ret;
        }
      case _1_N:
        if (!other.Ran().Contains(this->Ran()) || !this->Dom().Contains(other.Ran())) {
          //Nothing to subtract
          return std::list<IndexPair>{*this};
        } else {
          std::list<MDI> remainingDom = this->Dom()-other.Dom();
          std::list<IndexPair> ret;
          for (MDI dom: remainingDom) {
            ret.push_back(IndexPair(dom, this->Ran(), this->offset, this->usage));
          }
          return ret;
        }
      }
    case _1_N:
      switch (other.Type()) {
      case _N_N:
        if (!other.Dom().Contains(this->Dom())) {
        //Nothing to subtract
        return std::list<IndexPair>{*this};
      } else {
        MDI ranToRemove = (this->Dom().ApplyUsage(other.usage, other.Ran())).ApplyOffset(-other.offset);
        ERROR_UNLESS(ranToRemove.Size()==1, "Domain of removing a pair N-N from a 1-N pair must have size 1");
        if(!this->Ran().Contains(ranToRemove)) {
          //Nothing to subtract
          return std::list<IndexPair>{*this};
        } else {
          std::list<MDI> remainingRan = this->Ran()-ranToRemove;
          std::list<IndexPair> ret;
          for (MDI ran: remainingRan) {
            ret.push_back(IndexPair(this->Dom(), ran, this->offset, this->usage));
          }
          return ret;
        }
      }
      case _N_1:
        if (!this->Ran().Contains(other.Ran()) || !other.Dom().Contains(this->Dom())) {
          //Nothing to subtract
          return std::list<IndexPair>{*this};
        } else {
          std::list<MDI> remainingRan = this->Ran()-other.Ran();
          std::list<IndexPair> ret;
          for (MDI ran: remainingRan) {
            ret.push_back(IndexPair(this->Dom(), ran, this->offset, this->usage));
          }
          return ret;
        }
      case _1_N:
        if (!this->Ran().Contains(other.Ran())) {
          //Nothing to subtract
          return std::list<IndexPair>{*this};
        } else {
          std::list<MDI> remainingRan = this->Ran()-other.Ran();
          std::list<IndexPair> ret;
          for (MDI ran: remainingRan) {
            ret.push_back(IndexPair(this->Dom(), ran, this->offset, this->usage));
          }
          return ret;
        }
      }
    default:
      ERROR("This case should not occur");
      abort();
    }
  }

  IndexPairSet IndexPair::RemoveUnknowns(MDI unk2remove) {
    ERROR_UNLESS(this->Ran().Dimension()==unk2remove.Dimension(), "Removing unknowns of different dimension");
    switch (this->Type()) {
    case _N_N:
      if (Option<MDI> intersection = unk2remove & this->Ran()) {
        MDI ranToRemove = intersection.get();
        MDI domToRemove = (ranToRemove.RevertUsage(usage, this->Dom())).ApplyOffset(-offset);
        std::list<MDI> remainsDom = this->Dom()-domToRemove;
        std::list<MDI> remainsRan = this->Ran()-ranToRemove;
        ERROR_UNLESS(remainsDom.size()==remainsRan.size(), "Size error of remaining pairs");
        std::list<MDI>::iterator domIter = remainsDom.begin();
        std::list<MDI>::iterator ranIter = remainsRan.begin();
        IndexPairSet ret;
        while (domIter!=remainsDom.end()) {
          ret.insert(IndexPair(*domIter,*ranIter,this->offset, this->usage));
          domIter++;
          ranIter++;
        }
        return ret;
      } else {
        return {*this};
      }
    case _N_1:
      if (this->Ran()==unk2remove) {
        //Remove all:
        return {};
      }
      else {
        //Nothing to remove_
        return {*this};
      }
    case _1_N:
      if (Option<MDI> intersection = unk2remove & this->Ran()) {
        MDI ranToRemove = intersection.get();
        IndexPairSet ret;
        std::list<MDI> remainsRan = this->Ran()-ranToRemove;
        for (MDI r: remainsRan) {
          ret.insert(IndexPair(this->Dom(), r ,this->offset, this->usage));
        }
        return ret;
      } else {
        return {*this};
      }
    default:
      ERROR("This case should not occur");
      abort();
    }
  }

  IndexPairSet IndexPair::RemoveEquations(MDI eqs2remove) {
    ERROR_UNLESS(this->Dom().Dimension()==eqs2remove.Dimension(), "Removing equations of different dimension");
    switch (this->Type()) {
    case _N_N:
      if (Option<MDI> intersection = eqs2remove & this->Dom()) {
        MDI domToRemove = intersection.get();
        MDI ranToRemove = (domToRemove.ApplyUsage(usage, this->Ran())).ApplyOffset(offset);
        std::list<MDI> remainsDom = this->Dom()-domToRemove;
        std::list<MDI> remainsRan = this->Ran()-ranToRemove;
        ERROR_UNLESS(remainsDom.size()==remainsRan.size(), "Size error of remaining pairs");
        std::list<MDI>::iterator domIter = remainsDom.begin();
        std::list<MDI>::iterator ranIter = remainsRan.begin();
        IndexPairSet ret;
        while (domIter!=remainsDom.end()) {
          ret.insert(IndexPair(*domIter,*ranIter,this->offset, this->usage));
          domIter++;
          ranIter++;
        }
        return ret;
      } else {
        return {*this};
      }
    case _N_1:
      if (Option<MDI> intersection = eqs2remove & this->Dom()) {
        MDI domToRemove = intersection.get();
        IndexPairSet ret;
        std::list<MDI> remainsDom = this->Dom()-domToRemove;
        for (MDI dom: remainsDom) {
          ret.insert(IndexPair(dom,this->Ran(),this->offset, this->usage));
        }
        return ret;
      } else {
        return {*this};
      }
    case _1_N:
      if (this->Dom()==eqs2remove) {
        //Remove all:
        return {};
      }
      else {
        //Nothing to remove_
        return {*this};
      }
    default:
      ERROR("This case should not occur");
      abort();
    }
  }

  bool IndexPair::operator<(const IndexPair& other) const {
    return this->Dom() < other.Dom() || this->Ran() < other.Ran() || this->GetOffset() < other.GetOffset();
  }

  std::ostream& operator<<(std::ostream &os, const IndexPair &ip) {
    os << "(" << ip.Dom() << ", " << ip.Ran() << ")";
    /*if (ip.OS().Size()) {
      os << "Offset = {";
      for (int i: ip.OS()) 
        os << i << " ";
      os << "}";
    } 
    if (ip.GetUsage().Size()) {
      os << "Usage = {";
      for (int i: ip.GetUsage()) 
        os << i << " ";
      os << "}";
    } */
    return os;
  }

  bool IndexPair::Contains(const IndexPair& other) const {
    if (this->offset!=other.offset || this->usage!=other.usage)
      return false;
    if (this->dom.Size()<other.dom.Size() || this->ran.Size()<other.ran.Size())
      return false;
    if (this->dom.Contains(other.dom) & this->ran.Contains(other.ran))
      return true;
    else
      return false;
  }

  Option<IndexPair> IndexPair::operator&(const IndexPair& other) const {
    //TODO:
    return Option<IndexPair>();
  }

  IndexPairType IndexPair::Type() const {
    //TODO: Check if there is MtoN or NtoM with N,M > 1
    if (dom.Size()==ran.Size())
      return _N_N;
    else if (dom.Size()>ran.Size())
      return _N_1;
    else return _1_N;
  }


  std::ostream& operator<<(std::ostream &os, const std::list<IndexPair> &ipList) {
    std::list<std::string> ipsStList;
    foreach_(IndexPair ip, ipList){
      std::ostringstream ipSt;
      ipSt << ip;
      ipsStList.push_back(ipSt.str());
    }
    std::string ipsSt = "{" + boost::algorithm::join(ipsStList, ",") + "}";
    os << ipsSt;
    return os;
  }
}

}
