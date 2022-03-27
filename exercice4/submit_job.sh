#PBS -S /bin/bash
#PBS -N tp2_ex4
#PBS -e errorJob.txt
#PBS -j oe
#PBS -l walltime=0:01:00
#PBS -l select=1:ncpus=20:cpugen=skylake
#PBS -l place=excl
#PBS -m abe -M adnane.hamid@student.ecp.fr
#PBS -P progpar


# Load the same modules as for compilation
module load gcc/7.3.0
# module load intel-compilers/2019.3
# Go to the current directory
cd $PBS_O_WORKDIR

echo "Small drone"
exe/Sobel.exe images/Drone.pgm 500

echo "Big drone"
exe/Sobel.exe images/Drone_huge.pgm 500

/gpfs/opt/bin/fusion-whereami
date
time sleep 2
