#!/usr/bin/r

library(RcppAPT)
library(data.table)


rd <- reverseDepends("r-base-core")     # 516 x 2
rd <- rd[grepl("^r-", rd[,1]), ]        # 489 x 2

rd <- rd[order(rd[,2]), ]

setDT(rd)


## special treatment
rd[ version=="3.0.0~20130330-1", version := "3.0.0.20130330-1"]
rd[ version=="3.2.4-revised-1", version := "3.2.4.1-1"]

rd[version!="", oldVersion := version  <=  package_version("3.3.3-1")]
rd[ is.na(oldVersion), oldVersion := FALSE]



rd[ version=="", skip:=TRUE ]
rd[ is.na(skip), skip:=FALSE]
#print(rd)

regexp <- paste(paste0("^", rd[skip==FALSE, package], "$"), collapse="|")
dep <- getDepends(regexp)
setDT(dep)
#print(dep)

comp <- dep[deppkg=="libc6"]   # around 242
comp[, isCompiled:=TRUE]
#print(comp)

setkey(comp, srcpkg)
setkey(rd, package)

all <- rd[comp[, c(1,5)]]
all[order(version),]
#print(all)
all[oldVersion==TRUE,][order(version),]    # 167

all[, cran:=grepl("^r-cran", package) ]
all[, bioc:=grepl("^r-bioc", package) ]

all[bioc==TRUE & oldVersion==TRUE,]                # 17 BioC
all[bioc!=TRUE & cran!=TRUE & oldVersion==TRUE,]   # 3 other


cand <- all[ cran==TRUE & oldVersion==TRUE, ]      # 147
setkey(cand, package)

db <- tools::CRAN_package_db()
setDT(db)
db[, package:=paste0("r-cran-", tolower(Package))]
setkey(db, package)

foo <- db[ cand ]   # inner join

saveRDS(foo[, .(package, Package, Version, NeedsCompilation, oldVersion, skip)], file="debpackages.rds")

## on another machine, then
if (FALSE) {
    deb <- readRDS("~/debpackages.rds")
    for (i in 1:nrow(deb)) {
        deb[i, "dotCorFortran"] <- if (is.na(deb[i, "Package"])) NA else system(paste0("egrep -r -q \"\\.(C|Fortran)\\(\" ", deb[i, "Package"], "/R/*"))==0
        deb[i, "hasRegistration"] <- if (is.na(deb[i, "Package"])) NA else system(paste0("egrep -r -q \"R_registerRoutines\\(\" ", deb[i, "Package"], "/src/*"))==0
    }
    saveRDS(deb, "~/debpackagesout.rds")
}

if (FALSE) {
    deb <- readRDS("debpackagesout.rds")
    setDT(deb)
    #print(deb[ is.na(deb[, dotCorFortran]) | deb[, dotCorFortran]==TRUE | deb , 1:3])   ## 69
    print(deb[ is.na(dotCorFortran) | (dotCorFortran==TRUE & hasRegistration), 1:3])    ## 42
}
