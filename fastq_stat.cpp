#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
using namespace std;
const float VERSION = 0.1;
const int MAX_LENGTH = 1000;

class Stats {
    public:
        // Read and Base stats
        unsigned long int read_total;
        unsigned long int base_total;
        unsigned long int base_A, base_T, base_C, base_G, base_N;
        double Nppm;
        
        // Length stats
        unsigned int len_min;
        unsigned int len_max;
        double len_mean;
        
        // Qual stats
        int phred;
        unsigned int min_phred;
        unsigned int max_phred;
        vector<unsigned long> qual_vector;
        unsigned long q20, q30;
        
        // initialize
        void init(void) {
            read_total = 0;
            base_total = 0;
            base_A = base_T = 0;
            base_G = base_C = 0;
            base_N = Nppm=0;
            len_min = 1000 ;
            len_max = 0;
            len_mean = 0;
            
            min_phred = 1000;
            max_phred = 0;
            q20 = q30 = 0;
            qual_vector.resize(127,0);
            phred = 33;
        }
        
        void guess_phred() {
            if (max_phred > 74 && min_phred > 58) {
                phred = 64;
            } else if (max_phred <= 74 && min_phred >= 33) {
                phred = 33;
            } else {
                phred = 33;
            }
        }
        
        void getBaseType(string seq) {
            for(int i=0;i<seq.size();i++){
                if(seq[i] == 'A'){
                    base_A+=1;
                }else if(seq[i] == 'T'){
                    base_T+=1;
                }else if(seq[i] == 'C'){
                    base_C+=1;
                }else if(seq[i] == 'G'){
                    base_G+=1;
                }else if(seq[i] == 'N'){
                    base_N+=1;
                }
            }
        }

        void transform_quality(string qual) {
            for (int i = 0; i < qual.length(); i++) {

                unsigned int qual_val = (unsigned int)qual[i];
		min_phred = min_phred < qual_val ? min_phred : qual_val;
		max_phred = max_phred > qual_val ? max_phred : qual_val;
                qual_vector[qual_val] += 1;
            }
        }

        void qual_stats(void) {
            int index = 0;
            for(int i=20;i<=41;i+=1){
                index=i+phred;
                q20+=qual_vector[index];
                if(i>=30){
                    q30+=qual_vector[index];
                }
            }
        }
        
        void jsonify_stats() {
            string t1 = "\t";
            len_mean = base_total * 1.0 / read_total;
            double q20_percent = q20 * 100.0 / base_total;
            double q30_percent = q30 * 100.0 / base_total;
            double GC_percent = (base_C + base_G) * 100.0 / base_total;
            Nppm = base_N * 1000000.0 / base_total;
            
            cout << "Reads\tBases\tQ30%\tQ20%\tGC%\tNppm\tAve_len\tMin_len\tMax_len\n";
            cout << read_total << t1 << base_total << t1 << q30_percent << t1 << q20_percent << t1 << GC_percent << t1 << Nppm << t1 << len_mean << t1 << len_min << t1 << len_max << "\n";

        }
};
/*

	Author: 	haiwufan
	citation： 	https://github.com/rpetit3/fastq-stats

*/

int main() {
    Stats stats;
    stats.init();
    string name, seq, plus, qual;
    ifstream in("/dev/stdin", ios::in);
    while(true) {
        if(!getline(in,name,'\n')) break;
        if(!getline(in,seq,'\n')) break;
        if(!getline(in,plus,'\n')) break;
        if(!getline(in,qual,'\n')) break;
        int len = seq.length();

        if(seq.length()!=qual.length())    {
                cout << "Error: " << name << " base length not equal qual length!!!\n";
        exit(1) ;
        }
        stats.read_total++;
        stats.base_total+=len;

        stats.len_min = stats.len_min<len ? stats.len_min : len;
        stats.len_max = stats.len_max<len ? len : stats.len_max;

        stats.getBaseType(seq);
        stats.transform_quality(qual);

    }
    in.close();

//    Determine Stats
    stats.guess_phred();
    stats.qual_stats();
    stats.jsonify_stats();

    return 0;
}

