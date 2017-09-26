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

#include "dependencies.h"

#include <stddef.h>

#include "index.h"

Dependencies_::Dependencies_() :
    _autonomous(true), _stateVectorsCount(0), _discreteVectorsCount(0), _algebraicVectorsCount(
        0)
{
  _states = newMMO_IndexTable();
  _discretes = newMMO_IndexTable();
  _algebraics = newMMO_IndexTable();
  _algebraicsDefs = newMMO_IndexTable();
  _algebraicsState = newMMO_IndexTable();
  _algebraicsDiscrete = newMMO_IndexTable();
  _stateVectors = newMMO_IndexTable();
  _discreteVectors = newMMO_IndexTable();
  _algebraicVectorsDef = newMMO_IndexTable();
  _algebraicVectors = newMMO_IndexTable();
  _algebraicStates = newMMO_IndexTable();
}

Dependencies_::~Dependencies_()
{
}

void
Dependencies_::insert(VarInfo vi)
{
  Index idx = vi->index();
  Index *i = new Index(idx);
  if(vi->isDiscrete())
  {
    _discretes->insert(idx, i);
  }
  else if(vi->isState())
  {
    _states->insert(idx, i);
  }
  else if(vi->isAlgebraic())
  {
    _algebraicsDefs->insert(idx, i);
  }
  _autonomous = _autonomous && !vi->isTime();
}

void
Dependencies_::insert(Index idx, Index dep, DEP_Type t)
{
  MMO_IndexTable m = _select(t);
  Index *i = new Index(dep);
  m->insert(idx, i);
}

void
Dependencies_::insert(Index idx, DEP_Type t)
{
  DEP_Type type = t;
  Index *i = new Index(idx);
  if(t == DEP_ALGEBRAIC_VECTOR)
  {
    type = DEP_ALGEBRAIC;
    _algebraicVectors->insert(idx, i);
  }
  MMO_IndexTable m = _select(type);
  m->insert(idx, i);
}

void
Dependencies_::join(Dependencies d, Index st, DEP_Type t, int val)
{
  MMO_IndexTable m = _select(t);
  for(Index *idx = d->begin(t); !d->end(t); idx = d->next(t))
  {
    Index *index = new Index(*idx);
    if(index->hasRange() && !d->isVector(idx))
    {
      index->setFactor(0);
      index->setConstant(idx->modelicaValue(val));
      index->setArray();
      index->setLow(1);
      index->setHi(1);
    }
    if(t == DEP_ALGEBRAIC || t == DEP_ALGEBRAIC_VECTOR)
    {
      Index *state = _algebraicStates->lookup(*idx);
      if(_algebraicStates->lookup(*idx) == NULL)
      {
        state = new Index(st);
      }
      else
      {
        if(st.low() < state->low())
        {
          state->setLow(st.low());
        }
        if(st.hi() > state->hi())
        {
          state->setHi(st.hi());
        }
      }
      _algebraicStates->insert(*idx, state);
    }
    m->insert(*index, index);
  }
  _autonomous = _autonomous && d->autonomous();
}

bool
Dependencies_::_testIntersection(MMO_IndexTable m, Index key)
{
  if(!key.hasMap())
  {
    for(Index *test = m->begin(); !m->end(); test = m->next())
    {
      Intersection is = test->intersection(key);
      if(is.type() == IDX_SUBSET)
      {
        return false;
      }
    }
  }
  return true;
}

void
Dependencies_::join(Dependencies d, DEP_Type t)
{
  MMO_IndexTable m = _select(t);
  if(t == DEP_ALGEBRAIC_STATE || t == DEP_ALGEBRAIC_DISCRETE)
  {
    for(Index *idx = d->begin(t); !d->end(t); idx = d->next(t))
    {
      Index *i = new Index(*idx);
      Index key = d->key(t);
      if(_testIntersection(m, key))
      {
        m->insert(key, i);
      }
    }
  }
  else
  {
    for(Index *idx = d->begin(t); !d->end(t); idx = d->next(t))
    {
      Index *i = new Index(*idx);
      if(_testIntersection(m, *idx))
      {
        m->insert(*idx, i);
      }
    }
  }
  map<Index, string>::iterator it;
  for(it = d->_stateIds.begin(); it != d->_stateIds.end(); it++)
  {
    _stateIds[it->first] = it->second;
  }
  for(it = d->_algebraicIds.begin(); it != d->_algebraicIds.end(); it++)
  {
    _algebraicIds[it->first] = it->second;
  }
  _autonomous = _autonomous && d->autonomous();
}

void
Dependencies_::join(Dependencies d, Index index, DEP_Type t)
{
  MMO_IndexTable m = _select(t);
  if(t == DEP_ALGEBRAIC_STATE || t == DEP_ALGEBRAIC_DISCRETE)
  {
    for(Index *idx = d->begin(t); !d->end(t); idx = d->next(t))
    {
      Index *i = new Index(*idx);
      Index key = d->key(t);
      if(_testIntersection(m, key))
      {
        m->insert(key, i);
      }
    }
  }
  else
  {
    for(Index *idx = d->begin(t); !d->end(t); idx = d->next(t))
    {
      Index *i = new Index(*idx);
      if(_testIntersection(m, *idx))
      {
        if(t == DEP_ALGEBRAIC || t == DEP_ALGEBRAIC_VECTOR)
        {
          Index *state = _algebraicStates->lookup(*idx);
          if(_algebraicStates->lookup(*idx) == NULL)
          {
            state = new Index(index);
          }
          else
          {
            if(index.low() < state->low())
            {
              state->setLow(index.low());
            }
            if(index.hi() > state->hi())
            {
              state->setHi(index.hi());
            }
          }
          _algebraicStates->insert(*idx, state);
        }
        m->insert(*idx, i);
      }
    }
  }
  _autonomous = _autonomous && d->autonomous();
}

void
Dependencies_::join(Dependencies d, DEP_Type t, int val)
{
  MMO_IndexTable m = _select(t);
  for(Index *idx = d->begin(t); !d->end(t); idx = d->next(t))
  {
    Index *index = new Index(*idx);
    if(index->hasRange() && !d->isVector(idx))
    {
      index->setFactor(0);
      index->setConstant(idx->modelicaValue(val));
      index->setArray();
      index->setLow(1);
      index->setHi(1);
    }
    m->insert(*index, index);
  }
  _autonomous = _autonomous && d->autonomous();
}

void
Dependencies_::join(Dependencies d)
{
  if(d == NULL)
  {
    return;
  }
  join(d, DEP_STATE);
  join(d, DEP_DISCRETE);
  join(d, DEP_ALGEBRAIC);
  join(d, DEP_ALGEBRAIC_DEF);
  join(d, DEP_ALGEBRAIC_STATE);
  join(d, DEP_ALGEBRAIC_DISCRETE);
  join(d, DEP_STATE_VECTOR);
  join(d, DEP_DISCRETE_VECTOR);
  join(d, DEP_ALGEBRAIC_VECTOR);
  join(d, DEP_ALGEBRAIC_VECTOR_DEF);
}

void
Dependencies_::join(Dependencies d, int val)
{
  if(d == NULL)
  {
    return;
  }
  join(d, DEP_STATE, val);
  join(d, DEP_DISCRETE, val);
  join(d, DEP_ALGEBRAIC, val);
  join(d, DEP_ALGEBRAIC_DEF, val);
  join(d, DEP_ALGEBRAIC_STATE, val);
  join(d, DEP_ALGEBRAIC_DISCRETE, val);
  join(d, DEP_STATE_VECTOR, val);
  join(d, DEP_DISCRETE_VECTOR, val);
  join(d, DEP_ALGEBRAIC_VECTOR, val);
  join(d, DEP_ALGEBRAIC_VECTOR_DEF, val);
}

void
Dependencies_::join(Dependencies d, Index index)
{
  if(d == NULL)
  {
    return;
  }
  join(d, index, DEP_STATE);
  join(d, index, DEP_DISCRETE);
  join(d, index, DEP_ALGEBRAIC);
  join(d, index, DEP_ALGEBRAIC_DEF);
  join(d, index, DEP_ALGEBRAIC_STATE);
  join(d, index, DEP_ALGEBRAIC_DISCRETE);
  join(d, index, DEP_STATE_VECTOR);
  join(d, index, DEP_DISCRETE_VECTOR);
  join(d, index, DEP_ALGEBRAIC_VECTOR);
  join(d, index, DEP_ALGEBRAIC_VECTOR_DEF);
}

bool
Dependencies_::isVector(Index* idx)
{
  return find(*idx, DEP_ALGEBRAIC_VECTOR);
}

MMO_IndexTable
Dependencies_::_select(DEP_Type t)
{
  switch(t)
  {
    case DEP_STATE:
      return _states;
    case DEP_DISCRETE:
      return _discretes;
    case DEP_ALGEBRAIC:
      return _algebraics;
    case DEP_ALGEBRAIC_DEF:
      return _algebraicsDefs;
    case DEP_ALGEBRAIC_STATE:
      return _algebraicsState;
    case DEP_ALGEBRAIC_DISCRETE:
      return _algebraicsDiscrete;
    case DEP_STATE_VECTOR:
      return _stateVectors;
    case DEP_DISCRETE_VECTOR:
      return _discreteVectors;
    case DEP_ALGEBRAIC_VECTOR:
      return _algebraicVectors;
    case DEP_ALGEBRAIC_VECTOR_DEF:
      return _algebraicVectorsDef;
    default:
      break;
  }
  return _states;
}

Index *
Dependencies_::begin(DEP_Type t)
{
  MMO_IndexTable m = _select(t);
  Index *q = m->begin();
  if(q == NULL)
  {
    return NULL;
  }
  Index *i = new Index(*q);
  return i;
}

bool
Dependencies_::end(DEP_Type t)
{
  MMO_IndexTable m = _select(t);
  return m->end();
}

Index
Dependencies_::key(DEP_Type t)
{
  MMO_IndexTable m = _select(t);
  return m->key();
}

Index
Dependencies_::key(DEP_Type t, int at)
{
  MMO_IndexTable m = _select(t);
  return m->key(at);
}

Index *
Dependencies_::next(DEP_Type t)
{
  MMO_IndexTable m = _select(t);
  Index *q = m->next();
  if(q == NULL)
  {
    return NULL;
  }
  Index *i = new Index(*q);
  return i;
}

bool
Dependencies_::find(Index idx, DEP_Type t)
{
  MMO_IndexTable m = _select(t);
  Index *i = m->lookup(idx);
  return i != NULL;
}

int
Dependencies_::states()
{
  return _states->count();
}

int
Dependencies_::discretes()
{
  return _discretes->count();
}

int
Dependencies_::algebraics()
{
  return _algebraics->count();
}

int
Dependencies_::stateVectors()
{
  _stateVectorsCount = 0;
  for(Index *idx = _stateVectors->begin(); !_stateVectors->end(); idx =
      _stateVectors->next())
  {
    _stateVectorsCount += idx->range();
  }
  return _stateVectorsCount;
}

int
Dependencies_::discreteVectors()
{
  _discreteVectorsCount = 0;
  for(Index *idx = _discreteVectors->begin(); !_discreteVectors->end(); idx =
      _discreteVectors->next())
  {
    _discreteVectorsCount += idx->range();
  }
  return _discreteVectorsCount;
}

bool
Dependencies_::autonomous()
{
  return _autonomous;
}

void
Dependencies_::setAutonomous(bool a)
{
  _autonomous = a;
}

bool
Dependencies_::hasStates()
{
  return _states->count() > 0 || stateVectors() > 0;
}

bool
Dependencies_::hasDiscretes()
{
  return _discretes->count() > 0 || discreteVectors() > 0;
}

bool
Dependencies_::hasAlgebraics()
{
  return _algebraics->count() > 0;
}

bool
Dependencies_::hasAlgebraicStates()
{
  return _algebraicsState->count() > 0;
}

bool
Dependencies_::hasAlgebraicDiscretes()
{
  return _algebraicsDiscrete->count() > 0;
}

Dependencies
newDependencies()
{
  return new Dependencies_();
}

void
deleteDependencies(Dependencies m)
{
  delete m;
}

Index *
Dependencies_::algebraicState(Index* idx)
{
  return _algebraicStates->lookup(*idx);
}

void
Dependencies_::insert(Index idx, string stateId, DEP_Type type)
{
  if(type == DEP_STATE)
  {
    _stateIds[idx] = stateId;
  }
  else
  {
    _algebraicIds[idx] = stateId;
  }
}

string
Dependencies_::identifier(Index idx, DEP_Type type)
{
  if(type == DEP_STATE)
  {
    return _stateIds[idx];
  }
  else
  {
    return _algebraicIds[idx];
  }
  return "";
}

map<Index, string>
Dependencies_::_identifier(DEP_Type t)
{
  if(t == DEP_STATE)
  {
    return _stateIds;
  }
  else
  {
    return _algebraicIds;
  }
  return map<Index, string>();
}
