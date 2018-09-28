
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
