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

#include "solver.h"

#include <sstream>
#include <utility>

#include "../ast/expression.h"
#include "../ir/annotation.h"
#include "../ir/class.h"
#include "../ir/equation.h"
#include "../ir/event.h"
#include "../ir/expression.h"
#include "../ir/statement.h"
#include "../util/compile_flags.h"
#include "../util/error.h"
#include "../util/util.h"

/* QSS Solver class. */

QSS_::QSS_ (MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer) :
        _flags (flags), _writer (writer), _modelVars (), _modelDepsVars (), _zcVars (), _handlerPosVars (), _handlerNegVars (), _outputVars (), _initializeVars (), _freeVars (), _model (
                model), _name (model->name ()), _graph (model->states (), model->evs ()), _parallel (flags->graph ()), _hasDD (false)
{
    _modelDeps = newMMO_DependenciesTable ();
    _modelVectorDeps = newMMO_DependenciesTable ();
    if (_flags->parallel ())
    {
        _engine = newMMO_ParallelEngine (_flags, _model->evs ());
    }
    else
    {
        _engine = newMMO_SerialEngine (_flags, _model->evs ());
    }
    if (_flags->hasOutputFile ())
    {
        _name = _flags->outputFileName ();
    }
    _common = newSolverCommon (model, flags, writer, _modelVectorDeps, &_graph);
    _model->varTable ()->setPolyCoeffs (_model->annotation ()->polyCoeffs ());
}

QSS_::~QSS_ ()
{
    delete _modelDeps;
    delete _modelVectorDeps;
    delete _engine;
    delete _common;
}

string
QSS_::initData ()
{
    stringstream buffer;
    MMO_Annotation annot = _model->annotation ();
    buffer << "simulator->data = QSS_Data(" << _model->states () << "," << _model->discretes () << "," << _model->evs () << "," << _model->inputs () << ","
            << _model->algs () << ",\"" << _name << "\");" << endl;
    buffer << "  QSS_data modelData = simulator->data;" << endl;
    buffer << "  const double t = " << annot->initialTime () << ";" << endl;
    return (buffer.str ());
}

string
QSS_::initTime ()
{
    stringstream buffer;
    MMO_Annotation annot = _model->annotation ();
    string weightsStr = "NULL";
    map<Index, double> w = _common->weights ();
    if (w.size ())
    {
        weightsStr = "weights";
        buffer << "double weights[" << _model->evs () << "];";
        _writer->write (&buffer, WR_INIT_TIME_STRUCT);
        buffer << "for(i = 0; i <= " << _model->evs () << "; i++)";
        _writer->write (&buffer, WR_INIT_TIME_STRUCT);
        buffer << "{";
        _writer->write (&buffer, WR_INIT_TIME_STRUCT);
        buffer << "weights[i] = 0;";
        _writer->write (&buffer, WR_INIT_TIME_STRUCT);
        buffer << "}";
        _writer->write (&buffer, WR_INIT_TIME_STRUCT);
        map<Index, double>::iterator it;
        for (it = w.begin (); it != w.end (); it++)
        {
            Index idx = it->first;
            if (idx.isArray ())
            {
                buffer << "for(i = " << idx.mappedBegin () << "; i <= " << idx.mappedEnd () << "; i++)";
                _writer->write (&buffer, WR_INIT_TIME_STRUCT);
                buffer << "{";
                _writer->write (&buffer, WR_INIT_TIME_STRUCT);
            }
            buffer << "weights[" << idx.print ("i") << "] = " << it->second << ";";
            _writer->write (&buffer, WR_INIT_TIME_STRUCT);
            if (idx.hasRange ())
            {
                buffer << "}";
                _writer->write (&buffer, WR_INIT_TIME_STRUCT);
            }
        }
    }
    buffer << "simulator->time = QSS_Time(" << _model->states () << "," << _model->evs () << "," << _model->inputs () << "," << annot->initialTime () << ","
            << annot->scheduler () << "," << weightsStr << ");" << endl;
    return (buffer.str ());
}

string
QSS_::initOutput ()
{
    return (_common->initOutput ());
}

string
QSS_::initModel ()
{
    stringstream buffer;
    string zeroCrossing = "NULL", handlerPos = "NULL", handlerNeg = "NULL";
    if (!_writer->isEmpty (WR_ZC_SIMPLE) || !_writer->isEmpty (WR_ZC_GENERIC))
    {
        zeroCrossing = "MOD_zeroCrossing";
    }
    if (!_writer->isEmpty (WR_HANDLER_POS_SIMPLE) || !_writer->isEmpty (WR_HANDLER_POS_GENERIC))
    {
        handlerPos = "MOD_handlerPos";
    }
    if (!_writer->isEmpty (WR_HANDLER_NEG_SIMPLE) || !_writer->isEmpty (WR_HANDLER_NEG_GENERIC))
    {
        handlerNeg = "MOD_handlerNeg";
    }
    buffer << "simulator->model = QSS_Model(MOD_definition,MOD_dependencies," << zeroCrossing << "," << handlerPos << "," << handlerNeg << ");";
    return (buffer.str ());
}

string
QSS_::initHeader ()
{
    stringstream buffer;
    buffer << "#include <common/model.h>" << endl;
    buffer << "#include <common/commands.h>" << endl;
    buffer << "#include <qss/qss_model.h>" << endl;
    buffer << "#include <classic/classic_model.h>" << endl;
    return (buffer.str ());
}

string
QSS_::runCmd ()
{
    return ("./");
}

string
QSS_::makefile (SOL_Makefile m)
{
    return (_engine->makefile (m));
}

/*! @brief Compares two variable indexes and return the intersection (if any) between them.
 *
 * 	@param idx: Equation index. \f$ a_e*i+b_e \f$ where \f$ i \in [A_e,B_e] \f$
 * 	@param dIdx: Discrete variable index. \f$ a_ed*i+b_ed \f$ where \f$ i \in [A_ed,B_ed] \f$
 * 	@param infIdx: Influenced equation index.\f$ a_ie*i+b_ie \f$ where \f$ i \in [A_ie,B_ie] \f$
 * 	@param infDIdx: Influenced equation state variable index.\f$ a_ied*i+b_ied \f$ where \f$ i \in [A_ied,B_ied] \f$
 *
 *  Each index has the form: \f$ a*i+b \f$ where \f$ i \in [A,B] \f$
 *
 * 	The possible intersections taken into account are:
 * 		- IDX_DISJOINT: No intersection found.
 * 		- IDX_EQUAL: in this case we have \f$ a_ed = a_ied = 0 \wedge b_ed = b_ied \f$, and \f$ A_e = B_e  \vee A_e \neq B_e \f$.
 */

bool
QSS_::_indexDependencies (Index idx, Index *dIdx, Index infIdx, Index *infDIdx, map<int, int> *simpleMatrixDeps, WR_Section alloc, WR_Section init,
                          string allocStr, string initStr, string counter, Intersection intersection, int assignments)
{
    stringstream buffer;
    string indent = _writer->indent (1);
    bool found = true;
    NOD_Type nt = NOD_HD;
    int nodOffset = _model->states ();
    if (_parallel)
    {
        if (alloc == WR_ALLOC_LD_HZ)
        {
            nt = NOD_HZ;
        }
        if (alloc == WR_ALLOC_LD_DD)
        {
            nt = NOD_DD;
        }
    }
    switch (intersection.type ())
        {
        case IDX_DISJOINT:
            found = false;
            break;
        case IDX_EQUAL:
            if (idx.hasRange ())
            {
                buffer << "for(i = " << idx.mappedBegin () << "; i <= " << idx.mappedEnd () << "; i++)";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                buffer << "{";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                if (infIdx.hasRange ())
                {
                    _common->addLocalVar ("j", &_initializeVars);
                    buffer << indent << allocStr << "[i] += " << infIdx.range () << ";";
                    _writer->write (&buffer, alloc);
                    buffer << "}";
                    _writer->write (&buffer, alloc);
                    buffer << indent << "for(j = " << infIdx.begin () << "; j <= " << infIdx.end () << "; j++)";
                    _writer->write (&buffer, init);
                    buffer << indent << "{";
                    _writer->write (&buffer, init);
                    buffer << _writer->indent (2) << initStr << "[i][" << counter << "[i]++] = " << infIdx.print ("j", -infIdx.begin ()) << ";";
                    _writer->write (&buffer, init);
                    buffer << indent << "}";
                    _writer->write (&buffer, init);
                    buffer << "}";
                    _writer->write (&buffer, init);
                    if (_parallel)
                    {
                        Index inf (infIdx);
                        inf.setOffset (inf.offset () - infIdx.begin ());
                        Index st (idx);
                        st.setOffset (0);
                        for (int i = infIdx.begin (); i < infIdx.end (); i++)
                        {
                            Index insertIdx = inf.indexValue (i);
                            _common->graphInsert (st, insertIdx, nodOffset, nt, assignments);
                        }
                    }
                }
                else
                {
                    buffer << indent << allocStr << "[i]++;";
                    _writer->write (&buffer, alloc);
                    buffer << indent << initStr << "[i][" << counter << "[i]++] = " << infIdx.print ("i", -infIdx.begin ()) << ";";
                    _writer->write (&buffer, init);
                    buffer << "}";
                    _writer->write (&buffer, alloc, false);
                    _writer->write (&buffer, init);
                    if (_parallel)
                    {
                        Index inf (infIdx);
                        inf.setOffset (inf.offset () - infIdx.begin ());
                        Index st (idx);
                        st.setOffset (0);
                        _common->graphInsert (st, inf, nodOffset, nt, assignments);
                    }
                }
            }
            else
            {
                int nIdx = idx.mappedConstant ();
                string idxStr = idx.print ();
                if (infIdx.hasRange ())
                {
                    buffer << "for(i = " << infIdx.begin () << "; i <= " << infIdx.end () << "; i++)";
                    _writer->write (&buffer, init);
                    buffer << "{";
                    _writer->write (&buffer, init);
                    buffer << indent << initStr << "[" << idxStr << "][" << counter << "[" << idxStr << "]++] = " << infIdx.print ("i", -infIdx.begin ())
                            << ";";
                    _writer->write (&buffer, init);
                    buffer << "}";
                    _writer->write (&buffer, init);
                    if (_parallel)
                    {
                        Index inf (infIdx);
                        inf.setOffset (inf.offset () - infIdx.begin ());
                        for (int i = infIdx.low (); i <= infIdx.hi (); i++)
                        {
                            Index insertIdx = inf.indexValue (i);
                            Index st = idx.indexValue (nIdx);
                            _common->graphInsert (st, insertIdx, nodOffset, nt, assignments);
                        }
                    }
                }
                else
                {
                    buffer << initStr << "[" << idxStr << "][" << counter << "[" << idxStr << "]++] = " << infIdx.print ("i") << ";";
                    _writer->write (&buffer, init);
                    if (_parallel)
                    {
                        Index inf (infIdx);
                        inf.setOffset (inf.offset ());
                        Index st (idx);
                        _common->graphInsert (st, inf, nodOffset, nt, assignments);
                    }
                }
                map<int, int>::iterator smd = simpleMatrixDeps->find (nIdx);
                if (smd == simpleMatrixDeps->end ())
                {
                    simpleMatrixDeps->insert (pair<int, int> (nIdx, infIdx.range ()));
                }
                else
                {
                    smd->second += infIdx.range ();
                }
            }
            break;
        case IDX_SUBSET:
            {
                Index tmp (infIdx);
                if (dIdx->hasMap ())
                {
                    tmp.setMap (*dIdx);
                }
                int begin = idx.mappedBegin () + infDIdx->lowValue () - dIdx->lowValue ();
                buffer << "for(i = " << begin << "; i <= " << begin + infDIdx->range () - 1 << "; i++)";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                buffer << "{";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                buffer << indent << allocStr << "[i]++;";
                _writer->write (&buffer, alloc);
                buffer << indent << initStr << "[i][" << counter << "[i]++] = " << tmp.print ("i", -begin + infIdx.begin () - idx.begin ()) << ";";
                _writer->write (&buffer, init);
                buffer << "}";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                if (_parallel)
                {
                    Index inf (infIdx);
                    inf.setOffset (inf.offset () - begin);
                    Index st (idx);
                    st.setOffset (0);
                    st.setLow (begin + 1);
                    st.setHi (begin + infDIdx->range ());
                    _common->graphInsert (st, inf, nodOffset, nt, assignments);
                }
            }
            break;
        case IDX_SUBSET_OF:
            {
                Index tmp (infIdx);
                if (dIdx->hasMap ())
                {
                    tmp.setMap (*dIdx);
                }
                buffer << "for(i = " << idx.mappedBegin () << "; i <= " << idx.mappedEnd () << "; i++)";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                buffer << "{";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                buffer << indent << allocStr << "[i]++;";
                _writer->write (&buffer, alloc);
                buffer << indent << initStr << "[i][" << counter << "[i]++] = " << tmp.print ("i", -idx.mappedBegin () + idx.begin ()) << ";";
                _writer->write (&buffer, init);
                buffer << "}";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                if (_parallel)
                {
                    Index inf (*dIdx);
                    inf.setOffset (inf.offset () - dIdx->offset () + infIdx.mappedBegin ());
                    Index st (idx);
                    st.setOffset (0);
                    _common->graphInsert (st, inf, nodOffset, nt, assignments);
                }
            }
            break;
        case IDX_ARB_AB:
            {
                buffer << "for( i = " << idx.mappedBegin () << "; i <= ";
                buffer << idx.mappedBegin () + dIdx->lowValue () - infDIdx->hiValue () << "; i++)";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                buffer << "{";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                buffer << indent << allocStr << "[i]++;";
                _writer->write (&buffer, alloc);
                buffer << indent << initStr << "[i][" << counter << "[i]++] = " << dIdx->print ("i", -dIdx->offset () + infIdx.mappedBegin () + idx.low ())
                        << ";";
                _writer->write (&buffer, init);
                buffer << "}";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                if (_parallel)
                {
                    Index inf (infIdx);
                    inf.setOffset (inf.offset () - dIdx->offset () + infIdx.mappedBegin () + idx.low ());
                    Index st (idx);
                    st.setOffset (0);
                    st.setHi (idx.mappedBegin () + dIdx->lowValue () - infDIdx->hiValue () + 1);
                    _common->graphInsert (st, inf, nodOffset, nt, assignments);
                }
            }
            break;
        case IDX_ARB_BA:
            {
                int begin = idx.mappedBegin () + infIdx.begin () - idx.begin ();
                buffer << "for(i = " << begin << "; i <= ";
                buffer << idx.mappedEnd () << "; i++)";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                buffer << "{";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                buffer << indent << allocStr << "[i]++;";
                _writer->write (&buffer, alloc);
                buffer << indent << initStr << "[i][" << counter << "[i]++] = " << infIdx.print ("i", -begin) << ";";
                _writer->write (&buffer, init);
                buffer << "}";
                _writer->write (&buffer, alloc, false);
                _writer->write (&buffer, init);
                if (_parallel)
                {
                    Index inf (infIdx);
                    inf.setOffset (inf.offset () - begin);
                    Index st (idx);
                    st.setOffset (0);
                    st.setLow (begin + 1);
                    _common->graphInsert (st, inf, nodOffset, nt, assignments);
                }
            }
            break;
        default:
            {
                int loc = idx.mappedBegin () + intersection.value ();
                buffer << allocStr << "[" << loc << "]++;";
                _writer->write (&buffer, alloc);
                buffer << initStr << "[" << loc << "][" << counter << "[" << loc << "]++] = " << infIdx.print ("i") << ";";
                _writer->write (&buffer, init);
                if (_parallel)
                {
                    Index st = idx.indexValue (loc + 1);
                    st.setOffset (0);
                    _common->graphInsert (st, infIdx, nodOffset, nt, assignments);
                }
            }
            break;
        }
    return (found);
}

void
QSS_::_eventDeps (MMO_Event e, Index index, MMO_EventTable evt, DEP_Type type, map<Index, Index> events)
{
    string indent = _writer->indent (1);
    stringstream buffer;
    Dependencies deps = e->lhs ();
    int assignments = deps->discretes ();
    map<int, int> simpleZCNDeps;
    map<int, int> simpleDDDependencies;
    for (Index *hndIdx = deps->begin (type); !deps->end (type); hndIdx = deps->next (type))
    {
        _setInterval (hndIdx, &index);
        int evCount = evt->count ();
        for (int eNum = 0; eNum < evCount; eNum++)
        {
            MMO_Event ev = evt->val (eNum);
            Index ecIndex = evt->key (eNum);
            Dependencies zcDeps = ev->condition ()->exp ()->deps ();
            for (Index *zcIdx = zcDeps->begin (type); !zcDeps->end (type); zcIdx = zcDeps->next (type))
            {
                _setInterval (zcIdx, &ecIndex);
                Intersection is = hndIdx->intersection (*zcIdx);
                if (is.type () != IDX_DISJOINT)
                {
                    events[ecIndex] = ecIndex;
                }
                _indexDependencies (index, hndIdx, ecIndex, zcIdx, &simpleZCNDeps, WR_ALLOC_LD_HZ, WR_INIT_LD_HZ, "modelData->nHZ", "modelData->HZ", "events",
                                    is, assignments);
            }
            if (_parallel && type == DEP_DISCRETE)
            {
                Dependencies hndDeps = ev->deps ();
                for (Index *hdIdx = hndDeps->begin (type); !hndDeps->end (type); hdIdx = hndDeps->next (type))
                {
                    if (e->index ().hasRange () && e->index () == ev->index () && *hndIdx == *hdIdx && hndIdx->hasRange ())
                    {
                        continue;
                    }
                    _setInterval (hdIdx, &ecIndex);
                    Intersection is = hndIdx->intersection (*hdIdx);
                    if (is.type () != IDX_DISJOINT)
                    {
                        if (events.find (ecIndex) != events.end ())
                        {
                            continue;
                        }
                        events[ecIndex] = ecIndex;
                        if (hndIdx->hasRange ())
                        {
                            _hasDD = true;
                        }
                    }
                    _indexDependencies (index, hndIdx, ecIndex, hdIdx, &simpleDDDependencies, WR_ALLOC_LD_DD, WR_INIT_LD_DD, "modelData->nDD", "modelData->DD",
                                        "events", is, assignments);
                }
                for (Index *hdIdx = hndDeps->begin (DEP_DISCRETE_VECTOR); !hndDeps->end (DEP_DISCRETE_VECTOR); hdIdx = hndDeps->next (DEP_DISCRETE_VECTOR))
                {
                    if (e->index ().hasRange () && e->index () == ev->index () && *hndIdx == *hdIdx && hndIdx->hasRange ())
                    {
                        continue;
                    }
                    _setInterval (hdIdx, &ecIndex);
                    Intersection is = hndIdx->intersection (*hdIdx);
                    if (is.type () != IDX_DISJOINT)
                    {
                        if (events.find (ecIndex) != events.end ())
                        {
                            continue;
                        }
                        events[ecIndex] = ecIndex;
                        string fi = Util::getInstance ()->newVarName ("tmp", _model->varTable ());
                        _common->addLocalVar (fi, &_initializeVars);
                        string sIdx = e->index ().print (fi);
                        string hhIdx = ev->index ().print (fi);
                        buffer << "for(" << fi << " = " << is.begin () << "; " << fi << " <= " << is.end () << "; " << fi << "++)";
                        _writer->write (&buffer, WR_ALLOC_LD_DD, false);
                        _writer->write (&buffer, WR_INIT_LD_DD);
                        _writer->write ("{", WR_ALLOC_LD_DD);
                        _writer->write ("{", WR_INIT_LD_DD);
                        buffer << indent << "modelData->nDD[" << sIdx << "]++;";
                        _writer->write (&buffer, WR_ALLOC_LD_DD);
                        buffer << indent << "modelData->DD[" << sIdx << "][events[" << sIdx << "]++] = " << hhIdx << ";";
                        _writer->write (&buffer, WR_INIT_LD_DD);
                        _writer->write ("}", WR_ALLOC_LD_DD);
                        _writer->write ("}", WR_INIT_LD_DD);
                        _common->graphInsert (e->index (), ev->index (), _model->states (), NOD_DD, assignments);
                    }
                }
                /* if (!(ev->index () == e->index ()))
                 {
                 hndDeps = ev->lhs ();
                 for (Index *hdIdx = hndDeps->begin (type);
                 !hndDeps->end (type); hdIdx = hndDeps->next (type))
                 {
                 if (e->index ().hasRange () && e->index () == ev->index ()
                 && *hndIdx == *hdIdx && hndIdx->hasRange ())
                 {
                 continue;
                 }
                 _setInterval (hdIdx, &ecIndex);
                 Intersection is = hndIdx->intersection (*hdIdx);
                 if (is.type () != IDX_DISJOINT)
                 {
                 if (events.find (ecIndex) != events.end ())
                 {
                 continue;
                 }
                 events[ecIndex] = ecIndex;
                 if (hndIdx->hasRange ())
                 {
                 _hasDD = true;
                 }
                 }
                 _indexDependencies (index, hndIdx, ecIndex, hdIdx,
                 &simpleDDDependencies, WR_ALLOC_LD_DD,
                 WR_INIT_LD_DD, "modelData->nDD",
                 "modelData->DD", "events", is,
                 assignments);
                 }
                 }*/
            }
        }
    }
    for (map<int, int>::iterator it = simpleDDDependencies.begin (); it != simpleDDDependencies.end (); it++)
    {
        buffer << "modelData->nDD[" << it->first << "] += " << it->second << ";";
        _writer->write (&buffer, WR_ALLOC_LD_DD);
    }
    for (map<int, int>::iterator it = simpleZCNDeps.begin (); it != simpleZCNDeps.end (); it++)
    {
        buffer << "modelData->nHZ[" << it->first << "] += " << it->second << ";";
        _writer->write (&buffer, WR_ALLOC_LD_HZ);
    }
}

Graph
QSS_::graph ()
{
    if (_common->hasGraph ())
    {
        return (_graph);
    }
    return (Graph (0, 0));
}

void
QSS_::_eventAlgebraicDeps (MMO_Event e, Index index, MMO_EventTable evt, DEP_Type type, map<Index, Index> events)
{
    string indent = _writer->indent (1);
    stringstream buffer;
    Dependencies deps = e->lhs ();
    int assignments = deps->discretes ();
    map<int, int> simpleZCNDeps;
    DEP_Type eventType = DEP_STATE;
    if (type == DEP_ALGEBRAIC_DISCRETE)
    {
        eventType = DEP_DISCRETE;
    }
    for (Index *hndIdx = deps->begin (eventType); !deps->end (eventType); hndIdx = deps->next (eventType))
    {
        _setInterval (hndIdx, &index);
        int evCount = evt->count ();
        for (int eNum = 0; eNum < evCount; eNum++)
        {
            MMO_Event ev = evt->val (eNum);
            Index ecIndex = evt->key (eNum);
            if (events.find (ecIndex) != events.end ())
            {
                continue;
            }
            Dependencies zcDeps = ev->condition ()->exp ()->deps ();
            for (zcDeps->begin (type); !zcDeps->end (type); zcDeps->next (type))
            {
                Index algVar = zcDeps->key (type);
                _setInterval (&algVar, &ecIndex);
                Intersection is = hndIdx->intersection (algVar);
                _indexDependencies (index, hndIdx, ecIndex, &algVar, &simpleZCNDeps, WR_ALLOC_LD_HZ, WR_INIT_LD_HZ, "modelData->nHZ", "modelData->HZ", "events",
                                    is, assignments);
            }
        }
    }
    for (map<int, int>::iterator it = simpleZCNDeps.begin (); it != simpleZCNDeps.end (); it++)
    {
        buffer << "modelData->nHZ[" << it->first << "] += " << it->second << ";";
        _writer->write (&buffer, WR_ALLOC_LD_HZ);
    }
}

void
QSS_::_setInterval (Index *i1, Index *i2)
{
    if (i1->factor () != 0)
    {
        if (i2->hasRange ())
        {
            i1->setRange ();
        }
    }
}

void
QSS_::_eventVectorDependencies (Index index, Dependencies deps, WR_Section alloc, WR_Section init, string allocString, string initString, DEP_Type type)
{
    stringstream buffer;
    string indent = _writer->indent (1);
    string forIdx = "i";
    string eqsIdx = index.print ("i");
    int numDeps = deps->stateVectors ();
    if (type == DEP_DISCRETE_VECTOR)
    {
        numDeps = deps->discreteVectors ();
    }
    if (numDeps)
    {
        buffer << indent << "modelData->event[" << eqsIdx << "]." << allocString << " += " << numDeps << ";";
        _writer->write (&buffer, alloc);
    }
    for (Index *idx = deps->begin (type); !deps->end (type); idx = deps->next (type))
    {
        Index dIdx (*idx);
        string sIdx = dIdx.print (forIdx);
        buffer << "for(" << forIdx << " = " << idx->begin () << "; " << forIdx << " <= " << idx->end () << "; " << forIdx << "++)";
        _writer->write (&buffer, init);
        _writer->write ("{", init);
        buffer << indent << "modelData->event[" << eqsIdx << "]." << initString << "[events[" << eqsIdx << "]++] = " << sIdx << ";";
        _writer->write (&buffer, init);
        _writer->write ("}", init);
        if (_parallel && type == DEP_STATE_VECTOR)
        {
            buffer << "for(" << forIdx << " = " << idx->begin () << "; " << forIdx << " <= " << idx->end () << "; " << forIdx << "++)";
            _writer->write (&buffer, WR_INIT_STATE_HANDLERS);
            _writer->write ("{", WR_INIT_STATE_HANDLERS);
            buffer << indent << "modelData->SH[" << sIdx << "][states[" << sIdx << "]++] = " << eqsIdx << ";";
            _writer->write (&buffer, WR_INIT_STATE_HANDLERS);
            _writer->write ("}", WR_INIT_STATE_HANDLERS);
            buffer << "for(" << forIdx << " = " << idx->begin () << "; " << forIdx << " <= " << idx->end () << "; " << forIdx << "++)";
            _writer->write (&buffer, WR_ALLOC_STATE_HANDLERS);
            _writer->write ("{", WR_ALLOC_STATE_HANDLERS);
            buffer << indent << "modelData->nSH[" << sIdx << "]++;";
            _writer->write (&buffer, WR_ALLOC_STATE_HANDLERS);
            _writer->write ("}", WR_ALLOC_STATE_HANDLERS);
        }
    }
}

bool
QSS_::_controlIntersections (list<Intersection> lis, Intersection is)
{
    for (list<Intersection>::iterator it = lis.begin (); it != lis.end (); it++)
    {
        if (it->equal (is))
        {
            return (true);
        }
    }
    return (false);
}

/*! \brief Initialize model matrices.
 *
 * 	Description:
 * 		For each equation defined on the equation section of the model:
 * 		-# Generate code for the DS (Derivative -- State) incidence matrix.
 * 		-# For each state variable involved in the equation:
 * 			+# Generate code to update the number of influenced variables in SD (State--Derivatives) matrix.
 * 			+# Generate the transpose matrix SD. Flatten algebraic dependencies.
 *
 */

void
QSS_::initializeMatrices ()
{
    string indent;
    stringstream buffer;
    _initializeVars["int i;"] = "int i;";
    buffer << "int *states = (int*)malloc(" << _model->states () << "*sizeof(int));";
    _initializeVars[buffer.str ()] = buffer.str ();
    _freeVars["states"] = "states";
    buffer.str ("");
    if (_model->evs ())
    {
        buffer << "int *events = (int*)malloc(" << _model->evs () << "*sizeof(int));";
        _initializeVars[buffer.str ()] = buffer.str ();
        _freeVars["events"] = "events";
        buffer.str ("");
    }
    MMO_EquationTable et = _model->derivatives ();
    MMO_EventTable evt = _model->events ();
    stringstream bufferGen;
    bool genericEquation = false;
    bool hasInit = false;
    map<int, int> simpleHDDeps;
    for (MMO_Equation e = et->begin (); !et->end (); e = et->next ())
    {
        Index index = et->key ();
        string eqsIdx = e->lhs ().print ("i");
        Dependencies deps = e->exp ()->deps ();
        if (et->endGenericDefinition ())
        {
            genericEquation = false;
            bufferGen.str ("");
            if (hasInit)
            {
                indent = "";
                hasInit = false;
                _writer->write ("}", WR_ALLOC_LD_DS);
                _writer->write ("}", WR_ALLOC_LD_SD);
                _writer->write ("}", WR_INIT_LD_DS);
                _writer->write ("}", WR_INIT_LD_SD);
            }
        }
        if (et->beginGenericDefinition ())
        {
            genericEquation = true;
            if (!hasInit)
            {
                bufferGen << "for(i = " << index.begin () << "; i <= " << index.end () << "; i++)";
            }
        }
        if (deps->hasStates () && !bufferGen.str ().empty ())
        {
            indent = _writer->indent (1);
            hasInit = true;
            _writer->write (&bufferGen, WR_ALLOC_LD_DS, false);
            _writer->write (&bufferGen, WR_INIT_LD_DS, false);
            _writer->write (&bufferGen, WR_INIT_LD_SD, false);
            _writer->write (&bufferGen, WR_ALLOC_LD_SD);
            buffer << "{";
            _writer->write (&buffer, WR_ALLOC_LD_DS, false);
            _writer->write (&buffer, WR_INIT_LD_DS, false);
            _writer->write (&buffer, WR_INIT_LD_SD, false);
            _writer->write (&buffer, WR_ALLOC_LD_SD);
        }
        if (deps->states ())
        {
            buffer << indent << "modelData->nDS[" << eqsIdx << "] = " << deps->states () << ";";
            _writer->write (&buffer, WR_ALLOC_LD_DS);
        }
        if (!deps->autonomous ())
        {
            _common->addLocalVar ("td", &_initializeVars);
            if (e->lhs ().hasRange ())
            {
                buffer << indent << "for (i = " << e->lhs ().begin () << "; i <= " << e->lhs ().end () << ";i++)";
                _writer->write (&buffer, WR_INIT_TIME);
                _writer->write ("{", WR_INIT_TIME);
            }
            buffer << indent << "modelData->TD[td++] = " << eqsIdx << ";";
            _writer->write (&buffer, WR_INIT_TIME);
            if (e->lhs ().hasRange ())
            {
                _writer->write ("}", WR_INIT_TIME);
            }
        }
        map<Index, Index> defStates;
        for (Index *idx = deps->begin (DEP_STATE); !deps->end (DEP_STATE); idx = deps->next (DEP_STATE))
        {
            Index dIdx (*idx);
            defStates[dIdx] = dIdx;
            string sIdx = idx->print ("i");
            buffer << indent << "modelData->DS[" << eqsIdx << "][states[" << eqsIdx << "]++] = " << sIdx << ";";
            _writer->write (&buffer, WR_INIT_LD_DS);
            buffer << indent << "modelData->nSD[" << sIdx << "]++;";
            _writer->write (&buffer, WR_ALLOC_LD_SD);
            buffer << indent << "modelData->SD[" << sIdx << "][states[" << sIdx << "]++] = " << eqsIdx << ";";
            _writer->write (&buffer, WR_INIT_LD_SD);
            _common->addModelDeps (deps, dIdx, e->lhs (), _modelDeps);
            if (_parallel)
            {
                _common->graphInsert (dIdx, e->lhs ());
            }
        }
        _common->addAlgebriacDeps (deps, e->lhs (), defStates, "modelData->nDS", "modelData->nSD", "modelData->DS", "modelData->SD", WR_ALLOC_LD_ALG_DS,
                                   WR_ALLOC_LD_ALG_SD, WR_INIT_LD_ALG_DS, WR_INIT_LD_ALG_SD, "states", "states", DEP_ALGEBRAIC_STATE, _modelDeps);
        map<Index, list<Intersection> > HD;
        for (Index *dIdx = deps->begin (DEP_DISCRETE); !deps->end (DEP_DISCRETE); dIdx = deps->next (DEP_DISCRETE))
        {
            _setInterval (dIdx, &index);
            for (MMO_Event ev = evt->begin (); !evt->end (); ev = evt->next ())
            {
                Index eIndex = evt->key ();
                Dependencies eDeps = ev->lhs ();
                int assignments = eDeps->discretes ();
                for (Index *eIdx = eDeps->begin (DEP_DISCRETE); !eDeps->end (DEP_DISCRETE); eIdx = eDeps->next (DEP_DISCRETE))
                {
                    _setInterval (eIdx, &eIndex);
                    Intersection is = eIdx->intersection (*dIdx);
                    if (is.type () == IDX_DISJOINT)
                    {
                        continue;
                    }
                    if (_controlIntersections (HD[eIndex], is))
                    {
                        continue;
                    }
                    Index equationIndex = e->lhs ();
                    _indexDependencies (eIndex, eIdx, equationIndex, dIdx, &simpleHDDeps, WR_ALLOC_LD_HD, WR_INIT_LD_HD, "modelData->nHD", "modelData->HD",
                                        "events", is, assignments);
                    HD[eIndex].push_back (is);
                }
            }
        }
        for (Index *dIdx = deps->begin (DEP_ALGEBRAIC_DISCRETE); !deps->end (DEP_ALGEBRAIC_DISCRETE); dIdx = deps->next (DEP_ALGEBRAIC_DISCRETE))
        {
            Index algState = deps->key (DEP_ALGEBRAIC_DISCRETE);
            for (MMO_Event ev = evt->begin (); !evt->end (); ev = evt->next ())
            {
                Index eIndex = evt->key ();
                Dependencies eDeps = ev->lhs ();
                int assignments = eDeps->discretes ();
                for (Index *eIdx = eDeps->begin (DEP_DISCRETE); !eDeps->end (DEP_DISCRETE); eIdx = eDeps->next (DEP_DISCRETE))
                {
                    _setInterval (eIdx, &eIndex);
                    Intersection is = eIdx->intersection (algState);
                    if (is.type () == IDX_DISJOINT)
                    {
                        continue;
                    }
                    if (_controlIntersections (HD[eIndex], is))
                    {
                        continue;
                    }
                    Index equationIndex = e->lhs ();
                    if (dIdx->hasRange () && !index.hasRange ())
                    {
                        equationIndex.setRange ();
                        equationIndex.setLow (dIdx->low ());
                        equationIndex.setHi (dIdx->hi ());
                    }
                    else if (index.hasRange () && !dIdx->hasRange ())
                    {
                        equationIndex.setConstant (dIdx->constant ());
                        equationIndex.setFactor (0);
                        equationIndex.setLow (1);
                        equationIndex.setHi (1);
                    }
                    _indexDependencies (eIndex, eIdx, equationIndex, &algState, &simpleHDDeps, WR_ALLOC_LD_HD, WR_INIT_LD_HD, "modelData->nHD", "modelData->HD",
                                        "events", is, assignments);
                    HD[eIndex].push_back (is);
                }
            }
        }
        for (Index *dIdx = deps->begin (DEP_DISCRETE_VECTOR); !deps->end (DEP_DISCRETE_VECTOR); dIdx = deps->next (DEP_DISCRETE_VECTOR))
        {
            Index vectorIndex (index);
            vectorIndex.setLow (dIdx->low ());
            vectorIndex.setHi (dIdx->hi ());
            for (MMO_Event ev = evt->begin (); !evt->end (); ev = evt->next ())
            {
                Index eIndex = evt->key ();
                Dependencies eDeps = ev->lhs ();
                int assignments = eDeps->discretes ();
                for (Index *eIdx = eDeps->begin (DEP_DISCRETE); !eDeps->end (DEP_DISCRETE); eIdx = eDeps->next (DEP_DISCRETE))
                {
                    _setInterval (eIdx, &eIndex);
                    Intersection is = eIdx->intersection (*dIdx);
                    if (is.type () == IDX_DISJOINT)
                    {
                        continue;
                    }
                    if (_controlIntersections (HD[eIndex], is))
                    {
                        continue;
                    }
                    Index equationIndex = vectorIndex;
                    if (is.type () >= 0 || is.type () == IDX_EQUAL)
                    {
                        equationIndex = e->lhs ();
                    }
                    _indexDependencies (eIndex, eIdx, equationIndex, dIdx, &simpleHDDeps, WR_ALLOC_LD_HD, WR_INIT_LD_HD, "modelData->nHD", "modelData->HD",
                                        "events", is, assignments);
                    HD[eIndex].push_back (is);
                }
            }
        }
        _common->vectorDependencies (e->lhs (), deps, WR_ALLOC_LD_DS, WR_INIT_LD_DS, "modelData->nDS", "modelData->DS", WR_ALLOC_LD_SD, WR_INIT_LD_SD,
                                     "modelData->nSD", "modelData->SD", "states", "states", true, DEP_STATE_VECTOR, &_initializeVars);
    }
    if (genericEquation && hasInit)
    {
        _writer->write ("}", WR_ALLOC_LD_DS);
        _writer->write ("}", WR_ALLOC_LD_SD);
        _writer->write ("}", WR_INIT_LD_DS);
        _writer->write ("}", WR_INIT_LD_SD);
    }
    for (map<int, int>::iterator it = simpleHDDeps.begin (); it != simpleHDDeps.end (); it++)
    {
        buffer << "modelData->nHD[" << it->first << "] = " << it->second << ";";
        _writer->write (&buffer, WR_ALLOC_LD_HD);
    }
    hasInit = false;
    genericEquation = false;
    bool hasLHSStates = false;
    bool hasRHSStates = false;
    bool hasDiscretes = false;
    bufferGen.str ("");
    int genericDefInit = 0;
    for (MMO_Event e = evt->begin (); !evt->end (); e = evt->next ())
    {
        Index index = evt->key ();
        MMO_Expression exp = e->condition ()->exp ();
        Dependencies deps = exp->deps ();
        if (evt->endGenericDefinition ())
        {
            genericEquation = false;
            genericDefInit = 0;
            bufferGen.str ("");
            _writer->write ("}", WR_INIT_EVENT);
            if (hasInit)
            {
                hasInit = false;
                indent = "";
                _writer->write ("}", WR_ALLOC_LD_SZ);
                _writer->write ("}", WR_ALLOC_LD_ZS);
                _writer->write ("}", WR_INIT_LD_SZ);
                _writer->write ("}", WR_INIT_LD_ZS);
            }
            if (hasLHSStates)
            {
                hasLHSStates = false;
                indent = "";
                _writer->write ("}", WR_ALLOC_EVENT_LHSST);
                _writer->write ("}", WR_INIT_EVENT_LHSST);
                _writer->write ("}", WR_ALLOC_STATE_HANDLERS);
                _writer->write ("}", WR_INIT_STATE_HANDLERS);
            }
            if (hasRHSStates)
            {
                hasRHSStates = false;
                indent = "";
                _writer->write ("}", WR_ALLOC_EVENT_RHSST);
                _writer->write ("}", WR_INIT_EVENT_RHSST);
            }
            if (hasDiscretes)
            {
                hasDiscretes = false;
                indent = "";
                _writer->write ("}", WR_ALLOC_EVENT_DSC);
                _writer->write ("}", WR_INIT_EVENT_DSC);
                if (_hasDD == true)
                {
                    _hasDD = false;
                    _writer->write ("}", WR_ALLOC_LD_DD);
                    _writer->write ("}", WR_INIT_LD_DD);
                }
                _writer->write ("}", WR_ALLOC_LD_DH);
                _writer->write ("}", WR_INIT_LD_DH);
            }
        }
        if (evt->beginGenericDefinition ())
        {
            genericEquation = true;
            genericDefInit = index.begin ();
            if (!hasInit && !hasLHSStates && !hasRHSStates && !hasDiscretes)
            {
                bufferGen << "for(i = " << index.begin () << "; i <= " << index.end () << "; i++)";
            }
            _writer->write (&bufferGen, WR_INIT_EVENT, false);
            _writer->write ("{", WR_INIT_EVENT);
        }
        if (deps->hasStates () && !hasInit && genericEquation)
        {
            hasInit = true;
            indent = _writer->indent (1);
            _writer->write (&bufferGen, WR_ALLOC_LD_SZ, false);
            map<Index, Index> states;
            _writer->write (&bufferGen, WR_ALLOC_LD_ZS, false);
            _writer->write (&bufferGen, WR_INIT_LD_ZS, false);
            _writer->write (&bufferGen, WR_INIT_LD_SZ, false);
            buffer << "{";
            _writer->write (&buffer, WR_ALLOC_LD_SZ, false);
            _writer->write (&buffer, WR_ALLOC_LD_ZS, false);
            _writer->write (&buffer, WR_INIT_LD_ZS, false);
            _writer->write (&buffer, WR_INIT_LD_SZ);
        }
        string eIdx = index.print ("i", -genericDefInit);
        if (deps->states ())
        {
            buffer << indent << "modelData->nZS[" << eIdx << "] = " << deps->states () << ";";
            _writer->write (&buffer, WR_ALLOC_LD_ZS);
        }
        map<Index, Index> defStates;
        for (Index *idx = deps->begin (DEP_STATE); !deps->end (DEP_STATE); idx = deps->next (DEP_STATE))
        {
            defStates[*idx] = *idx;
            string sIdx = idx->print ("i", -genericDefInit + idx->begin ());
            buffer << indent << "modelData->nSZ[" << sIdx << "]++;";
            _writer->write (&buffer, WR_ALLOC_LD_SZ);
            buffer << indent << "modelData->ZS[" << eIdx << "][events[" << eIdx << "]++] = " << sIdx << ";";
            _writer->write (&buffer, WR_INIT_LD_ZS);
            buffer << indent << "modelData->SZ[" << sIdx << "][states[" << sIdx << "]++] = " << eIdx << ";";
            _writer->write (&buffer, WR_INIT_LD_SZ);
            if (_parallel)
            {
                _common->graphInsert (*idx, index, _model->states (), NOD_SZ);
            }
        }
        _common->addAlgebriacDeps (deps, index, defStates, "modelData->nZS", "modelData->nSZ", "modelData->ZS", "modelData->SZ", WR_ALLOC_LD_ALG_ZS,
                                   WR_ALLOC_LD_ALG_SZ, WR_INIT_LD_ALG_ZS, WR_INIT_LD_ALG_SZ, "events", "states");
        _common->vectorDependencies (index, deps, WR_ALLOC_LD_ZS, WR_INIT_LD_ZS, "modelData->nZS", "modelData->ZS", WR_ALLOC_LD_SZ, WR_INIT_LD_SZ,
                                     "modelData->nSZ", "modelData->SZ", "events", "states", false, DEP_STATE_VECTOR, &_initializeVars);
        map<Index, Index> eventDependencies;
        _eventDeps (e, index, evt, DEP_DISCRETE, eventDependencies);
        _eventAlgebraicDeps (e, index, evt, DEP_ALGEBRAIC_DISCRETE, eventDependencies);
        eventDependencies.clear ();
        _eventDeps (e, index, evt, DEP_STATE, eventDependencies);
        _eventAlgebraicDeps (e, index, evt, DEP_ALGEBRAIC_STATE, eventDependencies);
        deps = e->lhs ();
        if (deps->hasStates () && !hasLHSStates && genericEquation)
        {
            hasLHSStates = true;
            _writer->write (&bufferGen, WR_ALLOC_EVENT_LHSST, false);
            _writer->write (&bufferGen, WR_INIT_EVENT_LHSST, false);
            _writer->write (&bufferGen, WR_ALLOC_STATE_HANDLERS, false);
            _writer->write (&bufferGen, WR_INIT_STATE_HANDLERS, false);
            buffer << "{";
            _writer->write (&buffer, WR_ALLOC_EVENT_LHSST, false);
            _writer->write (&buffer, WR_INIT_EVENT_LHSST, false);
            _writer->write (&buffer, WR_ALLOC_STATE_HANDLERS, false);
            _writer->write (&buffer, WR_INIT_STATE_HANDLERS);
        }
        if (deps->hasDiscretes () && !hasDiscretes && genericEquation)
        {
            hasDiscretes = true;
            _writer->write (&bufferGen, WR_ALLOC_EVENT_DSC, false);
            _writer->write (&bufferGen, WR_INIT_EVENT_DSC, false);
            if (_hasDD == true)
            {
                _writer->write (&bufferGen, WR_ALLOC_LD_DD, false);
                _writer->write (&bufferGen, WR_INIT_LD_DD, false);
            }
            _writer->write (&bufferGen, WR_ALLOC_LD_DH, false);
            _writer->write (&bufferGen, WR_INIT_LD_DH, false);
            buffer << "{";
            _writer->write (&buffer, WR_ALLOC_EVENT_DSC, false);
            _writer->write (&buffer, WR_INIT_EVENT_DSC, false);
            if (_hasDD == true)
            {
                _writer->write (&buffer, WR_ALLOC_LD_DD, false);
                _writer->write (&buffer, WR_INIT_LD_DD, false);
            }
            _writer->write (&buffer, WR_ALLOC_LD_DH, false);
            _writer->write (&buffer, WR_INIT_LD_DH);
        }
        if (e->handlerType () == HND_POSITIVE)
        {
            buffer << indent << "modelData->event[" << eIdx << "].direction = 1;";
            _writer->write (&buffer, WR_INIT_EVENT);
        }
        else if (e->handlerType () == HND_NEGATIVE)
        {
            buffer << indent << "modelData->event[" << eIdx << "].direction = -1;";
            _writer->write (&buffer, WR_INIT_EVENT);
        }
        else if (e->handlerType () == HND_ZERO)
        {
            buffer << indent << "modelData->event[" << eIdx << "].direction = 0;";
            _writer->write (&buffer, WR_INIT_EVENT);
        }
        if (e->zcRelation () == ZC_LT)
        {
            buffer << indent << "modelData->event[" << eIdx << "].relation = 0;";
            _writer->write (&buffer, WR_INIT_EVENT);
        }
        else if (e->zcRelation () == ZC_LE)
        {
            buffer << indent << "modelData->event[" << eIdx << "].relation = 1;";
            _writer->write (&buffer, WR_INIT_EVENT);
        }
        else if (e->zcRelation () == ZC_GT)
        {
            buffer << indent << "modelData->event[" << eIdx << "].relation = 2;";
            _writer->write (&buffer, WR_INIT_EVENT);
        }
        else if (e->zcRelation () == ZC_GE)
        {
            buffer << indent << "modelData->event[" << eIdx << "].relation = 3;";
            _writer->write (&buffer, WR_INIT_EVENT);
        }
        if (deps->states ())
        {
            buffer << indent << "modelData->event[" << eIdx << "].nLHSSt = " << deps->states () << ";";
            _writer->write (&buffer, WR_ALLOC_EVENT_LHSST);
        }
        if (deps->discretes ())
        {
            buffer << indent << "modelData->event[" << eIdx << "].nLHSDsc = " << deps->discretes () << ";";
            _writer->write (&buffer, WR_ALLOC_EVENT_DSC);
        }
        for (Index *idx = deps->begin (DEP_STATE); !deps->end (DEP_STATE); idx = deps->next (DEP_STATE))
        {
            _setInterval (idx, &index);
            string idxStr = idx->print ("i", -genericDefInit + idx->begin ());
            buffer << indent << "modelData->event[" << eIdx << "].LHSSt[events[" << eIdx << "]++] = " << idxStr << ";";
            _writer->write (&buffer, WR_INIT_EVENT_LHSST);
            if (_parallel)
            {
                buffer << indent << "modelData->SH[" << idxStr << "][states[" << idxStr << "]++] = " << eIdx << ";";
                _writer->write (&buffer, WR_INIT_STATE_HANDLERS);
                buffer << indent << "modelData->nSH[" << idxStr << "]++;";
                _writer->write (&buffer, WR_ALLOC_STATE_HANDLERS);
                _common->graphInsert (*idx, index, _model->states (), NOD_SZ);
            }
        }
        for (Index *idx = deps->begin (DEP_DISCRETE); !deps->end (DEP_DISCRETE); idx = deps->next (DEP_DISCRETE))
        {
            _setInterval (idx, &index);
            string idxStr = idx->print ("i", -genericDefInit + idx->begin ());
            buffer << indent << "modelData->event[" << eIdx << "].LHSDsc[events[" << eIdx << "]++] = " << idxStr << ";";
            _writer->write (&buffer, WR_INIT_EVENT_DSC);
            if (_parallel)
            {
                buffer << indent << "modelData->DH[" << idxStr << "][discretes[" << idxStr << "]++] = " << eIdx << ";";
                _writer->write (&buffer, WR_INIT_LD_DH);
                buffer << indent << "modelData->nDH[" << idxStr << "]++;";
                _writer->write (&buffer, WR_ALLOC_LD_DH);
            }
        }
        _eventVectorDependencies (index, deps, WR_ALLOC_EVENT_DSC, WR_INIT_EVENT_DSC, "nLHSDsc", "LHSDsc", DEP_DISCRETE_VECTOR);
        _eventVectorDependencies (index, deps, WR_ALLOC_EVENT_LHSST, WR_INIT_EVENT_LHSST, "nLHSSt", "LHSSt", DEP_STATE_VECTOR);
        deps = e->deps ();
        if (deps->hasStates () && !hasRHSStates && genericEquation)
        {
            hasRHSStates = true;
            _writer->write (&bufferGen, WR_ALLOC_EVENT_RHSST, false);
            _writer->write (&bufferGen, WR_INIT_EVENT_RHSST, false);
            buffer << "{";
            _writer->write (&buffer, WR_ALLOC_EVENT_RHSST, false);
            _writer->write (&buffer, WR_INIT_EVENT_RHSST);
        }
        if (hasInit && hasLHSStates && hasRHSStates && hasDiscretes)
        {
            bufferGen.str ("");
        }
        if (deps->states ())
        {
            buffer << indent << "modelData->event[" << eIdx << "].nRHSSt = " << deps->states () << ";";
            _writer->write (&buffer, WR_ALLOC_EVENT_RHSST);
        }
        defStates.clear ();
        for (Index *idx = deps->begin (DEP_STATE); !deps->end (DEP_STATE); idx = deps->next (DEP_STATE))
        {
            defStates[*idx] = *idx;
            _setInterval (idx, &index);
            string idxStr = idx->print ("i", -genericDefInit + idx->begin ());
            buffer << indent << "modelData->event[" << eIdx << "].RHSSt[events[" << eIdx << "]++] = " << idxStr << ";";
            _writer->write (&buffer, WR_INIT_EVENT_RHSST);
            if (_parallel)
            {
                _common->graphInsert (*idx, index, _model->states (), NOD_SZ);
            }
        }
        _common->addAlgebriacDeps (deps, index, defStates, "modelData->event[" + eIdx + "].nRHSSt", "", "modelData->event[" + eIdx + "].RHSSt", "",
                                   WR_ALLOC_EVENT_ALG_RHSST, WR_NULL, WR_INIT_EVENT_ALG_RHSST, WR_NULL, "events", "");
        if (deps->stateVectors ())
        {
            buffer << indent << "modelData->event[" << eIdx << "].nRHSSt += " << deps->stateVectors () << ";";
            _writer->write (&buffer, WR_ALLOC_EVENT_RHSST);
        }
        for (Index *idx = deps->begin (DEP_STATE_VECTOR); !deps->end (DEP_STATE_VECTOR); idx = deps->next (DEP_STATE_VECTOR))
        {
            string forIdx = Util::getInstance ()->newVarName ("i", _model->varTable ());
            _common->addLocalVar (forIdx, &_initializeVars);
            string idxStr = idx->print (forIdx);
            buffer << "for(" << forIdx << " = " << idx->begin () << "; " << forIdx << " <= " << idx->end () << "; " << forIdx << "++)";
            _writer->write (&buffer, WR_INIT_EVENT_RHSST);
            _writer->write ("{", WR_INIT_EVENT_RHSST);
            buffer << indent << "modelData->event[" << eIdx << "].RHSSt[events[" << eIdx << "]++] = " << idxStr << ";";
            _writer->write (&buffer, WR_INIT_EVENT_RHSST);
            _writer->write ("}", WR_INIT_EVENT_RHSST);
        }
    }
    if (genericEquation)
    {
        if (hasInit)
        {
            _writer->write ("}", WR_ALLOC_LD_SZ);

            _writer->write ("}", WR_ALLOC_LD_ZS);
            _writer->write ("}", WR_INIT_LD_SZ);
            _writer->write ("}", WR_INIT_LD_ZS);
        }
        if (hasLHSStates)
        {
            _writer->write ("}", WR_ALLOC_EVENT_LHSST);
            _writer->write ("}", WR_INIT_EVENT_LHSST);
            _writer->write ("}", WR_ALLOC_STATE_HANDLERS);
            _writer->write ("}", WR_INIT_STATE_HANDLERS);
        }
        if (hasRHSStates)
        {
            _writer->write ("}", WR_ALLOC_EVENT_RHSST);
            _writer->write ("}", WR_INIT_EVENT_RHSST);
        }
        if (hasDiscretes)
        {
            _writer->write ("}", WR_ALLOC_EVENT_DSC);
            _writer->write ("}", WR_INIT_EVENT_DSC);
            if (_hasDD == true)
            {
                _writer->write ("}", WR_ALLOC_LD_DD);
                _writer->write ("}", WR_INIT_LD_DD);
            }
            _writer->write ("}", WR_ALLOC_LD_DH);
            _writer->write ("}", WR_INIT_LD_DH);
        }
        _writer->write ("}", WR_INIT_EVENT);
    }
    _model->initOutput ();
    if (!_writer->isEmpty (WR_ALLOC_LD_DH))
    {
        buffer << "int *discretes = (int*)malloc(" << _model->discretes () << "*sizeof(int));";
        _initializeVars[buffer.str ()] = buffer.str ();
        _freeVars["discretes"] = "discretes";
        buffer.str ("");
    }
    if (_model->outs ())
    {
        buffer << "int *outputs = (int*)malloc(" << _model->outs () << "*sizeof(int));";
        _initializeVars[buffer.str ()] = buffer.str ();
        _freeVars["outputs"] = "outputs";
        buffer.str ("");
        if (_model->discretes () && _writer->isEmpty (WR_ALLOC_LD_DH))
        {
            buffer << "int *discretes = (int*)malloc(" << _model->discretes () << "*sizeof(int));";
            _initializeVars[buffer.str ()] = buffer.str ();
            _freeVars["discretes"] = "discretes";
            buffer.str ("");
        }
    }
}

void
QSS_::model ()
{
    MMO_EquationTable equations = _model->derivatives ();
    VarSymbolTable vt = _model->varTable ();
    MMO_EquationTable algebraics = _model->algebraics ();
    stringstream buffer;
    vt->setPrintEnvironment (VST_MODEL_FUNCTIONS);
    string indent = _writer->indent (1);
    int order = _common->getOrder ();
    for (MMO_Equation eq = equations->begin (); !equations->end (); eq = equations->next ())
    {
        Index idx = equations->key ();
        if (idx.hasRange ())
        {
            _common->genericDefCondition (idx, eq->lhs (), WR_MODEL_GENERIC, &_modelVars);
            _common->print (eq->print (indent, "dx", "j", true, algebraics, EQ_DERIVATIVE, order, false, 0, true, idx.low ()), WR_MODEL_GENERIC);
            buffer << "}";
            _writer->write (&buffer, WR_MODEL_GENERIC);
            _common->insertLocalVariables (&_modelVars, eq->getVariables ());
        }
        else
        {
            buffer << indent << "case " << eq->lhs ().mappedConstant () << ":";
            _writer->write (&buffer, WR_MODEL_SIMPLE);
            _common->print (eq->print (_writer->indent (2), "dx", "", true, algebraics, EQ_DERIVATIVE, order, true), WR_MODEL_SIMPLE);
            buffer << _writer->indent (2) << "return;";
            _writer->write (&buffer, WR_MODEL_SIMPLE);
            _common->insertLocalVariables (&_modelVars, eq->getVariables ());
        }
    }
}

void
QSS_::_printDeps (Dependencies d, Index derivativeIndex, MMO_EquationTable equations, MMO_EquationTable algebraics, string idxStr, WR_Section s, int i,
                  bool constant)
{
    stringstream buffer;
    int indent = i;
    bool controlRange = (s == WR_MODEL_DEPS_SIMPLE);
    int order = _common->getOrder ();
    int xCoeff = _model->annotation ()->polyCoeffs ();
    bool constantPrint = constant;
    string aux1, aux2;
    set<Index> algebraicArguments;
    for (Index *dIdx = d->begin (DEP_ALGEBRAIC); !d->end (DEP_ALGEBRAIC); dIdx = d->next (DEP_ALGEBRAIC))
    {
        if (d->isVector (dIdx))
        {
            string vn;
            buffer.str ("");
            list<MMO_Equation> eqs = algebraics->equation (*dIdx);
            if (eqs.empty ())
            {
                Error::getInstance ()->add (0, EM_CG | EM_NO_EQ, ER_Error, "Print vector algebraic dependencies for index %s.", dIdx->print ().c_str ());
                return;
            }
            list<MMO_Equation>::iterator eq;
            for (eq = eqs.begin (); eq != eqs.end (); eq++)
            {
                if (!(*eq)->controlAlgebraicArguments (&algebraicArguments, (*eq)->algebraicArguments ()))
                {
                    continue;
                }
                string iter;
                Index lhsEq = (*eq)->lhs ();
                stringstream aLhs;
                if (lhsEq.hasRange ())
                {
                    iter = Util::getInstance ()->newVarName ("j", _model->varTable ());
                    _common->addLocalVar (iter, &_modelDepsVars);
                    buffer << _writer->indent (indent) << "for(" << iter << " = " << lhsEq.begin () << "; " << iter << " <= " << lhsEq.end () << "; " << iter
                            << "++)";
                    _writer->write (&buffer, s);
                    _writer->write ("{", s);
                    aLhs << "alg[(" << (*eq)->lhs ().print (iter) << ")";
                    aLhs << " * " << xCoeff;
                }
                else
                {
                    int cte = dIdx->mappedValue ((*eq)->lhs ().constant ());
                    aLhs << "alg[" << cte * xCoeff;
                }
                _common->print ((*eq)->print (_writer->indent (indent), aLhs.str (), iter, false,
                NULL,
                                              EQ_ALGEBRAIC, order, false, 0, false, dIdx->low (), 0),
                                s);
                if (lhsEq.hasRange ())
                {
                    _writer->write ("}", s);
                }
            }
        }
        else
        {
            string vn, vm;
            string varIdx = "j";
            list<MMO_Equation> eqs = algebraics->equation (*dIdx);
            if (eqs.empty ())
            {
                Error::getInstance ()->add (0, EM_CG | EM_NO_EQ, ER_Error, "Print algebraic dependencies for index %s.", dIdx->print ().c_str ());
                return;
            }
            Index *algebraicState = d->algebraicState (dIdx);
            list<MMO_Equation>::iterator eq;
            for (eq = eqs.begin (); eq != eqs.end (); eq++)
            {
                if (!(*eq)->controlAlgebraicArguments (&algebraicArguments, (*eq)->algebraicArguments ()))
                {
                    continue;
                }
                bool range = ((*eq)->lhs ().hasRange () && (*eq)->lhs ().range () != derivativeIndex.range ()) && (dIdx->factor () != 0);
                if (range)
                {
                    if (aux1.empty ())
                    {
                        aux1 = Util::getInstance ()->newVarName ("j", _model->varTable ());
                        _modelDepsVars["int " + aux1 + " = 0;"] = "int " + aux1 + " = 0;";
                    }
                    vn = aux1;
                }
                else
                {
                    vn = varIdx;
                }
                bool variableMod = false;
                constantPrint = constant;
                stringstream lhs;
                if (constant && algebraicState && algebraicState->hasRange () && dIdx->hasRange ())
                {
                    _common->addLocalVar (varIdx, &_modelDepsVars);
                    buffer << _writer->indent (indent) << "for(" << varIdx << " = " << algebraicState->begin () << ";" << varIdx << " <= "
                            << algebraicState->end () << "; " << varIdx << "++)";
                    _writer->write (&buffer, s);
                    buffer << _writer->indent (indent) << "{";
                    _writer->write (&buffer, s);
                    constantPrint = false;
                    indent++;
                }
                int cte = 0;
                if ((*eq)->lhs ().factor () == 0)
                {
                    cte = (*eq)->lhs ().mappedConstant ();
                    lhs << "alg[" << cte * xCoeff;
                    constantPrint = true;
                }
                else if (dIdx->factor () == 0)
                {
                    if (dIdx->isArray ())
                    {
                        cte = dIdx->mappedValue ((*eq)->lhs ().constant ());
                        lhs << "alg[" << cte * xCoeff;
                        cte = dIdx->constant ();
                        constantPrint = true;
                    }
                    else
                    {
                        lhs << "alg[(" << dIdx->print (varIdx) << ")";
                    }
                }
                else
                {
                    if (dIdx->factor () != 1 || dIdx->operConstant () != 0)
                    {
                        if (aux2.empty ())
                        {
                            aux2 = Util::getInstance ()->newVarName ("j", _model->varTable ());
                            _modelDepsVars["int " + aux2 + " = 0;"] = "int " + aux2 + " = 0;";
                        }
                        vm = aux2;
                        variableMod = true;
                        lhs << "alg[(" << (*eq)->lhs ().print (dIdx->definition ("j"), -(*eq)->lhs ().operConstant ()) << ")";
                        varIdx = vm;
                    }
                    else
                    {
                        lhs << "alg[(" << (*eq)->lhs ().print ("j", -(*eq)->lhs ().operConstant ()) << ")";
                        varIdx = vn;
                    }
                }
                if (!constantPrint)
                {
                    lhs << " * " << xCoeff;
                }
                if (range)
                {
                    _writer->write ((*eq)->printRange ("j", vn, _writer->indent (indent), (*eq)->lhs ()), s);
                }
                if (variableMod)
                {
                    _writer->write ((*eq)->printRange (vn, vm, _writer->indent (indent), *dIdx, true), s);
                }
                _common->print ((*eq)->print (_writer->indent (indent), lhs.str (), varIdx, false,
                NULL,
                                              EQ_ALGEBRAIC, order, constantPrint, 0, false, dIdx->low (), cte),
                                s);
                _common->insertLocalVariables (&_modelDepsVars, (*eq)->getVariables ());
                if (constant && algebraicState && algebraicState->hasRange () && dIdx->hasRange ())
                {
                    indent--;
                    buffer << _writer->indent (indent) << "}";
                    _writer->write (&buffer, s);
                    constantPrint = constant;
                }
                if (variableMod)
                {
                    buffer << _writer->indent (indent) << "}";
                    _writer->write (&buffer, s);
                }
                if (range)
                {
                    buffer << _writer->indent (indent) << "}";
                    _writer->write (&buffer, s);
                }
            }
        }
    }
    constantPrint = constant;
    for (Index *dIdx = d->begin (DEP_STATE); !d->end (DEP_STATE); dIdx = d->next (DEP_STATE))
    {
        stringstream lhs;
        list<MMO_Equation> eqs = equations->equation (*dIdx);
        if (eqs.empty ())
        {
            Error::getInstance ()->add (0, EM_CG | EM_NO_EQ, ER_Error, "Print state dependencies for index %s.", dIdx->print ().c_str ());
            return;
        }
        list<MMO_Equation>::iterator eq;
        for (eq = eqs.begin (); eq != eqs.end (); eq++)
        {
            string varIdx = "j";
            if (dIdx->hasRange () && controlRange)
            {
                _common->addLocalVar (varIdx, &_modelDepsVars);
                buffer << _writer->indent (indent) << "for(" << varIdx << " = " << (*eq)->lhs ().begin () << "; " << varIdx << " <= " << (*eq)->lhs ().end ()
                        << "; " << varIdx << "++)";
                _writer->write (&buffer, s);
                buffer << _writer->indent (indent++) << "{";
                _writer->write (&buffer, s);
                constantPrint = false;
            }
            int constantOffset = 0;
            stringstream varMap;
            if (dIdx->hasRange ())
            {
                string varIndex = (*eq)->lhs ().print (varIdx);
                varMap << _engine->variableMap (varIndex);
                lhs << "der[(" << varIndex << ")";
                if (!constantPrint)
                {
                    lhs << " * " << xCoeff;
                }
            }
            else
            {
                stringstream varIndex;
                constantOffset = (*eq)->lhs ().mappedValue (dIdx->constant ());
                varIndex << constantOffset;
                varMap << _engine->variableMap (varIndex.str ());
                lhs << "der[" << constantOffset * xCoeff;
                constantOffset = dIdx->constant ();
            }
            _writer->write (varMap.str (), s);
            _common->print ((*eq)->print (_writer->indent (indent), lhs.str (), varIdx, false,
            NULL,
                                          EQ_DEPENDENCIES, order, constantPrint, 0, true, dIdx->low (), constantOffset),
                            s);
            _writer->write (_engine->endMap (), s);
            _common->insertLocalVariables (&_modelDepsVars, (*eq)->getVariables ());
            if (dIdx->hasRange () && controlRange)
            {
                buffer << _writer->indent (--indent) << "}";
                _writer->write (&buffer, s);
                constantPrint = constant;
            }
        }
    }
}

void
QSS_::modelDeps ()
{
    MMO_EquationTable equations = _model->derivatives ();
    MMO_EquationTable algebraics = _model->algebraics ();
    stringstream buffer;
    string indent = _writer->indent (1);
    int order = _common->getOrder ();
    int coeff = _model->annotation ()->order () + 1;
    for (Dependencies d = _modelDeps->begin (); !_modelDeps->end (); d = _modelDeps->next ())
    {
        Index idx = _modelDeps->key ();
        Index eqIdx = equations->equationIndex (idx);
        if (idx.hasRange ())
        {
            _common->genericDefCondition (eqIdx, idx, WR_MODEL_DEPS_GENERIC, &_modelDepsVars);
            _printDeps (d, eqIdx, equations, algebraics, idx.print ("j"), WR_MODEL_DEPS_GENERIC, 1, false);
            buffer << "}";
            _writer->write (&buffer, WR_MODEL_DEPS_GENERIC);
        }
        else
        {
            buffer << indent << "case " << idx.print () << ":";
            _writer->write (&buffer, WR_MODEL_DEPS_SIMPLE);
            _printDeps (d, eqIdx, equations, algebraics, "", WR_MODEL_DEPS_SIMPLE, 2, true);
            if (equations->findGenericDependencies (idx.mappedValue ()))
            {
                buffer << _writer->indent (2) << "break;";
            }
            else
            {
                buffer << _writer->indent (2) << "return;";
            }
            _writer->write (&buffer, WR_MODEL_DEPS_SIMPLE);
        }
    }
    for (Dependencies d = _modelVectorDeps->begin (); !_modelVectorDeps->end (); d = _modelVectorDeps->next ())
    {
        stringstream lhs, bufferGen;
        bool genericDef = false;
        Index k = _modelVectorDeps->key ();
        for (Index *idx = d->begin (DEP_STATE_VECTOR); !d->end (DEP_STATE_VECTOR); idx = d->next (DEP_STATE_VECTOR))
        {
            string idxVar = "j";
            _common->genericDefCondition (k, k, WR_MODEL_DEPS_GENERIC, &_modelDepsVars);
            list<MMO_Equation> eqs = equations->equation (*idx);
            if (eqs.empty ())
            {
                Error::getInstance ()->add (0, EM_CG | EM_NO_EQ, ER_Error, "Vector dependencies for index %s.", idx->print ().c_str ());
                return;
            }
            list<MMO_Equation>::iterator eq;
            for (eq = eqs.begin (); eq != eqs.end (); eq++)
            {
                lhs.str ("");
                Index lhsEq = (*eq)->lhs ();
                if (idx->factor () != 0 && lhsEq.hasRange ())
                {
                    idxVar = Util::getInstance ()->newVarName ("j", _model->varTable ());
                    bufferGen << "for(" << idxVar << " = " << idx->begin () << "; " << idxVar << " <= " << idx->end () << "; " << idxVar << "++)" << endl;
                    bufferGen << "{" << endl;
                    genericDef = true;
                    _common->addLocalVar (idxVar, &_modelDepsVars);
                }
                _writer->write (&bufferGen, WR_MODEL_DEPS_GENERIC);
                if (lhsEq.hasRange ())
                {
                    lhs << "der[(" << idx->print (idxVar, -k.mappedBegin ()) << " * " << coeff;
                }
                else
                {
                    int cte = (*eq)->lhs ().mappedConstant () * coeff;
                    lhs << "der[" << cte;
                }
                _common->print ((*eq)->print (_writer->indent (1), lhs.str (), idxVar, true, algebraics, EQ_DEPENDENCIES, order), WR_MODEL_DEPS_GENERIC);
                _common->insertLocalVariables (&_modelDepsVars, (*eq)->getVariables ());
                if (genericDef)
                {
                    _writer->write ("}", WR_MODEL_DEPS_GENERIC);
                    genericDef = false;
                }
            }
            _writer->write ("}", WR_MODEL_DEPS_GENERIC);
        }
    }
}

void
QSS_::zeroCrossing ()
{
    _common->zeroCrossing (VST_MODEL_FUNCTIONS, &_zcVars);
}

void
QSS_::handler ()
{
    _common->handler (VST_MODEL_FUNCTIONS, &_handlerPosVars, &_handlerNegVars);
}

void
QSS_::output ()
{
    _common->output (VST_MODEL_OUTPUT_FUNCTIONS, &_outputVars);
}

void
QSS_::_init ()
{
    stringstream buffer;
    string iTime;
    _writer->print (_engine->prototype (SOL_INIT));
    _writer->beginBlock ();
    for (map<string, string>::iterator it = _initializeVars.begin (); it != _initializeVars.end (); it++)
    {
        _writer->print (it->second);
    }
    _writer->print (initData ());
    _writer->print (WR_ALLOC_LD);
    _writer->print (WR_START_CODE);
    _writer->print (WR_INIT_CODE);
    _writer->print (WR_ALLOC_LD_DS);
    _writer->print (WR_ALLOC_LD_ALG_DS);
    _writer->print (WR_ALLOC_LD_SD);
    _writer->print (WR_ALLOC_LD_ALG_SD);
    _writer->print (WR_ALLOC_LD_ZS);
    _writer->print (WR_ALLOC_LD_ALG_ZS);
    _writer->print (WR_ALLOC_LD_SZ);
    _writer->print (WR_ALLOC_LD_ALG_SZ);
    _writer->print (WR_ALLOC_LD_HZ);
    _writer->print (WR_ALLOC_LD_HD);
    if (_flags->parallel ())
    {
        _writer->print (WR_ALLOC_STATE_HANDLERS);
        _writer->print (WR_ALLOC_LD_DD);
        _writer->print (WR_ALLOC_LD_DH);
    }
    _writer->print (WR_ALLOC_EVENT_LHSST);
    _writer->print (WR_ALLOC_EVENT_RHSST);
    _writer->print ("QSS_allocDataMatrix(modelData);");

    _writer->print (WR_ALLOC_EVENT_ALG_RHSST);
    _writer->print (WR_ALLOC_EVENT_DSC);
    _writer->print ("QSS_allocDataMatrix(modelData);");
    _writer->print (WR_INIT_LD);
    _writer->print (WR_INIT_TIME);
    _common->printSection ("states", _model->states (), WR_INIT_LD_DS);
    if (_writer->isEmpty (WR_INIT_LD_DS))
    {
        _common->printSection ("states", _model->states (), WR_INIT_LD_ALG_DS);
    }
    else
    {
        _writer->print (WR_INIT_LD_ALG_DS);
    }
    _common->printSection ("states", _model->states (), WR_INIT_LD_SD);
    if (_writer->isEmpty (WR_INIT_LD_SD))
    {
        _common->printSection ("states", _model->states (), WR_INIT_LD_ALG_SD);
    }
    else
    {
        _writer->print (WR_INIT_LD_ALG_SD);
    }
    _common->printSection ("events", _model->evs (), WR_INIT_LD_ZS);
    if (_writer->isEmpty (WR_INIT_LD_ZS))
    {
        _common->printSection ("events", _model->evs (), WR_INIT_LD_ALG_ZS);
    }
    else
    {
        _writer->print (WR_INIT_LD_ALG_ZS);
    }
    _common->printSection ("states", _model->states (), WR_INIT_LD_SZ);
    if (_writer->isEmpty (WR_INIT_LD_SZ))
    {
        _common->printSection ("states", _model->states (), WR_INIT_LD_ALG_SZ);
    }
    else
    {
        _writer->print (WR_INIT_LD_ALG_SZ);
    }
    _common->printSection ("events", _model->evs (), WR_INIT_LD_HZ);
    _common->printSection ("events", _model->evs (), WR_INIT_LD_HD);
    if (_flags->parallel ())
    {
        _common->printSection ("states", _model->states (), WR_INIT_STATE_HANDLERS);
        _common->printSection ("events", _model->evs (), WR_INIT_LD_DD);
        _common->printSection ("discretes", _model->discretes (), WR_INIT_LD_DH);
    }
    _common->printSection ("events", _model->evs (), WR_INIT_EVENT_LHSST);
    _common->printSection ("events", _model->evs (), WR_INIT_EVENT_RHSST);
    if (_writer->isEmpty (WR_INIT_EVENT_RHSST))
    {
        _common->printSection ("states", _model->states (), WR_INIT_EVENT_ALG_RHSST);
    }
    else
    {
        _writer->print (WR_INIT_EVENT_ALG_RHSST);
    }
    _common->printSection ("events", _model->evs (), WR_INIT_EVENT_DSC);
    _common->printSection ("events", _model->evs (), WR_INIT_EVENT);
    iTime = initTime ();
    _writer->print (WR_INIT_TIME_STRUCT);
    _writer->print (iTime);
    _writer->print (initOutput ());
    _writer->print (WR_ALLOC_OUTPUT_STATES);
    _writer->print (WR_ALLOC_OUTPUT_ALG_STATES);
    _writer->print (WR_ALLOC_OUTPUT_DSC);
    _writer->print (WR_ALLOC_OUTPUT_ALG_DSC);
    if (_model->outs ())
    {
        buffer << "SD_allocOutputMatrix(modelOutput," << _model->states () << "," << _model->discretes () << ");";
        _writer->print (&buffer);
    }
    if (!_writer->isEmpty (WR_INIT_OUTPUT_STATES) || !_writer->isEmpty (WR_INIT_OUTPUT_ALG_STATES))
    {
        _common->printCleanVector ("states", _model->states ());
    }
    if (!_writer->isEmpty (WR_INIT_OUTPUT_DSC) || !_writer->isEmpty (WR_INIT_OUTPUT_ALG_DSC))
    {
        _common->printCleanVector ("discretes", _model->discretes ());
    }
    _common->printSection ("outputs", _model->outs (), WR_INIT_OUTPUT);
    _common->printSection ("outputs", _model->outs (), WR_INIT_OUTPUT_STATES);
    if (_writer->isEmpty (WR_INIT_OUTPUT_STATES))
    {
        _common->printSection ("outputs", _model->outs (), WR_INIT_OUTPUT_ALG_STATES);
    }
    else
    {
        _writer->print (WR_INIT_OUTPUT_ALG_STATES);
    }
    _common->printSection ("outputs", _model->outs (), WR_INIT_OUTPUT_DSC);
    if (_writer->isEmpty (WR_INIT_OUTPUT_DSC))
    {
        _common->printSection ("outputs", _model->outs (), WR_INIT_OUTPUT_ALG_DSC);
    }
    else
    {
        _writer->print (WR_INIT_OUTPUT_ALG_DSC);
    }
    _writer->print (initModel ());
    for (map<string, string>::iterator it = _freeVars.begin (); it != _freeVars.end (); it++)
    {
        _writer->print ("free(" + it->second + ");");
    }
    _writer->endBlock ();
    _writer->print ("}");
    _writer->print ("");
    _writer->print ("void");
    _writer->print ("CLC_initializeDataStructs (CLC_simulator simulator)");
    _writer->print ("{");
    _writer->print ("}");
}

void
QSS_::print (SOL_Function f)
{
    stringstream buffer;
    switch (f)
        {
        case SOL_MODEL_SETTINGS:
            _common->settings ();
            break;
        case SOL_MODEL:
            _print (f, _modelVars, WR_MODEL_SIMPLE, WR_MODEL_GENERIC, true);
            break;
        case SOL_DEPS:
            _print (f, _modelDepsVars, WR_MODEL_DEPS_SIMPLE, WR_MODEL_DEPS_GENERIC, false);
            break;
        case SOL_ZC:
            _print (f, _zcVars, WR_ZC_SIMPLE, WR_ZC_GENERIC, true);
            break;
        case SOL_HANDLER_POS:
            _print (f, _handlerPosVars, WR_HANDLER_POS_SIMPLE, WR_HANDLER_POS_GENERIC, true);
            break;
        case SOL_HANDLER_NEG:
            _print (f, _handlerNegVars, WR_HANDLER_NEG_SIMPLE, WR_HANDLER_NEG_GENERIC, true);
            break;
        case SOL_OUTPUT:
            _print (f, _outputVars, WR_OUTPUT_SIMPLE, WR_OUTPUT_GENERIC, true);
            break;
        case SOL_INIT:
            _init ();
            break;
        default:
            break;
        }
}

void
QSS_::_print (SOL_Function f, map<string, string> localVars, WR_Section simple, WR_Section generic, bool switchGen)
{
    stringstream buffer;
    string indent = _writer->indent (1);
    _writer->print (_engine->prototype (f));
    _writer->beginBlock ();
    for (map<string, string>::iterator it = localVars.begin (); it != localVars.end (); it++)
    {
        _writer->print (it->second);
    }
    if (!_writer->isEmpty (simple))
    {
        _writer->print ("switch(i)");
        _writer->print ("{");
        _writer->print (simple);
        if (!_writer->isEmpty (generic) && switchGen)
        {
            buffer << indent << "default:";
            _writer->print (&buffer);
            _writer->setIndent (2);
            _writer->print (generic);
            _writer->setIndent (0);
        }
        _writer->print ("}");
        if (!switchGen && !_writer->isEmpty (generic))
        {
            _writer->print (generic);
        }
    }
    else if (!_writer->isEmpty (generic))
    {
        _writer->print (generic);
    }
    _writer->endBlock ();
    buffer << "}" << endl;
    _writer->print (&buffer);
}

void
QSS_::initialCode ()
{
    _common->initialCode (&_initializeVars);
}

QSS
newQSS (MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer)
{
    return (new QSS_ (model, flags, writer));
}

void
deleteQSS (QSS m)
{
    delete m;
}

/* Classic Solver class. */

Classic_::Classic_ (MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer) :
        _flags (flags), _model (model), _writer (writer), _modelVars (), _zcVars (), _handlerPosVars (), _handlerNegVars (), _outputVars (), _initializeVars (), _name (
                model->name ()), _freeVars ()
{
    _modelDeps = newMMO_DependenciesTable ();
    if (_flags->hasOutputFile ())
    {
        _name = _flags->outputFileName ();
    }
    _common = newSolverCommon (model, flags, writer);
    _model->varTable ()->setPolyCoeffs (_model->annotation ()->polyCoeffs ());
}

Classic_::~Classic_ ()
{
}

string
Classic_::initData ()
{
    MMO_Annotation annot = _model->annotation ();
    stringstream buffer;
    buffer << "simulator->data = CLC_Data(" << _model->states () << "," << _model->discretes () << "," << _model->evs () << "," << _model->inputs () << ","
            << _model->algs () << ",\"" << _name << "\");" << endl;
    buffer << "  modelData = simulator->data;" << endl;
    buffer << "  const double t = " << annot->initialTime () << ";" << endl;
    return (buffer.str ());
}

string
Classic_::initTime ()
{
    return ("");
}

string
Classic_::initOutput ()
{
    return (_common->initOutput ());
}

string
Classic_::initModel ()
{
    stringstream buffer;
    string zeroCrossing = "NULL", handlerPos = "NULL", handlerNeg = "NULL";
    if (!_writer->isEmpty (WR_ZC_SIMPLE) || !_writer->isEmpty (WR_ZC_GENERIC))
    {
        zeroCrossing = "MOD_zeroCrossing";
    }
    if (!_writer->isEmpty (WR_HANDLER_POS_SIMPLE) || !_writer->isEmpty (WR_HANDLER_POS_GENERIC))
    {
        handlerPos = "MOD_handlerPos";
    }
    if (!_writer->isEmpty (WR_HANDLER_NEG_SIMPLE) || !_writer->isEmpty (WR_HANDLER_NEG_GENERIC))
    {
        handlerNeg = "MOD_handlerNeg";
    }
    buffer << "simulator->model = CLC_Model(MOD_definition," << zeroCrossing << "," << handlerPos << "," << handlerNeg << ", MOD_jacobian);";
    return (buffer.str ());
}

string
Classic_::initHeader ()
{
    stringstream buffer;
    buffer << "#include <common/model.h>" << endl;
    buffer << "#include <qss/qss_model.h>" << endl;
    buffer << "#include <classic/classic_model.h>" << endl;
    buffer << "static CLC_data modelData = NULL;" << endl;
    return (buffer.str ());
}

string
Classic_::runCmd ()
{
    return ("./");
}

string
Classic_::makefile (SOL_Makefile m)
{
    stringstream buffer;
    switch (m)
        {
        case SOL_LIBRARIES:
            if (_flags->debug ())
            {
                return ("LIBS    := -lqssd -ltimestepd");
            }
            else
            {
                return ("LIBS    := -lqss -ltimestep");
            }
        case SOL_INCLUDES:
            return ("INC    := -I" + Util::getInstance ()->environmentVariable ("MMOC_ENGINE"));
        }
    return ("");
}

void
Classic_::initializeMatrices ()
{
    stringstream buffer;
    stringstream bufferGen;
    string indent = _writer->indent (1);
    MMO_EventTable evt = _model->events ();
    bool genericEquation = false;
    bufferGen.str ("");
    buffer << "int *states = (int*)malloc(" << _model->states () << "*sizeof(int));";
    _initializeVars[buffer.str ()] = buffer.str ();
    _freeVars["states"] = "states";
    buffer.str ("");
    if (_model->evs ())
    {
        buffer << "int *events = (int*)malloc(" << _model->evs () << "*sizeof(int));";
        _initializeVars[buffer.str ()] = buffer.str ();
        _freeVars["events"] = "events";
        buffer.str ("");
    }
    MMO_EquationTable et = _model->derivatives ();
    bool hasInit = false;
    for (MMO_Equation e = et->begin (); !et->end (); e = et->next ())
    {
        Index index = et->key ();
        string eqsIdx = e->lhs ().print ("i");
        Dependencies deps = e->exp ()->deps ();
        if (et->endGenericDefinition ())
        {
            genericEquation = false;
            bufferGen.str ("");
            if (hasInit)
            {
                indent = "";
                hasInit = false;
                _writer->write ("}", WR_ALLOC_LD_DS);
                _writer->write ("}", WR_ALLOC_LD_SD);
                _writer->write ("}", WR_INIT_LD_DS);
                _writer->write ("}", WR_INIT_LD_SD);
            }
        }
        if (et->beginGenericDefinition ())
        {
            genericEquation = true;
            if (!hasInit)
            {
                bufferGen << "for(i = " << index.begin () << "; i <= " << index.end () << "; i++)";
            }
        }
        if (deps->hasStates () && !bufferGen.str ().empty ())
        {
            indent = _writer->indent (1);
            hasInit = true;
            _writer->write (&bufferGen, WR_ALLOC_LD_DS, false);
            _writer->write (&bufferGen, WR_INIT_LD_DS, false);
            _writer->write (&bufferGen, WR_INIT_LD_SD, false);
            _writer->write (&bufferGen, WR_ALLOC_LD_SD);
            buffer << "{";
            _writer->write (&buffer, WR_ALLOC_LD_DS, false);
            _writer->write (&buffer, WR_INIT_LD_DS, false);
            _writer->write (&buffer, WR_INIT_LD_SD, false);
            _writer->write (&buffer, WR_ALLOC_LD_SD);
        }
        if (deps->states ())
        {
            buffer << indent << "modelData->nDS[" << eqsIdx << "] = " << deps->states () << ";";
            _writer->write (&buffer, WR_ALLOC_LD_DS);
        }
        map<Index, Index> defStates;
        for (Index *idx = deps->begin (DEP_STATE); !deps->end (DEP_STATE); idx = deps->next (DEP_STATE))
        {
            Index dIdx (*idx);
            defStates[dIdx] = dIdx;
            string sIdx = idx->print ("i");
            buffer << indent << "modelData->DS[" << eqsIdx << "][states[" << eqsIdx << "]++] = " << sIdx << ";";
            _writer->write (&buffer, WR_INIT_LD_DS);
            buffer << indent << "modelData->nSD[" << sIdx << "]++;";
            _writer->write (&buffer, WR_ALLOC_LD_SD);
            buffer << indent << "modelData->SD[" << sIdx << "][states[" << sIdx << "]++] = " << eqsIdx << ";";
            _writer->write (&buffer, WR_INIT_LD_SD);
            _common->addModelDeps (deps, dIdx, e->lhs (), _modelDeps);
        }
        _common->vectorDependencies (e->lhs (), deps, WR_ALLOC_LD_DS, WR_INIT_LD_DS, "modelData->nDS", "modelData->DS", WR_ALLOC_LD_SD, WR_INIT_LD_SD,
                                     "modelData->nSD", "modelData->SD", "states", "states", true, DEP_STATE_VECTOR, &_initializeVars);
        _common->addAlgebriacDeps (deps, e->lhs (), defStates, "modelData->nDS", "modelData->nSD", "modelData->DS", "modelData->SD", WR_ALLOC_LD_ALG_DS,
                                   WR_ALLOC_LD_ALG_SD, WR_INIT_LD_ALG_DS, WR_INIT_LD_ALG_SD, "states", "states", DEP_ALGEBRAIC_STATE, _modelDeps);
    }
    if (genericEquation && hasInit)
    {
        _writer->write ("}", WR_ALLOC_LD_DS);
        _writer->write ("}", WR_ALLOC_LD_SD);
        _writer->write ("}", WR_INIT_LD_DS);
        _writer->write ("}", WR_INIT_LD_SD);
    }
    genericEquation = false;
    for (MMO_Event e = evt->begin (); !evt->end (); e = evt->next ())
    {
        Index index = evt->key ();
        string eIdx = index.print ("i", -index.begin ());
        if (evt->endGenericDefinition ())
        {
            genericEquation = false;
            _writer->write ("}", WR_INIT_EVENT);
        }
        if (evt->beginGenericDefinition ())
        {
            genericEquation = true;
            bufferGen << "for(i = " << index.begin () << "; i <= " << index.end () << "; i++)";
            _writer->write (&bufferGen, WR_INIT_EVENT, false);
            _writer->write ("{", WR_INIT_EVENT);
        }
        if (e->handlerType () == HND_POSITIVE)
        {
            buffer << indent << "modelData->event[" << eIdx << "].direction = 1;";
            _writer->write (&buffer, WR_INIT_EVENT);
        }
        else if (e->handlerType () == HND_NEGATIVE)
        {
            buffer << indent << "modelData->event[" << eIdx << "].direction = -1;";
            _writer->write (&buffer, WR_INIT_EVENT);
        }
        else if (e->handlerType () == HND_ZERO)
        {
            buffer << indent << "modelData->event[" << eIdx << "].direction = 0;";
            _writer->write (&buffer, WR_INIT_EVENT);
        }
    }
    if (genericEquation)
    {
        _writer->write ("}", WR_INIT_EVENT);
    }
    _model->initOutput ();
    if (_model->outs ())
    {
        buffer << "int *outputs = (int*)malloc(" << _model->outs () << "*sizeof(int));";
        _initializeVars[buffer.str ()] = buffer.str ();
        _freeVars["outputs"] = "outputs";
        buffer.str ("");
        if (_model->discretes ())
        {
            buffer << "int *discretes = (int*)malloc(" << _model->discretes () << "*sizeof(int));";
            _initializeVars[buffer.str ()] = buffer.str ();
            _freeVars["discretes"] = "discretes";
        }
    }
}

void
Classic_::model ()
{
    MMO_EquationTable equations = _model->derivatives ();
    VarSymbolTable vt = _model->varTable ();
    MMO_EquationTable algebraics = _model->algebraics ();
    stringstream buffer;
    vt->setPrintEnvironment (VST_CLASSIC_MODEL_FUNCTIONS);
    string indent = _writer->indent (1);
    int order = 1;
    _model->varTable ()->setPrintEnvironment (VST_CLASSIC_MODEL_FUNCTIONS);
    for (MMO_Equation eq = algebraics->begin (); !algebraics->end (); eq = algebraics->next ())
    {
        Index idx = algebraics->key ();
        Index lhs = eq->lhs ();
        if (lhs.hasRange ())
        {
            _common->addLocalVar ("i", &_modelVars);
            buffer << "for(i = " << lhs.begin () << "; i <= " << lhs.end () << "; i++)";
            _writer->write (&buffer, WR_MODEL_SIMPLE);
            buffer << "{";
            _writer->write (&buffer, WR_MODEL_SIMPLE);
            _common->print (eq->print (indent, "alg[" + lhs.print ("i") + "]", "i", false, algebraics, EQ_CLASSIC, order), WR_MODEL_SIMPLE);
            buffer << "}";
            _writer->write (&buffer, WR_MODEL_SIMPLE);
            _common->insertLocalVariables (&_modelVars, eq->getVariables ());
        }
        else
        {
            _common->print (eq->print (indent, "alg[" + lhs.print ("i") + "]", "", false, algebraics, EQ_CLASSIC, order, true), WR_MODEL_SIMPLE);
            _common->insertLocalVariables (&_modelVars, eq->getVariables ());
        }
    }
    for (MMO_Equation eq = equations->begin (); !equations->end (); eq = equations->next ())
    {
        Index idx = equations->key ();
        Index lhs = eq->lhs ();
        if (idx.hasRange ())
        {
            _common->addLocalVar ("i", &_modelVars);
            buffer << "for(i = " << idx.begin () << "; i <= " << idx.end () << "; i++)";
            _writer->write (&buffer, WR_MODEL_SIMPLE);
            buffer << "{";
            _writer->write (&buffer, WR_MODEL_SIMPLE);
            _common->print (eq->print (indent, "dx[" + lhs.print ("i") + "]", "i", false, algebraics, EQ_CLASSIC, order), WR_MODEL_SIMPLE);
            buffer << "}";
            _writer->write (&buffer, WR_MODEL_SIMPLE);
            _common->insertLocalVariables (&_modelVars, eq->getVariables ());
        }
        else
        {
            _common->print (eq->print (indent, "dx[" + lhs.print ("i") + "]", "", false, algebraics, EQ_CLASSIC, order, true), WR_MODEL_SIMPLE);
            _common->insertLocalVariables (&_modelVars, eq->getVariables ());
        }
    }
}

void
Classic_::_printDeps (Dependencies d, Index derivativeIndex, MMO_EquationTable equations, MMO_EquationTable algebraics, string idxStr, WR_Section s, int i,
                      bool constant, Index infIdx)
{
    stringstream buffer;
    int indent = i;
    bool controlRange = (s == WR_MODEL_DEPS_SIMPLE);
    int order = _common->getOrder ();
    int xCoeff = _model->annotation ()->polyCoeffs ();
    bool constantPrint = constant;
    string aux1, aux2;
    for (Index *dIdx = d->begin (DEP_ALGEBRAIC); !d->end (DEP_ALGEBRAIC); dIdx = d->next (DEP_ALGEBRAIC))
    {
        if (d->isVector (dIdx))
        {
            string vn;
            buffer.str ("");
            list<MMO_Equation> eqs = algebraics->equation (*dIdx);
            if (eqs.empty ())
            {
                Error::getInstance ()->add (0, EM_CG | EM_NO_EQ, ER_Error, "Print vector algebraic dependencies for index %s.", dIdx->print ().c_str ());
                return;
            }
            list<MMO_Equation>::iterator eq;
            for (eq = eqs.begin (); eq != eqs.end (); eq++)
            {
                string iter;
                Index lhsEq = (*eq)->lhs ();
                stringstream aLhs;
                if (lhsEq.hasRange ())
                {
                    iter = Util::getInstance ()->newVarName ("j", _model->varTable ());
                    _common->addLocalVar (iter, &_modelDepsVars);
                    buffer << _writer->indent (indent) << "for(" << iter << " = " << lhsEq.begin () << "; " << iter << " <= " << lhsEq.end () << "; " << iter
                            << "++)";
                    _writer->write (&buffer, s);
                    _writer->write ("{", s);
                    aLhs << "alg[(" << (*eq)->lhs ().print (iter) << ")";
                    aLhs << " * " << xCoeff;
                }
                else
                {
                    int cte = dIdx->mappedValue ((*eq)->lhs ().constant ());
                    aLhs << "alg[" << cte * xCoeff;
                }
                _common->print ((*eq)->print (_writer->indent (indent), aLhs.str (), iter, false,
                NULL,
                                              EQ_ALGEBRAIC, order, false, 0, false, dIdx->low (), 0),
                                s);
                if (lhsEq.hasRange ())
                {
                    _writer->write ("}", s);
                }
            }
        }
        else
        {
            string vn, vm;
            string varIdx = "j";
            list<MMO_Equation> eqs = algebraics->equation (*dIdx);
            if (eqs.empty ())
            {
                Error::getInstance ()->add (0, EM_CG | EM_NO_EQ, ER_Error, "Print algebraic dependencies for index %s.", dIdx->print ().c_str ());
                return;
            }
            Index *algebraicState = d->algebraicState (dIdx);
            list<MMO_Equation>::iterator eq;
            for (eq = eqs.begin (); eq != eqs.end (); eq++)
            {
                bool range = ((*eq)->lhs ().hasRange () && (*eq)->lhs ().range () != derivativeIndex.range ()) && (dIdx->factor () != 0);
                if (range)
                {
                    if (aux1.empty ())
                    {
                        aux1 = Util::getInstance ()->newVarName ("j", _model->varTable ());
                        _modelDepsVars["int " + aux1 + " = 0;"] = "int " + aux1 + " = 0;";
                    }
                    vn = aux1;
                }
                else
                {
                    vn = varIdx;
                }
                bool variableMod = false;
                constantPrint = constant;
                stringstream lhs;
                if (constant && algebraicState && algebraicState->hasRange () && dIdx->hasRange ())
                {
                    _common->addLocalVar (varIdx, &_modelDepsVars);
                    buffer << _writer->indent (indent) << "for(" << varIdx << " = " << algebraicState->begin () << ";" << varIdx << " <= "
                            << algebraicState->end () << "; " << varIdx << "++)";
                    _writer->write (&buffer, s);
                    buffer << _writer->indent (indent) << "{";
                    _writer->write (&buffer, s);
                    constantPrint = false;
                    indent++;
                }
                int cte = 0;
                if ((*eq)->lhs ().factor () == 0)
                {
                    cte = (*eq)->lhs ().mappedConstant ();
                    lhs << "alg[" << cte * xCoeff;
                    constantPrint = true;
                }
                else if (dIdx->factor () == 0)
                {
                    if (dIdx->isArray ())
                    {
                        cte = dIdx->mappedValue ((*eq)->lhs ().constant ());
                        lhs << "alg[" << cte * xCoeff;
                        cte = dIdx->constant ();
                        constantPrint = true;
                    }
                    else
                    {
                        lhs << "alg[(" << dIdx->print (varIdx) << ")";
                    }
                }
                else
                {
                    if (dIdx->factor () != 1 || dIdx->operConstant () != 0)
                    {
                        if (aux2.empty ())
                        {
                            aux2 = Util::getInstance ()->newVarName ("j", _model->varTable ());
                            _modelDepsVars["int " + aux2 + " = 0;"] = "int " + aux2 + " = 0;";
                        }
                        vm = aux2;
                        variableMod = true;
                        lhs << "alg[(" << (*eq)->lhs ().print (dIdx->definition ("j"), -(*eq)->lhs ().operConstant ()) << ")";
                        varIdx = vm;
                    }
                    else
                    {
                        lhs << "alg[(" << (*eq)->lhs ().print ("j", -(*eq)->lhs ().operConstant ()) << ")";
                        varIdx = vn;
                    }
                }
                if (!constantPrint)
                {
                    lhs << " * " << xCoeff;
                }
                if (range)
                {
                    _writer->write ((*eq)->printRange ("j", vn, _writer->indent (indent), (*eq)->lhs ()), s);
                }
                if (variableMod)
                {
                    _writer->write ((*eq)->printRange (vn, vm, _writer->indent (indent), *dIdx, true), s);
                }
                _common->print ((*eq)->print (_writer->indent (indent), lhs.str (), varIdx, false,
                NULL,
                                              EQ_ALGEBRAIC, order, constantPrint, 0, false, dIdx->low (), cte),
                                s);
                _common->insertLocalVariables (&_modelDepsVars, (*eq)->getVariables ());
                if (constant && algebraicState && algebraicState->hasRange () && dIdx->hasRange ())
                {
                    indent--;
                    buffer << _writer->indent (indent) << "}";
                    _writer->write (&buffer, s);
                    constantPrint = constant;
                }
                if (variableMod)
                {
                    buffer << _writer->indent (indent) << "}";
                    _writer->write (&buffer, s);
                }
                if (range)
                {
                    buffer << _writer->indent (indent) << "}";
                    _writer->write (&buffer, s);
                }
            }
        }
    }
    constantPrint = constant;
    for (Index *dIdx = d->begin (DEP_STATE); !d->end (DEP_STATE); dIdx = d->next (DEP_STATE))
    {
        stringstream lhs;
        list<MMO_Equation> eqs = equations->equation (*dIdx);
        if (eqs.empty ())
        {
            Error::getInstance ()->add (0, EM_CG | EM_NO_EQ, ER_Error, "Print state dependencies for index %s.", dIdx->print ().c_str ());
            return;
        }
        list<MMO_Equation>::iterator eq;
        for (eq = eqs.begin (); eq != eqs.end (); eq++)
        {
            string varIdx = "j";
            if (dIdx->hasRange () && controlRange)
            {
                _common->addLocalVar (varIdx, &_modelDepsVars);
                buffer << _writer->indent (indent) << "for(" << varIdx << " = " << (*eq)->lhs ().begin () << "; " << varIdx << " <= " << (*eq)->lhs ().end ()
                        << "; " << varIdx << "++)";
                _writer->write (&buffer, s);
                buffer << _writer->indent (indent++) << "{";
                _writer->write (&buffer, s);
                constantPrint = false;
            }
            stringstream varMap;
            lhs << "jac[jit++] ";
            MMO_Equation printEq = (*eq)->jacobianExp (infIdx);
            if (printEq != NULL)
            {
                _common->print (printEq->print (_writer->indent (indent), lhs.str (), varIdx, false,
                NULL,
                                                EQ_JACOBIAN, order, constantPrint, 0, true, dIdx->low (), constant),
                                s);
                _common->insertLocalVariables (&_modelDepsVars, (*eq)->getVariables ());
            }
            if (dIdx->hasRange () && controlRange)
            {
                buffer << _writer->indent (--indent) << "}";
                _writer->write (&buffer, s);
                constantPrint = constant;
            }
        }
    }
}

void
Classic_::_reorderSD (Dependencies d, const Index& idx, const string& indent, stringstream& buffer, WR_InsertType it)
{
    int i = 0, c = d->states () - 1;
    for (i = c; i >= 0; i--)
    {
        Index stateDep = d->key (DEP_STATE, i);
        string sIdx = idx.print ("i");
        string eqsIdx = stateDep.print ("i");
        buffer << indent << "modelData->SD[" << sIdx << "][states[" << sIdx << "]++] = " << eqsIdx << ";";
        _writer->removeFromSection (buffer.str (), WR_INIT_LD_SD);
        _writer->write (&buffer, WR_INIT_LD_SD, true, it);
    }
    return;
}

void
Classic_::modelDeps ()
{
    MMO_EquationTable equations = _model->derivatives ();
    MMO_EquationTable algebraics = _model->algebraics ();
    stringstream buffer;
    string indent = _writer->indent (1);
    for (Dependencies d = _modelDeps->begin (); !_modelDeps->end (); d = _modelDeps->next ())
    {
        Index idx = _modelDeps->key ();
        Index eqIdx = equations->equationIndex (idx);
        if (!idx.hasRange ())
        {
            buffer << indent << "case " << idx.print () << ":";
            _writer->write (&buffer, WR_MODEL_DEPS_SIMPLE);
            _printDeps (d, eqIdx, equations, algebraics, "", WR_MODEL_DEPS_SIMPLE, 3, true, idx);
            buffer << _writer->indent (3) << "break;";
            _writer->write (&buffer, WR_MODEL_DEPS_SIMPLE);
            if (equations->findGenericDependencies (idx.mappedValue ()))
            {
                _reorderSD (d, idx, indent, buffer, WR_APPEND_SIMPLE);
            }
        }
    }
    int i = 0, c = _modelDeps->count () - 1;
    for (i = c; i >= 0; i--)
    {
        Index idx = _modelDeps->key (i);
        Index eqIdx = equations->equationIndex (idx);
        if (idx.hasRange ())
        {
            Dependencies d = _modelDeps->val (i);
            _common->genericDefCondition (eqIdx, idx, WR_MODEL_DEPS_GENERIC, &_modelDepsVars);
            _printDeps (d, eqIdx, equations, algebraics, idx.print ("j"), WR_MODEL_DEPS_GENERIC, 2, false, idx);
            buffer << _writer->indent (1) << "}";
            _writer->write (&buffer, WR_MODEL_DEPS_GENERIC);
            _reorderSD (d, idx, indent, buffer, WR_APPEND_GENERIC);
        }
    }
}

void
Classic_::_jacobian ()
{
    stringstream buffer;
    string indent = _writer->indent (1);
    _writer->print (_prototype (SOL_DEPS));
    if (_generateJacobian ())
    {
        _writer->beginBlock ();
        for (map<string, string>::iterator it = _modelDepsVars.begin (); it != _modelDepsVars.end (); it++)
        {
            _writer->print (it->second);
        }
        _writer->print ("int jit = 0;");
        _writer->print ("int i = 0;");
        buffer << "for( i = 0; i < " << _model->states () << "; i++)";
        _writer->print (&buffer);
        _writer->print ("{");
        if (!_writer->isEmpty (WR_MODEL_DEPS_SIMPLE))
        {
            _writer->print ("switch (i)");
            _writer->print ("{");
            _writer->print (WR_MODEL_DEPS_SIMPLE);
            _writer->print ("}");
        }
        if (!_writer->isEmpty (WR_MODEL_DEPS_GENERIC))
        {
            _writer->print (WR_MODEL_DEPS_GENERIC);
        }
        _writer->print ("}");
        _writer->endBlock ();
    }
    buffer << "}" << endl;
    _writer->print (&buffer);
}

void
Classic_::zeroCrossing ()
{
    _common->zeroCrossing (VST_CLASSIC_MODEL_FUNCTIONS, &_zcVars);
}

void
Classic_::handler ()
{
    _common->handler (VST_CLASSIC_MODEL_FUNCTIONS, &_handlerPosVars, &_handlerNegVars);
}

void
Classic_::output ()
{
    _common->output (VST_CLASSIC_MODEL_FUNCTIONS, &_outputVars);
}

void
Classic_::_print (SOL_Function f, map<string, string> localVars, WR_Section simple, WR_Section generic, bool switchGen)
{
    stringstream buffer;
    string indent = _writer->indent (1);
    _writer->print (_prototype (f));
    _writer->beginBlock ();
    for (map<string, string>::iterator it = localVars.begin (); it != localVars.end (); it++)
    {
        _writer->print (it->second);
    }
    if (f == SOL_MODEL)
    {
        _writer->print ("modelData->scalarEvaluations++;");
    }
    else if (f == SOL_ZC)
    {
        _writer->print ("modelData->zeroCrossings++;");
    }
    if (!_writer->isEmpty (simple) && switchGen)
    {
        _writer->print ("switch(i)");
        _writer->print ("{");
        _writer->print (simple);
        if (!_writer->isEmpty (generic) && switchGen)
        {
            buffer << indent << "default:";
            _writer->print (&buffer);
            _writer->setIndent (2);
            _writer->print (generic);
            _writer->setIndent (0);
        }
        _writer->print ("}");
        if (!switchGen && !_writer->isEmpty (generic))
        {
            _writer->print (generic);
        }
    }
    else
    {
        if (!_writer->isEmpty (simple))
        {
            _writer->print (simple);
        }
        if (!_writer->isEmpty (generic))
        {
            _writer->print (generic);
        }
    }
    _writer->endBlock ();
    buffer << "}" << endl;
    _writer->print (&buffer);
}

Graph
Classic_::graph ()
{
    return (Graph (0, 0));
}

bool
Classic_::_generateJacobian ()
{
    return (_model->annotation()->solver() == ANT_IDA
            ||_model->annotation()->solver() == ANT_CVODE_AM
            ||_model->annotation()->solver() == ANT_CVODE_BDF);
}

void
Classic_::_init ()
{
    stringstream buffer;
    _writer->print (_prototype (SOL_INIT));
    _writer->beginBlock ();
    if (!_writer->isEmpty (WR_INIT_OUTPUT_STATES))
    {
        buffer << "int *states = (int*)malloc(" << _model->states () << "*sizeof(int));";
        _initializeVars[buffer.str ()] = buffer.str ();
        _freeVars["states"] = "states";
        buffer.str ("");
    }
    if (!_writer->isEmpty (WR_INIT_OUTPUT_DSC))
    {
        buffer << "int *discretes = (int*)malloc(" << _model->discretes () << "*sizeof(int));";
        _initializeVars[buffer.str ()] = buffer.str ();
        _freeVars["discretes"] = "discretes";
        buffer.str ("");
    }
    for (map<string, string>::iterator it = _initializeVars.begin (); it != _initializeVars.end (); it++)
    {
        _writer->print (it->second);
    }
    _writer->print (initData ());
    _writer->print (WR_ALLOC_LD);
    if (_generateJacobian ())
    {
        _writer->print (WR_ALLOC_LD_DS);
        _writer->print (WR_ALLOC_LD_SD);
        _writer->print (WR_ALLOC_LD_ALG_DS);
        _writer->print (WR_ALLOC_LD_ALG_SD);
    }
    _writer->print (WR_START_CODE);
    _writer->print (WR_INIT_CODE);
    _writer->print ("CLC_allocDataMatrix(modelData);");
    if (_generateJacobian ())
    {
        _common->printSection ("states", _model->states (), WR_INIT_LD_DS);
        if (_writer->isEmpty (WR_INIT_LD_DS))
        {
            _common->printSection ("states", _model->states (), WR_INIT_LD_ALG_DS);
        }
        else
        {
            _writer->print (WR_INIT_LD_ALG_DS);
        }
        _common->printSection ("states", _model->states (), WR_INIT_LD_SD);
        if (_writer->isEmpty (WR_INIT_LD_SD))
        {
            _common->printSection ("states", _model->states (), WR_INIT_LD_ALG_SD);
        }
        else
        {
            _writer->print (WR_INIT_LD_ALG_SD);
        }
    }
    _writer->print (WR_INIT_EVENT);
    _writer->print (initOutput ());
    _writer->print (WR_ALLOC_OUTPUT_STATES);
    _writer->print (WR_ALLOC_OUTPUT_DSC);
    if (_model->outs ())
    {
        buffer << "SD_allocOutputMatrix(modelOutput," << _model->states () << "," << _model->discretes () << ");";
        _writer->print (&buffer);
    }
    if (!_writer->isEmpty (WR_INIT_OUTPUT_STATES))
    {
        _common->printCleanVector ("states", _model->states ());
    }
    if (!_writer->isEmpty (WR_INIT_OUTPUT_DSC))
    {
        _common->printCleanVector ("discretes", _model->discretes ());
    }
    _common->printSection ("outputs", _model->outs (), WR_INIT_OUTPUT);
    _common->printSection ("outputs", _model->outs (), WR_INIT_OUTPUT_STATES);
    _common->printSection ("outputs", _model->outs (), WR_INIT_OUTPUT_DSC);
    _writer->print (initModel ());
    for (map<string, string>::iterator it = _freeVars.begin (); it != _freeVars.end (); it++)
    {
        _writer->print ("free(" + it->second + ");");
    }
    _writer->endBlock ();
    _writer->print ("}");
    _writer->print ("");
    _writer->print ("void");
    _writer->print ("QSS_initializeDataStructs (QSS_simulator simulator)");
    _writer->print ("{");
    _writer->print ("}");
}

void
Classic_::print (SOL_Function f)
{
    stringstream buffer;
    switch (f)
        {
        case SOL_MODEL_SETTINGS:
            _common->settings ();
            break;
        case SOL_MODEL:
            _print (f, _modelVars, WR_MODEL_SIMPLE, WR_MODEL_GENERIC, false);
            break;
        case SOL_ZC:
            _print (f, _zcVars, WR_ZC_SIMPLE, WR_ZC_GENERIC, true);
            break;
        case SOL_HANDLER_POS:
            _print (f, _handlerPosVars, WR_HANDLER_POS_SIMPLE, WR_HANDLER_POS_GENERIC, true);
            break;
        case SOL_HANDLER_NEG:
            _print (f, _handlerNegVars, WR_HANDLER_NEG_SIMPLE, WR_HANDLER_NEG_GENERIC, true);
            break;
        case SOL_INIT:
            _init ();
            break;
        case SOL_OUTPUT:
            _print (f, _outputVars, WR_OUTPUT_SIMPLE, WR_OUTPUT_GENERIC, true);
            break;
        case SOL_DEPS:
            _jacobian ();
            break;
        default:
            break;
        }
}

void
Classic_::initialCode ()
{
    _common->addLocalVar ("i", &_initializeVars);
    _common->initialCode (&_initializeVars);
}

string
Classic_::_prototype (SOL_Function f)
{
    switch (f)
        {
        case SOL_MODEL:
            return ("void\nMOD_definition(double *x, double *d, double *alg, double t, double *dx)\n{");
        case SOL_ZC:
            return ("void\nMOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)\n{");
        case SOL_HANDLER_POS:
            return ("void\nMOD_handlerPos(int i, double *x, double *d, double *alg, double t)\n{");
        case SOL_HANDLER_NEG:
            return ("void\nMOD_handlerNeg(int i, double *x, double *d, double *alg, double t)\n{");
        case SOL_OUTPUT:
            return ("void\nMOD_output(int i, double *x, double *d, double *alg, double t, double *out)\n{");
        case SOL_INIT:
            return ("void\nCLC_initializeDataStructs(CLC_simulator simulator)\n{");
        case SOL_CALLBACK:
            return ("setData(modelData,modelOutput,modelDefinition,modelSettings);");
        case SOL_DEPS:
            return ("void\nMOD_jacobian(double *x, double *d, double *alg, double t, double *jac)\n{");
        default:
            break;
        }
    return ("");
}

Classic
newClassic (MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer)
{
    return (new Classic_ (model, flags, writer));
}

void
deleteClassic (Classic m)
{
    delete m;
}

/* MMO_SerialEngine class. */

MMO_SerialEngine_::MMO_SerialEngine_ (MMO_CompileFlags flags, bool hybrid) :
        _flags (flags), _hybrid (hybrid)
{
}

MMO_SerialEngine_::~MMO_SerialEngine_ ()
{
}

string
MMO_SerialEngine_::prototype (SOL_Function f)
{
    switch (f)
        {
        case SOL_MODEL:
            return ("void\nMOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)\n{");
        case SOL_DEPS:
            return ("void\nMOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)\n{");
        case SOL_ZC:
            return ("void\nMOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)\n{");
        case SOL_HANDLER_POS:
            return ("void\nMOD_handlerPos(int i, double *x, double *d, double *alg, double t)\n{");
        case SOL_HANDLER_NEG:
            return ("void\nMOD_handlerNeg(int i, double *x, double *d, double *alg, double t)\n{");
        case SOL_OUTPUT:
            return ("void\nMOD_output(int i, double *x, double *d, double *alg, double t, double *out)\n{");
        case SOL_INIT:
            return ("void\nQSS_initializeDataStructs(QSS_simulator simulator)\n{");
        case SOL_CALLBACK:
            return ("setData(modelData,modelTime,modelOutput,modelDefinition,modelSettings);");
        default:
            break;
        }
    return ("");
}

string
MMO_SerialEngine_::makefile (SOL_Makefile m)
{
    switch (m)
        {
        case SOL_LIBRARIES:
            if (_flags->debug ())
            {
                return ("LIBS    := -lqssd -ltimestepd");
            }
            else
            {
                return ("LIBS    := -lqss -ltimestep");
            }
        case SOL_INCLUDES:
            return ("INC    := -I" + Util::getInstance ()->environmentVariable ("MMOC_ENGINE"));
        }
    return ("");
}

string
MMO_SerialEngine_::variableMap (string index)
{
    return ("");
}

string
MMO_SerialEngine_::endMap ()
{
    return ("");
}

void
MMO_SerialEngine_::eventTemp ()
{
}

MMO_SerialEngine
newMMO_SerialEngine (MMO_CompileFlags flags, bool hybrid)
{
    return (new MMO_SerialEngine_ (flags, hybrid));
}

void
deleteMMO_SerialEngine (MMO_SerialEngine m)
{
    delete m;
}

/* MMO_ParallelEngine class. */

MMO_ParallelEngine_::MMO_ParallelEngine_ (MMO_CompileFlags flags, bool hybrid) :
        _flags (flags), _hybrid (hybrid)
{
}

MMO_ParallelEngine_::~MMO_ParallelEngine_ ()
{
}

string
MMO_ParallelEngine_::variableMap (string index)
{
    stringstream buffer;
    buffer << "if (map[" << index << "] > NOT_ASSIGNED)" << endl << "{";
    return (buffer.str ());
}

string
MMO_ParallelEngine_::endMap ()
{
    return ("}");
}

void
MMO_ParallelEngine_::eventTemp ()
{
}

string
MMO_ParallelEngine_::prototype (SOL_Function f)
{
    switch (f)
        {
        case SOL_MODEL:
            return ("void\nMOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)\n{");
        case SOL_DEPS:
            return ("void\nMOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)\n{");
        case SOL_ZC:
            return ("void\nMOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)\n{");
        case SOL_HANDLER_POS:
            return ("void\nMOD_handlerPos(int i, double *x, double *d, double *alg, double t)\n{");
        case SOL_HANDLER_NEG:
            return ("void\nMOD_handlerNeg(int i, double *x, double *d, double *alg, double t)\n{");
        case SOL_OUTPUT:
            return ("void\nMOD_output(int i, double *x, double *d, double *alg, double t, double *out)\n{");
        case SOL_INIT:
            return ("void\nQSS_initializeDataStructs(QSS_simulator simulator)\n{");
        case SOL_CALLBACK:
            return ("setData(modelData,modelTime,modelOutput,modelDefinition,modelSettings);");
        default:
            break;
        }
    return ("");
}

string
MMO_ParallelEngine_::makefile (SOL_Makefile m)
{
    switch (m)
        {
        case SOL_LIBRARIES:
            if (_flags->debug ())
            {
                return ("LIBS    := -lqssd -ltimestepd");
            }
            else
            {
                return ("LIBS    := -lqss -ltimestep");
            }
        case SOL_INCLUDES:
            return ("INC    := -I" + Util::getInstance ()->environmentVariable ("MMOC_ENGINE"));
        }
    return ("");
}

MMO_ParallelEngine
newMMO_ParallelEngine (MMO_CompileFlags flags, bool hybrid)
{
    return (new MMO_ParallelEngine_ (flags, hybrid));
}

void
deleteMMO_ParallelEngine (MMO_ParallelEngine m)
{
    delete m;
}

SolverCommon_::SolverCommon_ (MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer, MMO_DependenciesTable modelVectorDeps, Graph *graph) :
        _model (model), _flags (flags), _writer (writer), _modelVectorDeps (modelVectorDeps), _name (model->name ()), _weights (), _graph (*graph), _generateGraph (
                true), _parallel (flags->graph ())
{
    if (_flags->hasOutputFile ())
    {
        _name = _flags->outputFileName ();
    }
}

SolverCommon_::~SolverCommon_ ()
{
}

void
SolverCommon_::addModelDeps (Dependencies deps, Index state, Index infIndex, MMO_DependenciesTable modelDeps, int index)
{
    Dependencies d = modelDeps->lookup (state);
    if (d == NULL)
    {
        d = newDependencies ();
    }
    d->insert (infIndex, DEP_STATE);
    if (infIndex.isArray () && !infIndex.hasRange ())
    {
        d->join (deps, infIndex, DEP_ALGEBRAIC, index);
    }
    else
    {
        d->join (deps, infIndex, DEP_ALGEBRAIC);
    }
    d->join (deps, infIndex, DEP_ALGEBRAIC_VECTOR);
    modelDeps->insert (state, d);
}

void
SolverCommon_::graphInsert (Index row, Index col, int offset, NOD_Type type, int assignments)
{
    if (_model->annotation ()->partitionMethod () == ANT_Manual || _generateGraph == false)
    {
        return;
    }
    if (row.hasMap () || col.hasMap ())
    {
        _generateGraph = false;
        return;
    }
    int rowOffset = 0, colOffset = 0;
    if (type == NOD_HD)
    {
        rowOffset = offset;
    }
    else if (type == NOD_HZ || type == NOD_DD)
    {
        colOffset = offset;
        rowOffset = offset;
    }
    else if (type == NOD_SZ)
    {
        colOffset = offset;
    }
    if (row.hasRange ())
    {
        int i, begin = row.begin (), end = row.end ();
        for (i = begin; i <= end; i++)
        {
            if (!((type == NOD_SD || type == NOD_HZ || type == NOD_DD) && row.mappedValue (i) == col.mappedValue (i)))
            {
                _graph.addGraphEdge (row.mappedValue (i) + rowOffset, col.mappedValue (i) + colOffset);
                _graph.addGraphEdge (col.mappedValue (i) + colOffset, row.mappedValue (i) + rowOffset);
                _graph.addHyperGraphEdge (row.mappedValue (i) + rowOffset, col.mappedValue (i) + colOffset);
                _graph.addHyperGraphEdge (row.mappedValue (i) + rowOffset, row.mappedValue (i) + rowOffset);
                _graph.addNodeWeight (row.mappedValue (i) + rowOffset, assignments);
            }
        }
    }
    else
    {
        if (!((type == NOD_SD || type == NOD_HZ || type == NOD_DD) && row.mappedValue () == col.mappedValue ()))
        {
            _graph.addGraphEdge (row.mappedValue () + rowOffset, col.mappedValue () + colOffset);
            _graph.addGraphEdge (col.mappedValue () + colOffset, row.mappedValue () + rowOffset);
            _graph.addHyperGraphEdge (row.mappedValue () + rowOffset, col.mappedValue () + colOffset);
            _graph.addHyperGraphEdge (row.mappedValue () + rowOffset, row.mappedValue () + rowOffset);
            _graph.addNodeWeight (row.mappedValue () + rowOffset, assignments);
        }
    }
}

void
SolverCommon_::addAlgebriacDeps (Dependencies deps, Index derivativeIndex, map<Index, Index> states, string alloc, string allocInverse, string init,
                                 string initInverse, WR_Section allocSection, WR_Section allocInverseSection, WR_Section initSection,
                                 WR_Section initInverseSection, string type, string inverseType, DEP_Type depType, MMO_DependenciesTable modelDeps)
{
    bool matrix = true;
    if (allocInverseSection == WR_NULL)
    {
        matrix = false;
    }
    bool eval = false;
    int nodOffset = 0;
    NOD_Type nt = NOD_SD;
    if (_parallel)
    {
        if (allocInverseSection == WR_ALLOC_LD_ALG_SZ || allocSection == WR_ALLOC_EVENT_ALG_RHSST || allocInverseSection == WR_ALLOC_LD_ALG_SD)
        {
            eval = true;
        }
        if (allocInverseSection == WR_ALLOC_LD_ALG_SZ || allocSection == WR_ALLOC_EVENT_ALG_RHSST)
        {
            nodOffset = _model->states ();
            nt = NOD_SZ;
        }
    }
    for (Index *index = deps->begin (depType); !deps->end (depType); index = deps->next (depType))
    {
        Index eqIndex = *index;
        Index idx = deps->key (depType);
        if (states.find (idx) == states.end ())
        {
            stringstream buffer;
            string indent;
            string variableString = "i";
            if (idx.hasRange () && eqIndex.hasRange ())
            {
                // X[i] = B[i];
                stringstream bufferGen;
                string eqIdx = derivativeIndex.print ("i");
                string stIdx = idx.print (variableString);
                bufferGen << "for ( i = " << eqIndex.begin () << "; i <= " << eqIndex.end () << "; i++) ";
                _writer->write (&bufferGen, allocSection, false);
                _writer->write ("{", allocSection);
                buffer << indent << alloc;
                if (matrix)
                {
                    buffer << "[" << eqIdx << "]";
                }
                buffer << "++;";
                _writer->write (&buffer, allocSection);
                _writer->write ("}", allocSection);
                _writer->write (&bufferGen, initSection, false);
                _writer->write ("{", initSection);
                buffer << indent << init;
                if (matrix)
                {
                    buffer << "[" << eqIdx << "]";
                }
                buffer << "[" << type << "[" << eqIdx << "]++] = " << stIdx << ";";
                _writer->write (&buffer, initSection);
                _writer->write ("}", initSection);
                _writer->write (&bufferGen, allocInverseSection, false);
                _writer->write ("{", allocInverseSection);
                buffer << indent << allocInverse << "[" << stIdx << "]++;";
                _writer->write (&buffer, allocInverseSection);
                _writer->write ("}", allocInverseSection);
                _writer->write (&bufferGen, initInverseSection);
                _writer->write ("{", initInverseSection);
                buffer << indent << initInverse << "[" << stIdx << "][" << inverseType << "[" << stIdx << "]++] = " << eqIdx << ";";
                _writer->write (&buffer, initInverseSection);
                _writer->write ("}", initInverseSection);
                Index infIndex = derivativeIndex;
                if (modelDeps != NULL)
                {
                    addModelDeps (deps, idx, derivativeIndex, modelDeps);
                }
                if (eval && _parallel)
                {
                    graphInsert (idx, derivativeIndex, nodOffset, nt);
                }
            }
            else if (!idx.hasRange () && eqIndex.hasRange ())
            {
                // X[i] = B[N];
                stringstream bufferGen;
                string eqIdx = derivativeIndex.print ("i");
                string stIdx = idx.print (variableString); // Has to be a constant definition.
                bufferGen << "for ( i = " << eqIndex.begin () << "; i <= " << eqIndex.end () << "; i++) ";
                _writer->write (&bufferGen, allocSection, false);
                _writer->write ("{", allocSection);
                buffer << indent << alloc;
                if (matrix)
                {
                    buffer << "[" << eqIdx << "]";
                }
                buffer << "++;";
                _writer->write (&buffer, allocSection);
                _writer->write ("}", allocSection);
                _writer->write (&bufferGen, initSection, false);
                _writer->write ("{", initSection);
                buffer << indent << init;
                if (matrix)
                {
                    buffer << "[" << eqIdx << "]";
                }
                buffer << "[" << type << "[" << eqIdx << "]++] = " << stIdx << ";";
                _writer->write (&buffer, initSection);
                _writer->write ("}", initSection);
                buffer << indent << allocInverse << "[" << stIdx << "] += " << eqIndex.range () << ";";
                _writer->write (&buffer, allocInverseSection);
                _writer->write (&bufferGen, initInverseSection);
                _writer->write ("{", initInverseSection);
                buffer << indent << initInverse << "[" << stIdx << "][" << inverseType << "[" << stIdx << "]++] = " << eqIdx << ";";
                _writer->write (&buffer, initInverseSection);
                _writer->write ("}", initInverseSection);
                if (modelDeps != NULL)
                {
                    addModelDeps (deps, idx, derivativeIndex, modelDeps);
                }
                if (eval && _parallel)
                {
                    graphInsert (idx, derivativeIndex, nodOffset, nt);
                }
            }
            else if (idx.hasRange () && !eqIndex.hasRange ())
            {
                // X[N] = B[i];
                Error::getInstance ()->add (0, EM_CG | EM_ALG_INDEX, ER_Error, "Algebraic index out of range %s.", idx.print ().c_str ());
            }
            else if (!idx.hasRange () && !eqIndex.hasRange ())
            {
                // X[N] = B[N];
                int eqIdx = derivativeIndex.mappedValue (eqIndex.constant ());
                string stIdx = idx.print (variableString);
                buffer << indent << alloc;
                if (matrix)
                {
                    buffer << "[" << eqIdx << "]";
                }
                buffer << "++;";
                _writer->write (&buffer, allocSection);
                buffer << indent << init;
                if (matrix)
                {
                    buffer << "[" << eqIdx << "]";
                }
                buffer << "[" << type << "[" << eqIdx << "]++] = " << stIdx << ";";
                _writer->write (&buffer, initSection);
                buffer << indent << allocInverse << "[" << stIdx << "]++;";
                _writer->write (&buffer, allocInverseSection);
                buffer << indent << initInverse << "[" << stIdx << "][" << inverseType << "[" << stIdx << "]++] = " << eqIdx << ";";
                _writer->write (&buffer, initInverseSection);
                if (modelDeps != NULL)
                {
                    addModelDeps (deps, idx, derivativeIndex.indexValue (eqIndex.operConstant ()), modelDeps, eqIndex.constant ());
                }
                if (eval && _parallel)
                {
                    graphInsert (idx, derivativeIndex.indexValue (eqIndex.operConstant ()), nodOffset, nt);
                }
            }
        }
    }
}

string
SolverCommon_::initOutput ()
{
    stringstream buffer;
    MMO_Annotation annot = _model->annotation ();
    string period = "NULL";
    string indent = "";
    int ssize = 0;
    if (annot->commInterval () == "CI_Sampled")
    {
        indent = _writer->indent (1);
        list<double> sample = annot->sample ();
        ssize = sample.size ();
        period = "period";
        buffer << indent << "double period[" << ssize << "];" << endl;
        int n = 0;
        for (list<double>::iterator i = sample.begin (); i != sample.end (); i++)
        {
            buffer << indent << "period[" << n << "] = " << *i << ";" << endl;
        }
    }
    string outputFunction = "NULL";
    if (_model->outs ())
    {
        outputFunction = "MOD_output";
    }
    buffer << indent << "simulator->output = SD_Output(\"" << _name << "\"," << _model->outs () << ",";
    buffer << _model->discretes () << "," << _model->states () << "," << period << "," << ssize << ",";
    buffer << annot->initialTime () << "," << annot->commInterval () << "," << annot->storeData ();
    buffer << "," << outputFunction << ");" << endl;
    buffer << indent << "SD_output modelOutput = simulator->output;" << endl;
    return (buffer.str ());
}

void
SolverCommon_::print (list<string> exps, WR_Section section)
{
    for (list<string>::iterator it = exps.begin (); it != exps.end (); it++)
    {
        _writer->write (*it, section);
    }
}

void
SolverCommon_::insertLocalVariables (map<string, string> *local, list<string> vars)
{
    for (list<string>::iterator it = vars.begin (); it != vars.end (); it++)
    {
        local->insert (pair<string, string> (*it, *it));
    }
}

void
SolverCommon_::addLocalVar (string name, map<string, string> *variables, string type, int size)
{
    string var;
    if (type.empty ())
    {
        var = "int " + name;
    }
    else
    {
        var = type + " " + name;
    }
    if (size > 0)
    {
        stringstream buffer;
        buffer << "[" << size << "];";
        var.append (buffer.str ());
    }
    else
    {
        var.append (" = 0;");
    }
    if (variables->find (var) == variables->end ())
    {
        variables->insert (pair<string, string> (var, var));
    }
}

void
SolverCommon_::eventPrologue (Index idx, WR_Section simple, WR_Section generic)
{
    stringstream buffer;
    string indent = _writer->indent (1);
    if (idx.hasRange ())
    {
        buffer << "if(i >= " << idx.mappedBegin () << " && i <= " << idx.mappedEnd () << ")";
        _writer->write (&buffer, generic);
        buffer << "{";
        _writer->write (&buffer, generic);
    }
    else
    {
        buffer << indent << "case " << idx.print () << ":";
        _writer->write (&buffer, simple);
    }
}

void
SolverCommon_::eventEpilogue (Index idx, WR_Section simple, WR_Section generic)
{
    stringstream buffer;
    string indent = _writer->indent (2);
    if (idx.hasRange ())
    {
        buffer << "}";
        _writer->write (&buffer, generic);
    }
    else
    {
        buffer << indent << "return;";
        _writer->write (&buffer, simple);
    }
}

void
SolverCommon_::handler (VST_Environment type, map<string, string> *handlerPosVars, map<string, string> *handlerNegVars)
{
    MMO_EventTable events = _model->events ();
    VarSymbolTable vt = _model->varTable ();
    stringstream buffer;
    vt->setPrintEnvironment (type);
    string indent = _writer->indent (2);
    for (MMO_Event ev = events->begin (); !events->end (); ev = events->next ())
    {
        Index idx = events->key ();
        if (ev->hasPositiveHandler ())
        {
            eventPrologue (idx, WR_HANDLER_POS_SIMPLE, WR_HANDLER_POS_GENERIC);
        }
        for (MMO_Statement st = ev->begin (HND_POSITIVE); !ev->end (); st = ev->next ())
        {
            if (idx.hasRange ())
            {
                print (st->print (_writer->indent (1), "i", -idx.mappedBegin () + idx.begin (), 1, idx.low ()), WR_HANDLER_POS_GENERIC);
            }
            else
            {
                print (st->print (indent), WR_HANDLER_POS_SIMPLE);
            }
            insertLocalVariables (handlerPosVars, st->getVariables ());
        }
        if (ev->hasPositiveHandler ())
        {
            eventEpilogue (idx, WR_HANDLER_POS_SIMPLE, WR_HANDLER_POS_GENERIC);
        }
        if (ev->hasNegativeHandler ())
        {
            eventPrologue (idx, WR_HANDLER_NEG_SIMPLE, WR_HANDLER_NEG_GENERIC);
        }
        for (MMO_Statement st = ev->begin (HND_NEGATIVE); !ev->end (); st = ev->next ())
        {
            if (idx.hasRange ())
            {
                print (st->print (_writer->indent (1), "i", -idx.mappedBegin () + idx.begin (), 1, idx.low ()), WR_HANDLER_NEG_GENERIC);
            }
            else
            {
                print (st->print (indent), WR_HANDLER_NEG_SIMPLE);
            }
            insertLocalVariables (handlerNegVars, st->getVariables ());
        }
        if (ev->hasNegativeHandler ())
        {
            eventEpilogue (idx, WR_HANDLER_NEG_SIMPLE, WR_HANDLER_NEG_GENERIC);
        }
    }
}

void
SolverCommon_::zeroCrossing (VST_Environment type, map<string, string> *zcVars)
{
    MMO_EventTable events = _model->events ();
    VarSymbolTable vt = _model->varTable ();
    MMO_EquationTable algebraics = _model->algebraics ();
    stringstream buffer;
    vt->setPrintEnvironment (type);
    string indent = _writer->indent (1);
    int order = getOrder ();
    for (MMO_Event ev = events->begin (); !events->end (); ev = events->next ())
    {
        Index idx = events->key ();
        if (ev->hasWeight ())
        {
            _weights.insert (pair<Index, double> (idx, ev->weight ()));
        }
        MMO_Equation exp = ev->condition ();
        if (idx.hasRange ())
        {
            eventPrologue (idx, WR_ZC_SIMPLE, WR_ZC_GENERIC);
            print (exp->print (indent, "zc", "i", true, algebraics, EQ_ZC, order, false, -idx.mappedBegin () + idx.begin (), true, idx.low ()), WR_ZC_GENERIC);
            buffer << "}";
            _writer->write (&buffer, WR_ZC_GENERIC);
            insertLocalVariables (zcVars, exp->getVariables ());
        }
        else
        {
            eventPrologue (idx, WR_ZC_SIMPLE, WR_ZC_GENERIC);
            print (exp->print (_writer->indent (2), "zc", "i", true, algebraics, EQ_ZC, order, true), WR_ZC_SIMPLE);
            buffer << _writer->indent (2) << "return;";
            _writer->write (&buffer, WR_ZC_SIMPLE);
            insertLocalVariables (zcVars, exp->getVariables ());
        }
    }
}

map<Index, double>
SolverCommon_::weights ()
{
    return (_weights);
}

int
SolverCommon_::getOrder ()
{
    MMO_Annotation annot = _model->annotation ();
    int order = 1;
    if (annot->symDiff ())
    {
        order = annot->order ();
    }
    return (order);
}

void
SolverCommon_::printCleanVector (string var, int size)
{
    stringstream buffer;
    string indent = _writer->indent (1);
    buffer << indent << "cleanVector(" << var << "," << 0 << "," << size << ");" << endl;
    _writer->print (&buffer);
}

void
SolverCommon_::printSection (string var, int size, WR_Section section)
{
    if (!_writer->isEmpty (section))
    {
        printCleanVector (var, size);
        _writer->print (section);
    }
}

void
SolverCommon_::initialCode (map<string, string> *initializeVars)
{
    string indent = _writer->indent (1);
    MMO_StatementTable st = _model->initialCode ();
    _model->varTable ()->setPrintEnvironment (VST_INIT);
    if (_model->annotation ()->solver () == ANT_DASSL || _model->annotation ()->solver () == ANT_DOPRI || _model->annotation ()->solver () == ANT_CVODE_BDF
            || _model->annotation ()->solver () == ANT_IDA || _model->annotation ()->solver () == ANT_CVODE_AM)
    {
        _model->varTable ()->setPrintEnvironment (VST_CLASSIC_INIT);
    }
    for (MMO_Statement stm = st->begin (); !st->end (); stm = st->next ())
    {
        list<string> stms = stm->print (indent);
        for (list<string>::iterator it = stms.begin (); it != stms.end (); it++)
        {
            _writer->write (*it, WR_INIT_CODE);
        }
        insertLocalVariables (initializeVars, stm->getVariables ());
    }
}

void
SolverCommon_::output (VST_Environment type, map<string, string> *outputVars)
{
    MMO_EquationTable outputs = _model->outputs ();
    VarSymbolTable vt = _model->varTable ();
    stringstream buffer, bufferGen, bufferGenDsc, bufferGenInit;
    vt->setPrintEnvironment (type);
    string indent = _writer->indent (1);
    bool genericEquation = false;
    bool hasInit = false, hasInitDsc = false, hasOutput = false;
    for (MMO_Equation eq = outputs->begin (); !outputs->end (); eq = outputs->next ())
    {
        Index index = outputs->key ();
        string indexStr = index.print ("i");
        Dependencies deps = eq->exp ()->deps ();
        if (outputs->endGenericDefinition ())
        {
            genericEquation = false;
            if (hasInit || hasInitDsc || hasOutput)
            {
                indent = "";
                _writer->write ("}", WR_INIT_OUTPUT);
                hasOutput = false;
            }

            if (hasInit)
            {
                indent = "";
                hasInit = false;
                _writer->write ("}", WR_ALLOC_OUTPUT_STATES);
                _writer->write ("}", WR_INIT_OUTPUT_STATES);
            }
            if (hasInitDsc)
            {
                indent = "";
                hasInitDsc = false;
                _writer->write ("}", WR_ALLOC_OUTPUT_DSC);
                _writer->write ("}", WR_INIT_OUTPUT_DSC);
            }
        }
        if (outputs->beginGenericDefinition ())
        {
            genericEquation = true;
            stringstream bufferTemp;
            bufferTemp << "for(i = " << index.begin () << "; i <= " << index.end () << "; i++)";
            if (!hasInit)
            {
                bufferGen << bufferTemp.str ();
            }
            if (!hasInitDsc)
            {
                bufferGenDsc << bufferTemp.str ();
            }
            bufferGenInit << bufferTemp.str ();
        }
        if (((deps->hasStates () || deps->hasDiscretes () || deps->hasAlgebraicStates () || deps->hasAlgebraicDiscretes ()) && genericEquation)
                && !bufferGenInit.str ().empty ())
        {
            hasOutput = deps->hasAlgebraicStates () || deps->hasAlgebraicDiscretes ();
            _writer->write (&bufferGenInit, WR_INIT_OUTPUT);
            _writer->write ("{", WR_INIT_OUTPUT);
        }
        if ((deps->hasStates () && !bufferGen.str ().empty ()) && genericEquation)
        {
            indent = _writer->indent (1);
            hasInit = true;
            _writer->write (&bufferGen, WR_ALLOC_OUTPUT_STATES, false);
            _writer->write (&bufferGen, WR_INIT_OUTPUT_STATES);
            _writer->write ("{", WR_ALLOC_OUTPUT_STATES);
            _writer->write ("{", WR_INIT_OUTPUT_STATES);
        }
        if ((deps->hasDiscretes () && !bufferGenDsc.str ().empty ()) && genericEquation)
        {
            indent = _writer->indent (1);
            hasInitDsc = true;
            _writer->write (&bufferGenDsc, WR_ALLOC_OUTPUT_DSC, false);
            _writer->write (&bufferGenDsc, WR_INIT_OUTPUT_DSC);
            _writer->write ("{", WR_ALLOC_OUTPUT_DSC);
            _writer->write ("{", WR_INIT_OUTPUT_DSC);
        }
        if (index.hasRange ())
        {
            genericDefCondition (index, eq->lhs (), WR_OUTPUT_GENERIC, outputVars);
            _model->varTable ()->setPrintEnvironment (type);
            print (eq->print (indent, "out", "j", true, _model->algebraics (), EQ_OUTPUT, 1, false, 0, false, index.low ()), WR_OUTPUT_GENERIC);
            buffer << "}";
            insertLocalVariables (outputVars, eq->getVariables ());
            _writer->write (&buffer, WR_OUTPUT_GENERIC);
            _model->varTable ()->setPrintEnvironment (VST_OUTPUT);
            string eqExpStr = eq->exp ()->print ("%d");
            if (eq->exp ()->exp ()->expressionType () == EXPNAMEDARGUMENT)
            {
                eqExpStr = *(eq->exp ()->exp ()->getAsNamedArgument ()->name ());
                if (eq->exp ()->hasIndexes ())
                {
                    eqExpStr.append ("[%d]");
                }
                buffer << indent << "sprintf(modelOutput->variable[" << indexStr << "].name,\"" << eqExpStr << "\",i);";
                _writer->write (&buffer, WR_INIT_OUTPUT);
            }
            else
            {
                buffer << indent << "sprintf(modelOutput->variable[" << indexStr << "].name,\"" << eqExpStr << "\",";
                buffer << eq->exp ()->indexes ("i") << ");";
                _writer->write (&buffer, WR_INIT_OUTPUT);
            }
        }
        else
        {
            buffer << indent << "case " << eq->lhs ().mappedConstant () << ":";
            _writer->write (&buffer, WR_OUTPUT_SIMPLE);
            _model->varTable ()->setPrintEnvironment (type);
            print (eq->print (_writer->indent (2), "out", "", true, _model->algebraics (), EQ_OUTPUT, 1, false, 0, false), WR_OUTPUT_SIMPLE);
            insertLocalVariables (outputVars, eq->getVariables ());
            buffer << _writer->indent (2) << "return;";
            _writer->write (&buffer, WR_OUTPUT_SIMPLE);
            _model->varTable ()->setPrintEnvironment (VST_OUTPUT);
            string eqExpStr = eq->exp ()->print ("");
            if (eq->exp ()->exp ()->expressionType () == EXPNAMEDARGUMENT)
            {
                eqExpStr = *(eq->exp ()->exp ()->getAsNamedArgument ()->name ());
            }
            buffer << indent << "sprintf(modelOutput->variable[" << indexStr << "].name,\"" << eqExpStr << "\");";
            _writer->write (&buffer, WR_INIT_OUTPUT);
        }
        if (deps->states ())
        {
            buffer << indent << "modelOutput->nOS[" << indexStr << "] = " << deps->states () << ";";
            _writer->write (&buffer, WR_ALLOC_OUTPUT_STATES);
        }
        map<Index, Index> defStates;
        for (Index *idx = deps->begin (DEP_STATE); !deps->end (DEP_STATE); idx = deps->next (DEP_STATE))
        {
            defStates[*idx] = *idx;
            string idxStr = idx->print ("i", -index.begin ());
            buffer << indent << "modelOutput->nSO[" << idxStr << "]++;";
            _writer->write (&buffer, WR_ALLOC_OUTPUT_STATES);
            buffer << indent << "modelOutput->SO[" << idxStr << "][states[" << idxStr << "]++] = " << indexStr << ";";
            _writer->write (&buffer, WR_INIT_OUTPUT_STATES);
            buffer << indent << "modelOutput->OS[" << indexStr << "][outputs[" << indexStr << "]++] = " << idxStr << ";";
            _writer->write (&buffer, WR_INIT_OUTPUT_STATES);
        }
        addAlgebriacDeps (deps, eq->lhs (), defStates, "modelOutput->nOS", "modelOutput->nSO", "modelOutput->OS", "modelOutput->SO", WR_ALLOC_OUTPUT_ALG_STATES,
                          WR_ALLOC_OUTPUT_ALG_STATES, WR_INIT_OUTPUT_ALG_STATES, WR_INIT_OUTPUT_ALG_STATES, "outputs", "states");
        if (deps->discretes ())
        {
            buffer << indent << "modelOutput->nOD[" << indexStr << "] = " << deps->discretes () << ";";
            _writer->write (&buffer, WR_ALLOC_OUTPUT_DSC);
        }
        defStates.clear ();
        for (Index *idx = deps->begin (DEP_DISCRETE); !deps->end (DEP_DISCRETE); idx = deps->next (DEP_DISCRETE))
        {
            defStates[*idx] = *idx;
            string idxStr = idx->print ("i", -index.begin ());
            buffer << indent << "modelOutput->nDO[" << idxStr << "]++;";
            _writer->write (&buffer, WR_ALLOC_OUTPUT_DSC);
            buffer << indent << "modelOutput->DO[" << idxStr << "][discretes[" << idxStr << "]++] = " << index.print ("i") << ";";
            _writer->write (&buffer, WR_INIT_OUTPUT_DSC);
            buffer << indent << "modelOutput->OD[" << indexStr << "][outputs[" << indexStr << "]++] = " << idxStr << ";";
            _writer->write (&buffer, WR_INIT_OUTPUT_DSC);
        }
        addAlgebriacDeps (deps, eq->lhs (), defStates, "modelOutput->nOD", "modelOutput->nDO", "modelOutput->OD", "modelOutput->DO", WR_ALLOC_OUTPUT_ALG_DSC,
                          WR_ALLOC_OUTPUT_ALG_DSC, WR_INIT_OUTPUT_ALG_DSC, WR_INIT_OUTPUT_ALG_DSC, "outputs", "discretes", DEP_ALGEBRAIC_DISCRETE);
        vectorDependencies (index, deps, WR_ALLOC_OUTPUT_STATES, WR_INIT_OUTPUT_STATES, "modelOutput->nOS", "modelOutput->OS", WR_ALLOC_OUTPUT_STATES,
                            WR_INIT_OUTPUT_STATES, "modelOutput->nSO", "modelOutput->SO", "outputs", "states", false, DEP_STATE_VECTOR, outputVars);
        vectorDependencies (index, deps, WR_ALLOC_OUTPUT_DSC, WR_INIT_OUTPUT_DSC, "modelOutput->nOD", "modelOutput->OD", WR_ALLOC_OUTPUT_DSC,
                            WR_INIT_OUTPUT_DSC, "modelOutput->nDO", "modelOutput->DO", "outputs", "discretes", false, DEP_DISCRETE_VECTOR, outputVars);
    }
    if ((genericEquation && hasInit) || (genericEquation && hasInitDsc) || (genericEquation && hasOutput))
    {
        _writer->write ("}", WR_INIT_OUTPUT);
    }
    if (genericEquation && hasInit)
    {
        _writer->write ("}", WR_ALLOC_OUTPUT_STATES);
        _writer->write ("}", WR_INIT_OUTPUT_STATES);
    }
    if (genericEquation && hasInitDsc)
    {
        _writer->write ("}", WR_ALLOC_OUTPUT_DSC);
        _writer->write ("}", WR_INIT_OUTPUT_DSC);
    }
}

void
SolverCommon_::genericDefCondition (Index eqIdx, Index idx, WR_Section section, map<string, string> *variables)
{
    stringstream buffer;
    if (idx.factor () == 1)
    {
        addLocalVar ("j", variables);
        buffer << "j = " << idx.printReverse ("i") << ";";
        _writer->write (&buffer, section);
        buffer << "if(j >=" << idx.begin () << " && j <= " << idx.end () << ")";
        _writer->write (&buffer, section);
        buffer << "{";
        _writer->write (&buffer, section);
    }
    else if (eqIdx.factor () != 0)
    {
        string tmp = Util::getInstance ()->newVarName ("tmp", _model->varTable ());
        addLocalVar (tmp, variables, "double");
        addLocalVar ("j", variables);
        buffer << tmp << " = (double)" << idx.printReverse ("i") << ";";
        _writer->write (&buffer, section);
        buffer << "if((int)" << tmp << " == " << tmp << " && " << tmp << " >= " << eqIdx.begin () << " && " << tmp << " <= " << eqIdx.end () << ")";
        _writer->write (&buffer, section);
        buffer << "{";
        _writer->write (&buffer, section);
        buffer << "j = (int) " << tmp << ";";
        _writer->write (&buffer, section);

    }
    else
    {
        addLocalVar ("j", variables);
        buffer << "j = i;";
        _writer->write (&buffer, section);
        buffer << "if(j == " << idx.printReverse ("i") << ")";
        _writer->write (&buffer, section);
        buffer << "{";
        _writer->write (&buffer, section);
    }
}

void
SolverCommon_::vectorDependencies (Index index, Dependencies deps, WR_Section alloc, WR_Section init, string allocString, string initString,
                                   WR_Section allocInverse, WR_Section initInverse, string allocInverseString, string initInverseString, string count,
                                   string countInverse, bool dependencies, DEP_Type type, map<string, string> *variables)
{
    stringstream buffer;
    string indent = _writer->indent (1);
    bool genericIndex = false;
    string forIdx = "i";
    bool eval = false;
    int offset = 0;
    NOD_Type nt = NOD_SD;
    if (_parallel)
    {
        if (alloc == WR_ALLOC_LD_DD)
        {
            offset = _model->states ();
            nt = NOD_DD;
            eval = true;
        }
        if (allocInverse == WR_ALLOC_LD_SZ)
        {
            offset = _model->states ();
            nt = NOD_SZ;
        }
        if (allocInverse == WR_ALLOC_LD_SZ || allocInverse == WR_ALLOC_LD_SD)
        {
            eval = true;
        }
    }
    if (index.factor () != 0)
    {
        genericIndex = true;
        forIdx = "j";
        addLocalVar ("j", variables);
    }
    string eqsIdx = index.print ("i");
    int numDeps = deps->stateVectors ();
    if (type == DEP_DISCRETE_VECTOR)
    {
        numDeps = deps->discreteVectors ();
    }
    if (numDeps)
    {
        buffer << indent << allocString << "[" << eqsIdx << "] += " << numDeps << ";";
        _writer->write (&buffer, alloc);
    }
    for (Index *idx = deps->begin (type); !deps->end (type); idx = deps->next (type))
    {
        Index dIdx (*idx);
        string sIdx = dIdx.print (forIdx);
        buffer << "for(" << forIdx << " = " << idx->begin () << "; " << forIdx << " <= " << idx->end () << "; " << forIdx << "++)";
        _writer->write (&buffer, init, false);
        _writer->write ("{", init);
        if (init != initInverse && !initInverseString.empty ())
        {
            _writer->write (&buffer, initInverse, false);
            _writer->write ("{", initInverse);
        }
        if (genericIndex)
        {
            buffer.str ("");
        }
        else if (!allocInverseString.empty ())
        {
            _writer->write (&buffer, allocInverse);
            _writer->write ("{", allocInverse);
        }
        buffer << indent << initString << "[" << eqsIdx << "][" << count << "[" << eqsIdx << "]++] = " << sIdx << ";";
        _writer->write (&buffer, init);
        if (!allocInverseString.empty ())
        {
            if (genericIndex)
            {
                buffer << indent << allocInverseString << "[" << dIdx.print ("i") << "] += " << index.range () << ";";
                _writer->write (&buffer, allocInverse);
            }
            else
            {
                buffer << indent << allocInverseString << "[" << sIdx << "]++;";
                _writer->write (&buffer, allocInverse);
            }
        }
        if (!initInverseString.empty ())
        {
            buffer << indent << initInverseString << "[" << sIdx << "][" << countInverse << "[" << sIdx << "]++] = " << eqsIdx << ";";
            _writer->write (&buffer, initInverse);
        }
        if (!genericIndex && !allocInverseString.empty ())
        {
            _writer->write ("}", allocInverse);
        }
        if (init != initInverse && !initInverseString.empty ())
        {
            _writer->write ("}", initInverse);
        }
        _writer->write ("}", init);
        if (dependencies)
        {
            Dependencies d = _modelVectorDeps->lookup (dIdx);
            if (d == NULL)
            {
                d = newDependencies ();
            }
            d->insert (index, DEP_STATE_VECTOR);
            _modelVectorDeps->insert (dIdx, d);
        }
        if (eval && _parallel)
        {
            graphInsert (dIdx, index, offset, nt);
        }
    }
}

SolverCommon
newSolverCommon (MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer, MMO_DependenciesTable modelVectorDeps, Graph *graph)
{
    return (new SolverCommon_ (model, flags, writer, modelVectorDeps, graph));
}

void
deleteSolverCommon (SolverCommon m)
{
    delete m;
}

bool
SolverCommon_::hasGraph ()
{
    return (_generateGraph);
}

void
SolverCommon_::settings ()
{
    stringstream buffer;
    buffer << "void\nMOD_settings(SD_simulationSettings settings)\n{";
    _writer->print (&buffer);
    buffer << "\t settings->debug = " << _flags->debug () << ";";
    _writer->print (&buffer);
    if (_flags->parallel ())
    {
        buffer << "\t settings->parallel = TRUE;";
        _writer->print (&buffer);
    }
    else
    {
        buffer << "\t settings->parallel = FALSE;";
        _writer->print (&buffer);
    }
    if (_model->evs () > 0)
    {
        buffer << "\t settings->hybrid = TRUE;";
        _writer->print (&buffer);
    }
    else
    {
        buffer << "\t settings->hybrid = FALSE;";
        _writer->print (&buffer);
    }
    buffer << "\t settings->method = " << _model->annotation ()->solver () << ";";
    _writer->print (&buffer);
    _writer->print ("}\n");
}

