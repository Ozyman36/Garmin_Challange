#include <bits/stdc++.h>
#include "iostream"
#include "vector"

using namespace std;


int main (int argc, char** argv){
    
    //ensure arguments are correct
    if (argc < 2){
        cout<<"Usage: uart_decode <csvfile>\n";
        return 1;
    }
   
    //firstly need to load the data from the file ( maybe a vector, since unsure of size)
    ifstream fin(argv[1]); //open file stream
    if(!fin){cerr<<"Failed to open file."; return 1;} //if file nor found

    vector<double> Voltage; Voltage.reserve(100000);//Vector to store the values from the csv file (maybe reserve space to remove overhead of dynamic allocation)
    string line;

    while(getline(fin, line)){
        if(line.empty()) continue; //might be blank lines???

        //csv is comma seperated. (we can assume its only one column? what if other file has more than one column?(can add that feature later))

        //check headers/non numerics and skip
        try{
            double val = stod(line); //convert string to double
            Voltage.push_back(val);
        } catch (...){
            //skip headers
        }
    }

    cout<<Voltage.size();
    return 1;

}