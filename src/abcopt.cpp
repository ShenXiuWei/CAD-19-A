#include "datatype.h"
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

void ABCOptimization(char* circuit_name, int output_index, int fault_index) {
    string read_blif_name = "";
    string write_verilog_name = "";

    if(output_index == -1)
    {
        read_blif_name = "beyond_circuit.blif";
        write_verilog_name = "beyond_original_circuit.v";
    }
    else
    {
        read_blif_name = "beyond_PO" + to_string(output_index) + "fault" + to_string(fault_index) + ".blif";
        write_verilog_name = "beyond_PO" + to_string(output_index) + "fault" + to_string(fault_index) + ".v";
    }

    int line_num = 0;
    ifstream fin;
    string tmps;
    fin.open(read_blif_name, ios::in);
    while(getline (fin, tmps))
    {
        line_num++;
    }
    fin.close();

    ofstream f_out;
    f_out.open("beyond_run.script", ios::out);
    f_out << "read_blif " << read_blif_name << endl;
    if (line_num <= 500) {
        f_out << "compress2rs" << endl;
        f_out << "resyn2rs" << endl;
        f_out << "choice2" << endl;
        f_out << "compress2rs" << endl;
        f_out << "resyn2rs" << endl;
        f_out << "compress2rs" << endl;
        f_out << "resyn2rs" << endl;
    } else {
        f_out << "resyn2" << endl;
    }
    f_out << "read_library verify.lib" << endl;
    f_out << "map -a" << endl;
    f_out << "write_verilog " << write_verilog_name << endl;
    f_out << "quit" << endl;
    f_out.close();
    
    system("./abc -f beyond_run.script");

}
