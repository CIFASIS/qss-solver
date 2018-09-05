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

#ifndef MMO_MODEL_INSTANCE_H_
#define MMO_MODEL_INSTANCE_H_

#include <boost/variant/variant.hpp>

#include "../ir/class.h"
#include "../util/compile_flags.h"
#include "../util/graph.h"
#include "../util/symbol_table.h"
#include "../util/util_types.h"
#include "writer.h"

namespace MicroModelica 
{
  namespace Generator 
  {

    /**
    *
    */
    typedef enum
    {
      MODEL_SETTINGS,
      MODEL,
      DEPS,
      ZC,
      HANDLER_POS,
      HANDLER_NEG,
      OUTPUT,
      JACOBIAN,
      CLC_INIT,
      QSS_INIT
    } Component;

    typedef enum
    {
      SD,
      SZ,
      HD,
      HZ,
      DD
    } NodeType;

    /**
    *
    */
    class ModelInstance
    {
      public:
        ModelInstance() {};
        ModelInstance(IR::Model& model, Util::CompileFlags& flags, WriterPtr writer);
        /**
        *
        */
        virtual
        ~ModelInstance() {};
        /**
        *
        * @return
        */
        void
        include();
        /**
        *
        */
        virtual void
        initializeDataStructures() = 0;
        /**
        *
        */
        virtual void
        definition() = 0;
        /**
        *
        */
        virtual void
        dependencies() = 0;
        /**
        *
        */
        void
        zeroCrossing();
        /**
        *
        */
        void
        handler();
        void 
        settings();
        /**
        *
        */
        void
        output();
        /**
        *
        * @return
        */
        virtual Graph
        computationalGraph() = 0;
        void 
        initialCode();
        virtual void 
        header();
        virtual void 
        generate() = 0;
      protected:
        std::string 
        componentDefinition(Component c);
        void 
        allocateOutput();
        void 
        initializeMatrix(Util::VariableDependencyMatrix vdm, Section alloc, Section init);
        void 
        handlerStatements(IR::StatementTable sts, Section simple, Section generic);
        string
        algebraics(Util::EquationDependencyMatrix eqdm, Util::depId key);
        void 
        allocateVectors();
        void 
        freeVectors();
      private:
        IR::Model           _model;
        Util::CompileFlags  _flags;
        WriterPtr           _writer;
    };

    /**
    *
    */
    class QSSModelInstance: public ModelInstance
    {
      public:
        QSSModelInstance(){};
        /**
        *
        * @param model
        * @param flags
        * @param writer
        */
        QSSModelInstance(IR::Model& model, Util::CompileFlags& flags, WriterPtr writer);
        /**
        *
        */
        ~QSSModelInstance() {};
        /**
        *
        */
        void
        initializeDataStructures();
        /**
        *
        */
        void
        definition();
        /**
        *
        */
        void
        dependencies();
        /**
        *
        * @return
        */
        Graph
        computationalGraph();
        void 
        generate();
        void 
        header();
    };

    /**
    *
    */
    class ClassicModelInstance: public ModelInstance
    {
      public:
        /**
        *
        * @param model
        * @param flags
        * @param writer
        */
        ClassicModelInstance(IR::Model& model, Util::CompileFlags& flags, WriterPtr writer);
        /**
        *
        */
        ~ClassicModelInstance() {};
        /**
        *
        */
        void
        initializeDataStructures();
        /**
        *
        */
        void
        definition();
        /**
        *
        */
        void
        dependencies();
        /**
        *
        * @return
        */
        Graph
        computationalGraph();
        void
        generate();
        void 
        header();
      private:
        void 
        allocateSolver();
        IR::Model           _model;
        Util::CompileFlags  _flags;
        WriterPtr           _writer;
    };
    
    typedef std::shared_ptr<ModelInstance> ModelInstancePtr;
  }
}
#endif  /* MMO_MODEL_INSTANCE_H */

