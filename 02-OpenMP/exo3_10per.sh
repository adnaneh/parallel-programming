#PBS -S /bin/bash
#PBS -N tp2_ex3_10per_sortie_de_cache
#PBS -e errorJob.txt
#PBS -j oe
#PBS -l walltime=0:00:30
#PBS -l select=1:ncpus=20:cpugen=skylake
#PBS -l place=excl
#PBS -m abe -M adnane.hamid@student.ecp.fr
#PBS -P progpar


# Load the same modules as for compilation
module load gcc/7.3.0
# module load intel-compilers/2019.3
# Go to the current directory
cd $PBS_O_WORKDIR


echo "Avec vectorisation"
for I in {32..4096..16}
do
(( ITER=10))
exe/omp_scalaire_10per.exe $ITER $I
done

/gpfs/opt/bin/fusion-whereami
date
time sleep 2
