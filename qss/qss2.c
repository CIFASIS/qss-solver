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
#include <qss/qss2.h>
#include <common/utils.h>
#define EPS 1e-40

void QSS2_PAR_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *lqu);
void QSS2_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *lqu);
double QSS2_PAR_maxi(double x1, double x2, double d);
double QSS2_maxi(double x1, double x2, double d);

#ifdef QSS_PARALLEL
void
QSS2_PAR_init (QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#else
void
QSS2_init (QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#endif
{
	int states = simData->states;
	quantizer->state->sts = states;
	quantizer->state->nBE = states;
  int i,j;
  quantizer->state->oldDx = (double *) malloc (states * sizeof(double));
  quantizer->state->qAux = (double *) malloc (states * sizeof(double));
  quantizer->state->change = (bool *) malloc (states * sizeof(bool));
  quantizer->state->tx = (double *) malloc (states * sizeof(double));
  quantizer->state->flag2 = (int *) malloc (states * sizeof(int));
  quantizer->state->qj = (double *) malloc (3*states * sizeof(double));
	quantizer->state->BE = (bool *) malloc (states * sizeof(bool));
  quantizer->state->nSD = simData->nSD;
  quantizer->state->SD = simData->SD;
 //quantizer->state->jac = simData->jac;
  int infs = quantizer->state->infs;
  infs = 0;
  for (i = 0; i < states; i++) infs += quantizer->state->nSD[i]; // cantidad de influencias
  quantizer->state->S = (int **) malloc (states * sizeof(int*)); // matriz de sparcity
	for(i = 0; i < infs; i++) quantizer->state->S[i] = (int *)malloc(states * sizeof(int));
	quantizer->state->a = (double *) malloc (infs * sizeof(double)); //vector jacobiano
	quantizer->state->u0 = (double *) malloc (infs * sizeof(double)); //vector restos orden cero
	quantizer->state->u1 = (double *) malloc (infs * sizeof(double)); //vector restos orden uno
	quantizer->state->infs = infs;
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
      quantizer->state->oldDx[i] = 0;
 			quantizer->state->change[i] = FALSE;
	    quantizer->state->flag2[i] = 0;
			quantizer->state->tx[i] = 0;
			quantizer->state->qj[cf0] = 0;
   		quantizer->state->qj[cf0 + 1] = 0;
   		quantizer->state->qj[cf0 + 2] = 0;
 			quantizer->state->BE[i] = TRUE;
 			//quantizer->state->BE[i] = FALSE;
      for (j=0; j < states; j++) quantizer->state->S[i][j] = -1;
    }

  int m, s = 0; 
  for (i = 0; i < states; i++)
  {
		for (m = 0; m < quantizer->state->nSD[i]; m++)
		{
				quantizer->state->S[i][quantizer->state->SD[i][m]] = s;
				s++;
		}
	}
	int aux;	
	for(i=0; i< states-1; i++)
	{
		for(j=i; j< states; j++)
		{
			aux = quantizer->state->S[i][j];
			quantizer->state->S[i][j] = quantizer->state->S[j][i];
			quantizer->state->S[j][i] = aux;
		}
	}	
	
	// Paso de integración inicial - 10.000 pasos
	quantizer->state->h = simData->ft*1e-4;
	
	// Determinar variables que NO van con BE
	if(simData->HD)
	{
		printf("LI: ");
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
	
	
	if(quantizer->state->nBE)
	{
		// Guardar vector de estados que van con BE
		quantizer->state->BES = (int *) malloc (quantizer->state->nBE * sizeof(int));
		printf("LIBE: ");
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
	
	/*
	// Forzar todo a LI
	for(i=0; i<states; i++)
		quantizer->state->BE[i] = FALSE;
	quantizer->state->nBE = 0;
	*/
	
	
	  
  //ver jacobiano
	printf("Jac\n");
	s=0;
	for (i = 0; i < states; i++)
  {
		for (m = 0; m < simData->nSD[i]; m++)
		{
				//quantizer->state->S[i][quantizer->state->SD[i][m]] = s;
				printf("J[%d][%d] = %g\n", i, simData->SD[i][m], simData->jac[s]);
				s++;
		}
	}
	
	//for(i=0; i<32; i++)
	//	printf("%g\n", simData->jac[i]);	
	
	
 	// Jacobiano hardcodeado	
 	// BBall
	if(states==4)
	{
	quantizer->state->a[quantizer->state->S[0][1]] = 1;
	quantizer->state->a[quantizer->state->S[2][3]] = 1;
	quantizer->state->a[quantizer->state->S[3][3]] = -0.1;
	}
	// barra	
	if(states>9)
	{
	quantizer->state->a[quantizer->state->S[0][0]] = -187.057;
	quantizer->state->a[quantizer->state->S[0][1]] = 187.057;
	quantizer->state->a[quantizer->state->S[1][0]] = 105.219;
	quantizer->state->a[quantizer->state->S[1][1]] = -280.585;
	quantizer->state->a[quantizer->state->S[1][2]] = 175.366;
	quantizer->state->a[quantizer->state->S[2][1]] = 116.91;
	quantizer->state->a[quantizer->state->S[2][2]] = -280.585;
	quantizer->state->a[quantizer->state->S[2][3]] = 163.675;
	quantizer->state->a[quantizer->state->S[3][2]] = 122.756;
	quantizer->state->a[quantizer->state->S[3][3]] = -280.585;
	quantizer->state->a[quantizer->state->S[3][4]] = 157.829;
	quantizer->state->a[quantizer->state->S[4][3]] = 126.263;
	quantizer->state->a[quantizer->state->S[4][4]] = -280.585;
	quantizer->state->a[quantizer->state->S[4][5]] = 154.322;
	quantizer->state->a[quantizer->state->S[5][4]] = 128.601;
	quantizer->state->a[quantizer->state->S[5][5]] = -280.585;
	quantizer->state->a[quantizer->state->S[5][6]] = 151.984;
	quantizer->state->a[quantizer->state->S[6][5]] = 130.272;
	quantizer->state->a[quantizer->state->S[6][6]] = -280.585;
	quantizer->state->a[quantizer->state->S[6][7]] = 150.313;
	quantizer->state->a[quantizer->state->S[7][6]] = 131.524;
	quantizer->state->a[quantizer->state->S[7][7]] = -280.585;
	quantizer->state->a[quantizer->state->S[7][8]] = 149.061;
	quantizer->state->a[quantizer->state->S[8][7]] = 132.498;
	quantizer->state->a[quantizer->state->S[8][8]] = -280.585;
	quantizer->state->a[quantizer->state->S[8][9]] = 148.087;
	quantizer->state->a[quantizer->state->S[9][8]] = 84.269;
	quantizer->state->a[quantizer->state->S[9][9]] = -84.5495;
	
	
	// Convertidor buck (no hace falta, se usa LI)
	/*
	quantizer->state->a[quantizer->state->S[10][10]] = -0.1;
	quantizer->state->a[quantizer->state->S[10][11]] = -10000;
	quantizer->state->a[quantizer->state->S[11][10]] = 10000;
	quantizer->state->a[quantizer->state->S[11][11]] = -1000;
	*/
	}
	// variable extra para probar
	if(states>12)
		quantizer->state->a[quantizer->state->S[12][12]] = -1;		
    
#ifdef QSS_PARALLEL
  quantizer->state->qMap = simData->lp->qMap;
#endif
  quantizer->state->finTime = simData->ft;
  quantizer->state->minStep = simData->params->minStep;
  quantizer->state->lSimTime = simTime;
  
  quantizer->state->nSZ = simData->nSZ;
  quantizer->state->SZ = simData->SZ;
  quantizer->ops->recomputeNextTimes = QSS2_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = QSS2_recomputeNextTime;
  quantizer->ops->nextTime = QSS2_nextTime;
  quantizer->ops->updateQuantizedState = QSS2_updateQuantizedState;
}

#ifdef QSS_PARALLEL
void
QSS2_PAR_recomputeNextTime (QA_quantizer quantizer, int i, double t, double *nTime,
		       double *x, double *lqu, double *q)
#else
void
QSS2_recomputeNextTime (QA_quantizer quantizer, int i, double t, double *nTime,
		       double *x, double *lqu, double *q)
#endif
{
	int i0 = i*3, i1 = i0 + 1, i2 = i1 + 1, k;
  int *flag2=quantizer->state->flag2;
	double *tx = quantizer->state->tx;
	bool *change = quantizer->state->change;
	int stind  = quantizer->state->lSimTime->minIndex;
  bool self = quantizer->state->lSimTime->minIndex == i && quantizer->state ->lSimTime -> type == ST_State;
	double diffQ, timeaux, coeff[3];
	double *A = quantizer->state->a;	
	double *U0 = quantizer->state->u0;	
	double *U1 = quantizer->state->u1;
  int **S = quantizer->state->S;
  int sts = quantizer->state->sts;
  double h = quantizer->state->h;
  bool *BE = quantizer->state->BE;
  
  if(!BE[i])
	{
		// Jacobian estimation
		if(t > 0)
		{
			diffQ = q[3*stind] - quantizer->state->qAux[stind];
			//if(diffQ) A[S[i][stind]] = (x[i1] - quantizer->state->oldDx[i])/diffQ;
			if(diffQ && stind <= sts) A[S[i][stind]] = (x[i1] - quantizer->state->oldDx[i])/diffQ;
			if(self && A[S[stind][stind]] > 0) A[S[stind][stind]] = 0;
		}
		else A[S[i][i]] = 0;	
		// affine parameters
		U0[S[i][i]] = x[i1] - q[i0]*A[S[i][i]];
		U1[S[i][i]] = 2 * x[i2] - q[i1]*A[S[i][i]];
		// store last x update
		tx[i] = t;
	 
		if(change[i] == TRUE) nTime[i] = t;
		else 
		{
			coeff[1] = q[i1] - x[i1];
			coeff[2] = -x[i2];
			if (flag2[i] != 1)
			{
				if (quantizer->state->lSimTime->type == ST_Event && A[S[i][i]]<0 && quantizer->state->nSZ[i]>0)
				{ //we check if variable i is involved in the zero crossing function that produced the current event
					for (k = 0; k<quantizer->state->nSZ[i]; k++)
					{
						if (quantizer->state->SZ[i][k] == quantizer->state->lSimTime->minIndex)
						{
							nTime[i] = t;
							flag2[i] = 1; //it does, so we restart the quantized state q[i]
						}
					}
				}
				if (flag2[i] != 1)
				{
					if(A[S[i][i]] < 0 && self && x[i2] != 0) nTime[i] = t + fabs((q[i1]-x[i1])/x[i2]*2);
					else nTime[i] = INF;
					coeff[0] = q[i0] + 2*lqu[i] - x[i0];
					timeaux = t + minPosRoot(coeff, 2);
					if (timeaux < nTime[i]) nTime[i] = timeaux;
					coeff[0] = q[i0] - 2*lqu[i] - x[i0];
					timeaux = t + minPosRoot(coeff, 2);
					if (timeaux < nTime[i]) nTime[i] = timeaux;
					if (q[i0]*q[i1]<0 && fabs(q[i0])>10*lqu[i]) 
					{
						timeaux = -q[i0]/q[i1] - 2*fabs(lqu[i]/q[i1]);
						if (nTime[i] > t + timeaux) nTime[i] = t + timeaux;
					}
					if (flag2[i] == 2 && self) flag2[i] = 0;
					double err1 = q[i0] - x[i0] + coeff[1]*(nTime[i] - t)/2 + coeff[2]*pow((nTime[i] - t) / 2, 2);
					if (fabs(err1) > 3*fabs(lqu[i])) nTime[i] = t + quantizer->state->finTime*quantizer->state->minStep;
				}
			} 
			else
			{    
				if (self)
				{
					flag2[i] = 2;
					nTime[i] = t;
				}
			}
		}
	}
	else
	{
		if(change[i] == TRUE)
			nTime[i] = t;
		else
			nTime[i] = t + h;
	}
}

#ifdef QSS_PARALLEL
void
QSS2_PAR_recomputeNextTimes (QA_quantizer quantizer, int vars, int *inf, double t,
			double *nTime, double *x, double *lqu, double *q)
#else
void
QSS2_recomputeNextTimes (QA_quantizer quantizer, int vars, int *inf, double t,
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
			QSS2_recomputeNextTime (quantizer, inf[i], t, nTime, x, lqu, q);
#ifdef QSS_PARALLEL
		}
#endif
	}
}

#ifdef QSS_PARALLEL
void
QSS2_PAR_nextTime (QA_quantizer quantizer, int i, double t, double *nTime,
	      double *x, double *lqu)
#else
void
QSS2_nextTime (QA_quantizer quantizer, int i, double t, double *nTime,
	      double *x, double *lqu)
#endif
{
	bool *BE = quantizer->state->BE;
	if(BE[i])
	  nTime[i] = INF;
	else
	{
		int i2 = i * 3 + 2;
		if (x[i2] == 0) nTime[i] = INF;
		else nTime[i] = t + sqrt(fabs(lqu[i]/x[i2]));
	}
}
#ifdef QSS_PARALLEL
double QSS2_PAR_maxi(double x1, double x2, double d)
#else
double QSS2_maxi(double x1, double x2, double d)
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
QSS2_PAR_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *lqu)
#else
void
QSS2_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *lqu)
#endif
{
	double *a = quantizer->state->a;	
	int **S = quantizer->state->S;	
	double h = quantizer->state->h;	
	int *BES = quantizer->state->BES;	
	int nBE = quantizer->state->nBE;	
	int i, j;
	// invAd = I-h*A
	double invAd[nBE][nBE];
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
	// inversa de I-h*A
	int nn = nBE*nBE;
  int pivotArray[nBE];
	int errorHandler;
	double lapackWorkspace[nBE];
	extern void dgetrf_ (int * m, int * n, double * A, int * LDA, int * IPIV, int * INFO);
	extern void dgetri_ (int * n, double * A, int * LDA, int * IPIV, double * WORK, int * LWORK, int * INFO);
	dgetrf_(&nBE, &nBE, invAd[0], &nBE, pivotArray, &errorHandler);
	dgetri_(&nBE, invAd[0], &nBE, pivotArray, lapackWorkspace, &nn, &errorHandler);
	
	double FE, max = 0, e = 0, mlqu = lqu[BES[0]];
	for(i=0; i<nBE; i++)
	{
		FE = q[3*BES[i]] + h*x[3*BES[i]+1];// FE - x(+) = x(-) + h*f(x(-),t-)
		for(j=0; j<nBE; j++)
			q[3*BES[i]] += invAd[i][j]*h*x[3*BES[j]+1]; // LIBE - x(+) = x(-) + h*(I-hA)^-1*f(x(-),t-)
		q[3*BES[i]+1] = 0; // derivada de q nula
		e = fabs(q[3*BES[i]]-FE) / QSS2_maxi(q[3*BES[i]], FE, 1e-20);
		if(e > max)
			max = e;
		if(lqu[BES[i]]>mlqu)
			mlqu = lqu[BES[i]];
	}
	
	// control de paso
	h = quantizer->state->h * sqrt(mlqu/max/100);
	if(quantizer->state->lSimTime->time + h > quantizer->state->finTime)
		quantizer->state->h = quantizer->state->finTime*(1-1e-10) - quantizer->state->lSimTime->time;
	else
		quantizer->state->h = h;
}



#ifdef QSS_PARALLEL
void
QSS2_PAR_updateQuantizedState (QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
#else
void
QSS2_updateQuantizedState (QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
#endif
{
	int i0 = i*3, i1 = i0 + 1, i2 = i1 + 1;		
	//int m, j, j0, j1, j2;
	//int nSD = quantizer->state->nSD[i];
	bool *change = quantizer->state->change;
	double t = quantizer->state->lSimTime->time;	
	double *U0 = quantizer->state->u0;
	double *U1 = quantizer->state->u1;
	double *A = quantizer->state->a;
	double *tx = quantizer->state->tx;
	int **S = quantizer->state->S;
	double elapsed;
	bool *BE = quantizer->state->BE;
	int nBE = quantizer->state->nBE;
	int *BES = quantizer->state->BES;	
	
  quantizer->state->oldDx[i] = x[i1];
  elapsed = t - tx[i];
  U0[S[i][i]] = U0[S[i][i]] + elapsed*U1[S[i][i]];
	tx[i] = t;
	
	if(BE[i])
  {
		quantizer->state->qAux[i] = q[i0];
		if(change[i] == TRUE)
			change[i] = FALSE;
		else
		{
			if(i == BES[0])
			{
				// LIBE
				int j;
				for(j=1; j<nBE; j++)
					change[BES[j]] = TRUE;
				QSS2_solver2x2_h(quantizer, x, q, lqu);
			}
		}
	}
	else
	{
		elapsed = t - quantizer->state->lSimTime->tq[i];
		quantizer->state->qAux[i] = q[i0] + elapsed*q[i1];
		quantizer->state->oldDx[i] = x[i1];
		elapsed = t - tx[i];
		U0[S[i][i]] = U0[S[i][i]] + elapsed*U1[S[i][i]];
		tx[i] = t;
		
		if(change[i] == TRUE)
		{// second variable update
			//q[i0] = qj[i0];
			//q[i1] = qj[i1];
			change[i] = FALSE;
		}
		else
		{// normal update
			int *flag2 = quantizer->state->flag2;  
			double h = 0, oldq1 = q[i1], ddx = x[i2]*2;	
			
			// LIQSS2	  
			if(A[S[i][i]] < 0)
			{
				if(ddx == 0)
				{
					ddx = A[S[i][i]]*A[S[i][i]]*q[i0] + A[S[i][i]]*U0[S[i][i]] + U1[S[i][i]];
					if(ddx == 0) ddx = 1e-40;
				}
				h = (quantizer->state->finTime - t)*10;
				q[i0] = ((x[i0]+h*U0[S[i][i]]+h*h/2*U1[S[i][i]])*(1-h*A[S[i][i]]) + (h*h/2*A[S[i][i]]- h)*(U0[S[i][i]]+h*U1[S[i][i]])) / (1 - h*A[S[i][i]] + h*h*A[S[i][i]]*A[S[i][i]]/2);
				if(fabs(q[i0] - x[i0]) > 2*lqu[i]) 
				{
					h = sqrt(fabs(2*lqu[i]/ddx));
					q[i0] = ((x[i0]+h*U0[S[i][i]]+h*h/2*U1[S[i][i]])*(1-h*A[S[i][i]]) + (h*h/2*A[S[i][i]]- h)*(U0[S[i][i]]+h*U1[S[i][i]])) / (1 - h*A[S[i][i]] + h*h*A[S[i][i]]*A[S[i][i]]/2);
				}
				while(fabs(q[i0] - x[i0]) > 2*lqu[i])
				{
					h = h*sqrt(lqu[i]/fabs(q[i0] - x[i0]));
					q[i0] = ((x[i0]+h*U0[S[i][i]]+h*h/2*U1[S[i][i]])*(1-h*A[S[i][i]]) + (h*h/2*A[S[i][i]]- h)*(U0[S[i][i]]+h*U1[S[i][i]])) / (1 - h*A[S[i][i]] + h*h*A[S[i][i]]*A[S[i][i]]/2);
				}
				q[i1] = (A[S[i][i]]*q[i0] + U0[S[i][i]] + h*U1[S[i][i]]) / (1-h*A[S[i][i]]);		
			}
			else
			{
				ddx = U1[S[i][i]];
				if (ddx > 0) q[i0] = x[i0] - lqu[i];
				else q[i0] = x[i0] + lqu[i];
				if (ddx != 0)
				{
					h = sqrt(2*lqu[i]/fabs(ddx));
					q[i1] = U0[S[i][i]] + h*U1[S[i][i]];
				}
				else q[i1] = U0[S[i][i]];
			}
			if (fabs(q[i0] - x[i0]) > 1.95*lqu[i]) q[i0] = x[i0];
			if (flag2[i] >= 1)
			{
				
				if(lqu[i]>1e-4)
				{
					if (oldq1>0) q[i0] = quantizer->state->qAux[i] + lqu[i]/1e2;
					else q[i0] = quantizer->state->qAux[i] - lqu[i]/1e2;
				}
				else
				{
					if (oldq1>0) q[i0] = quantizer->state->qAux[i] + lqu[i]/1e1;
					else q[i0] = quantizer->state->qAux[i] - lqu[i]/1e1;
					q[i1] = 0;
				}
				
				q[i1] = 0;
				/*
				if (oldq1>0) q[i0] = quantizer->state->qAux[i] + lqu[i]/1e2;
				else q[i0] = quantizer->state->qAux[i] - lqu[i]/1e2; 
				*/
			}
		}
	}
	int j, j0, j1, j2, m;
	int nSD = quantizer->state->nSD[i];
	// store values of dx/dt for every influenced variable
	if(t>0)
	{
		for (m = 0; m < nSD; m++)
		{
			j = quantizer->state->SD[i][m];
			j0 = j*3;
			j1 = j0 + 1;
			j2 = j1 + 1;
			quantizer->state->oldDx[j] = x[j1] + 2*(t-tx[j])*x[j2];
		}
	}
}
