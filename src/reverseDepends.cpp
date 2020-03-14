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

#if defined(RcppAPT_Good_System)
  #include <apt-pkg/init.h>
  #include <apt-pkg/cachefile.h>
  #include <apt-pkg/cachefilter.h>
  #include <apt-pkg/pkgcache.h>
  #include <apt-pkg/debsrcrecords.h>
  #if defined(APT_Version2)
    #include <apt-pkg/pkgsystem.h>
  #endif
#endif

#include <Rcpp.h>

// The DeNull function is in the current source version of libapt-pkg-dev but
// not all older ones so we create a variant here
inline const char *localDeNull(const char *s) {return (s == 0?"(null)":s);}  // #nocov

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

#if defined(RcppAPT_Good_System)

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
    return Rcpp::DataFrame::create(Rcpp::Named("package")          = res,
                                   Rcpp::Named("version")          = ver,
                                   Rcpp::Named("stringsAsFactors") = false);

#else

    std::vector<std::string> vs{""};
    return Rcpp::DataFrame::create(Rcpp::Named("package")          = vs,
                                   Rcpp::Named("version")          = vs,
                                   Rcpp::Named("stringsAsFactors") = false);

#endif

}

//' The APT Package Management system uses a data-rich caching
//' structure. This accessor function returns the Depends for
//' a set of packages matching the given regular expression.
//'
//' Note that the package lookup uses regular expressions. If only a
//' single package is desired, append a single \code{$} to terminate
//' the expression.  \emph{Ie} \code{r-cran-rcpp$} will \emph{not}
//' return results for \code{r-cran-rcpparmadillo} and
//' \code{r-cran-rcppeigen}.
//'
//' @title Return Depends for given packages
//' @param regexp A regular expression for the package name(s) with a
//' default of all (".")
//' @return A data frame with four columns listing (source) package, dependend
//' packages, comparison operator, and, where available, minimal version.
//' @author Dirk Eddelbuettel
//' @examples
//' reverseDepends("r-cran-rcpp$")
// [[Rcpp::export]]
Rcpp::DataFrame getDepends(const std::string regexp = ".") {       // #nocov start

#if defined(RcppAPT_Good_System)

    pkgInitConfig(*_config);    	// _config, _system defined as extern and in library
    pkgInitSystem(*_config, _system);

    pkgCacheFile cacheFile;
    pkgCache* cache = cacheFile.GetPkgCache();

    APT::CacheFilter::PackageNameMatchesRegEx pkgre(regexp);

    std::vector<std::string> srcpkg;
    std::vector<std::string> res;
    std::vector<std::string> ver;
    std::vector<int> op;

    for (pkgCache::PkgIterator pkg = cache->PkgBegin(); !pkg.end(); pkg++) {
        if (pkgre(pkg)) {
            const std::string pkgstr(pkg.Name());
            if (pkg.FullName(true) == pkgstr) { 	// we do not want the foo:i386 variant
                for (pkgCache::VerIterator Cur = pkg.VersionList(); Cur.end() != true; ++Cur) {
                    //Rcpp::Rcout << Cur.VerStr() << " -\n";
                    for (pkgCache::DepIterator Dep = Cur.DependsList(); Dep.end() != true; ++Dep) {
                        std::string txt = Dep.TargetPkg().FullName(true);
                        if (txt.find(pkgstr + ":") == std::string::npos) {
                            // Rcpp::Rcout << "\t" << Dep.TargetPkg().FullName(true)
                            //             << " (" << (int)Dep->CompareOp << " "
                            //             << localDeNull(Dep.TargetVer()) << ")\n";
                            srcpkg.push_back(pkgstr);
                            res.push_back(Dep.TargetPkg().FullName(true));
                            ver.push_back(localDeNull(Dep.TargetVer()));
                            op.push_back((int)Dep->CompareOp);
                        }
                    }
                    // Rcpp::Rcout << std::endl;
                }
            }
        }
    }
    Rcpp::DataFrame df = Rcpp::DataFrame::create(Rcpp::Named("srcpkg")           = srcpkg,
                                                 Rcpp::Named("deppkg")           = res,
                                                 Rcpp::Named("cmpop")            = op,
                                                 Rcpp::Named("version")          = ver,
                                                 Rcpp::Named("stringsAsFactors") = false);
    return df;

#else

    std::vector<std::string> vs{""};
    return Rcpp::DataFrame::create(Rcpp::Named("srcpkg")           = vs,
                                   Rcpp::Named("deppkg")           = vs,
                                   Rcpp::Named("cmpop")            = vs,
                                   Rcpp::Named("version")          = vs,
                                   Rcpp::Named("stringsAsFactors") = false);

#endif

} // #nocov end
