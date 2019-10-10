// C++ program to print all permutations  
// with repetition of characters  
#include <bits/stdc++.h> 
#include<string.h> 
using namespace std; 

  

  
/* arr[]  ---> Input Array 
  chosen[] ---> Temporary array to store indices of 
                   current combination 
   start & end ---> Staring and Ending indexes in arr[] 
   r ---> Size of a combination to be printed */
vector<string> CombinationRepetitionUtil(char chosen[], char arr[], 
                    int index, int r, int start, int end) 
{ 
    vector<string> seqs;

    if (index == r) 
    { 
        char seq[r];
        for (int i = 0; i < r; i++) 
            seq[i] = arr[chosen[i]];
        string seq_string = seq;
        cout << seq_string.substr(0,seq_string.length()-3) << endl;
        seqs.push_back(seq_string.substr(0,seq_string.length()-3));
        return seqs; 
    } 

    for (int i = start; i <= end; i++) 
    { 
        chosen[index] = i; 
        CombinationRepetitionUtil(chosen, arr, index + 1, r, i, end); 
    }
    
    return seqs;
}

template<typename V, typename Callable>
void for_each_combination(V &v, size_t gp_sz, Callable f) {
    V gp(gp_sz);
    auto total_n = std::pow(v.size(), gp.size());
    for (auto i = 0; i < total_n; ++i) {
        auto n = i;
        for (auto j = 0ul; j < gp.size(); ++j) {
            gp[gp.size() - j - 1] = v[n % v.size()];
            n /= v.size();
        }
        f(gp);
    }
}

vector<string> combinations(vector<int> v) {
    vector<string> out;
    for_each_combination(v, v.size(), [&](std::vector<int> &gp) {
        string seq;
        for (auto c: gp)
            //std::cout << char(c) << " ";
            seq.push_back((char) c);
        out.push_back(seq);
    });
    return out;
}

vector<string> CombinationRepetition(char arr[], int n, int r) 
{ 
    // Allocate memory 
    char chosen[r+1]; 
  
    // Call the recursice function 
    return CombinationRepetitionUtil(chosen, arr, 0, r, 0, n-1); 
} 