## RcppAPT [![Build Status](https://travis-ci.org/eddelbuettel/rcppapt.png)](https://travis-ci.org/eddelbuettel/rcppapt) [![License](http://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl-2.0.html)

Rcpp Interface to APT Package Manager

### Background

[Debian](http://www.debian.org) and its derivatives like [Ubuntu]() utilize a
powerful package managing backend / frontend combination in APT (A Packaging
Tool).  Accessible at the command-line via front-ends `apt`, `apt-get`,
`apt-cache`, ... as well as numerous GUI variants, it is implemented using a
library `libapt-pkg`.  This small package provides
[R](http://www.r-project.org) with access to this library via
[Rcpp](http://dirk.eddelbuettel.com/code/rcpp.html). 

### Examples

We can query packages by regular expression:

```
library(RcppAPT)
getPackages("^r-base-core.")
           Package      Installed        Section
1 r-base-core-dbg 3.1.2-1utopic0 universe/math
2 r-base-core-dbg           <NA> universe/math
3     r-base-core 3.1.2-1utopic0 universe/math
4     r-base-core           <NA> universe/math
``` 

which shows two sets for every matching package, presumably because my
machine is subscribed to both the standard Ubuntu distributionton package
repositories as well as the
[CRAN Ubuntu](http://cran.rstudio.com/bin/linux/ubuntu) installation.

We can also check for installability of a given package or set of packages:

```{.r}
hasPackages(c("r-cran-rcpp", "r-cran-rcppapt"))
   r-cran-rcpp r-cran-rcppapt 
          TRUE          FALSE 
```

which shows that [Rcpp](http://dirk.eddelbuettel.com/code/rcpp.html) is (of
course) available, but this (very new) package is (unsurprisingly) not
available pre-built.

### Status

The package is still fairly small, and functionality is (currently) limited
to the examples shown above.  It builds reliably on the supported systems,
and can 

But `libapt-pkg` is pretty mature, and feature-rich, so it is mostly j

### Installation

The package is available via [drat](http://dirk.eddelbuettel.com/code/drat.html):
```{.r}
drat:::add("eddelbuettel")
install.packages("RcppAPT")
```

I may upload it to CRAN where it may get rejected as it cannot build on
multiple architectures. _On vera..._

### Author

Dirk Eddelbuettel 

### License

GPL (>= 2)

