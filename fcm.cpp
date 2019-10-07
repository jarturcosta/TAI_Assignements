#include <iostream>
#include <fstream>
#include <vector> 
#include <map>
#include <cstring>
#include "permutations.cpp"
using namespace std;


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
    //cout << content << '\n';
    for(int i = 0; i < content.length()-k+1; ++i) {

        vector<string> seq;
        for (int j = i; j < i+k; ++j) {
            string s(1,content_ch_array[j]);
            seq.push_back(s);
        }

        // for(int j = 0; j < alphabet.size(); ++j) {
        //     if (alphabet[j] == seq.back()) {
        //         string string_seq = string_from_vector(seq);
        //         //cout << string_seq.substr(0,k-1)<< '\n';
        //         counter.find(string_seq.substr(0,k-2))->second[j]++;
        //     }
        // }
        string string_seq = string_from_vector(seq);

        string s2(1,string_seq.back());
        cout << s2 << endl;

        counter[string_seq.substr(0,k-1)][s2]++;
        cout << counter[string_seq.substr(0,k-1)][s2] << endl;

        // local_freq.find(s2)->second++;
        // counter[string_seq.substr(0,k-1)] = local_freq;
        //cout << seq << '\n';

    }

    return counter;
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
        cout << "-> " << permuts.at(i) << '\n';

        counter[permuts[i].substr(0,k-1)] = freqs;

    }

    return counter;
}



int main(int argc, char *argv[]) 
{
    char alphabet[] = "ACTG";
    int k = atoi(argv[2]);
    int alpha = atoi(argv[3]);
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

    char ab[] = "ATCG";
    allLexicographic(ab);

    counter = countOccurrences(counter, fileContent, k, options);
    cout << "-----" << endl;

    printCounter(counter);



    return 0;
}