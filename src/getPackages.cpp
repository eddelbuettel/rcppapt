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

//  Simple package list accessor
//
//  cf  http://stackoverflow.com/questions/341520/how-to-use-libapt-or-libept-in-debian-like-system-to-list-packages-and-get-the
//
//  Dirk Eddelbuettel, Feb 2015

#if defined(RcppAPT_Good_System)
  #include <apt-pkg/init.h>
  #include <apt-pkg/cachefile.h>
  #include <apt-pkg/cachefilter.h>
  #include <apt-pkg/pkgcache.h>
  #if defined(APT_Version2)
    #include <apt-pkg/pkgsystem.h>
  #endif
#endif

#include <Rcpp.h>

//' The APT Package Management system uses a data-rich caching
//' structure. This accessor function returns the names of installable
//' packages for a given regular expression.
//'
//' Note that the package lookup uses regular expressions. If only a
//' single package is desired, append a single \code{$} to terminate
//' the expression.  \emph{Ie} \code{r-cran-rcpp$} will \emph{not}
//' return results for \code{r-cran-rcpparmadillo} and
//' \code{r-cran-rcppeigen}.
//'
//' @title Retrieve Names of All Installable Packages
//' @param regexp A regular expression for the package name(s) with a
//' default of all (".").
//' @return A data frame with columns containing the
//' package name and version (or NA if unavailable).
//' @author Dirk Eddelbuettel
//' @examples
//' getPackages("^r-(base|doc)-")
// [[Rcpp::export]]
Rcpp::DataFrame getPackages(const std::string regexp = ".") {

#if defined(RcppAPT_Good_System)

    pkgInitConfig(*_config);    	// _config, _system defined as extern and in library
    pkgInitSystem(*_config, _system);

    pkgCacheFile cacheFile;
    pkgCache* cache = cacheFile.GetPkgCache();
    pkgDepCache dcache(cache);

    APT::CacheFilter::PackageNameMatchesRegEx pkgre(regexp);

    std::vector<std::string> name, ver; //, cand, sec;
    // first pass uses STL vectors and grows them
    for (pkgCache::PkgIterator package = cache->PkgBegin(); !package.end(); package++) {
        // if we match the regular expression, collect data
        if (pkgre(package)) {
            std::string pkgname(package.Name());
            if (package.FullName(true) == pkgname) { 	// we do not want the foo:i386 variant
                name.push_back(pkgname);
                std::string version = "NA";
                //Rcpp::Rcout << package.Name() << "--" << package.FullName(true) << std::endl;
                for (pkgCache::VerIterator cur = package.VersionList(); cur.end() != true; ++cur) {
                    version = cur.VerStr();
                }
                ver.push_back(version);
            }
        }
    }
    // second pass to set proper NA values for R
    Rcpp::CharacterVector N(ver.size()), V(ver.size());
    for (int i=0; i<V.size(); i++) {
        N[i] = name[i];
        V[i] = ver[i];
        if (ver[i] == "NA") V[i] = NA_STRING;
    }
    return Rcpp::DataFrame::create(Rcpp::Named("Package")          = N,
                                   Rcpp::Named("Version")          = V,
                                   Rcpp::Named("stringsAsFactors") = false);

#else

    std::vector<std::string> vs{""};
    return Rcpp::DataFrame::create(Rcpp::Named("Package")          = vs,
                                   Rcpp::Named("Version")          = vs,
                                   Rcpp::Named("stringsAsFactors") = false);

#endif

}
