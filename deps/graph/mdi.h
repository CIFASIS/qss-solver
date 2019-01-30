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

#ifndef DEPS_MDI_H_
#define DEPS_MDI_H_

/*#include <utility>
#include <set>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/icl/discrete_interval.hpp>

#include <ast/equation.h>
#include <causalize/graph/graph_definition.h>

#include <ast/ast_types.h>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <sstream>
#include <string>*/

namespace ICL = boost::icl;
namespace MicroModelica {
  namespace Deps {
    
    class MDI { //Multi-Dimensional Interval
    public:
      MDI(int d, ... );
      MDI(IntervalList intervalList);
      inline MDI() { intervals.resize(0); }
      inline MDI(IntervalVector intervals): intervals(intervals) { };
      inline int Dimension() const {return intervals.size(); }
      int Size () const;
      std::list<MDI> operator-(const MDI& other);
      std::list<MDI> Difference(const MDI& other) { return (*this)-other;} ;
      MDI ApplyOffset(Offset) const;
      MDI ApplyUsage(Usage, MDI ran = MDI({})) const;
      MDI RevertUsage(Usage usage, MDI dom = MDI({})) const;
       // karupayun - Para moverse usando la info de la conexion entre Dom y Ran
      MDI DomToRan(IndexPair ip) const; 
      MDI RanToDom(IndexPair ip) const;              
      bool operator<(const MDI& other) const;
      Option<MDI> operator&(const MDI& other) const;
      Option<MDI> Intersection (MDI& other) { return other & (*this) ;}
      friend std::ostream& operator<<(std::ostream& os, const MDI mdi);
      inline const IntervalVector & Intervals() const { return intervals; }
      bool Contains(const MDI& other) const;
      inline bool operator==(const MDI& other) const { return this->intervals==other.intervals; };
      inline bool operator!=(const MDI& other) const { return !((*this)==other); };

    private:
        IntervalVector intervals;
        typedef IntervalVector::iterator iterator;
        typedef IntervalVector::const_iterator const_iterator;
        inline const_iterator begin() const { return intervals.begin(); }
        inline iterator begin() { return intervals.begin(); }
        inline iterator end() { return intervals.end(); }

        IntervalList Partition(Interval iA, Interval iB);
        //MDI ApplyOffset(Offset offset);
        std::list<MDI> PutHead(Interval i, std::list<MDI> mdiList);
        std::list<MDI> Filter(std::list<MDI> mdiList, MDI mdi);
        std::list<MDI> CartProd(std::list<MDI> mdiList);
        std::list<MDI> PutLists(MDI mdi, std::list<MDI> mdiList);
    };
    /*****************************************************************************
     ****************************************************************************/
    typedef std::list<MDI> MDIL; // //Multi-Dimensional Interval List
    int sum_size (std::list <MDI> &mdis); // Sumas de los tamaños de los MDI's de la lista

    std::ostream& operator<<(std::ostream &os, const std::list<MDI> &mdiList);
    enum IndexPairType{
      _N_N, _N_1, _1_N
    };
  }
}
#endif /* DEPS_MDI_H_ */
