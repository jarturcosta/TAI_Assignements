#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <map>
#include <fstream>
#include <cstdlib>
#include "wavcb.h"

using namespace std;

int main(int argc, char** argv) {

    if (argc < 5) {
        cerr << "Usage: wavcb <input file> <block size> <overlap factor> <number of clusters> <number of iterations>" << endl;
        return 1;
    }
    
    std::string f = argv[1];
    SndfileHandle sndFileIn{"wav_files/" + f};
    if (sndFileIn.error()) {
        cerr << "Error: invalid input file" << endl;
        return 1;
    }

    if ((sndFileIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        cerr << "Error: file is not in WAV format" << endl;
        return 1;
    }

    if ((sndFileIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
        cerr << "Error: file is not in PCM_16 format" << endl;
        return 1;
    }
    
    const int overlap = {stoi(argv[3])};
    
    if (overlap <= 0) {
        cerr << "Error: Overlap factor is less than 0" << endl;
        return 1;
    }
    
    cout << "Inicialização de variaveis..." << endl;

    const int block_size = { stoi(argv[2]) };
    const int n_clusters = { stoi(argv[4]) };
    const int iterations = { stoi(argv[5]) };
    
    cout << "Instanciação da classe..." << endl;
    WAVcb cb {sndFileIn, block_size, overlap, n_clusters, iterations};
    
    cout << "Execução do programa principal..." << endl;
    vector<vector<double>> centroids = cb.get_codebooks(sndFileIn);
    
    cout << "Escrita do codebook" << endl;
    cb.write_codebook("codebook_" + f + ".csv", centroids);

    return 0;
}

