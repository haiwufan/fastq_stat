# fastq_stat
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


Output FORMAT:
e.g.
=========================================================
Reads	Bases	Q30%	Q20%	GC%	Nppm	Ave_len	Min_len	Max_len
25	3747	68.0278	78.0358	41.233	6672	149.88	131	151
=========================================================
