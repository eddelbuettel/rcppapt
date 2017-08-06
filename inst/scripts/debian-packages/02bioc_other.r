

pkgs <- rbind(all[bioc!=TRUE & cran!=TRUE & oldVersion==TRUE, 1],
              all[bioc==TRUE & oldVersion==TRUE, 1])[[1]]

dir.create("/tmp/scratch")
setwd("/tmp/scratch")

cat("deb-src http://deb.debian.org/debian unstable main\n",
    file="/etc/apt/sources.list", append=TRUE)
system("apt-get update")

for (p in pkgs) system(paste("apt-get source", p))

df <- data.frame(package=pkgs, stringsAsFactors=FALSE)
for (i in 1:nrow(df)) {
    p <- df[i, 1]
    df[i, "dotCorFortran"] <- system(paste0("egrep -r -q \"\\.(C|Fortran)\\(\" ", p, "*/R/*"))==0
    df[i, "hasRegistration"] <- system(paste0("egrep -r -q \"R_registerRoutines\\(\" ", p, "*/src/*"))==0
}

setDT(df)

