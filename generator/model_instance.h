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

#include <list>
#include <map>
#include <set>
#include <string>

#include "../ir/model.h"
#include "../util/compile_flags.h"
#include "../util/graph.h"
#include "../util/symbol_table.h"
#include "../util/util_types.h"
#include "writer.h"

/**
 *
 */
typedef enum
{
  SOL_MODEL_SETTINGS,
  SOL_MODEL,
  SOL_DEPS,
  SOL_ZC,
  SOL_HANDLER_POS,
  SOL_HANDLER_NEG,
  SOL_OUTPUT,
  SOL_INIT,
  SOL_CALLBACK
} SOL_Function;

/**
 *
 */
typedef enum
{
  SOL_LIBRARIES,
  SOL_INCLUDES
} SOL_Makefile;

/**
 *
 */
typedef enum
{
  SOL_SERIAL,
  SOL_PARALLEL
} SOL_Engine;

typedef enum
{
  NOD_SD,
  NOD_SZ,
  NOD_HD,
  NOD_HZ,
  NOD_DD
} NOD_Type;

/**
 *
 */
class MMO_ModelInstance
{
  public:
    /**
     *
     */
    virtual
    ~MMO_ModelInstance()
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
     * @param m
     * @return
     */
    virtual string
    makefile(SOL_Makefile m) = 0;
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
class QSSModelInstance: public MMO_ModelInstance
{
  public:
    /**
     *
     * @param model
     * @param flags
     * @param writer
     */
    QSSModelInstance(MMO_Model* model, MMO_CompileFlags* flags, MMO_Writer* writer);
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
     * @param m
     * @return
     */
    string
    makefile(SOL_Makefile m);
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
class ClassicModelInstance: public MMO_ModelInstance
{
  public:
    /**
     *
     * @param model
     * @param flags
     * @param writer
     */
    ClassicModelInstance(MMO_Model* model, MMO_CompileFlags* flags, MMO_Writer* writer);
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
     * @return
     */
    string
    runCmd();
    /**
     *
     * @param m
     * @return
     */
    string
    makefile(SOL_Makefile m);
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

#endif  /* MMO_MODEL_INSTANCE_H */

