#include "datatype.h"
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace::std;

void WriteBlifHeader(int input_num, int output_num, vector<string> io_names, char* circuit_name){
    string blif_name = "beyond_circuit.blif";
    ofstream f_out;
    f_out.open(blif_name.c_str(), ios::out);
    f_out << ".model main_circuit" << endl;
    f_out << ".inputs";
    for(int i = 0; i < input_num; i++){
        f_out << " " << io_names[i];
    }
    f_out << endl << ".outputs";
    for(int i = input_num; i < input_num + output_num; i++){
        f_out << " " << io_names[i] << "_main";
    }
    f_out << endl;
    f_out.close();
}
void WriteBlifEnd(char* circuit_name){
    string blif_name = "beyond_circuit.blif";
    ofstream f_out;
    f_out.open(blif_name.c_str(), ios::out|ios::app|ios::ate);
    f_out << ".end" << endl;
    f_out.close();
}
void WriteBlif_Constant(int input_num, int output_num, vector<string> io_names, int output_index, int constant_01, char* circuit_name){
    string blif_name = "beyond_circuit.blif";
    ofstream f_out;
    f_out.open(blif_name.c_str(), ios::out|ios::app|ios::ate);
    f_out << ".names " << io_names[input_num + output_index] << "_main" << endl << constant_01 << endl;
    f_out.close();
}
void WriteBlif(string optResult, char* circuit_name){
    string blif_name = "beyond_circuit.blif";
    int input_num_sat = 0;
    int output_num_sat = 0;
    int pattern_num_sat = 0;
    string temp = "";
    ifstream f_in;
    ofstream f_out;
    f_in.open(optResult.c_str(), ios::in);
    if(!f_in){
        cout << ">> ERROR: The input file " << optResult << " cannot be open." << endl;
        exit(-1);
    }
    f_out.open(blif_name.c_str(), ios::out|ios::app|ios::ate);
    f_out << ".names";
    f_in >> input_num_sat >> output_num_sat >> pattern_num_sat;
    for(int i = 0; i < input_num_sat + output_num_sat; i++){
        f_in >> temp;
        f_out << " " << temp;
        if(i == input_num_sat)
        {
            f_out << "_main";
        }
    }
    f_out << endl;
    vector<vector<string> > patterns (pattern_num_sat, vector<string>(input_num_sat + output_num_sat, ""));
    for(int i = 0; i < pattern_num_sat; i++){
        for(int j = 0; j < input_num_sat + output_num_sat; j++){
            f_in >> patterns[i][j];
        }
        if(patterns[i][input_num_sat] == "1")
        {
            for(int j = 0; j < input_num_sat; j++)
            {
                f_out << patterns[i][j];
            }
            f_out << " 1" << endl;
        }
    }
    f_in.close();
    f_out.close();

    vector<vector<string> >().swap(patterns);
}
