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
    #include <apt-pkg/error.h>
    #include <apt-pkg/macros.h>
    #include <apt-pkg/pkgsystem.h>
  #endif
#endif

#include <Rcpp.h>

//' The APT Package Management system uses a data-rich caching
//' structure. This accessor function returns the Build-Depends for
//' a set of packages matching the given regular expression.
//'
//' Note that the package lookup uses regular expressions. If only a
//' single package is desired, append a single \code{$} to terminate
//' the expression.  \emph{Ie} \code{r-cran-rcpp$} will \emph{not}
//' return results for \code{r-cran-rcpparmadillo} and
//' \code{r-cran-rcppeigen}.
//'
//' @title Return Build-Depends for given packages
//' @param regexp A regular expression for the package name(s) with a
//' default of all (".")
//' @return A character vector containing package names is returned.
//' @author Dirk Eddelbuettel
//' @examples
//' buildDepends("r-cran-rcpp$")
// [[Rcpp::export]]
std::vector<std::string> buildDepends(const std::string regexp = ".") {

#if defined(RcppAPT_Good_System)

    pkgInitConfig(*_config);    	// _config, _system defined as extern and in library
    pkgInitSystem(*_config, _system);

    pkgCacheFile cacheFile;
    pkgCache* cache = cacheFile.GetPkgCache();

    APT::CacheFilter::PackageNameMatchesRegEx pkgre(regexp);

    std::vector<std::string> res;

    pkgSourceList *List = cacheFile.GetSourceList();
    if (unlikely(List == NULL))
        return res;             // #nocov

    // Create the text record parsers
    pkgSrcRecords SrcRecs(*List);
    if (_error->PendingError() == true)
        return res;		// #nocov

    for (pkgCache::PkgIterator pkg = cache->PkgBegin(); !pkg.end(); pkg++) {
        if (pkgre(pkg)) {
            const std::string pkgstr = pkg.FullName(true);
            SrcRecs.Restart();

            debSrcRecordParser::Parser *Parse;
            while ((Parse = SrcRecs.Find(pkgstr.c_str(), false)) != 0) {
                std::vector<pkgSrcRecords::Parser::BuildDepRec> bdvec;
                Parse->BuildDepends(bdvec, false);

                for (unsigned int ctr=0; ctr < bdvec.size(); ctr++) {
                    res.push_back(bdvec[ctr].Package);
                }
            }
        }
    }
    return res;

#else

    std::vector<std::string> vs{""};
    return vs;

#endif

}


//' The APT Package Management system uses a data-rich caching
//' structure. This accessor function displays the information for
//' a set of packages matching the given regular expression. The
//' output corresponds to \code{apt-cache showsrc pkgname}.
//'
//' Note that the package lookup uses regular expressions. If only a
//' single package is desired, append a single \code{$} to terminate
//' the expression.  \emph{Ie} \code{r-cran-rcpp$} will \emph{not}
//' return results for \code{r-cran-rcpparmadillo} and
//' \code{r-cran-rcppeigen}.
//'
//' @title Display information for given packages
//' @param regexp A regular expression for the package name(s) with a
//' default of all (".")
//' @return A boolean is returned indicating whether or not the given
//' regular expression could be matched to source packages -- but the
//' function is invoked ' for the side effect of displaying information.
//' @author Dirk Eddelbuettel
//' @examples
//' showSrc("^r-(base|doc)-")
//' showSrc("r-cran-rcpp")  # also finds RcppEigen and RcppArmadillo
//' showSrc("r-cran-rcpp$") # just Rcpp
// [[Rcpp::export]]
bool showSrc(const std::string regexp = ".") {

#if defined(RcppAPT_Good_System)

    pkgInitConfig(*_config);    	// _config, _system defined as extern and in library
    pkgInitSystem(*_config, _system);

    pkgCacheFile cacheFile;
    pkgCache* cache = cacheFile.GetPkgCache();

    APT::CacheFilter::PackageNameMatchesRegEx pkgre(regexp);

    pkgSourceList *List = cacheFile.GetSourceList();
    if (unlikely(List == NULL)) {		// #nocov  start
        Rcpp::Rcout << "Error: No cached sources list. Maybe you have src-deb entries?\n";
        return false;
    } 									// #nocov end

    // Create the text record parsers
    pkgSrcRecords SrcRecs(*List);
    if (_error->PendingError() == true)	{	// #nocov  start
        Rcpp::Rcout << "Error: No sources records. Maybe you have src-deb entries?\n";
        return false;
    }										// #nocov end

    unsigned found = 0;

    for (pkgCache::PkgIterator pkg = cache->PkgBegin(); !pkg.end(); pkg++) {
        if (pkgre(pkg)) {
            const std::string pkgstr = pkg.FullName(true);
            Rcpp::Rcout << "--" << pkgstr << std::endl;
            SrcRecs.Restart();

            pkgSrcRecords::Parser *Parse;
            unsigned found_this = 0;
            while ((Parse = SrcRecs.Find(pkgstr.c_str(), false)) != 0) {
                Rcpp::Rcout << Parse->AsStr() << std::endl;;
                found++;
                found_this++;
                //Rcpp::Rcout << "(" << found << "," << found_this << ")" << std::endl;
            }
            //if (found_this == 0) {
            //     //_error->Warning(_("Unable to locate package %s"),*I);
            //     Rcpp::stop("Unable to locate package");
            //     continue;
            //}
        }
    }
    if (found == 0) {
        //_error->Notice(_("No packages found"));
        return false;		// #nocov
    }
    return true;

#else

    return false;

#endif

}

// The DeNull function is in the current source version of libapt-pkg-dev but
// not all older ones so we create a variant here
inline const char *localDeNull(const char *s) {return (s == 0?"(null)":s);}

//' The APT Package Management system uses a data-rich caching
//' structure. This accessor function displays the information for
//' a set of packages matching the given regular expression. It
//' corresponds somewhat to \code{apt-cache showpkg pkgname} but
//' displays more information.
//'
//' Note that the package lookup uses regular expressions. If only a
//' single package is desired, append a single \code{$} to terminate
//' the expression.  \emph{Ie} \code{r-cran-rcpp$} will \emph{not}
//' return results for \code{r-cran-rcpparmadillo} and
//' \code{r-cran-rcppeigen}.
//'
//' @title Display information for given packages
//' @param regexp A regular expression for the package name(s) with a
//' default of all (".")
//' @return A boolean is returned indicating whether or not the given
//' regular expression could be matched to source packages -- but the
//' function is invoked ' for the side effect of displaying information.
//' @author Dirk Eddelbuettel
//' @examples
//' dumpPackages("^r-(base|doc)-")
// [[Rcpp::export]]
bool dumpPackages(const std::string regexp = ".") {

#if defined(RcppAPT_Good_System)

    pkgInitConfig(*_config);    	// _config, _system defined as extern and in library
    pkgInitSystem(*_config, _system);

    pkgCacheFile cacheFile;
    pkgCache* cache = cacheFile.GetPkgCache();

    APT::CacheFilter::PackageNameMatchesRegEx pkgre(regexp);

    for (pkgCache::PkgIterator pkg = cache->PkgBegin(); !pkg.end(); pkg++) {
        // if we match the regular expression, collect data
        if (pkgre(pkg)) {
            Rcpp::Rcout << "Package: " << pkg.FullName(true) << std::endl;
            Rcpp::Rcout << "Versions: " << std::endl;
            for (pkgCache::VerIterator Cur = pkg.VersionList(); Cur.end() != true; ++Cur) {
                Rcpp::Rcout << Cur.VerStr();
                for (pkgCache::VerFileIterator Vf = Cur.FileList(); Vf.end() == false; ++Vf)
                    Rcpp::Rcout << " (" << Vf.File().FileName() << ")";
                Rcpp::Rcout << std::endl;
                for (pkgCache::DescIterator D = Cur.DescriptionList(); D.end() == false; ++D) {
                    Rcpp::Rcout << " Description Language: " << D.LanguageCode() << std::endl
                                << "                 File: " << D.FileList().File().FileName()
                                << std::endl
                                << "                  MD5: " << D.md5() << std::endl;
                }
                Rcpp::Rcout << std::endl;
            }

            Rcpp::Rcout << std::endl;

            Rcpp::Rcout << "Reverse Depends: " << std::endl;
            for (pkgCache::DepIterator D = pkg.RevDependsList(); D.end() != true; ++D) {
                Rcpp::Rcout << "  " << D.ParentPkg().FullName(true)
                            << ',' << D.TargetPkg().FullName(true);
                if (D->Version != 0)
                    Rcpp::Rcout << ' ' << localDeNull(D.TargetVer()) << std::endl;
                else
                    Rcpp::Rcout << std::endl;
            }

            Rcpp::Rcout << "Dependencies: " << std::endl;
            for (pkgCache::VerIterator Cur = pkg.VersionList(); Cur.end() != true; ++Cur) {
                Rcpp::Rcout << Cur.VerStr() << " - ";
                for (pkgCache::DepIterator Dep = Cur.DependsList(); Dep.end() != true; ++Dep)
                    Rcpp::Rcout << Dep.TargetPkg().FullName(true)
                                << " (" << (int)Dep->CompareOp << " "
                                << localDeNull(Dep.TargetVer()) << ") ";
                Rcpp::Rcout << std::endl;
            }

            Rcpp::Rcout << "Provides: " << std::endl;
            for (pkgCache::VerIterator Cur = pkg.VersionList(); Cur.end() != true; ++Cur) {
                Rcpp::Rcout << Cur.VerStr() << " - ";
                for (pkgCache::PrvIterator Prv = Cur.ProvidesList(); Prv.end() != true; ++Prv)
                    Rcpp::Rcout << Prv.ParentPkg().FullName(true) << " ";
                Rcpp::Rcout << std::endl;
            }
            Rcpp::Rcout << "Reverse Provides: " << std::endl;
            for (pkgCache::PrvIterator Prv = pkg.ProvidesList(); Prv.end() != true; ++Prv)
                Rcpp::Rcout << Prv.OwnerPkg().FullName(true) << " "
                            << Prv.OwnerVer().VerStr() << std::endl;
        }
    }
    return true;

#else

    return false;

#endif

}
