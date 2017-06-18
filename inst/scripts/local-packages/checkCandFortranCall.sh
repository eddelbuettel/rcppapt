#!/bin/bash


newfunc() {
    ## source directories are all named foo-1.2.3
    dirs=$(find . -maxdepth 1 -type d -name \*-\* | sort)

    for d in ${dirs}; do
        if test -d ${d}/src; then
            if grep -q -r \\.Fortran\( ${d}/R; then
                fortran=TRUE
            else 
                fortran=FALSE
            fi
            if grep -q -r \\.C\( ${d}/R; then
                ccall=TRUE
            else
                ccall=FALSE
            fi
            if grep -q "Priority: recommended" ${d}/DESCRIPTION; then
                recommended=TRUE
            else
                recommended=FALSE
            fi
            if [[ "${fortran}" == "TRUE" || "${ccall}" == "TRUE" ]]; then
                pretty=$(basename ${d})
                echo "${pretty} ${fortran} ${ccall} ${recommended}"
            fi
        fi
    done
}

oldfunc() {
    ## source directories are all named foo-1.2.3
    dirs=$(find . -maxdepth 1 -type d -name \*-\* | sort)

    for d in ${dirs}; do
        if test -d ${d}/src; then
            if grep -q -r \\.Fortran\( ${d}/R; then
                echo ".Fortran in ${d}"
            fi
            if grep -q -r \\.C\( ${d}/R; then
                echo ".C       in ${d}"
            fi
        fi
    done
}

#oldfunc
newfunc
