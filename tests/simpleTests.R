
library(RcppAPT)

## buildDepends
bd <- buildDepends("r-cran-rcpp$")
print(bd)
stopifnot(length(bd) > 0)               	# conservative test
stopifnot(is.element("r-base-dev", bd))		# key component

## dumpPackages
txt <- capture.output(res <- dumpPackages("littler"))
stopifnot(res)
stopifnot(length(txt) > 10)

## getPackages
pkg <- getPackages("^r-(base|doc)-")
stopifnot(inherits(pkg, "data.frame"))

## hasPackages
res <- hasPackages(c("r-base-core", "somethingThatDoesNotExist"))
stopifnot(any(res))

## reverseDepends
rd <- reverseDepends("r-cran-rcpp$")
stopifnot(inherits(rd, "data.frame"))
stopifnot(nrow(rd) > 5)                      # conservative test (ie get 11 as Nov 2016)

## showSrc
txt <- capture.output(res <- showSrc("r-cran-rcpp"))
stopifnot(res)
stopifnot(length(txt) > 10)
