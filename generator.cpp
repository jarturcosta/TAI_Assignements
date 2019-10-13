#include <iostream>
#include <fstream>
#include <vector> 
#include <map>
#include <cstring>
#include <iomanip>

using namespace std;

string genStartSegment(char alphabet[], int seqLen) {
    string out = "";

    for (int i = 0; i < seqLen-1; ++i) {

        int random = rand() % strlen(alphabet);
        out.push_back(alphabet[random]);
    }

    return out;
}

string generateText(map<string, map<string, float>> probabilities, int nLetters, char alphabet[],int k, int smoothing) {

    string str2 = genStartSegment(alphabet, k);

    map<string, map<string,float>>::iterator p;


    int i;
    for (i = k; i <= nLetters; ++i)
    {   

        string lastKLetters = (str2.substr( str2.length() - k + 1));

        p = probabilities.find(lastKLetters);

        float random = (rand() % 100);

        
        float prob;

        if(probabilities.count(lastKLetters) == 0) {
            for(int j = 0; j < strlen(alphabet); ++j) {
                string s(1,alphabet[j]);
                float x = smoothing;
                float y = smoothing*k;
                float prob = (float)x/(float)y;
                probabilities[lastKLetters][s] = prob;
            }

        }



        for(auto& x : probabilities) {
            if (x.first == lastKLetters){
                prob = 0;
                for(auto&& y: x.second){
                    prob = prob + (y.second * 100);
                    if(random <= prob){
                        random = 1000; 
                        str2.append(y.first);
                    }
                }
            }
        }
    }

    return str2;
}
