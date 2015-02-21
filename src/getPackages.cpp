// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4;  -*-
//
//  rapt -- Rcpp bindings to APT package information on Debian systems
//
//  Copyright (C) 2015           Dirk Eddelbuettel 
//
//  This file is part of rapt
//
//  rapt is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  rapt is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with rapt.  If not, see <http://www.gnu.org/licenses/>.

//  Simple package list accessor
//
//  cf  http://stackoverflow.com/questions/341520/how-to-use-libapt-or-libept-in-debian-like-system-to-list-packages-and-get-the
//
//  Dirk Eddelbuettel, Feb 2015

#include <apt-pkg/cachefile.h>
#include <apt-pkg/cachefilter.h>
#include <apt-pkg/pkgcache.h>

#include <Rcpp.h>

//' The APT Package Management system uses a data-rich caching 
//' structure. This accessor function returns the names of installable
//' packages for a given regular expression.
//' @title Retrieve Names of All Installable Packages
//' @param Regular expression for package name
//' @return A data frame with columns containing the 
//' package name, the installed version (or NA if not installed)  
//' and the section it is installed in (or NA).
//' @author Dirk Eddelbuettel
// [[Rcpp::export]]
Rcpp::DataFrame getPackages(const std::string re = ".") {

    pkgInitConfig(*_config);    	// _config, _system defined as extern and in library
    pkgInitSystem(*_config, _system);

    pkgCacheFile cacheFile;
    pkgCache* cache = cacheFile.GetPkgCache();
 
    APT::CacheFilter::PackageNameMatchesRegEx pkgre(re);

    std::vector<std::string> name, ver, sec;
    // first pass uses STL vectors and grows them
    for (pkgCache::PkgIterator package = cache->PkgBegin(); !package.end(); package++) {
        // if we match the regular expression, collect data
        if (pkgre(package)) {   
            name.push_back(std::string(package.Name()));
            const char *version = package.CurVersion();
            ver.push_back(version == NULL ? "NA" : version);
            const char *section = package.Section();
            sec.push_back(section == NULL ? "NA" : section);
        }
    }
    // second pass to set proper NA values for R
    Rcpp::CharacterVector V(ver.size()), S(sec.size());
    for (int i=0; i<V.size(); i++) {
        V[i] = ver[i];
        if (ver[i] == "NA") V[i] = NA_STRING; 
        S[i] = sec[i];
        if (sec[i] == "NA") S[i] = NA_STRING;
    }
    
    return Rcpp::DataFrame::create(Rcpp::Named("Package")      = name,
                                   Rcpp::Named("Installed")    = V,
                                   Rcpp::Named("Section")      = S);
}
