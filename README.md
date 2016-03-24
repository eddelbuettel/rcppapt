## RcppAPT [![Build Status](https://travis-ci.org/eddelbuettel/rcppapt.png)](https://travis-ci.org/eddelbuettel/rcppapt) [![License](http://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl-2.0.html) [![CRAN](http://www.r-pkg.org/badges/version/RcppAPT)](http://cran.r-project.org/package=RcppAPT) [![Downloads](http://cranlogs.r-pkg.org/badges/RcppAPT?color=brightgreen)](http://www.r-pkg.org/pkg/RcppAPT)

Rcpp Interface to APT Package Manager

### Background

[Debian](http://www.debian.org) and its derivatives like
[Ubuntu](http://www.ubuntu.com) utilize a powerful package managing backend /
frontend combination in APT (A Packaging Tool).  Accessible at the
command-line via front-ends `apt`, `apt-get`, `apt-cache`, ... as well as
numerous GUI variants, it is implemented using a library `libapt-pkg`.  This
small package provides [R](http://www.r-project.org) with access to this
library via [Rcpp](http://dirk.eddelbuettel.com/code/rcpp.html).

### Examples

We can query packages by regular expression:

```{.r}
library(RcppAPT)
getPackages("^r-base-.")
``` 

which returns a data frame with name, version (if installed) and section.

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
to the examples shown above.  It builds reliably on the supported systems.

But `libapt-pkg` is pretty mature, and feature-rich, so this package acts
mostly as a wrapper from R.

### Installation

The package is on [CRAN](http://cran.r-project.org) so a very standard

```{.r}
install.packages("RcppAPT")
```

will do. Make sure you install the
[libapt-pkg-dev](https://packages.debian.org/sid/libapt-pkg-dev) 
package first as it is a build-dependency.

Versions of the package may also be available via
[drat](http://dirk.eddelbuettel.com/code/drat.html) via:

```{.r}
drat:::add("eddelbuettel")
install.packages("RcppAPT")
```


### Author

Dirk Eddelbuettel 

### License

GPL (>= 2)

