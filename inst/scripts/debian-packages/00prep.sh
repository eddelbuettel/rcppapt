#!/bin/bash

## cd ~/git && docker run --rm -ti -v $(pwd):/mnt debian:unstable

apt-get update
apt-get -y dist-upgrade

apt-get -y install r-cran-rcpp r-cran-data.table libapt-pkg-dev less  # takes a moment

cd /mnt 
R CMD INSTALL rcppapt/       # assuming we're above rcppapt
