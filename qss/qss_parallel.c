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

#ifdef __linux__

#define _GNU_SOURCE 
#define __USE_GNU
#include <sys/mman.h>
#include <sched.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "../common/data.h"
#include <qss/qss_parallel.h>
#include <common/utils.h>

static pthread_t *tasks;

static pthread_barrier_t b;

static int lpArch[MAX_LPS];

void
PAR_readArchitecture(int lps)
{
  char fileName[256];
  sprintf(fileName, "%s", "arch.cfg");
  FILE *file;
  int i;
  file = fopen(fileName, "r");
  if(file != NULL)
  {
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int val;
    i = 0;
    while((read = getline(&line, &len, file)) != -1)
    {
      sscanf(line, "%d", &val);
      lpArch[i++] = val;
    }
    fclose(file);
    if(line != NULL)
    {
      free(line);
    }
    return;
  }
  for(i = 0; i < lps; i++)
  {
    lpArch[i] = i;
  }
}

int
PAR_initLPTasks(int lp)
{
  cpu_set_t set;
  int lpNumber = lpArch[lp];
  CPU_ZERO(&set);
  CPU_SET(lpNumber, &set);
  int retCode = PAR_NO_ERROR;
  if(pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &set))
  {
    retCode = PAR_ERR_SET_AFFINITY_MASK;
  }
  if(pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &set) != 0)
  {
    retCode = PAR_ERR_GET_AFFINITY_MASK;
  }
  struct sched_param p;
  p.__sched_priority = sched_get_priority_max(SCHED_FIFO);
  pthread_setschedparam(pthread_self(), SCHED_FIFO, &p);
  pthread_barrier_wait(&b);
  return retCode;
}

int
PAR_createLPTasks(QSS_sim simulate, QSS_simulator simulator)
{
  int i, lps = simulator->data->params->lps;
  PAR_readArchitecture(lps);
  tasks = checkedMalloc(lps * sizeof(*tasks));
  pthread_barrier_init(&b, NULL, lps);
  QSS_simulatorInstance si[lps];
  simulator->id = ROOT_SIMULATOR;
  for(i = 0; i < lps; i++)
  {
    si[i].root = simulator;
    si[i].id = i;
    if(pthread_create(&tasks[i], NULL, simulate, &(si[i])) != 0)
    {
      return PAR_ERR_CREATE_THREAD;
    }
  }
  for(i = 0; i < lps; i++)
  {
    pthread_join(tasks[i], NULL);
  }
  free(tasks);
  return PAR_NO_ERROR;
}

int
PAR_cleanLPTask(int lp)
{
  return PAR_NO_ERROR;
}

#else
#include <qss/qss_parallel.h>
int
PAR_createLPTasks (QSS_sim simulate, QSS_simulator simulator)
{
  return 0;
}

int
PAR_cleanLPTask (int lp)
{
  return 0;
}

int
PAR_initLPTasks (int lp)
{
  return 0;
}
#endif
