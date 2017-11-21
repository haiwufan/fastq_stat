# Fastq_Stat
Statistic illumina Fastq file

I cite github rpeti3 fasq-stats (https://github.com/rpetit3/fastq-stats) and build myself code!

INSTALL:

unzip source.zip

cd source

make

or

g++ fastq_stat.cpp -o fastq_stat

USAGE:

zcat YOUR_FASTQ.gz | fastq_stat
or
fastq_stat < file.fq
