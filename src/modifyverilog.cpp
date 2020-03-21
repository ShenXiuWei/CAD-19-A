#include "datatype.h"
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

int ModifyVerilogFormat(char* circuit_name, int verilog_index) {
    string read_verilog_name = "";
    string write_verilog_name = "";
    string circuit_name_ = circuit_name;
    ifstream f_in;
    ofstream f_out;
    int gatecnt = 0;

    if(verilog_index == 0)
    {
        read_verilog_name = "beyond_original_circuit.v";
        write_verilog_name = "beyond_before_fix_circuit.v";
    }
	else if(verilog_index == 1)
	{
		read_verilog_name = "beyond_mix_circuit.v";
        write_verilog_name = "beyond_before_flatten_circuit.v";
	}
    else if(verilog_index == 2)
    {
        read_verilog_name = "beyond_after_flatten_circuit.v";
        write_verilog_name = circuit_name_;
    }

    f_in.open(read_verilog_name.c_str(), ios::in);
    f_out.open(write_verilog_name.c_str(), ios::out);

    string str;
    string output_name;
    string check_01;
    while(getline(f_in, str))
    {
        check_01 = "";
        output_name = "";
        check_01.assign(str, 0, 4);
        if(check_01 == "zero")
        {
            output_name.append(str.begin() + 7, str.end() - 3);
            f_out << "buf  ( " << output_name << ", 1'b0 );" << endl; 
        }
        else if(check_01 == "one ")
        {
            output_name.append(str.begin() + 7, str.end() - 3);
            f_out << "buf  ( " << output_name << ", 1'b1 );" << endl;
        }
        else
        {
            if (check_01 == "and " || check_01 == "or  " || check_01 == "nand"||
            check_01 == "nor " || check_01 == "xor " || check_01 == "xnor") {
                gatecnt++;
            }
            f_out << str << endl;
        }
    }
    
    f_in.close();
    f_out.close();

    return gatecnt;
}
