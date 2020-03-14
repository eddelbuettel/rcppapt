#!/bin/bash

complainAndExit() {
    echo $1
    sed -e 's/@compilearg@/-DRcppAPT_Wrong_System/' \
        -e "s/@aptversionarg@//" \
        -e 's/@linkarg@//' \
        src/Makevars.in > src/Makevars
    exit 0
}

[ ! -f /usr/bin/dpkg ] && \
    complainAndExit "No dpkg binary. Is this a Debian-based system?"

[ ! -d /etc/apt ] && \
    complainAndExit "No /etc/apt directory. Is this a Debian-based system?"

[ ! -d /usr/include/apt-pkg ] && \
    complainAndExit "No /usr/include/apt-pkg/ directory. Is libapt-pkg-dev installed?"

[ ! -f /usr/bin/pkg-config ] && \
    complainAndExit "No pkg-config binary. Please install it."

## Ensure R_HOME is known and derive it from R if need be
: ${R_HOME:=$(R RHOME)}

# Use R to check if what pkg-config returns as the version of libapt-pkg-dev is at
# least version 2.0, and if so, return a snippet. We use this to define a compiler
# define so that the added include files needed for 2.0.0 or later will be included
aptver=$("${R_HOME}/bin/Rscript" -e 'cat(ifelse(as.package_version(readLines(pipe("pkg-config --modversion apt-pkg"))) >= as.package_version("1.9.0"), "-DAPT_Version2 -DAPT_COMPILING_APT", ""))')

sed -e 's/@compilearg@/-DRcppAPT_Good_System/' \
    -e "s/@aptversionarg@/${aptver}/" \
    -e 's/@linkarg@/-lapt-pkg/' \
    src/Makevars.in > src/Makevars

exit 0
