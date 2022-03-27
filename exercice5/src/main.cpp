#include <chrono>
#include <random>
#include <iostream>
#include <float.h>
#include <immintrin.h>
#include <omp.h>


int sequentiel(int* A, int* S, unsigned long int size) {
   int s = 0;
    for (unsigned long int i = 0; i < size; i++) {
        if(A[i]%2 == 0){
           S[s] = A[i];
           s ++;
        }
    }
    return s;
}

int parallele(int* A, int* S, unsigned long int size) {
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

int parallele(int* A, int* S, unsigned long int size, int* Storage) {
    int ind = 0;
    int pos = 0;
    int offset = 0;
    int thSize = (size + omp_get_num_threads() - 1) / (omp_get_num_threads());
    int minTh = thSize*omp_get_thread_num();
    int maxTh = minTh+thSize;
    #pragma omp parallel shared(ind)
    {

      if(maxTh > size){
         maxTh = size;
      }
      for(int i = minTh; i<maxTh; i++){
         if(A[i]%2 == 0){
            Storage[minTh + offset] = A[i];
            offset ++;
         }
      }
      
        #pragma omp critical
        {
        pos = ind;
        ind += offset;
        }
      
      for(int i = 0; i<offset; i++){
         S[pos] = Storage[minTh + i];
         pos ++;
      }

    }
    return ind;
}

int main() {
    
    srand (time(NULL));

    for(unsigned long int size = 1024; size<(1024*1024*4);size*=1.2) {
        unsigned long int iter = 100;

    int * A,* B,* C,* S1,* S2,* Storage;
    A = (int *) malloc(size * sizeof(int));
    Storage = (int *) malloc(size * sizeof(int));
    S1 = (int *) malloc(size * sizeof(int));
    S2 = (int *) malloc(size * sizeof(int));


    for (unsigned long int i = 0; i < size; i++) {
        A[i] = (rand() % 360 - 180);
    }

    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

   int nbSub1 = 0;
   int nbSub2 = 0;

    double min_duration = DBL_MAX;
    t0 = std::chrono::high_resolution_clock::now();
    for (auto it =0; it < iter; it++) {
        nbSub1 = sequentiel(A, S1, size);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double seq_duration = std::chrono::duration<double>(t1-t0).count();
    seq_duration /= (size*iter);

    t0 = std::chrono::high_resolution_clock::now();
    for (auto it =0; it < iter; it++) {
        nbSub2 = parallele(A, S2, size, Storage);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double par_duration = std::chrono::duration<double>(t1-t0).count();
    par_duration /= (size*iter);
    
    std::cout << size << " " << seq_duration/par_duration << std::endl;

    bool valide = nbSub1 == nbSub2;

    std::cout<<valide<<std::endl;

    free(A);
    free(S1);
    free(S2);
    free(Storage);
}
    return 0;
}
