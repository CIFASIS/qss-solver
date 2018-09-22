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

#include <causalize/graph/graph_definition.h>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/icl/interval_set.hpp>
#include <boost/lexical_cast.hpp>


using namespace boost;
using namespace std;
using namespace boost::icl;
#define MAKE_SPACE for(int __i=0; __i<depth; __i++) stri << " ";
#define TAB_SPACE 2
#define INSERT_TAB depth += TAB_SPACE;
#define DELETE_TAB depth -= TAB_SPACE;

namespace Causalize {
  template <class VertexProperty, class EdgeProperty> 
  class GraphPrinter{

    typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, VertexProperty, EdgeProperty> Graph;
    typedef typename boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, VertexProperty, EdgeProperty>::vertex_descriptor Vertex;
    typedef typename boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, VertexProperty, EdgeProperty>::out_edge_iterator EdgeIterator;

	  public:
		  GraphPrinter(const Graph &g): graph(g) {
      	typename Graph::vertex_iterator vi, vi_end;
	      for(tie(vi, vi_end) = vertices(graph); vi!= vi_end; vi++){
		      if(graph[*vi].type == E){
      			equationDescriptors.push_back(*vi);
      		}else{
      		  unknownDescriptors.push_back(*vi);		
    		  }
    	  }
      };

		  void printGraph(std::string name) {
	      stringstream stri;
	      ofstream out(name.c_str());
	      int depth = 0;
        typedef typename list<Vertex>::iterator Iterator;
      
	      stri << "graph G{" << endl;
	      INSERT_TAB
		      MAKE_SPACE
		      stri << "subgraph cluster0{" << endl;
		      INSERT_TAB
			      MAKE_SPACE
			      stri << "label = \"Equations\";" << endl;
			      MAKE_SPACE
			      stri << "edge [style=invis];" << endl;
			      MAKE_SPACE
            stringstream colors2;
			      for(Iterator it=equationDescriptors.begin(); it!=equationDescriptors.end(); it++){
				      Iterator aux = it;
				      aux++;
				      stri << "eq" << graph[*it].index;
				      if((aux) != equationDescriptors.end()){
					      stri << " -- ";		
				      }else{
					      stri << ";" << endl;		
				      }
              if (out_degree(*it,graph)==0)
                colors2 << "    " << graph[*it].index << "[ color=\"red\" ];" << endl;
			      }
			      for(Iterator it=equationDescriptors.begin(); it!=equationDescriptors.end(); it++){
			        MAKE_SPACE
#ifdef HAS_COUNT
				      stri << "eq" << graph[*it].index << " [label=\"Eq. " << graph[*it].index << "\nCount=" << graph[*it].count << "\"];" << endl;
#else 
				      stri << "eq" << graph[*it].index << " [label=\"Eq. " << graph[*it].index << "\"];" << endl;

#endif
            }
            stri << colors2.str();
		      DELETE_TAB
		      MAKE_SPACE
		      stri << "}" << endl;
	      DELETE_TAB
      
      
	      INSERT_TAB
		      MAKE_SPACE
		      stri << "subgraph cluster1{" << endl;
		      INSERT_TAB
			      MAKE_SPACE
			      stri << "label = \"Unknowns\";" << endl;
			      MAKE_SPACE
			      stri << "edge [style=invis];" << endl;
			      MAKE_SPACE
            stringstream colors;
			      for(Iterator it=unknownDescriptors.begin(); it!=unknownDescriptors.end(); it++){
				      Iterator aux = it;
				      aux++;
				      stri << "var" << graph[*it].index;
				      if((aux) != unknownDescriptors.end()){
					      stri << " -- ";		
				      }else{
					      stri << ";" << endl;		
				      }
			      }
			      for(Iterator it=unknownDescriptors.begin(); it!=unknownDescriptors.end(); it++){
		          MAKE_SPACE
#ifdef HAS_COUNT
				      stri << "var" << graph[*it].index << " [ label = \"" << graph[*it].unknown() << "\nCount=" << graph[*it].count << "\"];" << endl;
#else
				      stri << "var" << graph[*it].index << " [ label = \"" << graph[*it].unknown() << "\"];" << endl;
#endif
            }
		      DELETE_TAB
		      MAKE_SPACE
		      stri << colors.str();
		      stri << "}" << endl;
	      DELETE_TAB
      
	      INSERT_TAB
		      MAKE_SPACE
		      stri << "edge [constraint=false];" << endl;
		      for(Iterator eq_it = equationDescriptors.begin(); eq_it != equationDescriptors.end(); eq_it++){
			      EdgeIterator ei, ei_end;
			      for(tie(ei, ei_end) = out_edges(*eq_it, graph); ei != ei_end; ei++){
				      Vertex unknown = target(*ei, graph);
				      MAKE_SPACE;
				      string name;
    	        stri << "eq" << graph[*eq_it].index << " -- var" << graph[unknown].index;
              EdgeProperty ep = graph[*ei];
              stri << "[label = \"" << ep << "\"];";
			      }
		      }
	      DELETE_TAB
	      stri << "}" << endl;
	      out << stri.str();
	      out.close();
#ifdef __linux__
        size_t lastindex = name.find_last_of("."); 
        string rawname = name.substr(0, lastindex); 
	      stringstream command;
        command << "/usr/bin/dot -T eps " << name << " >" << rawname << ".eps";
        if (system(command.str().c_str()));
        command.str(std::string());
        command << "/usr/bin/dot -T jpg " << name << " >" << rawname << ".jpg";
        if (system(command.str().c_str()));
#endif
      }
	  private:
		  const Graph &graph;
		  std::list<Vertex> equationDescriptors;
		  std::list<Vertex> unknownDescriptors;
  };
}
