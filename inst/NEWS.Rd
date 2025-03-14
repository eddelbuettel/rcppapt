\name{NEWS}
\title{News for Package \pkg{RcppAPT}}
\newcommand{\ghpr}{\href{https://github.com/eddelbuettel/rcppapt/pull/#1}{##1}}
\newcommand{\ghit}{\href{https://github.com/eddelbuettel/rcppapt/issues/#1}{##1}}

\section{Changes in version 0.0.10 (2024-11-29)}{
  \itemize{
    \item Package maintenance updating continuous integration script versions
    as well as coverage link from README, and switching to Authors@R
    \item C++ compilation standards updated to C++17 to comply with libapt-pkg
  }
}

\section{Changes in version 0.0.9 (2022-05-25)}{
  \itemize{
    \item Simplified and standardized to only use public API
    \item No longer tests and accomodates pre-Apt 2.0 API
  }
}

\section{Changes in version 0.0.8 (2021-12-04)}{
  \itemize{
    \item New test file \code{version.R} ensures NEWS file documents
    current package version
    \item Travis artifacts and badges have been pruned
    \item Vignettes now use \pkg{simplermarkdown}
  }
}

\section{Changes in version 0.0.7 (2021-04-16)}{
  \itemize{
    \item Add rmarkdown to Suggests: as an implicit conditional dependency
    \item Switch vignette to minidown and its water framework, add
    minidown to Suggests as well
    \item Update two URLs in the README.md file
  }
}

\section{Changes in version 0.0.6 (2020-03-14)}{
  \itemize{
    \item Accommodate Apt 2.0 code changes by including more header files
    \item Change is backwards compatible and conditional
    \item Added configure call using \code{pkg-config} and package
    version comparison (using R) to determine if the define is needed
    \item Softened unit tests as we cannot assume optional source deb
    information to be present, so demo code runs but zero results
    tolerated
  }
}

\section{Changes in version 0.0.5 (2018-09-29)}{
  \itemize{
    \item \code{NAMESPACE} now sets symbol registration
    \item \code{configure} checks for suitable system, no longer errors
    if none found, but sets good/bad \code{define} for the build
    \item Existing C++ code is now conditional on having a 'good' build
    system, or else alternate code is used (which succeeds everywhere)
    \item Added \code{suitable()} returning a boolean with configure
    result
    \item Tests are conditional on \code{suitable()} to test good builds
    \item The Travis setup was updated
    \item The vignette was updated and expanded
  }
}

\section{Changes in version 0.0.4 (2017-07-16)}{
  \itemize{
    \item New function \code{getDepends}
    \item New function \code{reverseDepends}
    \item Added package registration code
    \item Added usage examples in \code{scripts} directory
    \item Added vignette, also in \code{docs} as rendered copy
  }
}
\section{Changes in version 0.0.3 (2016-12-07)}{
  \itemize{
    \item Added \code{dumpPackages}, \code{showSrc}
  }
}

\section{Changes in version 0.0.2 (2016-04-04)}{
  \itemize{
    \item Added \code{reverseDepends}, \code{dumpPackages}, \code{showSrc}
  }
}

\section{Changes in version 0.0.1 (2015-02-20)}{
  \itemize{
    \item Initial version with \code{getPackages} and \code{hasPackages}
  }
}
