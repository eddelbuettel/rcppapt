
#include <apt-pkg/init.h>
#include <apt-pkg/cachefile.h>
#include <apt-pkg/cachefilter.h>
#include <apt-pkg/pkgcache.h>

#include <Rcpp.h>

// [[Rcpp::export]]
bool showSrc(const std::string regexp = ".") {

    pkgInitConfig(*_config);    	// _config, _system defined as extern and in library
    pkgInitSystem(*_config, _system);

    pkgCacheFile cacheFile;
    pkgCache* cache = cacheFile.GetPkgCache();

    APT::CacheFilter::PackageNameMatchesRegEx pkgre(regexp);

    pkgSourceList *List = cacheFile.GetSourceList();
    if (unlikely(List == NULL))
        return false;

    // Create the text record parsers
    pkgSrcRecords SrcRecs(*List);
    if (_error->PendingError() == true)
        return false;

    unsigned found = 0;

    for (pkgCache::PkgIterator pkg = cache->PkgBegin(); !pkg.end(); pkg++) {
        if (pkgre(pkg)) {
            const std::string pkgstr = pkg.FullName(true);

            SrcRecs.Restart();
      
            pkgSrcRecords::Parser *Parse;
            unsigned found_this = 0;
            while ((Parse = SrcRecs.Find(pkgstr.c_str(), false)) != 0) {
                Rcpp::Rcout << Parse->AsStr() << std::endl;;
                found++;
                found_this++;
            }
            if (found_this == 0) {
                //_error->Warning(_("Unable to locate package %s"),*I);
                Rcpp::stop("Unable to locate package");
                continue;
            }
        }
    }
    if (found == 0)
        //_error->Notice(_("No packages found"));
        return false;
    return true;
}


// [[Rcpp::export]]
bool dumpPackages(const std::string regexp = ".") {

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
                         << "                 File: " << D.FileList().File().FileName() << std::endl
                         << "                  MD5: " << D.md5() << std::endl;
                }
                Rcpp::Rcout << std::endl;
            }
      
            Rcpp::Rcout << std::endl;
      
            Rcpp::Rcout << "Reverse Depends: " << std::endl;
            for (pkgCache::DepIterator D = pkg.RevDependsList(); D.end() != true; ++D) {
                Rcpp::Rcout << "  " << D.ParentPkg().FullName(true) << ',' << D.TargetPkg().FullName(true);
                if (D->Version != 0)
                    Rcpp::Rcout << ' ' << DeNull(D.TargetVer()) << std::endl;
                else
                    Rcpp::Rcout << std::endl;
            }
      
            Rcpp::Rcout << "Dependencies: " << std::endl;
            for (pkgCache::VerIterator Cur = pkg.VersionList(); Cur.end() != true; ++Cur) {
                Rcpp::Rcout << Cur.VerStr() << " - ";
                for (pkgCache::DepIterator Dep = Cur.DependsList(); Dep.end() != true; ++Dep)
                    Rcpp::Rcout << Dep.TargetPkg().FullName(true) << " (" << (int)Dep->CompareOp << " " << DeNull(Dep.TargetVer()) << ") ";
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
                Rcpp::Rcout << Prv.OwnerPkg().FullName(true) << " " << Prv.OwnerVer().VerStr() << std::endl;
        }
    }
    return true;
}
