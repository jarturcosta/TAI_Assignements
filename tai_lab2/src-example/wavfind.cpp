
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <sys/types.h>
#include <dirent.h>
#include "wavcb.h"
#include "wavcmp.h"
#include "wavfind.h"

using namespace std;

int main(int argc, char** argv) {
    
    if (argc < 2) {
        cerr << "Usage: wavfind <filename>" << endl;
        return 1;
    }

    SndfileHandle sndFile{ argv[1]};
    if (sndFile.error()) {
        cerr << "Error: invalid input file" << endl;
        return 1;
    }

    if ((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        cerr << "Error: file is not in WAV format" << endl;
        return 1;
    }

    if ((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
        cerr << "Error: file is not in PCM_16 format" << endl;
        return 1;
    }
    
    WAVfind finder {};
    //finder.get_codebook_means();
    finder.compare_codebooks( argv[1]);
    return 0;
}

