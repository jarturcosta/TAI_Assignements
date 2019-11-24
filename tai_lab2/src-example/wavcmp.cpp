#include <cstdlib>

#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wavquant.h"
#include "wavcmp.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536;

int main(int argc, char** argv) {
    
    cout << "Starting main class" << endl;

    if (argc < 3) {
        cerr << "Usage: wavcmp <input file> <bits to quantize>" << endl;
        return 1;
    }

    SndfileHandle sndFile{ argv[argc - 2]};
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
    
    
    int q = {stoi(argv[argc - 1])};
    WAVquant quantizer {q};
    WAVcmp cmp;
    
    cout << q << endl;
    
    size_t nFrames;
    vector<short> quantized_samples(FRAMES_BUFFER_SIZE * sndFile.channels());
    vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
    

    double snr = 0.0;

    while ((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
        samples.resize(nFrames * sndFile.channels());
        quantized_samples = quantizer.quantize(samples);

        snr += cmp.snr(samples, quantized_samples);
    }
    
    cout << "SNR: " << snr << endl;
        
        

    return 0;
    
}

