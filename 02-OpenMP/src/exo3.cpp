#include <chrono>
#include <random>
#include <iostream>
#include <float.h>
#include <immintrin.h>
#include <omp.h>


double sequentiel(int* A, int* B, int* S, unsigned long int size) {
    int s = 0;
    for (unsigned long int i = 0; i < size; i++) {
        if(A[i]%2 == 0){
           S[s] = A[i];
           s ++;
        }
    }
    return s;
}

double parallele(int* A,int* B, int*S, unsigned long int size) {
    int s = 0;
    #pragma omp parallel for reduction(+:s) 
    for (unsigned long int i = 0; i < size; i++) {
        if(A[i]%2 == 0){
           S[s] = A[i];
           s ++;
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
    int * A,* B,* C,* S1,* S2;
    A = (int *) malloc(size * sizeof(int));
    B = (int *) malloc(size * sizeof(int));
    S1 = (int *) malloc(size * sizeof(int));
    S2 = (int *) malloc(size * sizeof(int));


    for (unsigned long int i = 0; i < size; i++) {
        A[i] = (rand() % 360 - 180.0);
        B[i] = (rand() % 360 - 180.0);
    }

   

    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    double min_duration = DBL_MAX;
    t0 = std::chrono::high_resolution_clock::now();
    int s1 = 0;
    for (auto it =0; it < iter; it++) {
        s1 += sequentiel(A, B, S1, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double seq_duration = std::chrono::duration<double>(t1-t0).count();
    seq_duration /= (size*iter);

    t0 = std::chrono::high_resolution_clock::now();
    int s2 = 0;
    for (auto it =0; it < iter; it++) {
        s2 += parallele(A, B, S2, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double par_duration = std::chrono::duration<double>(t1-t0).count();
    par_duration /= (size*iter);
    
    std::cout << size << " " << seq_duration/par_duration <<" "<<s1/s2<<  std::endl;
    // std::cout << size << " " << seq_duration << " " << par_duration << std::endl;

    /*** Validation ***/
    bool valide = false;
    
    if(s1 == s2) {
	valide = true;
	}
    else {
            valide = false;
        }
    
    for (unsigned long int i = 0; i < size; i++) {
        if(S1[i] == S2[i]) {
            valide = true;
        }
        else {
            valide = false;
            break;
        }
    }
    // Libération de la mémoire : indispensable

    free(A);
    free(B);
    free(S1);
    free(S2);    

    return 0;
}
