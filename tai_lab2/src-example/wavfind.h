#ifndef WAVFIND_H
#define WAVFIND_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <sstream>
#include "wavcb.h"

class WAVfind {
    private:

        //std::map<string, vector<vector<double>>> codebook_and_means;

    public:

        // void print_codebook_and_means() {
        //     for(auto& entry : codebook_and_means) {
        //         cout << entry.first << " - " << endl;
        //         for(auto&& list: entry.second) {
        //             cout << "[ ";
        //             for (auto&& e : list) {
        //                 cout << e << " ";
        //             }
        //             cout << "] " << endl;
        //         }
        //     }
        // }
        
        void compare_codebooks(string filename) {
            SndfileHandle sndFileIn{filename};

            WAVcb cb {sndFileIn, 2, 2, 2, 1};
            
            map<string, vector<vector<double>>> codebook_and_means;
            codebook_and_means = get_codebook_means();

            cout << "Execução do programa principal..." << endl;
            vector<vector<double>> centroids = cb.get_codebooks(filename);
            
            cout << "Escrita do codebook" << endl;
            cb.write_codebook("codebook_to_find.csv", centroids);
            
            
            map<string,double> scores;
            for(auto& pair : codebook_and_means) {
                int index_X = 0;
                double diffSum = 0.0;
                for(auto&& centroid : pair.second) {
                    index_X++;
                    int index_Y=0;
                    for (auto&& e:centroid) {
                        index_Y++;
                        diffSum+=abs((double)e-(double)centroids[index_X][index_Y]);
                    }
                    cout << "diffSum (" << pair.first << ", centroid=" << index_X << ") = " << diffSum << endl;;
                }
                cout << "Total diffsum = " << diffSum << endl;
                scores[pair.first]=diffSum;

                
            }
            





        }

        map<string, vector<vector<double>>> get_codebook_means() {

            map<string, vector<vector<double>>> codebook_and_means;

            for (int j = 1; j <= 7; j++) {
                std::string s = std::to_string(j);
                string filename = "codebook_sample0" + s + ".wav.csv";
                cout << ">>>   " << filename << "   <<<" << endl;

                vector<vector<double>> centroids;
                std::ifstream input(filename);

                for (std::string line; getline(input, line);) {
                    vector<double> block;

                    std::stringstream ss(line);
                    double i;

                    while (ss >> i) {
                        block.push_back(i);

                        if (ss.peek() == ',' || ss.peek() == ' ') {
                            ss.ignore();
                        }
                    }
                    cout << "[ ";
                    for (i = 0; i < block.size(); i++) {
                        std::cout << block.at(i) << " ";
                    }
                    cout << "]" << endl; 
                    cout << "\n\n";

                    centroids.push_back(block);
                }
                
                codebook_and_means[filename]=centroids;
            }

            return codebook_and_means;

        }



};

#endif /* WAVFIND_H */

