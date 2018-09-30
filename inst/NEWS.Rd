\name{NEWS}
\title{News for Package \pkg{RcppAPT}}
\newcommand{\ghpr}{\href{https://github.com/eddelbuettel/rcppapt/pull/#1}{##1}}
\newcommand{\ghit}{\href{https://github.com/eddelbuettel/rcppapt/issues/#1}{##1}}

\section{Changes in version 0.0.5 (2017-09-29)}{
  \itemize{
    \item \code{NAMESPACE} now sets symbol registration
    \item \code{configure} checks for suitable system, no longer errors
    if none found, but sets good/bad \code{define} for the build
    \item Existing C++ code is now conditional on having a 'good' build
    system, or else alternate code is used (which succeeds everywhere)
    \item Added \code{suitable()} returning a boolean with configure
    result
    \item Tests are conditional of \code{suitable()} to test good builds
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
