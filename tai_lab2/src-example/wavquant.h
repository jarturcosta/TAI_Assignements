#ifndef WAVQUANT_H
#define WAVQUANT_H

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <math.h>
#include <sndfile.hh>

class WAVquant {
    private:
    int q;
    std::vector<short> quant_samples;

    public:
    WAVquant(int input_q) {
      q = input_q;
    }

    std::vector<short> quantize(const std::vector<short> samples){
        short temp_q;
        quant_samples.resize(samples.size());
        for(int i = 0; i < samples.size(); i++){
            temp_q = samples[i] >> q;
            quant_samples[i] = temp_q << q;
        }
        
        
        return quant_samples;
    }

};


#endif /* WAVQUANT_H */
