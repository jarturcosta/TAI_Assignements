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
        seqs.push_back(seq_string);
        return seqs; 
    } 

    for (int i = start; i <= end; i++) 
    { 
        chosen[index] = i; 
        CombinationRepetitionUtil(chosen, arr, index + 1, r, i, end); 
    } 
    return seqs;
}

vector<string> CombinationRepetition(char arr[], int n, int r) 
{ 
    // Allocate memory 
    char chosen[r+1]; 
  
    // Call the recursice function 
    return CombinationRepetitionUtil(chosen, arr, 0, r, 0, n-1); 
} 