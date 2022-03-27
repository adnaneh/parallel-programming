#include <iostream>
#include <chrono>


#include "structural_elements.hpp"
#include "container.hpp"
#include "global_parameters.hpp"
#include "ios.hpp"
#include "omp.h"

#define DEBUG 1

void sobel(u_char **Source, u_char **Resultat, unsigned int height, unsigned int width, unsigned int nthreads) {

    for (auto i = 1; i < height-1; i++) {
        for (auto j = 1; j < width-1; j++) {
            if ((i==0)||(i==height-1)||(j==0)||(j==width-1)) {Resultat[i][j]=0;}
            else {
                Resultat[i][j]  = std::abs(Source[i-1][j-1] + Source[i-1][j] + Source[i-1][j+1] - (Source[i+1][j-1] + Source[i+1][j] + Source[i+1][j+1]));
                Resultat[i][j] += std::abs(Source[i-1][j-1] + Source[i][j-1] + Source[i+1][j-1] - (Source[i-1][j+1] + Source[i][j+1] + Source[i+1][j+1]));
            }
        }
    }
}

void sobelParallel(u_char **Source, u_char **Resultat, unsigned int height, unsigned int width, unsigned int nthreads) {

    /*for (unsigned long l = 0; l < (height-2)*(width-2); l++) {
 *             unsigned int i = l/(width-2) + 1;
 *                         unsigned int j = l%(width-2) + 1;
 *                                     if ((i==0)||(i==height-1)||(j==0)||(j==width-1)) {Resultat[i][j]=0;}
 *                                                 else {
 *                                                                 Resultat[i][j]  = std::abs(Source[i-1][j-1] + Source[i-1][j] + Source[i-1][j+1] - (Source[i+1][j-1] + Source[i+1][j] + Source[i+1][j+1]));
 *                                                                                 Resultat[i][j] += std::abs(Source[i-1][j-1] + Source[i][j-1] + Source[i+1][j-1] - (Source[i-1][j+1] + Source[i][j+1] + Source[i+1][j+1]));
 *                                                                                             }
 *                                                                                                 }*/
    #pragma omp parallel
    {
        #pragma omp for
        for (auto i = 1; i < height-1; i++) {
            for (auto j = 1; j < width-1; j++) {
                if ((i==0)||(i==height-1)||(j==0)||(j==width-1)) {Resultat[i][j]=0;}
                else {
                    Resultat[i][j]  = std::abs(Source[i-1][j-1] + Source[i-1][j] + Source[i-1][j+1] - (Source[i+1][j-1] + Source[i+1][j] + Source[i+1][j+1]));
                    Resultat[i][j] += std::abs(Source[i-1][j-1] + Source[i][j-1] + Source[i+1][j-1] - (Source[i-1][j+1] + Source[i][j+1] + Source[i+1][j+1]));
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    unsigned int height, width;
    unsigned int nthreads = omp_get_max_threads();

    std::string image_filename(argv[1]);
    int ITER =  atoi(argv[2]);

    get_source_params(image_filename, &height, &width);
    std::cout << width << " " << height << std::endl;
    u_char **Source, **Resultat, **ResultatBis;

	image<u_char> imgSource(height, width, &Source);
    image<u_char> imgResultat(height, width, &Resultat);
    image<u_char> imgResultatBis(height, width, &ResultatBis);
       
    auto fail = init_source_image(Source, image_filename, height, width);
    if (fail) {
        std::cout << "Chargement impossible de l'image" << std::endl;
        return 0;
    }
 
    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
    for (auto it =0; it < ITER; it++) {
        sobel(Source, Resultat, height, width, nthreads);
    }
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(t1-t0).count()/ITER;



    std::chrono::high_resolution_clock::time_point t0b = std::chrono::high_resolution_clock::now();
    for (auto it =0; it < ITER; it++) {
        sobelParallel(Source, ResultatBis, height, width, nthreads);
    }
    std::chrono::high_resolution_clock::time_point t1b = std::chrono::high_resolution_clock::now();
    auto durationb = std::chrono::duration<double>(t1b-t0b).count()/ITER;

    std::cout << duration<<" "<<durationb << std::endl;
    bool egal = true;
    for(int i = 0; i<height; i++){
        for(int j = 0; j<width; j++){
            if(Resultat[i][j] != ResultatBis[i][j]){
                egal = false;
            }
        }
    }

    if(egal == false){
        std::cout<<"Not the same result"<<std::endl;
    }
    else{
        std::cout<<"Same result"<<std::endl;
    }

    #ifdef DEBUG
        image_filename=std::string("Sobel.pgm");
        save_gray_level_image(&imgResultat, image_filename, height, width);
    #endif
    
    return 0;
}

