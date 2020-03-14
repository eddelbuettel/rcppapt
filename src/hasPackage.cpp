//
//  RcppAPT -- Rcpp bindings to APT package information on Debian systems
//
//  Copyright (C) 2015 - 2020  Dirk Eddelbuettel
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

//  Simple boolean package test
//
//  cf  http://stackoverflow.com/questions/341520/how-to-use-libapt-or-libept-in-debian-like-system-to-list-packages-and-get-the
//
//  Dirk Eddelbuettel, Feb 2015

#if defined(RcppAPT_Good_System)
  #include <apt-pkg/init.h>
  #include <apt-pkg/cachefile.h>
  #include <apt-pkg/pkgcache.h>
  #if defined(APT_Version2)
    #include <apt-pkg/error.h>
    #include <apt-pkg/macros.h>
    #include <apt-pkg/pkgsystem.h>
  #endif
#endif

#include <Rcpp.h>

//' The APT Package Management system uses a data-rich caching
//' structure. This accessor function tests whether a given package
//' exists.
//' @title Test for Existence of Given Package
//' @param pkg A character vector with name of the package
//' @return A boolean result vector is returned indicating if the
//' package at the given position is available.
//' @author Dirk Eddelbuettel
//' @examples
//' hasPackages(c("r-base-core", "somethingThatDoesNotExist"))
// [[Rcpp::export]]
Rcpp::LogicalVector hasPackages(Rcpp::CharacterVector pkg) {

#if defined(RcppAPT_Good_System)

    pkgInitConfig(*_config);    	// _config, _system defined as extern and in library
    pkgInitSystem(*_config, _system);

    pkgCacheFile cacheFile;
    pkgCache* cache = cacheFile.GetPkgCache();

    Rcpp::LogicalVector res(pkg.size());
    for (int i=0; i<res.size(); i++) {
        std::string p(pkg[i]);
        pkgCache::PkgIterator package = cache->FindPkg(p);
        // if iterator not at end, a match was found
        res[i] = (package.end() != true);
    }
    res.names() = pkg;
    return res;

#else

    return Rcpp::LogicalVector::create(false);

#endif

}
