#include <iostream>
#include <fstream>

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


int main() 
{
    cout << readFile("genes.txt");

    return 0;
}