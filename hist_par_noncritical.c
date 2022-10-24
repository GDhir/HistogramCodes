#include "omp.h"

void pa1_hist_par(int nelts, int nbins, int *__restrict__ data, int *__restrict__ hist) 
{
  int i,nt;
  int histval[omp_get_max_threads()*nbins];

  // int histval[nbins];
  //   for ( i = 0; i < nbins; i++ ) {
  //      histval[i] = 0;
  // }

  #pragma omp parallel private(i) shared(histval)
  {
    int tn = omp_get_thread_num();

    #pragma omp single
    {
      nt = omp_get_num_threads();
    }

    #pragma omp for nowait
    for ( i = 0; i < nbins; i++ ) {
       hist[i] = 0;
    }

    #pragma omp for
    for( int j = 0; j < nt; j++ ) {
      for( int i = 0; i < nbins; i++ )
          histval[j*nbins + i] = 0;
    }

    #pragma omp for
    for ( i = 0; i < nelts; i++ ) {

      histval[ tn*nbins + data[i] ] += 1;

    }

    // if( omp_get_thread_num()%2 == 0 ) {

    //   for( i = nbins - 1; i >= 0; i-- ) {
    //     #pragma omp atomic
    //     hist[ i ] += histval[i];
    //   }
    // }
    // else {

    //   for( i = 0; i < nbins; i++ ) {
    //     #pragma omp atomic
    //     hist[ i ] += histval[i];
    //   }
    // }
    #pragma omp single
    {
      for( i = 0; i < nbins; i++ ) { 
        for( int j = 0; j < nt; j++ ) {
              hist[i] += histval[i + j*nbins];
        }
      }
    }

  }
}
