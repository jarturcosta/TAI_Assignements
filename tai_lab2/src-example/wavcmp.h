#include <vector>
#include <math.h>
#include <iostream>


using namespace std;

class WAVcmp {
public:
    
    double signal_energy(const std::vector<double> samples) {
        double s_e = 0.0;
        
        for (int i = 0; i < samples.size(); i++) {
            double current = samples[i];
            s_e += pow(current,2);
        }
        
        return s_e;

    }
    
    double noise_energy(const std::vector<double> samples, std::vector<double> quant_samples) {
        


        double n_e = 0.0;
        
        for (int i = 0; i < samples.size(); i++) {
            double current_s = samples[i];
            double current_q = quant_samples[i];
            n_e += pow(current_s - current_q, 2);
        }
        
        
        return n_e;
    }
    
    double snr(const std::vector<short> sample_file, vector<short> quant_file) {
        using namespace std;
        
        std::vector<double> sample_vec(sample_file.begin(), sample_file.end());
        std::vector<double> quant_vec(quant_file.begin(), quant_file.end());
        
        double Es = signal_energy(sample_vec);
        double En = noise_energy(sample_vec, quant_vec);
        double snr = 10*log10(Es/En);
        
        return snr;
    }
};
