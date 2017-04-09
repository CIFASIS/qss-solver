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

#ifndef GENERATOR_TYPES_H_
#define GENERATOR_TYPES_H_
class Classic_;
class MMO_Engine_;
class MMO_Files_;
class MMO_FileWriter_;
class MMO_Generator_;
class MMO_MemoryWriter_;
class MMO_ParallelEngine_;
class MMO_SerialEngine_;
class MMO_Solver_;
class MMO_Writer_;
class QSS_;
class SolverCommon_;

/**
 *
 */
typedef class MMO_Generator_ *MMO_Generator;
/**
 *
 */
typedef class MMO_Solver_ *MMO_Solver;
/**
 *
 */
typedef class QSS_ *QSS;
/**
 *
 */
typedef class Classic_ *Classic;
/**
 *
 */
typedef class SolverCommon_ *SolverCommon;
/**
 *
 */
typedef class MMO_Files_ *MMO_Files;
/**
 *
 */
typedef class MMO_Writer_ *MMO_Writer;
/**
 *
 */
typedef class MMO_MemoryWriter_ *MMO_MemoryWriter;
/**
 *
 */
typedef class MMO_FileWriter_ *MMO_FileWriter;
/**
 *
 */
typedef class MMO_Engine_ *MMO_Engine;
/**
 *
 */
typedef class MMO_SerialEngine_ *MMO_SerialEngine;
/**
 *
 */
typedef class MMO_ParallelEngine_ *MMO_ParallelEngine;

#endif  /* GENERATOR_TYPES_H_ */
