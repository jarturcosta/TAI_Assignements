#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include "generator.cpp"
#include <iomanip>
#include <math.h>


using namespace std;

float modelEntropy = 0.0;

string readFile(string filename) {
    string content = "";
    string line;
    ifstream myfile (filename);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            content+=line;
        }
        myfile.close();
    }

    return content;
}


string string_from_vector(const vector<string> &pieces) {
    string s;
    for (const auto &piece : pieces) s += piece;
    return s;
}


map<string, map<string,int>> countOccurrences(string content, int k, vector<string> alphabet) {
    map<string, map<string,int>> counter;

    char content_ch_array[content.length() + 1];
    strcpy(content_ch_array, content.c_str());

    for(int i = 0; i < content.length()-k+1; ++i) {
        vector<string> seq;
        for (int j = i; j < i+k; ++j) {
            string s(1,content_ch_array[j]);
            seq.push_back(s);
        }

        string string_seq = string_from_vector(seq);
        string s2(1,string_seq.back());



        counter[string_seq.substr(0,k-1)][s2]++;
        for(auto&& l: alphabet) {
            if(counter[string_seq.substr(0,k-1)].count(l) == 0) {
                counter[string_seq.substr(0,k-1)][l] = 0;

            }
        }


    }

    return counter;
}

// Printing function for the probability counter
void printCounterDouble(map<string, map<string,float>> counter) {
    std::cout << std::fixed;
    std::cout << std::setprecision(4);

    for(auto& x : counter) {
        cout << x.first << " - ";
        cout << "[ ";
        int cnt = 0;
        for(auto&& y: x.second) {
            cnt++;
            cout << y.first << ":" << (float) y.second;
            if(cnt != x.second.size()) {
                cout << "\t";
            }
        }
        cout << " ]\n";
    }
}

void printCounter(map<string, map<string,int>> counter) {
    std::cout << std::fixed;
    std::cout << std::setprecision(4);

    for(auto& x : counter) {
        cout << x.first << " - ";
        cout << "[ ";
        int cnt = 0;
        for(auto&& y: x.second) {
            cnt++;
            cout << y.first << ":" << y.second;
            if(cnt != x.second.size()) {
                cout << "\t";
            }
        }
        cout << " ]\n";
    }
}

void saveProbsToFile(map<string, map<string, float>> probs){
    std::ofstream statistics;
    statistics.open ("prob_stats.csv");
    statistics << "Combinations" << "," << "e" << "," << "P(e|c)"<< std::endl;
    for(auto& c : probs) { for(auto&& e: c.second){ statistics << c.first << "," <<  e.first << "," <<  (float) e.second << std::endl;}
    }
}



map<string, map<string,float>> calculateProbabilities(map<string, map<string,int>> counter, int smoothing)  {
    map<string, map<string,float>> probabilities;
    int overallSum = 0;

    for(auto& seq: counter) {
        for(auto&& freq: seq.second) {
            overallSum+=freq.second;
        }
    }

    for(auto& seq: counter) {
        int sum = 0;
        float Hc = 0.0;
        for(auto&& freq: seq.second) {
            sum+=freq.second;
        }
        for(auto&& freq: seq.second) {
            float x = freq.second+smoothing;
            float y = sum+smoothing*seq.second.size();
            float prob = (float)x/(float)y;
            float bits = -log10(prob)*prob;
            probabilities[seq.first][freq.first] = prob;
            Hc+=bits;
        }
        //cout << seq.first << " - Local entropy: " << Hc << endl;
        float localProb = (float) sum/ (float) overallSum;

        modelEntropy+=localProb*Hc;

    }

    return probabilities;
}




int main(int argc, char *argv[])
{
    setw(3);
    setprecision(5);
    cout.precision(3);
    srand (time(NULL));


    char alphabet[] = "ACTG";
    vector<string> options;
    for (int i = 0; i < strlen(alphabet); ++i) {
        string s(1,alphabet[i]);
        options.push_back(s);
    }
    string filename = argv[1];

    int count = 0; 
    while(argv[++count] != NULL);
    cout << "-> " << count << endl; 

    if(count > 2) {
        int k = atoi(argv[2]);
        int smoothing = atoi(argv[3]);


        string fileContent = readFile(filename);
        map<string, map<string, int>> counter = countOccurrences(fileContent, k, options);


        cout << "Probabilities:" << endl;
        map<string, map<string, float>> probs = calculateProbabilities(counter,smoothing);
        printCounterDouble(probs);
        cout << "Model entropy: " << (float) modelEntropy << endl;

        string genText = generateText(probs,600, alphabet, k, smoothing);
        cout << "New text: " << genText << endl;
    } else {
        string fileContent = readFile(filename);
        for (int k = 1; k < 10;++k) {
            cout << "Context order = " << k << endl;
            map<string, map<string, int>> counter = countOccurrences(fileContent, k, options);
            for (int alpha = 1; alpha < 10;++alpha) {
                cout << "\tSmoothing = " << alpha << endl;
                map<string, map<string, float>> probs = calculateProbabilities(counter,alpha);

                cout << "\tModel entropy: " << (float) modelEntropy << endl;
                modelEntropy = 0.0;
                string genText = generateText(probs,1000, alphabet, k, alpha);
                cout << "\tNew text: " << genText << endl;
                
            }

        }
        
    }

    return 0;
}