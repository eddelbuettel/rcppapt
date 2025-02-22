pkg <- "RcppAPT"
nrdfile <- system.file("NEWS.Rd", package = pkg)
isCI <- (Sys.getenv("CI") == "true") || (Sys.getenv("TRAVIS") == "true") || (Sys.getenv("CONTINUOUS_INTEGRATION") == "true") || (Sys.getenv("CODECOV_TOKEN") != "")
if (file.exists(nrdfile)) {
    nrd <- readLines(nrdfile)
    ver <- as.package_version(gsub(".* ([0-9\\.]+) .*", "\\1", nrd[grepl("\\{Changes", nrd)]))
    dcf <- read.dcf(system.file("DESCRIPTION", package = pkg))
    cur <- as.package_version(dcf[[1,"Version"]])
    lcur <- length(strsplit(format(cur), "\\.")[[1]])
    if (cur != ver[1] && !isCI && lcur == 3)
        stop("Expected NEWS entry for ", cur, " but saw ", ver[1], call. = FALSE)
}
