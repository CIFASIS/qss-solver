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

#ifndef AST_TYPES
#define AST_TYPES
#include <boost/tuple/tuple.hpp>
#include <boost/fusion/include/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/foreach.hpp>
#include <boost/optional/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <list>

template <typename T>
struct List: public std::list<T> { 
public:
  List(): std::list<T>() {}; 
  List(T t) { push_back(t); }
  List(T t1, T t2) { push_back(t1);push_back(t2); }
};

template <typename T1, typename T2>
struct Pair: public boost::tuple<T1,T2> {
public:
  Pair(T1 t1, T2 t2): boost::tuple<T1,T2>(t1,t2) {};
};

template <typename T>
struct Option: public boost::optional<T> {
public:
  Option(): boost::optional<T>(){};
  Option(boost::optional<T> t): boost::optional<T>(t){};
  Option(T const &t ): boost::optional<T>(t){};

};

template <typename T>
std::vector<T> operator+(const std::vector<T> &A, const std::vector<T> &B)
{
    std::vector<T> AB;
    AB.reserve( A.size() + B.size() );                // preallocate memory
    AB.insert( AB.end(), A.begin(), A.end() );        // add A;
    AB.insert( AB.end(), B.begin(), B.end() );        // add B;
    return AB;
}

template <typename T>
std::vector<T> &operator+=(std::vector<T> &A, const std::vector<T> &B)
{
    A.reserve( A.size() + B.size() );                // preallocate memory without erase original data
    A.insert( A.end(), B.begin(), B.end() );         // add B;
    return A;                                        // here A could be named AB
}

namespace Modelica {
  namespace AST {
    enum TypePrefix {input, output, discrete, parameter, flow, stream, constant};
    enum ClassPrefix {partial, class_prefix, model, block, record, connector, type, operator_prefix, expandable, pure, impure, package, function };

    typedef std::vector<Option<TypePrefix> > TypePrefixes;
    typedef std::vector<Option<ClassPrefix> > ClassPrefixes;
    const char *classPrefix(Option<ClassPrefix>);
    const char *typePrefix(Option<TypePrefix>);
  }
}

// Some helpful macros
#define ApplyThis(X) boost::apply_visitor(*this,X)
#define Apply(X,Y) boost::apply_visitor(X,Y)
#define foreach_ BOOST_FOREACH
#define member_(X,Y) X Y##_; X Y() const; void set_##Y(X x); X &Y##_ref();
#define member_imp(C,X,Y) X C::Y() const { return Y##_;} void C::set_##Y(X x) { Y##_=x; } X &C::Y##_ref() { return Y##_; }
#define comparable(X) bool operator==(const X & other) const; 
#define printable(X) friend std::ostream& operator<<(std::ostream& out, const X &);
#define INDENTSPACE 2
#define INDENT std::string(depth,' ')
#define BEGIN_BLOCK depth+=INDENTSPACE;
#define END_BLOCK depth-=INDENTSPACE;
#define SKIP_BLOCK_START long depth_save = depth;depth=0;
#define SKIP_BLOCK_END depth=depth_save;
extern long depth;

#endif
