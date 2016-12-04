// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4;  -*-
//
//  RcppAPT -- Rcpp bindings to APT package information on Debian systems
//
//  Copyright (C) 2015 - 2016  Dirk Eddelbuettel
//
//  This file is part of RcppAPT
//
//  RcppAPT is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  RcppAPT is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with RcppAPT.  If not, see <http://www.gnu.org/licenses/>.

#include <apt-pkg/init.h>
#include <apt-pkg/cachefile.h>
#include <apt-pkg/cachefilter.h>
#include <apt-pkg/pkgcache.h>
#include <apt-pkg/debsrcrecords.h>

#include <Rcpp.h>

// The DeNull function is in the current source version of libapt-pkg-dev but
// not all older ones so we create a variant here
inline const char *localDeNull(const char *s) {return (s == 0?"(null)":s);}

//' The APT Package Management system uses a data-rich caching
//' structure. This accessor function returns the Reverse-Depends for
//' a set of packages matching the given regular expression. 
//'
//' Note that the package lookup uses regular expressions. If only a
//' single package is desired, append a single \code{$} to terminate
//' the expression.  \emph{Ie} \code{r-cran-rcpp$} will \emph{not}
//' return results for \code{r-cran-rcpparmadillo} and
//' \code{r-cran-rcppeigen}.
//'
//' @title Return Reverse-Depends for given packages
//' @param regexp A regular expression for the package name(s) with a
//' default of all (".")
//' @return A data frame with two column listing packages and, where
//' available, minimal version. 
//' @author Dirk Eddelbuettel
//' @examples
//' reverseDepends("r-cran-rcpp$")
// [[Rcpp::export]]
Rcpp::DataFrame reverseDepends(const std::string regexp = ".") {

    pkgInitConfig(*_config);    	// _config, _system defined as extern and in library
    pkgInitSystem(*_config, _system);

    pkgCacheFile cacheFile;
    pkgCache* cache = cacheFile.GetPkgCache();

    APT::CacheFilter::PackageNameMatchesRegEx pkgre(regexp);

    std::vector<std::string> res;
    std::vector<std::string> ver;

    for (pkgCache::PkgIterator pkg = cache->PkgBegin(); !pkg.end(); pkg++) {
        if (pkgre(pkg)) {
            const std::string pkgstr(pkg.Name());
            for (pkgCache::DepIterator depit = pkg.RevDependsList(); depit.end() != true; ++depit) {
                std::string candidate = depit.TargetPkg().FullName(true);
                std::string parent = depit.ParentPkg().FullName(true);
                if (candidate == pkgstr && parent.find(pkgstr + ":") == std::string::npos) {
                    res.push_back(parent);
                    if (depit->Version != 0) {
                        ver.push_back(localDeNull(depit.TargetVer()));
                    } else {
                        ver.push_back("");
                    }
                }
            }


        }
    }
    return Rcpp::DataFrame::create(Rcpp::Named("package") = res,
                                   Rcpp::Named("version") = ver);
}
