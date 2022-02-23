#! /bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
setup larsoft v09_23_01 -q prof:e20
cd /dune/app/users/angalone/larsoft_mydev
source localProducts_larsoft_v09_23_01_e20_prof/setup
cd MyCode
mrbsetenv
mrbslp



#"larsoft" "v09_23_01_01" "Linux64bit+3.10-2.17" "e19:prof" ""
