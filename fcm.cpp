#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include "combinations.cpp"
#include "generator.cpp"
#include <iomanip>


using namespace std;

// Global entropy of the model as a global variable
float modelEntropy = 0.0;

// Function that reads the file and returns the file content as a string
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


// Auxiliary function that constructs a string out of a vector of strings, concatenating them
string string_from_vector(const vector<string> &pieces) {
    string s;
    for (const auto &piece : pieces) s += piece;
    return s;
}

// Function that counts the occurrences of each character that come after a specific context.
// It populates the empty counter map that recieves as an argument and returns it.
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
        for(auto&& l: alphabet) {
            // cout << l << " " << counter[string_seq.substr(0,k-1)].count(l) << endl;
            if(counter[string_seq.substr(0,k-1)].count(l) == 0) {
                // cout << "doesn't exist: " << counter[string_seq.substr(0,k-1)].count(l) << "in " << string_seq.substr(0,k-1) << endl;
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
    statistics << "Combinations" << "," << "P(A|c)" << "," << "P(T|c)" << "," << "P(C|c)" << "," << "P(G|c)" << std::endl;
    for(auto& c : probs) { for(auto&& e: c.second){ statistics << c.first << "," <<  e.first << "," <<  (float) e.second << std::endl;}
    }
}


// Function that recieves the counter with occurences of each symbol in each context and calculates the probabilities of the events associated with it.
// It also calculates the local entropies, prints them and updates the overall entropy of the calculated model
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

        modelEntropy+=localProb*Hc;

    }

    return probabilities;
}


// Function that calculates all the combinations with repetitions given the alphabet and the order of the model
map<string, map<string,int>> initializeCounter (int k, vector<string> options, char *alphabet) {
    map<string, map<string,int>> counter;
    char arr[k];
    for (int i = 0; i < k; ++i) {
        arr[i] = alphabet[i];
    }
    int n = sizeof(arr)/sizeof(arr[0]);
    int r = 3;
    vector<string> permuts = CombinationRepetition(arr,n,r);

    for(size_t i=0; i<permuts.size(); ++i){
        cout << ">>> " << permuts[i] << endl;
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
    setw(3);
    setprecision(5);
    cout.precision(3);

    char alphabet[] = "ACTG";
    //char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,. ";
    int k = atoi(argv[2]);
    int smoothing = atoi(argv[3]);
    string filename = argv[1];
    vector<string> options;
    for (int i = 0; i < strlen(alphabet); ++i) {
        string s(1,alphabet[i]);
        options.push_back(s);
    }

    if (k > strlen(alphabet)) {
        cout << "Order of the context can't be higher than the alphabet size!\n";
        return 1;
    }

    string fileContent = readFile(filename);
    map<string, map<string,int>> counter = initializeCounter(4,options,alphabet);
    counter = countOccurrences(counter, fileContent, k, options);


    cout << "Probabilities:" << endl;
    map<string, map<string, float>> probs = calculateProbabilities(counter,smoothing);
    printCounterDouble(probs);
    saveProbsToFile(probs);

    cout << "Model entropy: " << (float) modelEntropy << endl;

    string genText = generateText(probs,1000, alphabet, k);
    cout << "New text: " << genText << endl;

    return 0;
}