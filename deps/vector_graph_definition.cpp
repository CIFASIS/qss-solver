/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

/*
 *  Created on: 2 Aug 2016
 *      Author: Diego Hollmann
 */

#include <causalize/vector/vector_graph_definition.h>
#include <ast/expression.h>
#include <boost/variant/get.hpp>
#include <util/debug.h>
#include <stdarg.h>


namespace Causalize {

  VectorUnknown::VectorUnknown(VarInfo varInfo, Modelica::AST::Reference var): Unknown(varInfo, var) {
    if (!varInfo.indices()) {
      dimension = 0;
    } else {
      dimension = varInfo.indices().get().size();
    }
  }

  void VectorUnknown::SetIndex(Modelica::AST::ExpList index) {
    ERROR_UNLESS((int)index.size()==dimension, "Indexes size different than unknown dimension");
    if (dimension!=0) {
      if (Modelica::AST::is<Modelica::AST::Call>(expression)) {
        get<Modelica::AST::Reference>(get<Modelica::AST::Call>(expression).args_.front()).ref_.front().get<1>() = index;
      } else if (Modelica::AST::is<Modelica::AST::Reference>(expression)) {
        get<Modelica::AST::Reference>(expression).ref_.front().get<1>() = index;
      } else {
        ERROR("Wrong unknown expression type");
      }
    }
  }

  std::ostream& operator<<(std::ostream &os, const IndexPairSet &ips) {
    std::list<std::string> ipsStList;
    foreach_(IndexPair ip, ips){
      std::ostringstream ipSt;
      ipSt << ip;
      ipsStList.push_back(ipSt.str());
    }
    std::string ipsSt = "{" + boost::algorithm::join(ipsStList, ",") + "}";
    os << ipsSt;
    return os;
  }


  /*****************************************************************************
   ****                              Offset                                  ****
   *****************************************************************************/
  Offset Offset::operator-() const {
    std::vector<int> ret(offset.size());
    for(int i = 0; i<(int)offset.size(); i++) {
      ret[i] = -offset[i];
    }
    return ret;
  };
  /*****************************************************************************
   ****************************************************************************/



  /*****************************************************************************
   ****                               MDI                                   ****
   *****************************************************************************/
  MDI::MDI(int d, ...) {
    intervals.resize(d);
    va_list vl;
    va_start(vl,d);
    for (int i=0;i<d;i++) {
      int lower=va_arg(vl,int);
      int upper=va_arg(vl,int);
      intervals[i]=CreateInterval(lower, upper);
    }
    va_end(vl);
  }

  MDI::MDI(IntervalList intervalList): intervals(IntervalVector(intervalList.begin(), intervalList.end())) { }

  int MDI::Size() const{
    int size = 1;
    foreach_(Interval i, intervals) {
      size*=boost::icl::size(i);
    }
    return size;
  }

  bool MDI::operator<(const MDI& other) const {
    return (this->intervals)<(other.intervals);
  }

  std::list<Interval> MDI::Partition(Interval iA, Interval iB) {
    std::list<Interval> ret;
    int a = iA.lower();
    int b = iA.upper();
    int c = iB.lower();
    int d = iB.upper();
    if ((a<c) && (d<b)) {
      ret.push_back(CreateInterval(a,c-1));
      ret.push_back(CreateInterval(c,d));
      ret.push_back(CreateInterval(d+1,b));
    } else if ((c<=a) && (a<=d) && (d<b)) {
  //      return {CreateInterval(a,d), CreateInterval(d+1,b)}; /* c++11 */
      ret.push_back(CreateInterval(a,d));
      ret.push_back(CreateInterval(d+1,b));
    } else if ((a<c) && (c<=b) && (b<=d)) {
      ret.push_back(CreateInterval(a,c-1));
      ret.push_back(CreateInterval(c,b));
    } else {
      ret.push_back(CreateInterval(a,b));
    }
    return ret;
  }

  std::list<MDI> MDI::PutHead(Interval i, std::list<MDI> mdiList) {
    std::list<MDI> mdiListRet;
    for(MDI xs: mdiList) {
      IntervalList ys=IntervalList(xs.intervals.begin(), xs.intervals.end());
      ys.push_front(i);
      mdiListRet.push_back(ys);
    }
    return mdiListRet;
  }

  std::list<MDI> MDI::PutLists(MDI mdi, std::list<MDI> mdiList) {
    std::list<MDI> mdiListRet;
    for(Interval i: mdi.intervals) {
      std::list<MDI> zss = PutHead(i, mdiList);
      for(MDI zs: zss) {
        mdiListRet.push_back(zs);
      }
    }
    return mdiListRet;
  }

  std::ostream& operator<<(std::ostream &os, const MDI mdi) {
    std::list<std::string> xsStList;
    for(Interval x: mdi.intervals) {
      std::stringstream ss;
      if ( x.lower()== x.upper()) 
        ss << "[" << x.lower() << "]";
      else 
        ss << "["<< x.lower() << ":" << x.upper() << "]";
      xsStList.push_back(ss.str());
    }
    os << "<" << boost::algorithm::join(xsStList, ",") << ">";
    return os;
  }

  std::list<MDI> MDI::CartProd(std::list<MDI> xss) {
    std::list<MDI> yss;
    if (xss.size()==0) return yss;
    else if (xss.size()==1) {
      IntervalVector xs = xss.front().intervals;
      for(Interval i: xs) {
        IntervalList ys;
        ys.push_back(i);
        yss.push_back(ys);
      }
      return yss;
    } else {
      std::list<MDI> zss = xss;
      zss.pop_front();
      return PutLists(xss.front(), CartProd(zss));
    }
  }

  std::list<MDI> MDI::Filter(std::list<MDI> mdiList, MDI mdi) {
    std::list<MDI> mdiListRet;
    for(MDI m: mdiList) {
      ERROR_UNLESS(m.Dimension()==mdi.Dimension(), "Dimension error #1");
      if (m.Dimension()!=mdi.Dimension()) {
        std::cout << "Dimension error #2\n";
        abort();
      }
      MDI::iterator iterXS = m.begin();
      MDI::iterator iterYS = mdi.begin();
      bool hasInter = true;
      for(int i=0; i<(int)m.Dimension(); i++) {
        hasInter&= intersects(*iterXS,*iterYS);
        iterXS++;
        iterYS++;
      }
      if (!hasInter) {
        mdiListRet.push_back(m);
      }
    }
    return mdiListRet;
  }

  MDI MDI::ApplyOffset(Offset offset) const {
    //TODO: It is mandatory to "Apply" or "Revert" usage before applying this method
//    ERROR_UNLESS((int)offset.Size()==this->Dimension(),"Dimension error applying offset"); //TODO: Review this error
    if (this->Dimension()==0 || offset.Size()==0) {
      //nothing to apply
      return *this;
    }
    IntervalVector copyIntervals = intervals;
    for(int i=0; i<(int)copyIntervals.size(); i++) {
      copyIntervals[i] = CreateInterval(copyIntervals[i].lower()+offset[i],copyIntervals[i].upper()+offset[i]);
    }
    return MDI(copyIntervals);
  }

  MDI MDI::ApplyUsage(Usage usage, MDI ran) const {
		if (usage.Size()==0 || usage.isUnused() || ran.Dimension()==0) {
      return ran;
    }
    IntervalVector newIntervals(usage.Size());
    for(int i=0; i<(int)usage.Size(); i++) {
      if (usage[i]>=0) {
        newIntervals[i] = intervals[usage[i]];
      }
      else {
        ERROR_UNLESS(ran.Dimension()>=i, "Range argument size error");
        newIntervals[i] = ran.intervals[i];
      }
    }
    return MDI(newIntervals);
  }

  MDI MDI::RevertUsage(Usage usage, MDI dom) const {
//    ERROR_UNLESS(usage.Size()==dom.Dimension(), "Dimension error reverting usage");
    if (usage.Size()==0 || usage.isUnused() || dom.Dimension()==0) {
      return dom;
    }
    else {
      IntervalVector newIntervals(usage.Size());
      int usages = 0;
      for (int i=0; i<usage.Size(); i++) {
        if (usage[i]>=0) {
          newIntervals[usage[i]] = this->intervals[i];
          usages++;
        }
      }
      newIntervals.resize(usages);
      return MDI(newIntervals);
    }
  }

  MDI MDI::DomToRan(IndexPair ip) const {
	MDI rta = this->ApplyUsage(ip.GetUsage(), ip.Ran());
	rta = rta.ApplyOffset(ip.GetOffset());
	return rta;
  } 
  
  
  MDI MDI::RanToDom(IndexPair ip) const {
    MDI rta = this->RevertUsage(ip.GetUsage(), ip.Dom());
    rta = rta.ApplyOffset(-ip.GetOffset());
	return rta;
  }
	
  std::list<MDI> MDI::operator-(const MDI &other) {
    if (this->Dimension()!=other.Dimension()) {
      ERROR("Dimension error #3\n");
    }
    std::list<MDI> ret;
    MDI::iterator iterA = this->begin();
    MDI::const_iterator iterB = other.begin();
    std::list<MDI> prod;
    for(int i=0; i<this->Dimension(); i++) {
      prod.push_back(Partition(*iterA,*iterB)); // particiona en cada eje por posibles subconjuntos
      iterA++;
      iterB++;
    }
    ret = CartProd(prod); // Genera todos
    return Filter(ret, other); // Filtra los que intersecan
  }


  Option<MDI> MDI::operator&(const MDI &other) const {
    if (this->Dimension() != other.Dimension()) { //TODO: Is this condition OK?
			std::cout << *this << " " << other << std::endl;
			std::cout << this->Dimension() << " " << other.Dimension() << std::endl;
      ERROR("Dimension error #5\n");
    }
    IntervalList intersection;
    for(int i=0; i<this->Dimension(); i++) {
      //If i-th interval does not intersect with its corresponding interval in the other MDI: return an empty MDI
      if (!intersects(this->intervals[i],other.intervals[i])) return Option<MDI>();
      else intersection.push_back((this->intervals[i])&(other.intervals[i]));
    }
    //All intervals intersect with its corresponding interval in the other MDI: return the resulting intersection MDI
    return MDI(intersection);
  }

  bool MDI::Contains(const MDI &other) const {
    if (this->Dimension()!=other.Dimension())
      return false; // @karupayun: Is this ok?? And for example a line inside a square?
    else {
      for (int i=0; i<(int)this->intervals.size(); i++){
        if (!boost::icl::contains(this->intervals[i],other.intervals[i]))
          return false;
      }
      //If each interval of "this" contains its corresponding interval of "other" return true
      return true;
    }
    return false;
  }
  /*****************************************************************************
   ****************************************************************************/
	int sum_size (std::list <MDI> &mdis){
		int rta = 0;
		for (auto mdi : mdis){
			rta += mdi.Size();
		}
		return rta;
	}

  /*****************************************************************************
   ****                            INDEX PAIR                               ****
   *****************************************************************************/
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

  /*****************************************************************************
   ****************************************************************************/



  /*****************************************************************************
   ****                              LABEL                                  ****
   *****************************************************************************/
  Label::Label(IndexPairSet ips): ips(ips) {
   this->RemoveDuplicates();
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
          case _N_N:
            switch (otherIP->Type()) {
            case _N_N:
              if (checkingIP->GetUsage()==otherIP->GetUsage()) {
                if (checkingIP->GetOffset()==otherIP->GetOffset()) { // Same usage same offset => are equals: SHOULD NOT OCCUR
                  ERROR("This case should not occur since should not be equal pairs in a set");
                  abort();
                } else { // Same usage different offset => there is no intersection, nothing to remove
                  removeSomething = false;
                  continue;
                }
              } else { //Different usage => ERROR: Not supported yet
                ERROR("Multiple usages of a same vector with different index usages in a same for equation not supported");
                abort();
              }
            case _N_1:
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
            case _1_N:
              if (checkingIP->Dom().Contains(otherIP->Dom())) { //There is intersection => remove the N-N Index Pair, since it must be a 1-1 pair.
                newIPS.erase(*checkingIP);
                removeSomething = true;
                continue;
              }
            }
          case _N_1:
            switch (otherIP->Type()) {
            case _N_N:
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
            case _N_1:
              if (checkingIP->Ran()==otherIP->Ran()) { // Same range => are equals: SHOULD NOT OCCUR
                ERROR("This case should not occur since should not be equal pairs in a set");
                abort();
              }
              else { //No intersection => nothing to remove
                removeSomething = false;
                continue;
              }
            case _1_N:
              //This case should not occur
              ERROR("This case should not occur since should could not be N-1 and 1-N pairs in a same label");
              abort();
            }
          case _1_N:
            switch (otherIP->Type()) {
            case _N_N:
              if (otherIP->Dom().Contains(checkingIP->Dom())) { //There is intersection => remove the N-N Index Pair, since it must be a 1-1 pair.
                newIPS.erase(*otherIP);
                removeSomething = true;
                continue;
              }
            case _N_1:
              //This case should not occur
              ERROR("This case should not occur since should could not be N-1 and 1-N pairs in a same label");
              abort();
            case _1_N:
              if (checkingIP->Dom()==otherIP->Dom()) { // Same range => are equals: SHOULD NOT OCCUR
                ERROR("This case should not occur since should not be equal pairs in a set");
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
  /*****************************************************************************
   ****************************************************************************/


  std::ostream& operator<<(std::ostream &os, const std::list<MDI> &mdiList) {
    std::list<std::string> mdiStList;
    for(MDI mdi: mdiList) {
      std::stringstream ss;
      ss << mdi;
      mdiStList.push_back(ss.str());
    }
    os << "{" << boost::algorithm::join(mdiStList, ",") << "}";
    return os;
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


  unsigned long int EdgeCount(IndexPairSet labels) {
    unsigned long int count = 0;
    /*foreach_(IndexPair ip, labels) {
      unsigned long int eq_count = IntervalCount(get<0>(ip).first);
      unsigned long int unk_count = IntervalCount(get<1>(ip).first);
      count += (eq_count > unk_count ? eq_count : unk_count);
    //unsigned long int eq_ount = 
      
    }*/
    return count;
  }


}
