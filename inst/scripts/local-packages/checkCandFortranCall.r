#!/usr/bin/env r

if (Sys.info()[["user"]] == "edd") setwd("~/src/debian/CRAN/")   ## on my box

suppressMessages({
    library(data.table)
    library(RcppAPT)
})

dat <- read.table(pipe("./checkCandFortranCall.sh"),
                  col.names=c("package", "dotFortran", "dotC", "recommended"),
                  stringsAsFactors=FALSE)
setDT(dat)
#saveRDS(dat, file="~/git/rcppapt/inst/scripts/local-packages/dat.rds")
sapply(dat[, -1], table)  # counts by type

dat[, cranname:=paste0("r-cran-", tolower(strsplit(package, "-")[[1]][1])), by=package]

deb <- getDepends(paste(dat[,cranname], collapse="|"))
setDT(deb)
setkey(deb, srcpkg)
setkey(dat, cranname)

subdeb <- deb[ deppkg=="r-base-core", tail(.SD, 1), by=srcpkg]
setkey(subdeb, srcpkg)
subdeb[ dat ]

subdeb[ dat ][order(version),]


#dat[recommended==TRUE, ]

#dat[recommended==FALSE, ]
