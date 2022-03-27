#include <chrono>
#include <random>
#include <iostream>
#include <float.h>
#include <immintrin.h>
#include <omp.h>


double sequentiel(double* A, double* B, double* S, unsigned long int size) {
    double s = 0;
    for (unsigned long int i = 0; i < size; i++) {
        if(i % 10 ==0){
            s += A[i] * B[i];
        }
    }
    return s;
}

double parallele(double* A,double* B, double*S, unsigned long int size) {
    double s = 0;
    std::cout <<omp_get_num_threads() <<std::endl; 
    #pragma omp parallel shared(s)
    {
        #pragma omp for reduction(+:s) schedule(static , 10)
        for (unsigned long int i = 0; i < size; i++) {
            if(i >=0.2*size && i<0.3*size){
                s += A[i] * B[i];
            }
        }
    }
    return s;
    }


int main(int argc, char* argv[]) {
    
    unsigned long int iter = atoi(argv[1]);

    /* initialize random seed: */
    srand (time(NULL));

    const unsigned long int size = atoi(argv[2])*atoi(argv[2]);

    // Création des données de travail
    double * A,* B,* C,* S1,* S2;
    A = (double *) malloc(size * sizeof(double));
    B = (double *) malloc(size * sizeof(double));
    S1 = (double *) malloc(size * sizeof(double));
    S2 = (double *) malloc(size * sizeof(double));


    for (unsigned long int i = 0; i < size; i++) {
        A[i] = (double)(rand() % 360 - 180.0);
        B[i] = (double)(rand() % 360 - 180.0);
    }

   

    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    double min_duration = DBL_MAX;
    t0 = std::chrono::high_resolution_clock::now();
    double s1 = 0;
    for (auto it =0; it < iter; it++) {
        s1 += sequentiel(A, B, S1, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double seq_duration = std::chrono::duration<double>(t1-t0).count();
    seq_duration /= (size*iter);

    t0 = std::chrono::high_resolution_clock::now();
    double s2 = 0;
    for (auto it =0; it < iter; it++) {
        s2 += parallele(A, B, S2, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double par_duration = std::chrono::duration<double>(t1-t0).count();
    par_duration /= (size*iter);
    
    std::cout << size << " " << seq_duration/par_duration << " " <<s1/s2 << std::endl;
    // std::cout << size << " " << seq_duration << " " << par_duration << std::endl;

    /*** Validation ***/
    bool valide = false;
    
    if(s1 == s2) {
	valide = true;
	}
    else {
            valide = false;
        }

    // Libération de la mémoire : indispensable

    free(A);
    free(B);
    free(S1);
    free(S2);    

    return 0;
}
