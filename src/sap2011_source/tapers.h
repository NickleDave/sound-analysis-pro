//---------------------------------------------------------------------------

#ifndef tapersH
#define tapersH
//---------------------------------------------------------------------------
#define PI 3.14159265358979
#define ABS(a) ((a) < (0) ? -(a) : (a))
#define SQR(a) ((a) == 0.0 ? 0.0 : (a)*(a))
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))
#define DIAG1 0
#define JM_ADD 0
//---------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>

bool savgol(float c[], int np, int nl, int nr, int ld, int m);
bool ludcmp(float **a, int n, int *indx, float *d);
void lubksb(float **a, int n, int *indx, float b[]);

void multitap(int num_points, int nwin, double *lam, float npi
    , double *tapers, double *tapsum, float TapersOut[]);

void jtinvit_(int *nm, int *n, double *d, double *e, double *e2, int *m, double *w, int *ind, double *z, int *ierr, double *rv1, double *rv2,
	 double *rv3, double *rv4, double *rv6);

void jtridib_(int *n, double *eps1, double *d, double *e, double *e2, double *lb, double *ub, int *m11, int *m, double *w, int *ind, int *ierr,
	 double *rv4, double *rv5);

void proTapers(int WindowSize, int tpr, float wd, float TapersOut[]);

#endif
