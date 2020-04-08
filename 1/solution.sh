rm -r 8091
mkdir 8091 && cd 8091
mkdir Kuzin && cd Kuzin
date > Ivan.txt
date --date "next Mon" > filedate.txt
cat Ivan.txt filedate.txt > result.txt
cat result.txt 
