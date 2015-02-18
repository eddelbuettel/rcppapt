// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4;  -*-
//
// cf  http://stackoverflow.com/questions/341520/how-to-use-libapt-or-libept-in-debian-like-system-to-list-packages-and-get-the

#include <apt-pkg/cachefile.h>
#include <apt-pkg/pkgcache.h>

#include <Rcpp.h>

// [[Rcpp::export]]
std::vector<std::string> getPackages() {

    pkgInitConfig(*_config);    	// _config and _system are defined as extern and in the library
    pkgInitSystem(*_config, _system);

    pkgCacheFile cache_file;
    pkgCache* cache = cache_file.GetPkgCache();
    
    std::vector<std::string> res;
    for (pkgCache::PkgIterator package = cache->PkgBegin(); !package.end(); package++) {
        res.push_back(std::string(package.Name()));
    }

    return res;
}
