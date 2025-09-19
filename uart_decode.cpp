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

    //start processing data
    //though it's affected by noise, which gets worse as the signal goes on
    //remove noise first.... need to go find methods to denoise?!!!
    /*What do we know about the signal:
        -115200 baud (speed of bits per sec)
            -T_bit = 1/115200 = 8.68 micro_sec (so each bit lasts for about 9 microseconds)
        -Sampling rate of 1MHz
            -T_sample =1/1000000 = 1 micro_sec/ sample
            -so each point on the csv is 1 microsecond apart.
        -How many samples for one bit?
            -T_bit/T_sample=8.68/1 = 8.68 
            - one uart bit spans about 8.7 samples
        -Then falling edges of a start bit are 8.68 samples apart (to keep in mind when denoising so as not blur out the edges)
    Use averages or median to reduce the noise(light filters)???
    */

    const double fs = 1'000'000.0;
    const double baud = 115200.0;
    const double spb = fs / baud; // ~8.6806 samples/bit

    // from reseach a Simple 5-tap moving average to denoise (suffices as the edges are about 8 samples apart)
    vector<double> Voltage_s(Voltage.size());// denoised signal (need to plot the denoised signal to compare)
    for(size_t i=0;i<Voltage.size();++i){
        double s = 0; //neigbhoring samples sum
        int n=0; //count samples (important foe edges)

        for(int k=-2;k<=2;++k){ //5 samples
            long j = (long)i + k;
            if(0 <= j && j < (long)Voltage.size()){ s += Voltage[j]; n++; }
        }
        Voltage_s[i] = s / max(1,n);
    }

    /*
    //Write out the denoised signal to compare
    ofstream fout("uart_denoised.csv");
    if(!fout){
        cerr<<"Could not open output file.\n";
    } else {
        for (size_t i = 0; i<Voltage.size(); ++i){
            fout<< Voltage[i] << "," << Voltage_s[i] <<"\n";
        }

        fout.close();
        cout<<"wrote uart_denoised.csv\n";
    }
        */
    //signal looks good for now( remember to attach images)

    return 1;

}