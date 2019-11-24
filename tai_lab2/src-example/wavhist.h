#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;
	int nChannels;

  public:
	WAVHist(const SndfileHandle& sfh) {
		nChannels = sfh.channels();
		counts.resize(nChannels);
	}

	void update(const std::vector<short>& samples) {
		size_t n { };
		for(auto s : samples)
			counts[n++ % counts.size()][s]++;
	}

	void dump(const size_t channel) const {
		for(auto [value, counter] : counts[channel])
			if (value < 10500) {
				std::cout << value << '\t' << counter << '\n';

			}
	}

	void dumpAverage(int step, int channel) {

		// vector<double> avgs;
		//for (int i = 0; i < nChannels; ++i) {
			int sum = 0, start = 0, cnt = 0;
			for(auto [value, counter] : counts[channel]) {
				cnt++;
				sum+=counter;
				if (cnt % step == 0 && cnt > 0) {
					std::cout /*<< start << " - " << cnt << ": Avg = "*/ << (double)sum/(double)step << std::endl;
					sum = 0;
					start = cnt;
				}
				
			}
		//}

			

	}
};

#endif

