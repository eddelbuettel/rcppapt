##  RcppAPT -- Rcpp bindings to APT package information on Debian systems
##
##  Copyright (C) 2018  Dirk Eddelbuettel
##
##  This file is part of RcppAPT
##
##  RcppAPT is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 2 of the License, or
##  (at your option) any later version.
##
##  RcppAPT is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with RcppAPT.  If not, see <http:##www.gnu.org/licenses/>.

.onAttach <- function(libname, pkgname) {
    if (interactive() && !suitable())
        ## With apologies to HAL 9000 ...
        packageStartupMessage("I'm sorry, Dave. I'm afraid I can't do that.")  # #nocov
    NULL
}
