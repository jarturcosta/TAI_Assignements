

#include <cstdlib>
#include "wavquant.h"


using namespace std;
constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

/*
 * 
 */
int main(int argc, char** argv) {
    
    cout << "WAVquant";

    if (argc < 3) {
        cerr << "Usage: wavhist <input file> <channel>" << endl;
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
    

    WAVquant quantizer{q};
    size_t nFrames;
    vector<short> quantized_samples(FRAMES_BUFFER_SIZE * sndFile.channels());
    vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
    
    SndfileHandle output{ argv[argc - 3], SFM_WRITE, sndFile.format(),sndFile.channels(), sndFile.samplerate()};
    
    while ((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
        samples.resize(nFrames * sndFile.channels());
        quantized_samples = quantizer.quantize(samples);
        output.writef(quantized_samples.data(), nFrames);
        cout << "Ye" << endl;
    }

    return 0;
}
