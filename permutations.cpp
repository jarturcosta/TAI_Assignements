// C++ program to print all permutations  
// with repetition of characters  
#include <bits/stdc++.h> 
#include<string.h> 
using namespace std; 

vector<string> allPermutations;
  

int compare (const void * a, const void * b);  

void allLexicographicRecur (char *str, char* data,  
                            int last, int index)  
{  
    int i, len = strlen(str);  

    for ( i = 0; i < len; i++ )  
    {  
        data[index] = str[i] ;  

        if (index == last)  {
            //cout << data << endl;
            string data_string(data);
            allPermutations.push_back(data_string);
            
        }
            
        else // Recur for higher indexes  
            allLexicographicRecur (str, data, last, index+1);  
    }  
}  
  
/* This function sorts input string, 
allocate memory for data (needed  
for allLexicographicRecur()) and  
calls allLexicographicRecur() for  
printing all permutations */
void allLexicographic(char *str)  
{  
    int len = strlen (str) ;  
  
    // Create a temp array that will  
    // be used by allLexicographicRecur()  
    char *data = (char *) malloc (sizeof(char) * (len + 1)) ;  
    data[len] = '\0';  
  
    // Sort the input string so that 
    // we get all output strings in  
    // lexicographically sorted order  
    qsort(str, len, sizeof(char), compare);  
  
    // Now print all permutaions  
    allLexicographicRecur (str, data, len-1, 0);  
  
    // Free data to avoid memory leak  
    free(data);  
}  
  
// Needed for library function qsort()  
int compare (const void * a, const void * b)  
{  
    return ( *(char *)a - *(char *)b );  
}

vector<string> getPermutations() {
    return allPermutations;
}
  
// // Driver code  
// int main()  
// {  
//     char str[] = "ABC";  
//     cout << "All permutations with repetition of "<<  
//                                 str <<" are: "<<endl ;  
//     allLexicographic(str);  
//     return 0;  
// }  
  
// This code is contributed by rathbhupendra 