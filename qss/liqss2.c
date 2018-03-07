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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <qss/liqss2.h>
#include <common/utils.h>
#define EPS 1e-40

void LIQSS2_PAR_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *lqu);
void LIQSS2_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *lqu);
double LIQSS2_PAR_maxi(double x1, double x2, double d);
double LIQSS2_maxi(double x1, double x2, double d);

#ifdef QSS_PARALLEL
void
LIQSS2_PAR_init (QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#else
void
LIQSS2_init (QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#endif
{
	int i, j, m, s;
	int states = simData->states;
	quantizer->state->sts = states;
	quantizer->state->nBE = states;
  quantizer->state->oldDx = (double *) malloc (states * sizeof(double));
  quantizer->state->qAux = (double *) malloc (states * sizeof(double));
  quantizer->state->xprev = (double *) malloc (states * sizeof(double));
  quantizer->state->change = (bool *) malloc (states * sizeof(bool));
  quantizer->state->tx = (double *) malloc (states * sizeof(double));
  quantizer->state->flag2 = (int *) malloc (states * sizeof(int));
	quantizer->state->BE = (bool *) malloc (states * sizeof(bool));
  quantizer->state->nSD = simData->nSD;
  quantizer->state->SD = simData->SD;
  quantizer->state->jac = simData->jac;
  int infs = quantizer->state->infs = 0;
  for (i = 0; i < states; i++) infs += quantizer->state->nSD[i]; // cantidad de influencias
  quantizer->state->S = (int **) malloc (states * sizeof(int*)); // matriz de sparcity
	for(i = 0; i < infs; i++) quantizer->state->S[i] = (int *)malloc(states * sizeof(int));
	quantizer->state->a = (double *) malloc (infs * sizeof(double)); //vector jacobiano
	quantizer->state->u0 = (double *) malloc (infs * sizeof(double)); //vector restos orden cero
	quantizer->state->u1 = (double *) malloc (infs * sizeof(double)); //vector restos orden uno
	quantizer->state->infs = infs;
	quantizer->state->band = FALSE;
	
	// Inicializaciones
	for (i = 0; i < infs; i++)
	{
		quantizer->state->a[i] = 0;
		quantizer->state->u0[i] = 0;
		quantizer->state->u1[i] = 0;
	}
  for (i = 0; i < states; i++)
    {
      int cf0 = i*3;
      simData->x[cf0 + 2] = 0;
      simData->q[cf0] = simData->x[cf0];
      simData->q[cf0 + 1] = 0;
      quantizer->state->qAux[i] = simData->x[cf0];
      quantizer->state->xprev[i] = simData->x[cf0];
      quantizer->state->oldDx[i] = 0;
 			quantizer->state->change[i] = FALSE;
	    quantizer->state->flag2[i] = 0;
			quantizer->state->tx[i] = 0;
 			quantizer->state->BE[i] = TRUE;
      for (j=0; j < states; j++) quantizer->state->S[i][j] = -1;
    }
	// Paso de integración
	quantizer->state->hmin = simData->ft*1e-4;
	printf("ft/h = %g\n", simData->ft/quantizer->state->hmin);
	quantizer->state->h = quantizer->state->hmin;
	quantizer->state->agrandar = 0;
	quantizer->state->achicar = 0;
	// Determinar variables que NO van con BE
	if(simData->HD)
	{
		printf("LIQSS2: ");
		for(i=0; i<simData->events; i++)
		{
			for(j=0; j<simData->nHD[i]; j++)
			{
				for (m = 0; m < simData->nSD[simData->HD[i][j]]; m++)
				{
					if(quantizer->state->BE[simData->SD[simData->HD[i][j]][m]])
					{
						quantizer->state->BE[simData->SD[simData->HD[i][j]][m]] = FALSE;
						quantizer->state->nBE--;
						printf("%d ", simData->SD[simData->HD[i][j]][m]);
					}
				}
			}
		}
	}
	printf("\n");
	// Guardar vector de estados que van con BE	
	if(quantizer->state->nBE)
	{
		quantizer->state->BES = (int *) malloc (quantizer->state->nBE * sizeof(int));
		printf("LIDF2: ");
		j=0;
		for(i=0; i< states; i++)
		{
			if(quantizer->state->BE[i])
			{
				quantizer->state->BES[j] = i;
				printf("%d ", i);
				j++;
			}
		}
		printf("\n");
	}
	//Jacobiano simbólico
	s=0;
	for (i = 0; i < states; i++)
  {
		for (m = 0; m < simData->nSD[i]; m++)
		{
				quantizer->state->S[quantizer->state->SD[i][m]][i] = s;
				//printf("J[%d][%d]] = %g;\n", simData->SD[i][m], i, simData->jac[s]);
				quantizer->state->a[quantizer->state->S[simData->SD[i][m]][i]] = simData->jac[s++];
		}
	}
	// Forzar todo a LI
	if(0)
	{
		for(i=0; i<states; i++)
			quantizer->state->BE[i] = FALSE;
		quantizer->state->nBE = 0;
	}
	
	//quantizer->state->BE[4] = FALSE;
	//quantizer->state->nBE--;
	
#ifdef QSS_PARALLEL
  quantizer->state->qMap = simData->lp->qMap;
#endif
  quantizer->state->finTime = simData->ft;
  quantizer->state->minStep = simData->params->minStep;
  quantizer->state->lSimTime = simTime;
  
  quantizer->state->nSZ = simData->nSZ;
  quantizer->state->SZ = simData->SZ;
  quantizer->ops->recomputeNextTimes = LIQSS2_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = LIQSS2_recomputeNextTime;
  quantizer->ops->nextTime = LIQSS2_nextTime;
  quantizer->ops->updateQuantizedState = LIQSS2_updateQuantizedState;
}

#ifdef QSS_PARALLEL
void
LIQSS2_PAR_recomputeNextTime (QA_quantizer quantizer, int var, double t, double *nTime,
		       double *x, double *lqu, double *q)
#else
void
LIQSS2_recomputeNextTime (QA_quantizer quantizer, int var, double t, double *nTime,
		       double *x, double *lqu, double *q)
#endif
{
  bool *BE = quantizer->state->BE;
  
  if(!BE[var])
	{
		int cf0 = var * 3, cf1 = cf0 + 1, cf2 = cf1 + 1, i;
		double *a = quantizer->state->a;
		double *u0 = quantizer->state->u0;
		double *u1 = quantizer->state->u1;
		bool self = quantizer->state->lSimTime->minIndex == var
				&& quantizer->state->lSimTime->type == ST_State;
		double diffQ, timeaux;
		double coeff[3];
		double *tu = quantizer->state->tx;
		int *flag2 = quantizer->state->flag2;
		int **S = quantizer->state->S;
		
		// Jacobian estimation
		if(self)
		{
			if(t > 0)
			{
				diffQ = q[cf0] - quantizer->state->qAux[var];
				if(diffQ)
					a[S[var][var]] = (x[cf1] - quantizer->state->oldDx[var]) / diffQ;
				if(a[S[var][var]] > 0)
					a[S[var][var]] = 0;
			}
			else
			{
				a[S[var][var]] = 0;
			}
		}
		// affine parameters
		u0[S[var][var]] = x[cf1] - q[cf0] * a[S[var][var]];
		u1[S[var][var]] = 2 * x[cf2] - q[cf1] * a[S[var][var]];
		// store last x update
		tu[var] = t;
		
		coeff[1] = q[cf1] - x[cf1];
		coeff[2] = -x[cf2];
		if(flag2[var] != 1)
		{
			if((quantizer->state->lSimTime->type == ST_Event) && (a[var] < 0)
					&& (quantizer->state->nSZ[var] > 0))
			{ //we check if var is involved in the zero crossing function that produced the current event
				for(i = 0; i < quantizer->state->nSZ[var]; i++)
				{
					if(quantizer->state->SZ[var][i] == quantizer->state->lSimTime->minIndex)
					{
						nTime[var] = t;
						flag2[var] = 1; //it does, so we restart the quantized state q[var]
					}
				}
			}
			if(flag2[var] != 1)
			{
				if(a[var] < 0 && self && x[cf2] != 0)
					nTime[var] = t + fabs((q[cf1] - x[cf1]) / x[cf2]*2);
				else
					nTime[var] = INF;
				coeff[0] = q[cf0] + 2 * lqu[var] - x[cf0];
				timeaux = t + minPosRoot(coeff, 2);
				if(timeaux < nTime[var]) nTime[var] = timeaux;
				coeff[0] = q[cf0] - 2 * lqu[var] - x[cf0];
				timeaux = t + minPosRoot(coeff, 2);
				if(timeaux < nTime[var]) nTime[var] = timeaux;
				if(q[cf0] * q[cf1] < 0 && fabs(q[cf0]) > 10 * lqu[var])
				{
					timeaux = -q[cf0] / q[cf1] - 2 * fabs(lqu[var] / q[cf1]);
					if(nTime[var] > t + timeaux) nTime[var] = t + timeaux;
				}
				if(flag2[var] == 2 && self) flag2[var] = 0;
				double err1 = q[cf0] - x[cf0] + coeff[1] * (nTime[var] - t) / 2
						+ coeff[2] * pow((nTime[var] - t) / 2, 2);
				if(fabs(err1) > 3 * fabs(lqu[var]))
					nTime[var] = t + quantizer->state->finTime * quantizer->state->minStep;
			}
		}
		else
		{
			// if (fabs(aold-a[var])>fabs(a[var])/2) nTime[var] = t + quantizer->state->finTime * quantizer->state->minStep*rand()/RAND_MAX;;
	//      nTime[var] = t + quantizer->state->finTime * quantizer->state->minStep*0;
			if(self)
			{
				flag2[var] = 2;
				nTime[var] = t;
			}
		}
	}
	else
	{
		bool *change = quantizer->state->change;
		double h = quantizer->state->h;
		
		if(change[var] == TRUE)
			nTime[var] = t;
		else
			nTime[var] = t + h;
	}
}

#ifdef QSS_PARALLEL
void
LIQSS2_PAR_recomputeNextTimes (QA_quantizer quantizer, int vars, int *inf, double t,
			double *nTime, double *x, double *lqu, double *q)
#else
void
LIQSS2_recomputeNextTimes (QA_quantizer quantizer, int vars, int *inf, double t,
			double *nTime, double *x, double *lqu, double *q)
#endif
{
	int i;
#ifdef QSS_PARALLEL
	int *map = quantizer->state->qMap;
#endif
	for (i = 0; i < vars; i++)
	{
#ifdef QSS_PARALLEL
		if (map[inf[i]] != NOT_ASSIGNED)
		{
#endif
			LIQSS2_recomputeNextTime (quantizer, inf[i], t, nTime, x, lqu, q);
#ifdef QSS_PARALLEL
		}
#endif
	}
}

#ifdef QSS_PARALLEL
void
LIQSS2_PAR_nextTime (QA_quantizer quantizer, int var, double t, double *nTime,
	      double *x, double *lqu)
#else
void
LIQSS2_nextTime (QA_quantizer quantizer, int var, double t, double *nTime,
	      double *x, double *lqu)
#endif
{
	bool *BE = quantizer->state->BE;
	if(BE[var])
	  nTime[var] = INF;
	else
	{
		int cf2 = var * 3 + 2;
		if(x[cf2] == 0) nTime[var] = INF;
		else nTime[var] = t + sqrt(fabs(lqu[var] / x[cf2]));
	}
}
#ifdef QSS_PARALLEL
double LIQSS2_PAR_maxi(double x1, double x2, double d)
#else
double LIQSS2_maxi(double x1, double x2, double d)
#endif
{
	double M;
	if(x1>x2 && x1>d) M = x1;
	else if(x2>x1 && x2>d) M = x2;
	else M = d;
	return M;
}

#ifdef QSS_PARALLEL
void
LIQSS2_PAR_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *lqu)
#else
void
LIQSS2_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *lqu)
#endif
{
	double *a = quantizer->state->a;	
	int **S = quantizer->state->S;	
	double h = quantizer->state->h;	
	int *BES = quantizer->state->BES;	
	int nBE = quantizer->state->nBE;	
	int i, j;
	double *xprev = quantizer->state->xprev;
	double *dxprev = quantizer->state->oldDx;
	// step-size control variables
	double M[4] = {1, 0, 1, -1}, H[4] = {1, 0, 0, 0}, *invM = M; //modify H[4] = hnew/hold
	double hmin = quantizer->state->hmin;
	double RTE;
	
	// invAd = I-h*A
	double invAd[nBE][nBE];
	if(quantizer->state->band & 1) // pasos con LIDF2
	{
		
		for(i=0; i<nBE; i++)
		{
			for(j=0; j<nBE; j++)
			{
				if(S[BES[i]][BES[j]] != -1)
				{
					if(i==j) invAd[i][j] = 1 - (2/3.0)*h*a[S[BES[i]][BES[j]]];
					else invAd[i][j] = - (2/3.0)*h*a[S[BES[i]][BES[j]]];
				}
				else
				{
					if(i==j) invAd[i][j] = 1;
					else invAd[i][j] = 0;
				}
			}
		}
	}
	else // primer paso con LIE
	{
		for(i=0; i<nBE; i++)
		{
			for(j=0; j<nBE; j++)
			{
				if(S[BES[i]][BES[j]] != -1)
				{
					if(i==j) invAd[i][j] = 1 - h*a[S[BES[i]][BES[j]]];
					else invAd[i][j] = - h*a[S[BES[i]][BES[j]]];
				}
				else
				{
					if(i==j) invAd[i][j] = 1;
					else invAd[i][j] = 0;
				}
			}
		}
	}
	// inversa de I-h*A
	int nn = nBE;
  int pivotArray[nBE];
	int errorHandler;
	double lapackWorkspace[nBE];
	extern void dgetrf_ (int * m, int * n, double * A, int * LDA, int * IPIV, int * INFO);
	extern void dgetri_ (int * n, double * A, int * LDA, int * IPIV, double * WORK, int * LWORK, int * INFO);
	dgetrf_(&nBE, &nBE, invAd[0], &nBE, pivotArray, &errorHandler);
	dgetri_(&nBE, invAd[0], &nBE, pivotArray, lapackWorkspace, &nn, &errorHandler);
	
	if(!quantizer->state->band) // LIE
	//if(1)
	{
		for(i=0; i<nBE; i++)
		{
			//printf("%g\t%g\n", h, quantizer->state->finTime);
			xprev[i] = q[3*BES[i]]; // guardado paso anterior
			for(j=0; j<nBE; j++)
				q[3*BES[i]] = q[3*BES[i]] + invAd[i][j]*h*x[3*BES[j]+1]; // LIE - x(+) = x(-) + h*(I-hA)^-1*f(x(-),t-)
			q[3*BES[i]+1] = 0; // derivada de q nula
			x[3*BES[i]+1] = 0; // derivada de q nula
			dxprev[BES[i]] = x[3*BES[i]+1];
		}
	}
	else //LIDF2
	{
		// printf("%d - ", nBE);
		// paréntesis;
		double aux[nBE], AB2, err, e = 0, tol = lqu[BES[0]];
		for(i=0; i<nBE; i++)
		{
			aux[i] = (-xprev[i] + 2*h*x[3*BES[i]+1])/3; //2*h
			xprev[i] = q[3*BES[i]];
			for(j=0; j<nBE; j++)
			{
				if(S[BES[i]][BES[j]] != -1)
				{
					if(i==j) aux[i] += ((4/3.0) - (2/3.0)*h*a[S[BES[i]][BES[j]]])*q[3*BES[j]];
					else aux[i] += (-(2/3.0)*h*a[S[BES[i]][BES[j]]])*q[3*BES[j]];
				}
				else
				{
					if(i==j) aux[i] += (4/3.0)*q[3*BES[j]];
				}
			}
		}
		// inversa * paréntesis
		for(i=0; i<nBE; i++)
		{
			AB2 = q[3*BES[i]] + h*(3*x[3*BES[i]+1] - dxprev[BES[i]])/2;
			q[3*BES[i]] = 0;
			for(j=0; j<nBE; j++)
				q[3*BES[i]] += invAd[i][j]*aux[j];
			q[3*BES[i]+1] = 0; // derivada de q nula
			dxprev[BES[i]] = x[3*BES[i]+1];
						
			err = fabs(q[3*BES[i]]-AB2);
			if(err > e)
				e = err;
			if(lqu[BES[i]]<tol)
				tol = lqu[BES[i]];
				//tol=1e-3;
		}
		//control de paso
		RTE = pow(tol/e, 1/3.0);
		//RTE= 1;
		//printf("%g \t %g\n", RTE, quantizer->state->lSimTime->time);
		
		double hold = h;
		// Error muy grande => achicar el paso
		if(RTE < 1 && 1)
		{
			quantizer->state->achicar++;
			if(quantizer->state->achicar>4) // controlo que el error sea chico en varios pasos
			{
				h = h*RTE*0.8; // recalculo h con margen de 80% de RTE
				//printf("- %g\n", RTE);
				if(h<hmin) h = hmin; // Control de que no se achique demasiado el paso => evita cuelgues
				//printf("%g  -\n", h/quantizer->state->h);
				quantizer->state->achicar = 0;
				quantizer->state->h = h;
				//H[4] = h/hold;
				// uso vector de nordsieck para transformar los pasos anteriores
				for(i=0; i<nBE; i++)
					xprev[i] = (invM[2]*H[0]*M[0]+invM[3]*H[3]*M[2])*q[3*BES[i]] + (invM[2]*H[0]*M[1]+invM[3]*H[3]*M[3])*xprev[i];
			}
		}
		// Error muy chico => agrandar el paso
		else if(RTE > 1 && 1)
		{
			quantizer->state->agrandar++;
			if(quantizer->state->agrandar>4)// controlo que el error sea chico en varios pasos
			{
				//if(RTE>2) RTE = 2;
				h = h*RTE*0.8; // recalculo h con margen de 80% de RTE 
				//printf("+ %g\n", RTE);
				//h *= 2;
				if(h<hmin) h = hmin; // Control de que no se agrande demasiado el paso
				quantizer->state->agrandar = 0;
				quantizer->state->h = h;
				//H[4] = h/hold;
				// uso vector de nordsieck para transformar los pasos anteriores
				for(i=0; i<nBE; i++)
					xprev[i] = (invM[2]*H[0]*M[0]+invM[3]*H[3]*M[2])*q[3*BES[i]] + (invM[2]*H[0]*M[1]+invM[3]*H[3]*M[3])*xprev[i];
				/*	
				// Hago que termine justo en el t final
				if(quantizer->state->lSimTime->time + h > quantizer->state->finTime && 1)
				{
					printf("\nF\n");
					quantizer->state->h = quantizer->state->finTime*(1-1e-9) - quantizer->state->lSimTime->time;
					for(i=0; i<nBE; i++)
							xprev[i] = (invM[2]*H[0]*M[0]+invM[3]*H[3]*M[2])*q[3*BES[i]] + (invM[2]*H[0]*M[1]+invM[3]*H[3]*M[3])*xprev[i];
				}
				*/
			}
		}
		//printf("%g\n", quantizer->state->finTime - (quantizer->state->lSimTime->time + h));
		// Hago que termine justo en el t final
		if(quantizer->state->lSimTime->time + h > quantizer->state->finTime && 1)
		{
			//printf("\nF\n");
			quantizer->state->h = quantizer->state->finTime*(1-1e-9) - quantizer->state->lSimTime->time;
			H[4] = quantizer->state->h/hold;
			for(i=0; i<nBE; i++)
					xprev[i] = (invM[2]*H[0]*M[0]+invM[3]*H[3]*M[2])*q[3*BES[i]] + (invM[2]*H[0]*M[1]+invM[3]*H[3]*M[3])*xprev[i];
		}
		
	}
	if(!quantizer->state->band) quantizer->state->band = TRUE;
}


#ifdef QSS_PARALLEL
void
LIQSS2_PAR_updateQuantizedState (QA_quantizer quantizer, int var, double *q, double *x, double *lqu)
#else
void
LIQSS2_updateQuantizedState (QA_quantizer quantizer, int var, double *q, double *x, double *lqu)
#endif
{
	bool *BE = quantizer->state->BE;
	
	if(BE[var])
  {
		bool *change = quantizer->state->change;
		int nBE = quantizer->state->nBE;
		int *BES = quantizer->state->BES;	
		
		if(change[var] == TRUE)
			change[var] = FALSE;
		else
		{
			if(var == BES[0])
			{
				// LIE
				int j;
				for(j=0; j<nBE; j++)
					change[BES[j]] = TRUE;
				LIQSS2_solver2x2_h(quantizer, x, q, lqu);
			}
		}
	}
	else
	{
		double t = quantizer->state->lSimTime->time;
		double *a = quantizer->state->a;
		double *u0 = quantizer->state->u0;
		double *u1 = quantizer->state->u1;
		double *tu = quantizer->state->tx;
		int cf0 = var * 3, cf1 = cf0 + 1, cf2 = cf1 + 1;
		double elapsed;
		double h;
		int *flag2 = quantizer->state->flag2;
		int **S = quantizer->state->S;

		elapsed = t - quantizer->state->lSimTime->tq[var];
		quantizer->state->qAux[var] = q[cf0] + elapsed * q[cf1];
		quantizer->state->oldDx[var] = x[cf1];
		elapsed = t - tu[var];
		u0[S[var][var]] = u0[S[var][var]] + elapsed*u1[S[var][var]];
		tu[var] = t;
		double ddx = x[cf2] * 2;
		double oldq1 = q[cf1];

		if(a[S[var][var]] < 0)
		{
			if(ddx == 0)
			{
				ddx = a[S[var][var]] * a[S[var][var]] * q[cf0] + a[S[var][var]] * u0[S[var][var]] + u1[S[var][var]];
				if(ddx == 0)
					ddx = 1e-40;
			}

			h = (quantizer->state->finTime - t);
			q[cf0] = ((x[cf0] + h * u0[S[var][var]] + h * h / 2 * u1[S[var][var]]) * (1 - h * a[S[var][var]])
					+ (h * h / 2 * a[S[var][var]] - h) * (u0[S[var][var]] + h * u1[S[var][var]]))
					/ (1 - h * a[S[var][var]] + h * h * a[S[var][var]] * a[S[var][var]] / 2);
			if(fabs(q[cf0] - x[cf0]) > 2 * lqu[var])
			{
				h = sqrt(fabs(2 * lqu[var] / ddx));
				q[cf0] = ((x[cf0] + h * u0[S[var][var]] + h * h / 2 * u1[S[var][var]]) * (1 - h * a[S[var][var]])
						+ (h * h / 2 * a[S[var][var]] - h) * (u0[S[var][var]] + h * u1[S[var][var]]))
						/ (1 - h * a[S[var][var]] + h * h * a[S[var][var]] * a[S[var][var]] / 2);
			}
			while(fabs(q[cf0] - x[cf0]) > 2 * lqu[var])
			{
				h = h * sqrt(lqu[var] / fabs(q[cf0] - x[cf0]));
				q[cf0] = ((x[cf0] + h * u0[S[var][var]] + h * h / 2 * u1[S[var][var]]) * (1 - h * a[S[var][var]])
						+ (h * h / 2 * a[S[var][var]] - h) * (u0[S[var][var]] + h * u1[S[var][var]]))
						/ (1 - h * a[S[var][var]] + h * h * a[S[var][var]] * a[S[var][var]] / 2);
			}
			q[cf1] = (a[S[var][var]] * q[cf0] + u0[S[var][var]] + h * u1[S[var][var]]) / (1 - h * a[S[var][var]]);

			if(x[cf2] * ddx < 0)	// no hace falta
			{
				q[cf1] = -u1[S[var][var]] / a[S[var][var]];
				q[cf0] = (q[cf1] - u0[S[var][var]]) / a[S[var][var]];
				if(fabs(q[cf0] - x[cf0]) > 2 * lqu[var])
				{
					if(q[cf0] > x[cf0]) q[cf0] = x[cf0] + lqu[var];
					else q[cf0] = x[cf0] - lqu[var];
				}
			}
		}
		else
		{
			ddx = u1[S[var][var]];
			if(ddx > 0) q[cf0] = x[cf0] - lqu[var];
			else q[cf0] = x[cf0] + lqu[var];
			if(ddx != 0)
			{
				h = sqrt(2 * lqu[var] / fabs(ddx));
				q[cf1] = u0[S[var][var]] + h * u1[S[var][var]];
			}
			else q[cf1] = u0[S[var][var]];
		}
		if(fabs(q[cf0] - x[cf0]) > 2 * lqu[var]) q[cf0] = x[cf0];
		if(flag2[var] >= 1)
		{
			if(oldq1 > 0) q[cf0] = quantizer->state->qAux[var] + lqu[var] / 100;
			else q[cf0] = quantizer->state->qAux[var] - lqu[var] / 100;
			q[cf1] = 0;
		}
	}
}
