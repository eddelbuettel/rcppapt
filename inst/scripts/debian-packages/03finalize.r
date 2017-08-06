
# nmu <- deb[ is.na(deb[, dotCorFortran]) | deb[, dotCorFortran]==TRUE, 1]


nmu <- deb[ is.na(dotCorFortran) | (dotCorFortran & hasRegistration), 1] #42
oth <- df[dotCorFortran & hasRegistration, 1]

nmu <- rbind(nmu, oth)  ## 46


regexp <- paste(paste0("^", nmu[[1]], "$"), collapse="|")

res <- getPackages(regexp)

for (i in 1:nrow(res))
    cat("nmu", paste(res[i,], collapse="_"), ". ANY . -m 'Rebuild against R 3.4.*, see #861333'\n")

