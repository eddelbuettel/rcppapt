//
//  RcppAPT -- Rcpp bindings to APT package information on Debian systems
//
//  Copyright (C) 2018  Dirk Eddelbuettel
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


#include <Rcpp.h>

//' The APT Package Management system uses a data-rich caching
//' structure. This accessor function tests whether a given package
//' exists.
//'
//' CRAN does not manage to blacklist this package for builds where it 
//' has little to no chance of building (macOS amongt them). So we now
//' build everywhere whether it makes sense or not.
//'
//' @title Test for Suitability of System
//' @return A boolean result vector is returned indicating if the 
//' system is making any sense at all.
//' @author Dirk Eddelbuettel
//' @examples
//' suitable()
// [[Rcpp::export]]
bool suitable() {

#if defined(RcppAPT_Good_System)
  return true;
#endif

#if defined(RcppAPT_Wrong_System)
  return false;
#endif

  Rcpp::warning("No system defined.Suspicious setup.\n");
  return false;
}
