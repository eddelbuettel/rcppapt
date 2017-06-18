#!/usr/bin/r

library(RcppAPT)

rd <- reverseDepends("r-base-core")     # 514 x 2
rd <- rd[grepl("^r-", rd[,1]), ]        # 487 x 2

rd[,1] <- as.character(rd[,1])
rd[,2] <- as.character(rd[,2])

rd <- rd[order(rd[,2]), ]

library(data.table)
setDT(rd)

#rd[, 3] <- package_version(rd[, 2], FALSE)     # fails for ~

## special treatment
rd[ version=="3.0.0~20130330-1", version := "3.0.0.20130330-1"]
rd[ version=="3.2.4-revised-1", version := "3.2.4.1-1"]

rd[version!="", oldVersion := version  <=  package_version("3.3.3-1")]
rd[ is.na(oldVersion), oldVersion := FALSE]

## now nrow(rd[ oldVersion==TRUE, ]) is 429

##db <- tools::CRAN_package_db()
##subdb <- db[, c("Package", "Version", "NeedsCompilation")]




rd[ version=="", skip:=TRUE ]
rd[ is.na(skip), skip:=FALSE]
n <- nrow(rd)
for (i in 1:n) {                        # takes a few minutes too ...
    #print(rd[i,])
    txt <- paste0(rd[i, package], "$")
    if (!rd[i,skip]) {
        dep <- getDepends(txt)
        if (nrow(dep) > 0) rd[i, isCompiled:="libc6" %in% as.character(dep[,"deppkg"])]
        #print(rd[i,])
    }
}

## dim(rd[ oldVersion==TRUE & skip!=TRUE & isCompiled , ])  # 171

print(rd[ oldVersion==TRUE & skip!=TRUE & isCompiled , ], top=205)

rd[, cran:=grepl("^r-cran", package) ]
rd[ oldVersion==TRUE & skip!=TRUE & isCompiled & cran, ]

cand <- rd[ oldVersion==TRUE & skip!=TRUE & isCompiled & cran, ]   # 151
setkey(cand, package)

db <- tools::CRAN_package_db()
setDT(db)
db[, package:=paste0("r-cran-", tolower(Package))]
setkey(db, package)


foo <- db[ cand ]   # inner join
foo[, .(package, Package, Version, NeedsCompilation, oldVersion, skip)]

saveRDS(foo[, .(package, Package, Version, NeedsCompilation, oldVersion, skip)], file="debpackages.rds")

## egrep "\.(C|Fortran)\(" ade4/R/*

## on another machine
deb <- readRDS("~/debpackages.rds")
for (i in 1:nrow(deb)) { deb[i, "dotCorFortran"] <- if (is.na(deb[i, "Package"])) NA else system(paste0("egrep -r -q \"\\.(C|Fortran)\\(\" ", deb[i, "Package"], "/R/*"))==0 }

deb <- readRDS("debpackagesout.rds")
setDT(deb)
deb[ is.na(deb[, dotCorFortran]) | deb[, dotCorFortran]==TRUE, 1:3]   ## 75

