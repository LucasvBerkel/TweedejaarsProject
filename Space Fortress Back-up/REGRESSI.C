#include <alloc.h>
#include <stdlib.h>
#include <io.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include "nr.h"
#include "nrutil.h"
#include <stdarg.h>
#include "myext.h"

void Regression();
int Lin_Reg();
int Lin_Reg_Without_Outliers();
int function();
int linstatistics(int par, double *MSE, int *lista, float **covar);
int nonlinstatistics(int par, double *MSE, int *lista, float **covar);
int Fprob(double F,  int M, int N);
int Display_Model(double T_A, double BETA, int E, double ALFA,
		  double T_A_OLD, double BETA_OLD, int E_OLD, double ALFA_OLD);


int 	alarm=0;
float 	p_value;
double  sum_times;
float  	*x;
float  	*y;
int     ndata;
int     xloc, yloc;

void Regression()
{
 double	        SIGMA, residu, y_model, y_real;
 double	        delta=0.000001, MSE=0.0;
 float 		*sig, p_value_old;
 int   		i,j,k,VV_OLD;
 float   	*a;
 int   		ma=4;
 int   		*lista, *dumvar2;
 int   		mfit=4;
 float   	**covar, **alpha, **dumvar1;
 float   	chisq;
 float   	alamda;
 float		chisq_old, chisq_mid, chisq_new, h;

 dumvar1 = matrix(1,1,1,1);
 dumvar2 = ivector(1,1);
 lista = ivector(1,4);
 covar = matrix(1,4,1,4); /* rijen, kolommen */
 alpha = matrix(1,4,1,4); /* rijen, kolommen */
 a = vector(1,4);

 if(train.alfa_ind < -2000) lin_reg_flag = ON;
 if(train.alfa_ind > 0) lin_reg_flag = ON;
 if(train.beta_ind <= 0) lin_reg_flag = ON;
 if(train.asymp_ind <= 0) lin_reg_flag = ON;
 if(train.p_p_ind < 0) lin_reg_flag = ON;
 if(lin_reg_flag == ON) Lin_Reg();
 else
   {
    LOP_ALFA = (double)train.alfa_ind/1000;
    LOP_BETA = (double)train.beta_ind;
    LOP_E    = train.p_p_ind;
    LOP_T_A  = (double)train.asymp_ind;
    linstatistics(1, &MSE, dumvar2, dumvar1);
   }

 if (lin_reg_flag == ON)
   {
   linstatistics(1, &MSE, dumvar2, dumvar1);
   if ((LOP_ALFA < -3.0)||(train.varperc_ind < 5)||(LOP_BETA > 30000.0)||(p_value > 0.01))
     {
     if (LOP_E <= 1) LOP_E = 1; else LOP_E = Ind_Round(log(LOP_E));
     Lin_Reg();
     linstatistics(1, &MSE, dumvar2, dumvar1);
     }
   else lin_reg_flag = OFF;
   }

 if (lin_reg_flag == ON)
  {
   if ((LOP_ALFA < -3.0)||(train.varperc_ind < 5)||(LOP_BETA > 30000.0)||(p_value > 0.01))
     {
      LOP_BETA = sum_times/(double)NN;
      train.beta_ind = Ind_Round(LOP_BETA);
      LOP_ALFA = -0.001;
      train.alfa_ind = -1;
      LOP_T_A = 0.0;
      train.asymp_ind = 0;
      LOP_E = 1;
      train.p_p_ind = 1;
      train.varperc_ind = -1;
      train.Fprob_ind = -1;
      free_matrix(dumvar1,1,1,1,1);
      free_ivector(dumvar2,1,1);
      free_ivector(lista,1,4);
      free_matrix(covar,1,4,1,4); /* rijen, kolommen */
      free_matrix(alpha,1,4,1,4); /* rijen, kolommen */
      free_vector(a,1,4);
      lin_reg_flag = OFF;
      return;
     }
    lin_reg_flag = OFF;
   }

SIGMA = sqrt(MSE);
if (SIGMA < delta) SIGMA = delta;

 j = 0;
 for(i=1; i <= NN ;i++)
  {
   h = LOP_T_A + LOP_BETA * pow(LOP_E + i, LOP_ALFA);
   if (h < delta) h = delta;
   y_model = log(h);
   h = ((float)column_data[i-1]/20.0);
   if (h < delta) h = delta;
   y_real = log(h);
   residu = (y_model-y_real)/SIGMA;
   if( fabs(residu) < 2.5) j++;
  }

 ndata = j;
 x = vector(1,ndata);
 y = vector(1,ndata);
 sig = vector(1,ndata);

 j = 0;
 for(i=1; i <= NN ;i++)
  {
   h = LOP_T_A + LOP_BETA * pow(LOP_E + i, LOP_ALFA);
   if (h < delta) h = delta;
   y_model = log(h);
   h = ((float)column_data[i-1]/20.0);
   if (h < delta) h = delta;
   y_real = log(h);
   residu = (y_model-y_real)/SIGMA;
   if( fabs(residu) < 2.5)
   {
    j++;
    y[j] = y_real;
    x[j] = (float)i;
    sig[j] = 1.0;
   }
  }

 Lin_Reg_Without_Outliers();
 Nonlinstatistics(1, &MSE, dumvar2, dumvar1);

 ALFA_OLD = LOP_ALFA;
 BETA_OLD = LOP_BETA;
 E_OLD = LOP_E;
 T_A_OLD = LOP_T_A;
 p_value_old = p_value;
 VV_OLD = train.varperc_ind;

 alarm = 0;
 lista[1] = 1;
 lista[2] = 2;
 lista[3] = 3;
 lista[4] = 4;

Again:
 a[1] = (float)LOP_T_A;
 a[2] = (float)LOP_BETA;
 a[3] = (float)LOP_E;
 a[4] = (float)LOP_ALFA;

alamda = -1.0;
chisq = 1.0;
chisq_old = 0.0;
chisq_mid = 0.0;
chisq_new = 0.0;

i=1;

do
{
 mrqmin(x, y, sig, ndata, a, ma, lista, mfit, covar, alpha, &chisq, function, &alamda);
 i++;
 chisq_old = chisq_mid;
 chisq_mid = chisq_new;
 chisq_new = chisq;
 if(alarm == -1) goto Numesc;
 if((alamda < 0.0000001) || (fabs(chisq - chisq_old) < 2.0) || (i > 50) || (chisq < 5)) i = 0;
}
while (i != 0);
i=0;

alamda = 0.0;
mrqmin(x, y, sig, ndata, a, ma, lista, mfit, covar, alpha, &chisq, function, &alamda);
if(alarm == -1) goto Numesc;

LOP_T_A = (double) a[1];
LOP_BETA = (double) a[2];
LOP_E = (int) a[3];
LOP_ALFA = (double) a[4];

if (LOP_T_A < 4.0)
 {
  LOP_T_A  = T_A_OLD;
  LOP_E    = E_OLD;
  LOP_ALFA = ALFA_OLD;
  LOP_BETA = BETA_OLD;
  mfit = 3;
  lista[1] = 2;
  lista[2] = 3;
  lista[3] = 4;
  goto Again;
 }

Nonlinstatistics(mfit, &MSE, lista, covar);
if (p_value > 0.05) alarm = -1;

Numesc :
if (alarm != 0)
  {
   LOP_BETA = BETA_OLD;
   LOP_ALFA = ALFA_OLD;
   LOP_T_A = T_A_OLD;
   LOP_E = E_OLD;
   train.Fprob_ind = Ind_Round((double)p_value_old*1000.0);
   train.varperc_ind = VV_OLD;
  }

free_vector(x,1,ndata);
free_vector(y,1,ndata);
free_vector(sig,1,ndata);
free_ivector(lista,1,4);
free_matrix(covar,1,4,1,4); /* rijen, kolommen */
free_matrix(alpha,1,4,1,4); /* rijen, kolommen */
free_vector(a,1,4);
free_matrix(dumvar1,1,1,1,1);
free_ivector(dumvar2,1,1);
return;
}

int function(x, a, yfit, dyda, ma)
float x, a[], *yfit, dyda[];
int ma;
{
float  p, q, r, s, k=0.000001;

if ( fabs(a[4]) > 5.0)
 {
   xloc=yloc=20;
   gprintf(&xloc, &yloc, "%s", "à > 5");
   gprintf(&xloc, &yloc, "%s","<Druk een toets om door te gaan.>");
   getch();
   return(-1);
 }

p = x + a[3];
if (p < k) p = k;
q = pow(p, a[4]);
r = a[2] * q;
s = r + a[1];
if (s < k) s = k;
*yfit = log(s);

//printf("*yfit = %f  ", *yfit);
dyda[1] = 1/s;
//printf("dyda[1] = %f  ", dyda[1]);
dyda[2] = q/s;
//printf("dyda[2] = %f\n", dyda[2]);
dyda[3] = r*a[4]/(p*s);
//printf("dyda[3] = %f\n", dyda[3]);
dyda[4] = r*log(p)/s;

return(0);
}


void mrqmin(x,y,sig,ndata,a,ma,lista,mfit,covar,alpha,chisq,funcs,alamda)
float x[],y[],sig[],a[],**covar,**alpha,*chisq,*alamda;
int ndata,ma,lista[],mfit;
int (*funcs)();
{
 int k,kk,j,ihit;
 static float *da,*atry,**oneda,*beta,ochisq;
 float *vector(),**matrix();
 void mrqcof(),gaussj(),covsrt(),nrerror(),free_matrix(),free_vector();

 if (*alamda < 0.0) {
  oneda=matrix(1,mfit,1,1);
  atry=vector(1,ma);
  da=vector(1,ma);
  beta=vector(1,ma);
  kk=mfit+1;
  for (j=1;j<=ma;j++) {
   ihit=0;
   for (k=1;k<=mfit;k++)
    if (lista[k] == j) ihit++;
   if (ihit == 0)
    lista[kk++]=j;
   else if (ihit > 1) {
    xloc=yloc=20;
    gprintf(&xloc, &yloc, "%s","Bad LISTA permutation in MRQMIN-1");
    gprintf(&xloc, &yloc, "%s","<Druk een toets om door te gaan.>");
    getch();
    alarm=-1; goto Jump; }
  }
  if (kk != ma+1) {
      xloc=yloc=20;
      gprintf(&xloc, &yloc, "%s","Bad LISTA permutation in MRQMIN-2");
      gprintf(&xloc, &yloc, "%s","<Druk een toets om door te gaan.>");
      getch();
      alarm=-1; goto Jump; }
  *alamda=0.001;
  mrqcof(x,y,sig,ndata,a,ma,lista,mfit,alpha,beta,chisq,funcs);
  if (alarm == -1) goto Jump;
  ochisq=(*chisq);
 }
 for (j=1;j<=mfit;j++) {
  for (k=1;k<=mfit;k++) covar[j][k]=alpha[j][k];
  covar[j][j]=alpha[j][j]*(1.0+(*alamda));
  oneda[j][1]=beta[j];
 }
 gaussj(covar,mfit,oneda,1);
 if(alarm==-1) goto Jump;
 for (j=1;j<=mfit;j++)
  da[j]=oneda[j][1];
 if (*alamda == 0.0) {
  covsrt(covar,ma,lista,mfit);
Jump:
  free_vector(beta,1,ma);
  free_vector(da,1,ma);
  free_vector(atry,1,ma);
  free_matrix(oneda,1,mfit,1,1);
  return;
 }
 for (j=1;j<=ma;j++) atry[j]=a[j];
 for (j=1;j<=mfit;j++)
  atry[lista[j]] = a[lista[j]]+da[j];
 mrqcof(x,y,sig,ndata,atry,ma,lista,mfit,covar,da,chisq,funcs);
 if (alarm == -1) goto Jump;
 if (*chisq < ochisq) {
  *alamda *= 0.1;
  ochisq=(*chisq);
  for (j=1;j<=mfit;j++) {
   for (k=1;k<=mfit;k++) alpha[j][k]=covar[j][k];
   beta[j]=da[j];
   a[lista[j]]=atry[lista[j]];
  }
 } else {
  *alamda *= 10.0;
  *chisq=ochisq;
 }
 return;
}

void mrqcof(x,y,sig,ndata,a,ma,lista,mfit,alpha,beta,chisq,funcs)
float x[],y[],sig[],a[],**alpha,beta[],*chisq;
int ndata,ma,lista[],mfit;
int (*funcs)(); /* ANSI: int (*funcs)(float,float *,float *,float *,int); */
{

 int k,j, i;
 float ymod,wt,sig2i,dy,*dyda,*vector();
 void free_vector();

 dyda=vector(1,ma);
 for (j=1;j<=mfit;j++) {
  for (k=1;k<=j;k++) alpha[j][k]=0.0;
  beta[j]=0.0;
 }
 *chisq=0.0;
 for (i=1;i<=ndata;i++) {
  alarm=(*funcs)(x[i],a,&ymod,dyda,ma);
  if (alarm==-1) goto Leave;
  sig2i=1.0/(sig[i]*sig[i]);
  dy=y[i]-ymod;
  for (j=1;j<=mfit;j++) {
   wt=dyda[lista[j]]*sig2i;
   for (k=1;k<=j;k++)
    alpha[j][k] += wt*dyda[lista[k]];
   beta[j] += dy*wt;
  }
  (*chisq) += dy*dy*sig2i;
 }
 for (j=2;j<=mfit;j++)
  for (k=1;k<=j-1;k++) alpha[k][j]=alpha[j][k];
Leave:
 free_vector(dyda,1,ma);
}

#define SWAP(a,b) {float temp=(a);(a)=(b);(b)=temp;}

void gaussj(a,n,b,m)
float **a,**b;
int n,m;
{
 int *indxc,*indxr,*ipiv;
 int i,icol,irow,j,k,l,ll,*ivector();
 float big,dum,pivinv;
 void nrerror(),free_ivector();

 if (alarm == -1) return;
 indxc=ivector(1,n);
 indxr=ivector(1,n);
 ipiv=ivector(1,n);
 for (j=1;j<=n;j++) ipiv[j]=0;
 for (i=1;i<=n;i++) {
  big=0.0;
  for (j=1;j<=n;j++)
   if (ipiv[j] != 1)
    for (k=1;k<=n;k++) {
     if (ipiv[k] == 0) {
      if (fabs(a[j][k]) >= big) {
       big=fabs(a[j][k]);
       irow=j;
       icol=k;
      }
     } else if (ipiv[k] > 1) {
	 xloc=yloc=20;
	 gprintf(&xloc, &yloc, "%s","GAUSSJ: Singular Matrix-1");
	 gprintf(&xloc, &yloc, "%s","<Druk een toets om door te gaan.>");
	 getch();
	 alarm=-1; goto Quit;}
    }
  ++(ipiv[icol]);
  if (irow != icol) {
   for (l=1;l<=n;l++) SWAP(a[irow][l],a[icol][l])
   for (l=1;l<=m;l++) SWAP(b[irow][l],b[icol][l])
  }
  indxr[i]=irow;
  indxc[i]=icol;
  if (a[icol][icol] == 0.0) {
     xloc=yloc=20;
     gprintf(&xloc, &yloc, "%s","GAUSSJ: Singular Matrix-2");
     gprintf(&xloc, &yloc, "%s","<Druk een toets om door te gaan.>");
     getch();
     alarm=-1; goto Quit;}
  pivinv=1.0/a[icol][icol];
  a[icol][icol]=1.0;
  for (l=1;l<=n;l++) a[icol][l] *= pivinv;
  for (l=1;l<=m;l++) b[icol][l] *= pivinv;
  for (ll=1;ll<=n;ll++)
   if (ll != icol) {
    dum=a[ll][icol];
    a[ll][icol]=0.0;
    for (l=1;l<=n;l++) a[ll][l] -= a[icol][l]*dum;
    for (l=1;l<=m;l++) b[ll][l] -= b[icol][l]*dum;
   }
 }
 for (l=n;l>=1;l--) {
  if (indxr[l] != indxc[l])
   for (k=1;k<=n;k++)
    SWAP(a[k][indxr[l]],a[k][indxc[l]]);
 }
Quit:
 free_ivector(ipiv,1,n);
 free_ivector(indxr,1,n);
 free_ivector(indxc,1,n);
}

#undef SWAP

void covsrt(covar,ma,lista,mfit)
float **covar;
int ma,lista[],mfit;
{
 int i,j;
 float swap;

 if (alarm == -1) return;
 for (j=1;j<ma;j++)
  for (i=j+1;i<=ma;i++) covar[i][j]=0.0;
 for (i=1;i<mfit;i++)
  for (j=i+1;j<=mfit;j++) {
   if (lista[j] > lista[i])
    covar[lista[j]][lista[i]]=covar[i][j];
   else
    covar[lista[i]][lista[j]]=covar[i][j];
  }
 swap=covar[1][1];
 for (j=1;j<=ma;j++) {
  covar[1][j]=covar[j][j];
  covar[j][j]=0.0;
 }
 covar[lista[1]][lista[1]]=swap;
 for (j=2;j<=mfit;j++) covar[lista[j]][lista[j]]=covar[1][j];
 for (j=2;j<=ma;j++)
  for (i=1;i<=j-1;i++) covar[i][j]=covar[j][i];
}


int Lin_Reg()
{

 int   		i;
 double 	x=0.0, y=0.0,  sum_x=0.0, sum_x2=0.0, sum_xy=0.0,
		sum_y=0.0, sum_y2=0.0, numerator, denominator, h,
		log_beta, k =0.000001;

 for(i=0; i < NN ;i++)
  {
   x = log((double)(LOP_E + i + 1));
   sum_x += x;
   sum_x2 = sum_x2 + (x * x);
   h = ((double)column_data[i]/20.0) - LOP_T_A;
   if (h < k) h = k;
   y= log(h);
   sum_y  = sum_y + y;
   sum_y2 = sum_y2+(y * y);
   sum_xy = sum_xy+(x * y);
  }

//  printf("sum_x   = %f\n", sum_x);
//  printf("sum_x2  = %f\n", sum_x2);
//  printf("sum_y   = %f\n", sum_y);
//  printf("sum_y2  = %f\n", sum_y2);
//  printf("sum_xy  = %f\n", sum_xy);

  numerator = (sum_x * sum_y)-(sum_xy * (double)NN);
  denominator = (sum_x * sum_x)-(sum_x2 * (double)NN);
  if (fabs(denominator) < k)
    LOP_ALFA = -1.0;
  else
    LOP_ALFA = (numerator/denominator);
  log_beta = (sum_y/(double)(NN)) - LOP_ALFA*(sum_x/(double)(NN));
  LOP_BETA = exp(log_beta);

  train.alfa_ind    = Ind_Round(1000*LOP_ALFA);
  train.beta_ind    = Ind_Round(LOP_BETA);
  train.asymp_ind   = Ind_Round(LOP_T_A);
  train.p_p_ind     = LOP_E;

return(0);
}

int Lin_Reg_Without_Outliers()
{
 int   		i;
 double 	xx=0.0, yy=0.0,  sum_x=0.0, sum_x2=0.0, sum_xy=0.0,
		sum_y=0.0, sum_y2=0.0, numerator, denominator, h,
		log_beta, k =0.000001;

 for(i=1; i <= ndata ;i++)
  {
   xx = log((double)(LOP_E + x[i] + 1));
   sum_x += xx;
   sum_x2 = sum_x2 + (xx * xx);
   h = exp(y[i]);
   h = h - LOP_T_A;
   if (h<k)h=k;
   yy = log(h);
   sum_y += yy;
   sum_y2 = sum_y2+(yy * yy);
   sum_xy = sum_xy+(xx * yy);
  }

numerator = (sum_x * sum_y)-(sum_xy * (double)ndata);
denominator = (sum_x * sum_x)-(sum_x2 * (double)ndata);
LOP_ALFA = (numerator/denominator);
log_beta = (sum_y/(double)ndata) - LOP_ALFA*(sum_x/(double)ndata);
LOP_BETA = exp(log_beta);
train.alfa_ind    = Ind_Round(1000*LOP_ALFA);
train.beta_ind    = Ind_Round(LOP_BETA);
train.asymp_ind   = Ind_Round(LOP_T_A);
train.p_p_ind     = LOP_E;
return(0);
}

int linstatistics(int par, double *MSE, int *lista, float **covar)
{
 double		SSE=0.0, SSM=0.0, SSQ=0.0, MSM=0.0, SSXX=0.0;
 double         sum_x=0.0, sum_x2=0.0, sum_y=0.0, y, y_model, y_gem;
 int   		i, dgf_1=0, dgf_2=0;
 double		h, k = 0.000001, F, VV;

 sum_times = 0.0;
 for(i=0; i < NN ;i++)
 {
   h = LOP_E + i + 1;
   if (h < 1) h = 1;
   h = log(h);
   sum_x += h;
   sum_x2 = sum_x2 + (h * h);
   h = (double)column_data[i]/20.0;
   sum_times += h;
   h = h - LOP_T_A;
   if (h < k) h = k;
   h = log(h);
   sum_y += h;
  }

  SSXX = sum_x2 - ((sum_x * sum_x)/(double)NN);
  y_gem = sum_y/(double)NN;
  dgf_1 = par;
  dgf_2 = NN - 1 - par;
  for(i=0; i < NN ;i++)
  {
    h = (double)column_data[i]/20.0;
    h = h - LOP_T_A;
    if (h < k) h = k;
    y = log(h) ;
    h = LOP_BETA * pow(LOP_E + i + 1, LOP_ALFA);
    if (h < k) h = k;
    y_model = log(h);
    SSM = SSM + ((y_gem - y_model)*(y_gem - y_model));
    SSE = SSE + ((y - y_model)*(y - y_model));
  }
  SSQ = SSM + SSE;
  VV = (SSM*100.0/SSQ);
  MSM = SSM/((double)dgf_1);
  *MSE = SSE/((double)dgf_2);
  F = MSM/(*MSE);

  train.Fprob_ind = Fprob(F, dgf_1, dgf_2);
  train.varperc_ind = Ind_Round(VV);

return(0);
}

int nonlinstatistics(int par, double *MSE, int *lista, float **covar)
{
double	       SSE=0.0, SSM=0.0, SSQ=0.0, MSM=0.0;
double         sum_y=0.0, y_model, y_gem;
int   	       i, dgf_1=0, dgf_2=0;
double	       h, k = 0.000001, F, VV;

for(i=1;i<=ndata;i++) sum_y += y[i];
y_gem = sum_y/(double)ndata;
dgf_1 = par;
dgf_2 = ndata - 1 - par;
for(i=1;i<=ndata;i++)
 {
  h = LOP_T_A + LOP_BETA*pow(LOP_E + x[i], LOP_ALFA);
  if(h<k)h=k;
  y_model = log(h);
  SSM = SSM + ((y_gem-y_model)*(y_gem-y_model));
  SSE = SSE + ((y[i] -y_model)*(y[i]-y_model));
 }
SSQ = SSM + SSE;
VV = (SSM*100.0/SSQ);
MSM = SSM/((double)dgf_1);
*MSE = SSE/((double)dgf_2);
F = MSM/(*MSE);
train.Fprob_ind = Fprob(F, dgf_1, dgf_2);
train.varperc_ind = Ind_Round(VV);
return(0);
}

int Fprob(double F,  int M, int N) /* calculates probability from F,
				      M (dgf numerator) and N (dgf denom) */
{
double		Z;
double	 	alfa, beta, denom, num, FF, P, PX, X;
int 		PROMILLAGE, MN;

   P = 1.0;
   if (F <= 0) { P=999.0; return((int)P); }
   if ((M <= 0) || (N <= 0)) { P=999.0; return((int)P); }

   alfa = 2.0 /(9.0*(double)M);
   beta = 2.0 /(9.0*(double)N);
   MN = N;
   FF = F;

  if (F < 1.0)
    {
    X = alfa;
    alfa = beta;
    beta = X;
    FF = 1.0/FF;
    MN = M;
    P = 0.0;
    }

  num = (1.0 - beta)*(long double)pow((double)FF, 1.0/3.0) - 1.0 + alfa;
  denom = (long double)sqrt((double)beta*pow((double)FF, 2.0/3.0) + (double)alfa);
  Z = (long double)fabs((double)num/(double)denom);
  if (MN > 3)
    {
    PX = (1.0 + Z*(0.196854 + Z*(0.115194 + Z*(0.000344 + Z*0.019527))));
    PX = (long double)pow((double)PX, 4.0);
    P = (1.0 - P) - ((0.5 - P)/PX);
    PROMILLAGE = Ind_Round((double)P*1000.0);
    p_value = P;
    return(PROMILLAGE);
    }
   else
    {
    Z = Z*(1.0 + 0.08*(long double)pow((double)Z, 4.0)/(long double)pow((double)MN, 3.0));
    PX = 1.0 + Z*(0.196854 + Z*(0.115194 + Z*(0.000344 + Z*0.019527)));
    PX = (long double)pow((double)PX, 4.0);
    P = (1.0 - P) - ((0.5 - P)/PX);
    PROMILLAGE = Ind_Round((double)P*1000.0);
    p_value = P;
    return(PROMILLAGE);
    }
}

int Display_Model(double T_A, double BETA, int E, double ALFA,
		  double T_A_OLD, double BETA_OLD, int E_OLD, double ALFA_OLD)
{
int i, midx, midy, lowy, Max, Height, Width;

Open_Graphics();
Height = textheight("H");
Width = textwidth("Z");
setbkcolor(WHITE);
setcolor(BLUE);
midx = getmaxx()/2;
midy = getmaxy()/2;
lowy = midy + 3*(int)(midy/4);
line(0, lowy, 2*midx, lowy);
line(1, lowy, 1, 0);

Max = (int)((float)Search_Max(NN)/20.0);

for(i=0; i<NN; i+=(int)(NN/10))
  {
  xloc = (int)(i*2*(float)midx/NN);
  yloc = lowy+15;
  line(xloc, lowy-3, xloc, lowy+3);
  gprintf(&xloc, &yloc, "%d", i);
  }

for(i=0; i<6; i++)
  {
  xloc = 4;
  yloc = lowy - i*lowy/6 - 15;
  line(xloc-3, yloc+15, xloc+3, yloc+15);
  gprintf(&xloc, &yloc, "%d", (int)((float)Max/6.0)*i);
  }

moveto(0, lowy);
setcolor(GREEN);
for(i=0; i<NN; i++)
  lineto( (int)(((float)i/NN)*2*midx), lowy - (int)(((float)column_data[i]/(20.0*(float)Max))*lowy));
setlinestyle(SOLID_LINE, 0xFFFF, 3);
moveto(0, lowy);
setcolor(BLUE);
for(i=0; i<NN; i++)
  lineto( (int)(((float)i/NN)*2*midx), lowy - (int)(((T_A_OLD + BETA_OLD*pow(E_OLD+i+1, ALFA_OLD))/(float)Max)*lowy) );
moveto(0, lowy);
setcolor(RED);
for(i=0; i<NN; i++)
  lineto( (int)(((float)i/NN)*2*midx), lowy - (int)(((T_A + BETA*pow(E+i+1, ALFA))/(float)Max)*lowy));

setcolor(BLUE);
xloc = midx - 6*Width;
yloc = 1*Height;
gprintf(&xloc, &yloc, "%s %d", "Fortvernietigingstijden (in sec.) Fase", train.probe_flag);
yloc = 3*Height;
i=4;
yloc = lowy + i*Height;
gprintf(&xloc, &yloc, "%s", "fortvernietigings-nr. ÄÄ");
xloc = 2*Width;
yloc = 2*midy - 2*Height;
gprintf(&xloc, &yloc, "%s","<Druk een toets om door te gaan.>");
getch();

Close_Graphics();
return(0);
}
