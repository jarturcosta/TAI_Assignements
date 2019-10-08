#include <iostream>
#include <fstream>
#include <vector> 
#include <map>
#include <cstring>
#include "permutations.cpp"
#include <iomanip>

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

map<string, map<string,int>> countOccurrences(map<string, map<string,int>> counter, string content, int k, vector<string> alphabet) {
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
    }

    return counter;
}


void printCounterDouble(map<string, map<string,float>> counter) {
    cout.precision(2);

    for(auto& x : counter) {
        cout << x.first << " - ";
        cout << "[ ";
        int cnt = 0;
        for(auto&& y: x.second) {
            cnt++;
	        cout << y.first << ":" << (float) y.second;
            if(cnt != x.second.size()) {
                cout << "   \t";
            } else {
                cout << " ";
            }
        }
        cout << "]\n";
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
        cout << seq.first << " - Local entropy: " << Hc << endl;
        float localProb = (float) sum/ (float) overallSum;
        cout << seq.first << " - Local probability: " << localProb << endl;

        modelEntropy+=localProb*Hc;

    }
	
    //printCounterDouble(probabilities);


    return probabilities;
}



map<string, map<string,int>> initializeCounter (int k, vector<string> options, char *alphabet) {
    map<string, map<string,int>> counter;
    allLexicographic(alphabet);
    vector<string> permuts = getPermutations();

    for(size_t i=0; i<permuts.size(); ++i){
        map<string,int> freqs;
        for (int j = 0; j < k; ++j) {
            freqs[options.at(j)]=0;
        }

        counter[permuts[i].substr(0,k-1)] = freqs;

    }

    return counter;
}





int main(int argc, char *argv[]) 
{
    setw(2);
    setprecision(5);
    cout.precision(2);

    

    char alphabet[] = "ACTG";
    int k = atoi(argv[2]);
    int smoothing = atoi(argv[3]);
    string filename = argv[1];
    vector<string> options;
    for (int i = 0; i < strlen(alphabet); ++i) {
        string s(1,alphabet[i]);
        options.push_back(s);
    }




    if (k > sizeof(alphabet)) {
        cout << "Order of the context can't be higher than the alphabet size!\n";
        return 1;
    }

    string fileContent = readFile(filename);
    map<string, map<string,int>> counter = initializeCounter(4,options,alphabet);

    counter = countOccurrences(counter, fileContent, k, options);

    cout << "Probabilities:" << endl;
    printCounterDouble(calculateProbabilities(counter,smoothing));
    cout << "Model entropy: " << (float) modelEntropy << endl;



    return 0;
}