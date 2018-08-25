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
      INIT,
      CALLBACK
    } Component;

    /**
    *
    */
    typedef enum
    {
      LIBRARIES,
      INCLUDES
    } Makefile;

    /**
     *
    */
    typedef enum
    {
      SERIAL,
      PARALLEL
    } Engine;

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
        /**
        *
        */
        virtual
        ~ModelInstance()
        {
        };
        /**
        *
        * @return
        */
        virtual string
        header() = 0;
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
        virtual void
       zeroCrossing() = 0;
        /**
        *
        */
        virtual void
        handler() = 0;
        /**
        *
        */
        virtual void
        output() = 0;
        /**
        *
        * @return
        */
        virtual Graph
        computationalGraph() = 0;
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
        ~QSSModelInstance();
        /**
        *
        * @return
        */
        string
        header();
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
        */
        void
        zeroCrossing();
        /**
        *
        */
        void
        handler();
        /**
        *
        */
        void
        output();
        /**
        *
        * @return
        */
        Graph
        computationalGraph();
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
        ~ClassicModelInstance();
        /**
        *
        * @return
        */
        string
        header();
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
        */
        void
        zeroCrossing();
        /**
        *
        */
        void
        handler();
        /**
        *
        */
        void
        output();
        /**
        *
        * @return
        */
        Graph
        computationalGraph();
      private:
        IR::Model               _model;
        Util::CompileFlags  _flags;
        WriterPtr           _writer;
    };
    
    typedef std::shared_ptr<ModelInstance> ModelInstancePtr;
  }
}
#endif  /* MMO_MODEL_INSTANCE_H */

