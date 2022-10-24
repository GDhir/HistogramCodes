#include "omp.h"

void pa1_hist_par(int nelts, int nbins, int *__restrict__ data, int *__restrict__ hist) 
{
  int i;

  #pragma omp parallel private(i)
  {
    
    #pragma omp for nowait
    for ( i = 0; i < nbins; i++ ) {
       hist[i] = 0;
    }

    int histval[nbins];
    for ( i = 0; i < nbins; i++ ) {
       histval[i] = 0;
    }

    #pragma omp for nowait
    for ( i = 0; i < nelts; i++ ) {

      histval[ data[i] ] += 1;

    }

    if( omp_get_thread_num()%2 == 0 ) {

      for( i = nbins - 1; i >= 0; i-- ) {
        #pragma omp atomic
        hist[ i ] += histval[i];
      }
    }
    else {

      for( i = 0; i < nbins; i++ ) {
        #pragma omp atomic
        hist[ i ] += histval[i];
      }
    }
  }
}
