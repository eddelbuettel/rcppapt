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

Moreover, we can look at the package information of a given package:

```{.r}
R> showSrc("r-cran-rcppeigen")                                                                                                                                                                                      
Package: r-cran-rcppeigen
Binary: r-cran-rcppeigen
Version: 0.3.2.5.0-1
Priority: optional
Section: universe/misc
Maintainer: Dirk Eddelbuettel <edd@debian.org>
Build-Depends: debhelper (>= 7.0.0), r-base-dev (>= 3.2.1), cdbs, r-cran-rcpp (>= 0.11.0), r-cran-matrix (>= 1.1-0), r-cran-pkgkitten
Architecture: any
Standards-Version: 3.9.6
Format: 1.0
Directory: pool/universe/r/r-cran-rcppeigen
Files:
 21fab9cddd6eba9b142d75979e9f9cff 1821 r-cran-rcppeigen_0.3.2.5.0-1.dsc
 bfe590cecab6443ab1ed8e9f7ed3e872 1235077 r-cran-rcppeigen_0.3.2.5.0.orig.tar.gz
 aa4d0bef9eb0b5c435de3a423f25c86b 10044 r-cran-rcppeigen_0.3.2.5.0-1.diff.gz
Package-List:
 r-cran-rcppeigen deb gnu-r optional arch=any
Checksums-Sha1:
 21cbca890a783b6f411e1372853b566f8de3c5d7 1821 r-cran-rcppeigen_0.3.2.5.0-1.dsc
 8dcd6af5fab034c779c1fcf14ccb6474cfa85c38 1235077 r-cran-rcppeigen_0.3.2.5.0.orig.tar.gz
 16a9761d7f03493797d1c7bc11d1867e59b5cfe0 10044 r-cran-rcppeigen_0.3.2.5.0-1.diff.gz
Checksums-Sha256:
 cb8b53b7d98875d9e64bb3d40a874baa1dbb72cde524195ae91573d8616285fe 1821 r-cran-rcppeigen_0.3.2.5.0-1.dsc
 29ada6457e96d9ec03a7102bdf0f24fe4a186a0a5284c2e37579f3f6e9838f98 1235077 r-cran-rcppeigen_0.3.2.5.0.orig.tar.gz
 ca4fadb4b3cac9c5c1860ca4b54e783b18a78cfbec3efae71255d2678c48afd7 10044 r-cran-rcppeigen_0.3.2.5.0-1.diff.gz

[1] TRUE
R> 
```

The `buildDepends()` function extracts just the build dependencies:

```{.r}
R> buildDepends("r-cran-rcppeigen")
[1] "debhelper"        "r-base-dev"       "cdbs"            
[4] "r-cran-rcpp"      "r-cran-matrix"    "r-cran-pkgkitten"
R> 
```

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

