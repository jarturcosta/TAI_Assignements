#ifndef WAVCB_H
#define WAVCB_H

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <math.h>
#include <sndfile.hh>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <map>
#include <fstream>
#include <time.h>

using namespace std;
const size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading/writing frames

class WAVcb {
private:
    std::vector<vector<double>> sample_blocks;
    std::vector<short> samples;
    std::map<vector<double>, vector<vector<double>>> cluster_means;
    std::vector<vector<double>> clusters;
    int block_size;
    int overlap;
    int n_clusters;
    int iterations;

public:

    WAVcb(SndfileHandle& sfh, int b_size, int ol_factor, int clust, int iter) {
        samples = vector<short> (FRAMES_BUFFER_SIZE * sfh.channels());
        sample_blocks = {};
        block_size = b_size;
        overlap = ol_factor;
        n_clusters = clust;
        iterations = iter;
    }

    void populate_blocks(SndfileHandle sndFileIn) {
        std::cout << "\tInicio da função..." << endl;
        size_t n_frames;
        while (n_frames = sndFileIn.readf(samples.data(), FRAMES_BUFFER_SIZE)) {
            std::cout << "\tInicio da iteração..." << endl;
            std::cout << "\tResizing..." << endl;
            std::cout << "\tFrames=" << n_frames * FRAMES_BUFFER_SIZE << endl;
            samples.resize(n_frames * sndFileIn.channels());

            vector<double> samples_double(samples.begin(), samples.end());
            std::cout << "\tSamples double size = " << samples_double.size() << endl;

            std::cout << "\tInicio do ciclo exterior..." << endl;
            for (unsigned int i = 0; i < samples_double.size(); i += overlap) {
                vector<double> block = {};

                //std::cout << "\t\tInicio do ciclo interior..." << endl;
                for (int j = 0; j < block_size && i + block_size - 1 < samples_double.size(); j++) {
                    //std::cout << "\t\t\tPushback: " << samples_double[j] << endl;
                    block.push_back(samples_double[j]);
                }
                //std::cout << "\t\tblock size = " << block.size() << endl;
                if (block.size() == block_size) {
                    //std::cout << "\t\tPushback do bloco" << endl;
                    sample_blocks.push_back(block);
                }
            }
        }
        std::cout << "\tBlocks size: " << sample_blocks.size() << endl;
    }

    bool exists_in_vector(int elem, vector<int> vec) {
        std::cout << "\tCheck if in vec" << endl;
        bool exists = false;
        if (vec.size() != 0) {
            for (int i = 0; i < vec.size(); i++) {
                std::cout << "\t\t" << elem << ", " << vec[i] << endl;
                if (elem == vec[i]) {
                    exists = true;
                }
            }
        }

        std::cout << "\tDone" << endl;
        return exists;
    }

    void populate_means(int n_clusters) {
        std::cout << "\tInicio da função..." << endl;
        srand(time(NULL));
        vector<int> block_indexes = {};

        std::cout << "\tSize=" << sample_blocks.size() << endl;
        std::cout << "\tInicio do ciclo exterior (for)..." << endl;
        for (unsigned int i = 0; i < n_clusters; i++) {
            std::cout << "\t\tIteração nº " << i << endl;
            int chosen_mean = -1;
            std::cout << "\t\tInicio do ciclo de repetição (while)" << endl;

            while (true) {
                chosen_mean = rand() % (sample_blocks.size() - 1);
                std::cout << "\t\t" << chosen_mean << endl;
                if (!exists_in_vector(chosen_mean, block_indexes)) {
                    break;
                }
            }
            std::cout << "\t\tMean escolhida: " << chosen_mean << endl;
            std::cout << "\t\tPush back da mean..." << endl;

            block_indexes.push_back(chosen_mean);



            vector<double> mean(sample_blocks[chosen_mean].begin(), sample_blocks[chosen_mean].end());
            std::cout << "\t\tVetor de mean definido..." << endl;
            vector<vector<double>> cluster;
            pair<vector<double>, vector<vector<double>>> clusters = make_pair(mean, cluster);
            std::cout << "\t\tIncerção dos clusters..." << endl;
            cluster_means.insert(clusters);
        }
    }

    double dist_between_vecs(vector<double> vec_1, vector<double> vec_2) {
        int d = 0;
        for (unsigned int i = 0; i < vec_1.size(); i++) {
            d += pow((vec_1[i] - vec_2[i]), 2);
        }
        return d / vec_1.size();
    }

    void cluster_blocks() {
        //std::cout << "\tInicio do ciclo exterior" << endl;
        

        for (unsigned int i = 0; i < sample_blocks.size(); i++) {
            //std::cout << "Iter " << i << "/" << sample_blocks.size() << endl;
            double min_d = 0.0;
            pair<vector<double>, vector<double>> closest_to_mean;
            int mutex = 1;
            
            //std::cout << "\t\tInicio do ciclo interior" << endl;
            for (auto const& [mean, cluster] : cluster_means) {
                double d = dist_between_vecs(mean, sample_blocks[i]);
                //std::cout << "\t\t\tDistance = " << d << endl;
                if (mutex == 1) {
                   // std::cout << "\t\t\tFirst iteration " << endl;
                    closest_to_mean.first = mean;
                    closest_to_mean.second = sample_blocks[i];
                    min_d = d;
                    mutex--;
                } else {
                    //std::cout << "\t\t\tNot first iteration"<< endl;
                    if (d != 0.0 && d < min_d) {
                        //std::cout << "\t\t\tNew mean" << endl;
                        closest_to_mean.first = mean;
                        closest_to_mean.second = sample_blocks[i];
                        min_d = d;
                    }
                }
            }
            
            //std::cout <<"Fim do ciclo interior"<< endl;

            vector<vector<double>> c = cluster_means.at(closest_to_mean.first);
            //std::cout << "c definido"<< endl;
            c.push_back(closest_to_mean.second);
            //std::cout << "Pushback do closest"<< endl;

            cluster_means[closest_to_mean.first] = c;
            //std::cout << "Fim do ciclo" << endl;
        }
    }

    void update_cluster_means() {
        for (auto const& [mean, cluster] : cluster_means) {
            vector<double> averages = get_cluster_averages(cluster);

            auto mean_handler = cluster_means.extract(mean);
            mean_handler.key() = averages;
            cluster_means.insert(move(mean_handler));

        }
    }

    vector<double> get_cluster_averages(vector<vector<double>> cluster) {
        vector<double> cluster_averages = {};

        for (unsigned int i = 0; i < block_size; i++) {
            double sum = 0.0;

            for (unsigned int j = 0; j < cluster.size(); j++) {
                sum += cluster[j][i];
            }

            cluster_averages.push_back(sum / cluster.size());

        }
        return cluster_averages;
    }

    vector<vector<double>> get_codebooks(SndfileHandle snf) {
        clock_t tStart = clock();
        std::cout << "Popular blocos..." << endl;
        populate_blocks(snf);
        std::cout << "Popular means..." << endl;
        populate_means(n_clusters);
        printf("Tempo a inicializar blocos e means: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        vector<vector<double>> cluster_centroids = {};
        
        tStart = clock();
        std::cout << "Inicio das iterações..." << endl;
        for (unsigned int i = 0; i < iterations; i++) {
            std::cout << "Iteração nº " << i << ": Cluster de blocos e atualização das means..." << endl;
            cluster_blocks();
            update_cluster_means();
        }
        printf("Tempo em clustering e atualização de clusters: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        std::cout << "Agrupar centroids..." << endl;
        for (auto const& [mean, cluster] : cluster_means) {
            cluster_centroids.push_back(mean);
        }

        return cluster_centroids;
    }

    void write_codebook(string filename, vector<vector<double>> centroids) {
        std::ofstream file;
        file.open(filename);
        for (auto block : centroids) {
            for (auto vec : block) {
                file << vec << ", ";
            }
            file << "\n";
        }
        file.close();
    }


};

#endif /* WAVCB_H */

