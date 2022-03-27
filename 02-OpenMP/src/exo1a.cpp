#include <iostream>
#include <omp.h>

int main() {
#pragma omp parallel
{
// on récupère le numéro du thread
int i = omp_get_thread_num();
std::cout << "Thread #" << i << " says hello ! \n";
}
	return 0;
}