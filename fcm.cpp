#include <iostream>
#include <fstream>
#include <vector> 
#include <map>
#include <cstring>
#include "permutations.cpp"
using namespace std;
#include <iomanip>


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


vector<string> combination_with_repetiton(int n, int p, const vector<string>& alphabet){ 
        n--; 
        vector<int> v(p+1, 0);
        vector<string> allCombinations;
        while (true){ 
            for (int i = 0; i < p; ++i){
                if (v[i] > n){ 
                    v[i + 1] += 1; 
                    for (int k = i; k >= 0; --k){ 
                            v[k] = v[i + 1]; 
                    } 
                } 
            } 

            if (v[p] > 0) {break;}
            string comb = "";
            for (size_t i = 0; i < n; ++i) 
                comb.append(alphabet[v[i]].c_str());
            allCombinations.push_back(comb);
            v[0] += 1; 
        }

        return allCombinations;
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
        //cout << counter[string_seq.substr(0,k-1)][s2] << endl;

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
    for(auto& seq: counter) {
        int sum = 0;
        for(auto&& freq: seq.second) {
            sum+=freq.second;
        }
        for(auto&& freq: seq.second) {
            float x = freq.second+smoothing;
            float y = sum+smoothing*seq.second.size();
            probabilities[seq.first][freq.first] = (float)x/(float)y;
        }
    }
	
    printCounterDouble(probabilities);

    return probabilities;
}



void printCounter(map<string, map<string,int>> counter) {
    for(auto& x : counter) {
        cout << x.first << " - ";
        cout << "[ ";
        for(auto&& y: x.second)
	        std::cout << y.first << ":" << y.second << " ";
        cout << "]\n";
    }
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
        //cout << "-> " << permuts.at(i) << '\n';

        counter[permuts[i].substr(0,k-1)] = freqs;

    }

    return counter;
}



int main(int argc, char *argv[]) 
{
    setw(2);
    setprecision(5);
    

    char alphabet[] = "ACTG";
    int k = atoi(argv[2]);
    int smoothing = atoi(argv[3]);
    string filename = argv[1];


    if (k > sizeof(alphabet)) {
        cout << "Order of the context can't be higher than the alphabet size!\n";
        return 1;
    }

    string fileContent = readFile(filename);

    vector<string> options = {"A","G","C","T"};
    combination_with_repetiton(4,3,options);

    map<string, map<string,int>> counter = initializeCounter(4,options,alphabet);
    printCounter(counter);

    //char ab[] = "ATCG";
    //allLexicographic(ab);

    counter = countOccurrences(counter, fileContent, k, options);

    //printCounter(counter);
    calculateProbabilities(counter,smoothing);


    return 0;
}