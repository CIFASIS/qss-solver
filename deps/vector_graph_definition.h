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

#ifndef VECTOR_GRAPH_DEFINITION_
#define VECTOR_GRAPH_DEFINITION_

#include <utility>
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
#include <string>

namespace ICL = boost::icl;
namespace Causalize {
  /// @brief This is the property for a vertex in the incidence graph. Nodes can be of two types: Equation or Unknow.
  class IndexPair;
  struct VectorUnknown: Unknown {
    int dimension;
    std::vector<int> dimensionList;
    VectorUnknown(){};
    VectorUnknown(VarInfo varInfo, Modelica::AST::Reference var);
    void SetIndex(Modelica::AST::ExpList index);
  };

  struct VectorVertexProperty: VertexProperty {
  /// @brief The number of equations or unknowns left to causalize in this node
    int count;
    VectorUnknown unknown;
  };

  /// @brief A pair representing a usage of a variable in an equation
  typedef ICL::discrete_interval<int> Interval;
  inline Interval CreateInterval(int a, int b) {
    return ICL::discrete_interval<int>(a,b, ICL::interval_bounds::closed());
  }
  typedef std::list<Interval> IntervalList;
  typedef std::vector<Interval> IntervalVector;


  /*****************************************************************************
   ****                              Usage                                  ****
   *****************************************************************************/
  class Usage {
  public:
    inline Usage():usage() { };
    inline Usage(int size):usage(std::vector<int>(size)) { }
    inline Usage(int size, int value):usage(std::vector<int>(size, value)) { }
    inline int& operator[](int index) { return usage[index]; }
    inline const int& operator[](int index) const { return usage[index]; }
    inline void push_back(const int i) { usage.push_back(i); }
    inline bool operator==(const Usage& other) const { return this->usage == other.usage; };
    inline bool operator!=(const Usage& other) const { return this->usage != other.usage; };
    inline int Size() { return usage.size(); }
    inline bool isUnused() {
      for(int i: usage) {
        if (i!=-1) return false;
      }
      return true;
    }
    typedef std::vector<int>::iterator iterator;
    typedef std::vector<int>::const_iterator const_iterator;
    inline const_iterator begin() const { return usage.begin(); }
    inline iterator begin() { return usage.begin(); }
    inline iterator end() { return usage.end(); }
  private:
    std::vector<int> usage;
  };
  /*****************************************************************************
   ****************************************************************************/



  /*****************************************************************************
   ****                              Offset                                  ****
   *****************************************************************************/
  class Offset {
  public:
    inline Offset(std::vector<int> offset): offset(offset) { };
    inline Offset(): offset() { };
    inline bool operator<(const Offset& other) const { return this->offset < other.offset; };
    inline bool operator==(const Offset& other) const { return this->offset == other.offset; };
    inline bool operator!=(const Offset& other) const { return this->offset != other.offset; };
    inline int operator[](const int& index) const { return offset[index]; };
    inline bool isZeros() { 
      for(int i: offset) {
        if (i!=0) return false;
      }
      return true;
    }
    Offset operator-() const;
    typedef std::vector<int>::iterator iterator;
    typedef std::vector<int>::const_iterator const_iterator;
    inline const_iterator begin() const { return offset.begin(); }
    inline iterator begin() { return offset.begin(); }
    inline iterator end() { return offset.end(); }
    inline unsigned int Size () const { return offset.size(); }
private:
      std::vector<int> offset;
  };
  /*****************************************************************************
   ****************************************************************************/


  /*****************************************************************************
   ****                               MDI                                   ****
   *****************************************************************************/
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

  /*****************************************************************************
   ****                           INDEX PAIR                                ****
   *****************************************************************************/
  class IndexPair {
  public:
    inline IndexPair() { };
    inline IndexPair(MDI dom_, MDI ran_, Offset os, Usage us): dom(dom_), ran(ran_), offset(os), usage(us) { };
    inline MDI Dom() const { return dom; }
    inline MDI Ran() const { return ran; }
    inline Offset GetOffset() const { return offset; }
    inline Usage GetUsage() const { return usage; }
    std::list<IndexPair> operator-(const IndexPair& other) const;
    std::list<IndexPair> Difference(const IndexPair& other) { return (*this) - other; }
    std::set<IndexPair> RemoveUnknowns(MDI unk2remove);
    std::set<IndexPair> RemoveEquations(MDI eqs2remove);
    bool operator<(const IndexPair& other) const;
    Option<IndexPair> operator&(const IndexPair& other) const;
    friend std::ostream& operator<<(std::ostream& os, const IndexPair& ip);
    bool Contains(const IndexPair& other) const;
    IndexPairType Type() const;
  private:
    MDI dom, ran;
    Offset offset;
    Usage usage;
  };
  /*****************************************************************************
   ****************************************************************************/


  std::ostream& operator<<(std::ostream &os, const std::list<IndexPair> &ipList);


  typedef std::set<IndexPair> IndexPairSet;
  std::ostream& operator<<(std::ostream& os, const IndexPairSet& ips);

  /*****************************************************************************
   ****                              LABEL                                  ****
   *****************************************************************************/
  class Label {
  public:
    inline Label() {};
    Label(IndexPairSet ips);
    void RemovePairs(IndexPairSet ips);
    void RemoveUnknowns(MDI const unk2remove);
    void RemoveEquations(MDI const mdi);
    unsigned long int EdgeCount();
    inline bool IsEmpty() { return ips.size()==0; }
    inline const IndexPairSet & Pairs() const { return ips; }
    friend std::ostream& operator<<(std::ostream& os, const Label& label);
  private:
    IndexPairSet ips;
    void RemoveDuplicates();
  };
  /*****************************************************************************
   ****************************************************************************/



  unsigned long int EdgeCount(IndexPairSet);


  /// @brief This is the definition of the Incidence graph for the vector case.
  typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, VectorVertexProperty, Label> VectorCausalizationGraph;
  /// @brief This a node from the vectorized incidence graph
  typedef Causalize::VectorCausalizationGraph::vertex_descriptor VectorVertex;
  /// @brief An equation vertex is the same as a regular vertex
  typedef VectorVertex VectorEquationVertex;
  /// @brief An unknown vertex is the same as a regular vertex
  typedef VectorVertex VectorUnknownVertex;
  /// @brief This is an edge of the vectorized causalization graph
  typedef VectorCausalizationGraph::edge_descriptor VectorEdge;
  /// @brief This struct represents a set of causalized vars for the vector algorithm

  struct CausalizedVar{
    VectorUnknown unknown;
    Equation equation;
    IndexPairSet pairs;
  };
}
#endif
