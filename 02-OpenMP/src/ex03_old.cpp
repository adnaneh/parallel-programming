#include <chrono>
#include <random>
#include <iostream>
#include <float.h>
#include <immintrin.h>
#include <omp.h>


double sequentiel(double* A, double* B, double* S, unsigned long int size) {
    double s = 0;
    for (unsigned long int i = 0; i < size; i++) {
        if(i < size/10){
            s += A[i] * B[i];
        }
    }
    return s;
}

double parallele_static_1(double* A,double* B, double*S, unsigned long int size) {
    double s = 0;
 
    #pragma omp parallel shared(s)
    {
        #pragma omp for reduction(+:s) schedule(static , size/200)
        for (unsigned long int i = 0; i < size; i++) {
            if(i < size/10){
                s += A[i] * B[i];
            }
        }
    }
    return s;
    }

double parallele_static_2(double* A,double* B, double*S, unsigned long int size) {
    double s = 0;
 
    #pragma omp parallel shared(s)
    {
        #pragma omp for reduction(+:s) schedule(static , size/10)
        for (unsigned long int i = 0; i < size; i++) {
            if(i < size/10){
                s += A[i] * B[i];
            }
        }
    }
    return s;
    }

double parallele_dynamic_1(double* A,double* B, double*S, unsigned long int size) {
    double s = 0;
 
    #pragma omp parallel shared(s)
    {
        #pragma omp for reduction(+:s) schedule(dynamic , size/200)
        for (unsigned long int i = 0; i < size; i++) {
            if(i < size/10){
                s += A[i] * B[i];
            }
        }
    }
    return s;
    }

double parallele_dynamic_2(double* A,double* B, double*S, unsigned long int size) {
    double s = 0;
 
    #pragma omp parallel shared(s)
    {
        #pragma omp for reduction(+:s) schedule(dynamic , size/10)
        for (unsigned long int i = 0; i < size; i++) {
            if(i < size/10){
                s += A[i] * B[i];
            }
        }
    }
    return s;
    }

double parallele_guided_1(double* A,double* B, double*S, unsigned long int size) {
    double s = 0;
 
    #pragma omp parallel shared(s)
    {
        #pragma omp for reduction(+:s) schedule(guided , size/200)
        for (unsigned long int i = 0; i < size; i++) {
            if(i < size/10){
                s += A[i] * B[i];
            }
        }
    }
    return s;
    }

double parallele_guided_2(double* A,double* B, double*S, unsigned long int size) {
    double s = 0;
 
    #pragma omp parallel shared(s)
    {
        #pragma omp for reduction(+:s) schedule(guided , size/10)
        for (unsigned long int i = 0; i < size; i++) {
            if(i < size/10){
                s += A[i] * B[i];
            }
        }
    }
    return s;
    }

double parallele_auto(double* A,double* B, double*S, unsigned long int size) {
    double s = 0;
 
    #pragma omp parallel shared(s)
    {
        #pragma omp for reduction(+:s) schedule(auto)
        for (unsigned long int i = 0; i < size; i++) {
            if(i < size/10){
                s += A[i] * B[i];
            }
        }
    }
    return s;
    }


double parallele_runtime(double* A,double* B, double*S, unsigned long int size) {
    double s = 0;
 
    #pragma omp parallel shared(s)
    {
        #pragma omp for reduction(+:s) schedule(runtime)
        for (unsigned long int i = 0; i < size; i++) {
            if(i < size/10){
                s += A[i] * B[i];
            }
        }
    }
    return s;
    }

double parallele_default(double* A,double* B, double*S, unsigned long int size) {
    double s = 0;
 
    #pragma omp parallel shared(s)
    {
        #pragma omp for reduction(+:s)
        for (unsigned long int i = 0; i < size; i++) {
            if(i < size/10){
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
        s2 += parallele_auto(A, B, S2, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double par_duration_auto = std::chrono::duration<double>(t1-t0).count();
    par_duration_auto /= (size*iter);
    
    t0 = std::chrono::high_resolution_clock::now();
    double s3 = 0;
    for (auto it =0; it < iter; it++) {
        s3 += parallele_dynamic_1(A, B, S2, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double par_duration_dynamic_1 = std::chrono::duration<double>(t1-t0).count();
    par_duration_dynamic_1 /= (size*iter);
    
    t0 = std::chrono::high_resolution_clock::now();
    double s4 = 0;
    for (auto it =0; it < iter; it++) {
        s4 += parallele_dynamic_2(A, B, S2, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double par_duration_dynamic_2 = std::chrono::duration<double>(t1-t0).count();
    par_duration_dynamic_2 /= (size*iter);
    
    t0 = std::chrono::high_resolution_clock::now();
    double s5 = 0;
    for (auto it =0; it < iter; it++) {
        s5 += parallele_guided_1(A, B, S2, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double par_duration_guided_1 = std::chrono::duration<double>(t1-t0).count();
    par_duration_guided_1/= (size*iter);
    
    t0 = std::chrono::high_resolution_clock::now();
    double s6 = 0;
    for (auto it =0; it < iter; it++) {
        s6 += parallele_guided_2(A, B, S2, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double par_duration_guided_2 = std::chrono::duration<double>(t1-t0).count();
    par_duration_guided_2 /= (size*iter);
    
    t0 = std::chrono::high_resolution_clock::now();
    double s7 = 0;
    for (auto it =0; it < iter; it++) {
        s7 += parallele_runtime(A, B, S2, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double par_duration_runtime = std::chrono::duration<double>(t1-t0).count();
    par_duration_runtime /= (size*iter);
    
    t0 = std::chrono::high_resolution_clock::now();
    double s8 = 0;
    for (auto it =0; it < iter; it++) {
        s8 += parallele_static_1(A, B, S2, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double par_duration_static_1 = std::chrono::duration<double>(t1-t0).count();
    par_duration_static_1 /= (size*iter);
    
    t0 = std::chrono::high_resolution_clock::now();
    double s9 = 0;
    for (auto it =0; it < iter; it++) {
        s9 += parallele_static_2(A, B, S2, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double par_duration_static_2 = std::chrono::duration<double>(t1-t0).count();
    par_duration_static_2 /= (size*iter);
    
    t0 = std::chrono::high_resolution_clock::now();
    double s10 = 0;
    for (auto it =0; it < iter; it++) {
        s10 += parallele_default(A, B, S2, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double par_duration_default = std::chrono::duration<double>(t1-t0).count();
    par_duration_default /= (size*iter);
    
    
    
    std::cout << size << " " << seq_duration/par_duration_auto << " " <<s1/s2 
    << " " << seq_duration/par_duration_dynamic_1 << " " <<s1/s3
    << " " << seq_duration/par_duration_dynamic_2 << " " <<s1/s4 
    << " " << seq_duration/par_duration_guided_1 << " " <<s1/s5
    << " " << seq_duration/par_duration_guided_2 << " " <<s1/s6 
    << " " << seq_duration/par_duration_runtime << " " <<s1/s7
    << " " << seq_duration/par_duration_static_1 << " " <<s1/s8 
    << " " << seq_duration/par_duration_static_2 << " " <<s1/s9
    << " " << seq_duration/par_duration_default << " " <<s1/s10 
    << std::endl;
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
