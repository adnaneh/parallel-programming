#PBS -S /bin/bash
#PBS -N tp2_base
#PBS -e errorJob.txt
#PBS -j oe
#PBS -l walltime=0:01:00
#PBS -l select=1:ncpus=20:cpugen=skylake
#PBS -l place=excl
#PBS -m abe -M adnane.hamid@student.ecp.fr
#PBS -P progpar

module load gcc/7.3.0

cd $PBS_O_WORKDIR
exe/omp_moy.exe

/gpfs/opt/bin/fusion-whereami
date
time sleep 2
