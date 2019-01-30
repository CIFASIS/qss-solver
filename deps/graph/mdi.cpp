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

#include "mdi.h"
/*#include <ast/expression.h>
#include <boost/variant/get.hpp>
#include <util/debug.h>
#include <stdarg.h>*/

namespace MicroModelica {
  namespace Deps {
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

}
}

