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

#include "qss_partition.h"

#include <stddef.h>

#include "../common/data.h"

#ifdef __linux__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../common/scotch.h"
#include "../common/metis.h"
#include "../common/patoh.h"
#include "../common/utils.h"
#include "qss_graph.h"

int PRT_getInt(char opt[128])
{
  int ret;
  sscanf(opt, "%d", &ret);
  return ret;
}

float PRT_getFloat(char opt[128])
{
  float ret;
  sscanf(opt, "%f", &ret);
  return ret;
}

double PRT_getDouble(char opt[128])
{
  double ret;
  sscanf(opt, "%lf", &ret);
  return ret;
}

int PRT_getMetisOption(char opt[128])
{
  // Parse Metis Options
  if (strcmp(opt, "METIS_OP_PMETIS") == 0) return METIS_OP_PMETIS;
  if (strcmp(opt, "METIS_OP_kMETIS") == 0) return METIS_OP_KMETIS;
  if (strcmp(opt, "METIS_OP_OMETIS") == 0) return METIS_OP_OMETIS;

  /*! Options codes (i.e., options[]) */
  if (strcmp(opt, "METIS_OPTION_PTYPE") == 0) return METIS_OPTION_PTYPE;
  if (strcmp(opt, "METIS_OPTION_OBJTYPE") == 0) return METIS_OPTION_OBJTYPE;
  if (strcmp(opt, "METIS_OPTION_CTYPE") == 0) return METIS_OPTION_CTYPE;
  if (strcmp(opt, "METIS_OPTION_IPTYPE") == 0) return METIS_OPTION_IPTYPE;
  if (strcmp(opt, "METIS_OPTION_RTYPE") == 0) return METIS_OPTION_RTYPE;
  if (strcmp(opt, "METIS_OPTION_DBGLVL") == 0) return METIS_OPTION_DBGLVL;
  if (strcmp(opt, "METIS_OPTION_NITER") == 0) return METIS_OPTION_NITER;
  if (strcmp(opt, "METIS_OPTION_NCUTS") == 0) return METIS_OPTION_NCUTS;
  if (strcmp(opt, "METIS_OPTION_SEED") == 0) return METIS_OPTION_SEED;
  if (strcmp(opt, "METIS_OPTION_NO2HOP") == 0) return METIS_OPTION_NO2HOP;
  if (strcmp(opt, "METIS_OPTION_MINCONN") == 0) return METIS_OPTION_MINCONN;
  if (strcmp(opt, "METIS_OPTION_CONTIG") == 0) return METIS_OPTION_CONTIG;
  if (strcmp(opt, "METIS_OPTION_COMPRESS") == 0) return METIS_OPTION_COMPRESS;
  if (strcmp(opt, "METIS_OPTION_CCORDER") == 0) return METIS_OPTION_CCORDER;
  if (strcmp(opt, "METIS_OPTION_PFACTOR") == 0) return METIS_OPTION_PFACTOR;
  if (strcmp(opt, "METIS_OPTION_NSEPS") == 0) return METIS_OPTION_NSEPS;
  if (strcmp(opt, "METIS_OPTION_UFACTOR") == 0) return METIS_OPTION_UFACTOR;
  if (strcmp(opt, "METIS_OPTION_NUMBERING") == 0) return METIS_OPTION_NUMBERING;

  /* Used for command-line parameter purposes */
  if (strcmp(opt, "METIS_OPTION_HELP") == 0) return METIS_OPTION_HELP;
  if (strcmp(opt, "METIS_OPTION_TPWGTS") == 0) return METIS_OPTION_TPWGTS;
  if (strcmp(opt, "METIS_OPTION_NCOMMON") == 0) return METIS_OPTION_NCOMMON;
  if (strcmp(opt, "METIS_OPTION_NOOUTPUT") == 0) return METIS_OPTION_NOOUTPUT;
  if (strcmp(opt, "METIS_OPTION_BALANCE") == 0) return METIS_OPTION_BALANCE;
  if (strcmp(opt, "METIS_OPTION_GTYPE") == 0) return METIS_OPTION_GTYPE;
  if (strcmp(opt, "METIS_OPTION_UBVEC") == 0) return METIS_OPTION_UBVEC;

  /*! Partitioning Schemes */
  if (strcmp(opt, "METIS_PTYPE_RB") == 0) return METIS_PTYPE_RB;
  if (strcmp(opt, "METIS_PTYPE_KWAY") == 0) return METIS_PTYPE_KWAY;

  /*! Graph types for meshes */
  if (strcmp(opt, "METIS_GTYPE_DUAL") == 0) return METIS_GTYPE_DUAL;
  if (strcmp(opt, "METIS_GTYPE_NODAL") == 0) return METIS_GTYPE_NODAL;

  /*! Coarsening Schemes */
  if (strcmp(opt, "METIS_CTYPE_RM") == 0) return METIS_CTYPE_RM;
  if (strcmp(opt, "METIS_CTYPE_SHEM") == 0) return METIS_CTYPE_SHEM;

  /*! Initial partitioning schemes */
  if (strcmp(opt, "METIS_IPTYPE_GROW") == 0) return METIS_IPTYPE_GROW;
  if (strcmp(opt, "METIS_IPTYPE_RANDOM") == 0) return METIS_IPTYPE_RANDOM;
  if (strcmp(opt, "METIS_IPTYPE_EDGE") == 0) return METIS_IPTYPE_EDGE;
  if (strcmp(opt, "METIS_IPTYPE_NODE") == 0) return METIS_IPTYPE_EDGE;
  if (strcmp(opt, "METIS_IPTYPE_METISRB") == 0) return METIS_IPTYPE_METISRB;

  /*! Refinement schemes */
  if (strcmp(opt, "METIS_RTYPE_FM") == 0) return METIS_RTYPE_FM;
  if (strcmp(opt, "METIS_RTYPE_GREEDY") == 0) return METIS_RTYPE_GREEDY;
  if (strcmp(opt, "METIS_RTYPE_SEP2SIDED") == 0) return METIS_RTYPE_SEP2SIDED;
  if (strcmp(opt, "METIS_RTYPE_SEP1SIDED") == 0) return METIS_RTYPE_SEP1SIDED;

  /*! Debug Levels */
  if (strcmp(opt, "METIS_DBG_INFO") == 0) return METIS_DBG_INFO;
  if (strcmp(opt, "METIS_DBG_TIME") == 0) return METIS_DBG_TIME;
  if (strcmp(opt, "METIS_DBG_COARSEN") == 0) return METIS_DBG_COARSEN;
  if (strcmp(opt, "METIS_DBG_REFINE") == 0) return METIS_DBG_REFINE;
  if (strcmp(opt, "METIS_DBG_IPART") == 0) return METIS_DBG_IPART;
  if (strcmp(opt, "METIS_DBG_MOVEINFO") == 0) return METIS_DBG_MOVEINFO;
  if (strcmp(opt, "METIS_DBG_SEPINFO") == 0) return METIS_DBG_SEPINFO;
  if (strcmp(opt, "METIS_DBG_CONNINFO") == 0) return METIS_DBG_CONNINFO;
  if (strcmp(opt, "METIS_DBG_CONTIGINFO") == 0) return METIS_DBG_CONTIGINFO;
  if (strcmp(opt, "METIS_DBG_MEMORY") == 0) return METIS_DBG_MEMORY;

  /* Types of objectives */
  if (strcmp(opt, "METIS_OBJTYPE_CUT") == 0) return METIS_OBJTYPE_CUT;
  if (strcmp(opt, "METIS_OBJTYPE_VOL") == 0) return METIS_OBJTYPE_VOL;
  if (strcmp(opt, "METIS_OBJTYPE_NODE") == 0) return METIS_OBJTYPE_NODE;

  return PRT_getInt(opt);
}

void PRT_setMetisOptions(idx_t options[METIS_NOPTIONS], QSS_data data)
{
  int i, t = data->params->partitionerOptions.nMetis;
  for (i = 0; i < t; i++) {
    options[PRT_getMetisOption(data->params->partitionerOptions.metis[i].lvalue)] =
        PRT_getMetisOption(data->params->partitionerOptions.metis[i].value);
  }
}

int PRT_getPatohOption(char opt[128])
{
  if (strcmp(opt, "PATOH_CONPART") == 0) return PATOH_CONPART;
  if (strcmp(opt, "PATOH_CUTPART") == 0) return PATOH_CUTPART;

  /* ------------------- Parameter Initialization -------------------*/
  if (strcmp(opt, "PATOH_SUGPARAM_DEFAULT") == 0) return PATOH_SUGPARAM_DEFAULT;
  if (strcmp(opt, "PATOH_SUGPARAM_SPEED") == 0) return PATOH_SUGPARAM_SPEED;
  if (strcmp(opt, "PATOH_SUGPARAM_QUALITY") == 0) return PATOH_SUGPARAM_QUALITY;

  /* ------------------- Balance  -------------------*/
  if (strcmp(opt, "PATOH_BALANCE_STRICT") == 0) return PATOH_BALANCE_STRICT;
  if (strcmp(opt, "PATOH_BALANCE_ADAPTIVE") == 0) return PATOH_BALANCE_ADAPTIVE;
  if (strcmp(opt, "PATOH_BALANCE_RELAXED") == 0) return PATOH_BALANCE_RELAXED;

  /* ------------------- Coarsening  ------------------- */
  /* -------- Visit Order -------------- */
  if (strcmp(opt, "PATOH_VO_RAND") == 0) return PATOH_VO_RAND;
  if (strcmp(opt, "PATOH_VO_SCD") == 0) return PATOH_VO_SCD;
  if (strcmp(opt, "PATOH_VO_SMAXNS") == 0) return PATOH_VO_SMAXNS;
  if (strcmp(opt, "PATOH_VO_SMINNS") == 0) return PATOH_VO_SMINNS;
  if (strcmp(opt, "PATOH_VO_SMINNSSUM") == 0) return PATOH_VO_SMINNSSUM;
  if (strcmp(opt, "PATOH_VO_SWEEP") == 0) return PATOH_VO_SWEEP;

  /* -------- Matching -------------- */
  if (strcmp(opt, "PATOH_CRS_HCM") == 0) return PATOH_CRS_HCM;
  if (strcmp(opt, "PATOH_CRS_PHCM") == 0) return PATOH_CRS_PHCM;
  if (strcmp(opt, "PATOH_CRS_MANDIS") == 0) return PATOH_CRS_MANDIS;
  if (strcmp(opt, "PATOH_CRS_AVGDIS") == 0) return PATOH_CRS_AVGDIS;
  if (strcmp(opt, "PATOH_CRS_CANBERA") == 0) return PATOH_CRS_CANBERA;
  if (strcmp(opt, "PATOH_CRS_ABS") == 0) return PATOH_CRS_ABS;
  if (strcmp(opt, "PATOH_CRS_GCM") == 0) return PATOH_CRS_GCM;
  if (strcmp(opt, "PATOH_CRS_SHCM") == 0) return PATOH_CRS_SHCM;

  /* --------- Agglomeratives ---------*/
  if (strcmp(opt, "PATOH_CRS_HCC") == 0) return PATOH_CRS_HCC;
  if (strcmp(opt, "PATOH_CRS_HPC") == 0) return PATOH_CRS_HPC;
  if (strcmp(opt, "PATOH_CRS_ABSHCC") == 0) return PATOH_CRS_ABSHCC;
  if (strcmp(opt, "PATOH_CRS_ABSHPC") == 0) return PATOH_CRS_ABSHPC;
  if (strcmp(opt, "PATOH_CRS_CONC") == 0) return PATOH_CRS_CONC;
  if (strcmp(opt, "PATOH_CRS_GCC") == 0) return PATOH_CRS_GCC;
  if (strcmp(opt, "PATOH_CRS_SHCC") == 0) return PATOH_CRS_SHCC;
  if (strcmp(opt, "PATOH_CRS_FIRST_NET_MATCH") == 0) return PATOH_CRS_FIRST_NET_MATCH;
  /* --------------- Net Base Agglomeratives -------------- */
  if (strcmp(opt, "PATOH_CRS_NC") == 0) return PATOH_CRS_NC;
  if (strcmp(opt, "PATOH_CRS_MNC") == 0) return PATOH_CRS_MNC;

  /* ------------------- Inital Partitionin ------------------- */
  if (strcmp(opt, "PATOH_IPA_GHGP") == 0) return PATOH_IPA_GHGP;
  if (strcmp(opt, "PATOH_IPA_AGGMATCH") == 0) return PATOH_IPA_AGGMATCH;
  if (strcmp(opt, "PATOH_IPA_BF") == 0) return PATOH_IPA_BF;
  if (strcmp(opt, "PATOH_IPA_BINPACK") == 0) return PATOH_IPA_BINPACK;
  if (strcmp(opt, "PATOH_IPA_RANDOM1") == 0) return PATOH_IPA_RANDOM1;
  if (strcmp(opt, "PATOH_IPA_RANDOM2") == 0) return PATOH_IPA_RANDOM2;
  if (strcmp(opt, "PATOH_IPA_RANDOM3") == 0) return PATOH_IPA_RANDOM3;
  if (strcmp(opt, "PATOH_IPA_GHG_MAXPIN") == 0) return PATOH_IPA_GHG_MAXPIN;
  if (strcmp(opt, "PATOH_IPA_GHG_MAXNET") == 0) return PATOH_IPA_GHG_MAXNET;
  if (strcmp(opt, "PATOH_IPA_GHG_MAXPOSGAIN") == 0) return PATOH_IPA_GHG_MAXPOSGAIN;
  if (strcmp(opt, "PATOH_IPA_COMP_GHGP") == 0) return PATOH_IPA_COMP_GHGP;
  if (strcmp(opt, "PATOH_IPA_GREEDY_COMP_GHGP") == 0) return PATOH_IPA_GREEDY_COMP_GHGP;
  if (strcmp(opt, "PATOH_IPA_ALL") == 0) return PATOH_IPA_ALL;

  /* ------------------- Refinement ------------------- */
  if (strcmp(opt, "PATOH_REFALG_NONE") == 0) return PATOH_REFALG_NONE;
  if (strcmp(opt, "PATOH_REFALG_T_BFM") == 0) return PATOH_REFALG_T_BFM;
  if (strcmp(opt, "PATOH_REFALG_T_FM") == 0) return PATOH_REFALG_T_FM;
  if (strcmp(opt, "PATOH_REFALG_D_BFM") == 0) return PATOH_REFALG_D_BFM;
  if (strcmp(opt, "PATOH_REFALG_D_FM") == 0) return PATOH_REFALG_D_FM;
  if (strcmp(opt, "PATOH_REFALG_BKL") == 0) return PATOH_REFALG_BKL;
  if (strcmp(opt, "PATOH_REFALG_KL") == 0) return PATOH_REFALG_KL;
  if (strcmp(opt, "PATOH_REFALG_MLG_BFM") == 0) return PATOH_REFALG_MLG_BFM;
  if (strcmp(opt, "PATOH_REFALG_MLG_FM") == 0) return PATOH_REFALG_MLG_FM;
  if (strcmp(opt, "PATOH_REFALG_FMKL") == 0) return PATOH_REFALG_FMKL;

  /* ------------------- Output Detail ------------------- */
  if (strcmp(opt, "PATOH_OD_ONLYRESTIME") == 0) return PATOH_OD_ONLYRESTIME;
  if (strcmp(opt, "PATOH_OD_NONE") == 0) return PATOH_OD_NONE;
  if (strcmp(opt, "PATOH_OD_LOW") == 0) return PATOH_OD_LOW;
  if (strcmp(opt, "PATOH_OD_MEDIUM") == 0) return PATOH_OD_MEDIUM;
  if (strcmp(opt, "PATOH_OD_HIGH") == 0) return PATOH_OD_HIGH;
  return PRT_getInt(opt);
}

void PRT_setPatohOptions(PaToH_Parameters *args, QSS_data data)
{
  int i, t = data->params->partitionerOptions.nPatoh;
  for (i = 0; i < t; i++) {
    char lvalue[128], value[128];
    sprintf(lvalue, "%s", data->params->partitionerOptions.patoh[i].lvalue);
    sprintf(value, "%s", data->params->partitionerOptions.patoh[i].value);
    if (strcmp(lvalue, "outputdetail") == 0) args->outputdetail = PRT_getPatohOption(value);
    if (strcmp(lvalue, "seed") == 0) args->seed = PRT_getInt(value);
    if (strcmp(lvalue, "doinitperm") == 0) args->doinitperm = PRT_getInt(value);

    /* --------- net discard  parameters -----------*/
    if (strcmp(lvalue, "bisec_fixednetsizetrsh") == 0) args->bisec_fixednetsizetrsh = PRT_getInt(value);
    if (strcmp(lvalue, "bisec_netsizetrsh") == 0) args->bisec_netsizetrsh = PRT_getFloat(value);
    if (strcmp(lvalue, "bisec_partmultnetsizetrsh") == 0) args->bisec_partmultnetsizetrsh = PRT_getInt(value);

    /*----------------- V-cycle parameter ----------------*/
    if (strcmp(lvalue, "bigVcycle") == 0) args->bigVcycle = PRT_getInt(value);
    if (strcmp(lvalue, "smallVcycle") == 0) args->smallVcycle = PRT_getInt(value);
    if (strcmp(lvalue, "usesamematchinginVcycles") == 0) args->usesamematchinginVcycles = PRT_getInt(value);

    /* ---------- use heap/bucket parameters ---------- */
    if (strcmp(lvalue, "usebucket") == 0) args->usebucket = PRT_getInt(value);
    if (strcmp(lvalue, "maxcellinheap") == 0) args->maxcellinheap = PRT_getInt(value);
    if (strcmp(lvalue, "heapchk_mul") == 0) args->heapchk_mul = PRT_getInt(value);
    if (strcmp(lvalue, "heapchk_div") == 0) args->heapchk_div = PRT_getInt(value);

    /* ----------------- Memory Allocation Parameters ------------------*/
    if (strcmp(lvalue, "MemMul_CellNet") == 0) args->MemMul_CellNet = PRT_getInt(value);
    if (strcmp(lvalue, "MemMul_Pins") == 0) args->MemMul_Pins = PRT_getInt(value);
    if (strcmp(lvalue, "MemMul_General") == 0) args->MemMul_General = PRT_getInt(value);

    /* ================ Coarsening Parameters ================ */
    if (strcmp(lvalue, "crs_VisitOrder") == 0) args->crs_VisitOrder = PRT_getPatohOption(value);
    if (strcmp(lvalue, "crs_alg") == 0) args->crs_alg = PRT_getPatohOption(value);
    if (strcmp(lvalue, "crs_coarsento") == 0) args->crs_coarsento = PRT_getInt(value);
    if (strcmp(lvalue, "crs_coarsentokmult") == 0) args->crs_coarsentokmult = PRT_getInt(value);
    if (strcmp(lvalue, "crs_coarsenper") == 0) args->crs_coarsenper = PRT_getInt(value);
    if (strcmp(lvalue, "crs_maxallowedcellwmult") == 0) args->crs_maxallowedcellwmult = PRT_getFloat(value);
    if (strcmp(lvalue, "crs_idenafter") == 0) args->crs_idenafter = PRT_getInt(value);
    if (strcmp(lvalue, "crs_iden_netsizetrh") == 0) args->crs_iden_netsizetrh = PRT_getInt(value);
    if (strcmp(lvalue, "crs_useafteralg") == 0) args->crs_useafteralg = PRT_getInt(value);
    if (strcmp(lvalue, "crs_useafter") == 0) args->crs_useafter = PRT_getInt(value);

    /* ================ Initial Partitioning Parameters ================ */
    /*--- both init part & refinement -----*/
    if (strcmp(lvalue, "nofinstances") == 0) args->nofinstances = PRT_getInt(value);

    /* -------------- initial partitioning parameters ---------------*/
    if (strcmp(lvalue, "initp_alg") == 0) args->initp_alg = PRT_getPatohOption(value);
    if (strcmp(lvalue, "initp_runno") == 0) args->initp_runno = PRT_getInt(value);
    if (strcmp(lvalue, "initp_ghg_trybalance") == 0) args->initp_ghg_trybalance = PRT_getInt(value);
    if (strcmp(lvalue, "initp_refalg") == 0) args->initp_refalg = PRT_getPatohOption(value);

    /* ================ Refinement Parameters ================ */
    if (strcmp(lvalue, "ref_alg") == 0) args->ref_alg = PRT_getPatohOption(value);
    if (strcmp(lvalue, "ref_useafter") == 0) args->ref_useafter = PRT_getInt(value);
    if (strcmp(lvalue, "ref_useafteralg") == 0) args->ref_useafteralg = PRT_getInt(value);
    if (strcmp(lvalue, "ref_passcnt") == 0) args->ref_passcnt = PRT_getInt(value);
    if (strcmp(lvalue, "ref_maxnegmove") == 0) args->ref_maxnegmove = PRT_getInt(value);
    if (strcmp(lvalue, "ref_maxnegmovemult") == 0) args->ref_maxnegmovemult = PRT_getFloat(value);
    if (strcmp(lvalue, "ref_dynamiclockcnt") == 0) args->ref_dynamiclockcnt = PRT_getInt(value);
    if (strcmp(lvalue, "ref_slow_uncoarsening") == 0) args->ref_slow_uncoarsening = PRT_getFloat(value);

    /* -------------- imbalance parameters--------------*/
    if (strcmp(lvalue, "balance") == 0) args->balance = PRT_getInt(value);
    if (strcmp(lvalue, "init_imbal") == 0) args->init_imbal = PRT_getDouble(value);
    if (strcmp(lvalue, "final_imbal") == 0) args->final_imbal = PRT_getDouble(value);
    if (strcmp(lvalue, "fast_initbal_mult") == 0) args->ref_dynamiclockcnt = PRT_getDouble(value);
    if (strcmp(lvalue, "init_sol_discard_mult") == 0) args->init_sol_discard_mult = PRT_getFloat(value);
    if (strcmp(lvalue, "final_sol_discard_mult") == 0) args->final_sol_discard_mult = PRT_getFloat(value);
  }
}

int PRT_getScotchOption(char opt[128])
{
  if (strcmp(opt, "SCOTCH_STRATDEFAULT") == 0) return SCOTCH_STRATDEFAULT;
  if (strcmp(opt, "SCOTCH_STRATQUALITY") == 0) return SCOTCH_STRATQUALITY;
  if (strcmp(opt, "SCOTCH_STRATSPEED") == 0) return SCOTCH_STRATSPEED;
  if (strcmp(opt, "SCOTCH_STRATBALANCE") == 0) return SCOTCH_STRATBALANCE;
  if (strcmp(opt, "SCOTCH_STRATSAFETY") == 0) return SCOTCH_STRATSAFETY;
  if (strcmp(opt, "SCOTCH_STRATSCALABILITY") == 0) return SCOTCH_STRATSCALABILITY;
  if (strcmp(opt, "SCOTCH_STRATRECURSIVE") == 0) return SCOTCH_STRATRECURSIVE;
  if (strcmp(opt, "SCOTCH_STRATREMAP") == 0) return SCOTCH_STRATREMAP;
  if (strcmp(opt, "SCOTCH_STRATLEVELMAX") == 0) return SCOTCH_STRATLEVELMAX;
  if (strcmp(opt, "SCOTCH_STRATLEVELMIN") == 0) return SCOTCH_STRATLEVELMIN;
  if (strcmp(opt, "SCOTCH_STRATLEAFSIMPLE") == 0) return SCOTCH_STRATLEAFSIMPLE;
  if (strcmp(opt, "SCOTCH_STRATSEPASIMPLE") == 0) return SCOTCH_STRATSEPASIMPLE;
  return PRT_getInt(opt);
}

void PRT_setScotchOptions(SCOTCH_Strat *strat, QSS_data data)
{
  int i, t = data->params->partitionerOptions.nScotch;
  for (i = 0; i < t; i++) {
    SCOTCH_stratGraphMapBuild(strat, PRT_getScotchOption(data->params->partitionerOptions.scotch[i].lvalue), data->params->lps,
                              PRT_getDouble(data->params->partitionerOptions.scotch[i].value));
  }
}

void PRT_readPartition(PRT_partition partition, QSS_data data, char *name)
{
  char fileName[256];
  sprintf(fileName, "%s.part", name);
  FILE *file;
  int i, nvtxs = partition->endHandlers;
  int lps = partition->lps;
  file = fopen(fileName, "r");
  bool wrongFile = FALSE;
  if (file != NULL) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int val;
    i = 0;
    while ((read = getline(&line, &len, file)) != -1) {
      sscanf(line, "%d", &val);
      if (val < 0 || val > lps) {
        fprintf(stderr, "Wrong partition file.\n");
        wrongFile = TRUE;
        break;
      }
      partition->values[i++] = val;
    }
    if (i > nvtxs) {
      wrongFile = TRUE;
    }
    fclose(file);
    if (line != NULL) {
      free(line);
    }
    if (wrongFile == TRUE) {
      abort();
    }
    return;
  }
}

void PRT_getPartition(int *val, PRT_partition partition, grp_t nvtxs)
{
  int i;
  for (i = 0; i < nvtxs; i++) {
    partition->values[i] = val[i];
  }
}

void PRT_generateIntStructure(int *n, int *e, int *np, int **x, int **xa, int **vw, int **ew, int **val, grp_t nvtxs, grp_t edges,
                              grp_t nparts, grp_t *xadj, grp_t *adjcny, grp_t *vwgt, grp_t *ewgt, SD_PartitionMethod pm)
{
  int i;
  n[0] = nvtxs;
  e[0] = edges;
  np[0] = nparts;
  vw[0] = (int *)checkedMalloc(nvtxs * sizeof(int));
  for (i = 0; i < nvtxs; i++) {
    vw[0][i] = vwgt[i];
  }
  ew[0] = (int *)checkedMalloc(edges * sizeof(int));
  for (i = 0; i < edges; i++) {
    ew[0][i] = ewgt[i];
  }
  if (pm == SD_Scotch || pm == SD_Metis) {
    x[0] = (int *)checkedMalloc((nvtxs + 1) * sizeof(int));
    x[0][0] = 0;
    for (i = 1; i <= nvtxs; i++) {
      x[0][i] = xadj[i];
    }
    xa[0] = (int *)checkedMalloc(edges * sizeof(int));
    for (i = 0; i < edges; i++) {
      xa[0][i] = adjcny[i];
    }
  } else if (pm == SD_Patoh || pm == SD_HMetis) {
    x[0] = (int *)checkedMalloc((edges + 1) * sizeof(int));
    x[0][0] = 0;
    for (i = 1; i <= edges; i++) {
      x[0][i] = xadj[i];
    }
    int npins = xadj[edges];
    xa[0] = (int *)checkedMalloc(npins * sizeof(int));
    for (i = 0; i < npins; i++) {
      xa[0][i] = adjcny[i];
    }
  }
  val[0] = (int *)checkedMalloc(nvtxs * sizeof(int));
  for (i = 1; i < nvtxs; i++) {
    val[0][i] = 0;
  }
}

void PRT_createPartitions(PRT_partition partition, QSS_data data, char *name)
{
  if (data->params->pm == SD_Manual) {
    PRT_readPartition(partition, data, name);
    return;
  }
  char fileName[256];
  char graphType[64] = "static";
  grp_t nparts = (data->params->lps == 0 ? 64 : data->params->lps);
  grp_t nvtxs = data->states + data->events;
  FILE *file;
  grp_t *xadj = NULL, *adjncy = NULL, *vwgt = NULL, *ewgt = NULL;
  grp_t i, edges;
  SD_PartitionMethod pm = data->params->pm;
  if (GRP_readGraph(name, data, &xadj, &adjncy, &edges, 1, &vwgt, &ewgt, 0, NULL) == GRP_ReadError) {
    fprintf(stderr, "Could not read generated graph files.");
    abort();
  }
  if (nvtxs > nparts) {
    switch (pm) {
    case SD_Metis: {
      idx_t ncon = 1, edgecut;
      idx_t options[METIS_NOPTIONS];
      METIS_SetDefaultOptions(options);
      options[METIS_OPTION_CONTIG] = 1;
      options[METIS_OPTION_PTYPE] = METIS_PTYPE_KWAY;
      options[METIS_OPTION_OBJTYPE] = METIS_OBJTYPE_VOL;
      options[METIS_OPTION_SEED] = 1;
      PRT_setMetisOptions(options, data);
      METIS_PartGraphKway(&nvtxs, &ncon, xadj, adjncy, vwgt, NULL, ewgt, &nparts, NULL, NULL, options, &edgecut, partition->values);
    } break;
    case SD_HMetis: {
      char hgraphName[256];
      pid_t pid;
      if ((pid = fork())) {
        wait(NULL);
        sprintf(hgraphName, "%s.hmetis.part.%ld", name, nparts);
        file = fopen(hgraphName, "r");
        if (file) {
          char *line = NULL;
          size_t len = 0;
          ssize_t read;
          int index = 0;
          while ((read = getline(&line, &len, file)) != -1) {
            grp_t part;
            sscanf(line, "%ld", &part);
            partition->values[index++] = part;
          }
          fclose(file);
        }
      } else {
        strcpy(hgraphName, name);
        strcat(hgraphName, ".hmetis");
        file = fopen(hgraphName, "w");
        if (file) {
          int i, j;
          if (vwgt == NULL) {
            fprintf(file, "%ld %ld 1\n", edges, nvtxs);
          } else {
            fprintf(file, "%ld %ld 11\n", edges, nvtxs);
          }
          int begin, end;
          for (i = 0; i < edges; i++) {
            begin = xadj[i];
            end = xadj[i + 1];
            fprintf(file, "%ld ", ewgt[i]);
            for (j = begin; j < end; j++) {
              fprintf(file, "%ld ", adjncy[j] + 1);
            }
            fprintf(file, "\n");
          }
          if (vwgt != NULL) {
            for (i = 0; i < nvtxs; i++) {
              fprintf(file, "%ld\n", vwgt[i]);
            }
          }
          fclose(file);
        }
        char parts[10];
        sprintf(parts, "%ld", nparts);
        execlp("./khmetis", "./khmetis", hgraphName, parts, "5", "10", "1", "1", "0", "0", NULL);
        abort();
      }
    } break;
    case SD_Scotch: {
      // Run scotch partition
      int n, e, np, *x, *xa, *vw, *ew, *val;
      PRT_generateIntStructure(&n, &e, &np, &x, &xa, &vw, &ew, &val, nvtxs, edges, nparts, xadj, adjncy, vwgt, ewgt, pm);
      SCOTCH_Graph *graph_sc = SCOTCH_graphAlloc();
      SCOTCH_Strat *strat = SCOTCH_stratAlloc();
      SCOTCH_stratInit(strat);
      PRT_setScotchOptions(strat, data);
      if (SCOTCH_graphBuild(graph_sc, 0, n, x, x + 1, vw, NULL, e, xa, ew) != 0) {
        printf("Error: Scotch Graph Build\n");
      }
      if (SCOTCH_graphPart(graph_sc, np, strat, val) != 0) {
        printf("Error: Scotch Graph Partition\n");
      }
      SCOTCH_stratExit(strat);
      SCOTCH_graphFree(graph_sc);
      PRT_getPartition(val, partition, nvtxs);
    } break;
    case SD_Patoh: {
      int n, e, np, *x, *xa, *vw, *ew, *val;
      PRT_generateIntStructure(&n, &e, &np, &x, &xa, &vw, &ew, &val, nvtxs, edges, nparts, xadj, adjncy, vwgt, ewgt, pm);
      int nconst = 1, edgecut, *partweights;
      PaToH_Parameters args;
      PaToH_Initialize_Parameters(&args, PATOH_CUTPART, PATOH_SUGPARAM_QUALITY);
      args._k = nparts;
      args.final_imbal = 0.1;
      args.seed = 1;
      PRT_setPatohOptions(&args, data);
      partweights = (int *)malloc(args._k * nconst * sizeof(int));
      PaToH_Alloc(&args, n, e, nconst, vw, ew, x, xa);
      PaToH_Part(&args, n, e, nconst, 0, vw, ew, x, xa, NULL, val, partweights, &edgecut);
      free(partweights);
      PaToH_Free();
      PRT_getPartition(val, partition, nvtxs);
    } break;
    default:
      break;
    }
  }
  switch (pm) {
  case SD_Metis:
    sprintf(fileName, "%s-Metis-%s-%ld.partition", name, graphType, nparts);
    break;
  case SD_HMetis:
    sprintf(fileName, "%s-HMetis-%s-%ld.partition", name, graphType, nparts);
    break;
  case SD_Scotch:
    sprintf(fileName, "%s-Scoth-%s-%ld.partition", name, graphType, nparts);
    break;
  case SD_Patoh:
    sprintf(fileName, "%s-Patoh-%s-%ld.partition", name, graphType, nparts);
    break;
  default:
    break;
  }
  file = fopen(fileName, "w");
  if (file) {
    for (i = 0; i < nvtxs; i++) {
      fprintf(file, "%ld\n", partition->values[i]);
    }
    fclose(file);
  }
  free(xadj);
  free(adjncy);
  if (vwgt != NULL) {
    free(vwgt);
  }
  if (ewgt != NULL) {
    free(ewgt);
  }
}

PRT_partition PRT_Partition(QSS_data data, char *name)
{
  grp_t nvtxs = data->states + data->events;
  int lps = data->params->lps, i;
  PRT_partition p = checkedMalloc(sizeof(*p));
  p->values = (grp_t *)checkedMalloc(nvtxs * sizeof(grp_t));
  memset(p->values, 0, nvtxs * sizeof(grp_t));
  p->beginStates = 0;
  p->beginHandlers = data->states;
  p->endStates = data->states;
  p->endHandlers = data->states + data->events;
  p->lps = lps;
  p->nOutputs = (int *)checkedMalloc(nvtxs * sizeof(int));
  cleanVector(p->nOutputs, 0, nvtxs);
  p->outputs = (int **)checkedMalloc(nvtxs * sizeof(int *));
  for (i = 0; i < nvtxs; i++) {
    p->outputs[i] = (int *)checkedMalloc(lps * sizeof(int));
    cleanVector(p->outputs[i], 0, lps);
  }
  p->nDsc = (int *)checkedMalloc(lps * sizeof(int));
  p->dscInf = (int **)checkedMalloc(lps * sizeof(int *));
  p->asgDscInf = (int **)checkedMalloc(lps * sizeof(int *));
  cleanVector(p->nDsc, 0, lps);
  for (i = 0; i < lps; i++) {
    p->dscInf[i] = (int *)checkedMalloc(data->states * sizeof(int));
    cleanVector(p->dscInf[i], 0, data->states);
    p->asgDscInf[i] = (int *)checkedMalloc(data->states * sizeof(int));
    cleanVector(p->asgDscInf[i], NOT_ASSIGNED, data->states);
  }
  if (lps > 1) {
    PRT_createPartitions(p, data, name);
  }
#ifdef DEBUG
  printf("States: %d\n", data->states);
  printf("Handlers: %d\n", data->events);
  printf("Begin states: %d\n", p->beginStates);
  printf("End states: %d\n", p->endStates);
  printf("Begin handlers: %d\n", p->beginHandlers);
  printf("End handlers: %d\n\n", p->endHandlers);
#endif
  return p;
}

void PRT_freePartition(PRT_partition partition)
{
  int i, nvtxs = partition->endHandlers, lps = partition->lps;
  free(partition->nOutputs);
  for (i = 0; i < nvtxs; i++) {
    free(partition->outputs[i]);
  }
  free(partition->nDsc);
  for (i = 0; i < lps; i++) {
    free(partition->dscInf[i]);
    free(partition->asgDscInf[i]);
  }
  free(partition->dscInf);
  free(partition->asgDscInf);
  free(partition->outputs);
  free(partition->values);
  free(partition);
}

#else

PRT_partition PRT_Partition(QSS_data data, char *name) { return NULL; }

void PRT_freePartition(PRT_partition partition) { return; }

#endif
