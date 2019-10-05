#include <iostream>
#include <fstream>
#include <vector> 
#include <map>

using namespace std;


string readFile(string filename) {
    string line;
    ifstream myfile (filename);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            cout << line << '\n';
        }
        myfile.close();
    }

    return line;
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

void printCounter(map<string, vector<int>> counter) {
    for(auto& x : counter) {
        cout << x.first << " - ";
        cout << "[ ";
        for(auto&& y: x.second)
	        std::cout << y << " ";
        cout << "]\n";
    }
}

map<string, vector<int>> initializeCounter (int k, vector<string> alphabet) {
    map<string, vector<int>> counter;
    vector<string> allCombinations = combination_with_repetiton(k, k-1, alphabet);

    for(size_t i=0; i<allCombinations.size(); ++i){
        vector<int> freqs;
        for (int j = 0; j < k; ++j) {
            freqs.push_back(0);
        }

        counter[allCombinations[i]] = freqs;

    }

    return counter;
}



int main(int argc, char *argv[]) 
{
    char alphabet[] = {'A','G','C','T'};
    int k = atoi(argv[2]);
    int alpha = atoi(argv[3]);
    string filename = argv[1];
    cout << sizeof(alphabet) << '\n';
    cout << argv[1] << '\n';

    if (k > sizeof(alphabet)) {
        cout << "Order of the context can't be higher than the alphabet size!";
        return 1;
    }

    string fileContent = readFile(filename);


    vector<string> options = {"A","G","C","T"};
    combination_with_repetiton(4,3,options);

    map<string, vector<int>> counter = initializeCounter(4,options);
    printCounter(counter);


    return 0;
}