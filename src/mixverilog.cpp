#include "datatype.h"
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <sstream>
using namespace std;

void MixVerilog(char* circuit_name, vector<string> io_names, int input_num, int output_num, vector< vector<int> > &realPI_to_PO){
    ofstream f_out;
    vector<string> wire_name;
    vector<pair<int, int> > fault_status;
    int check_all_correct = 0;
    for(int i = 0; i < output_num; i++)
    {
        pair<int, int> temp_pair;
        string file_name = "beyond_PO" + to_string(i) + "fault0.blif";
        ifstream f_in_0(file_name.c_str());
        if(f_in_0.is_open())
        {
            temp_pair.first = 1;
            check_all_correct++;
        }
        else
        {
            temp_pair.first = 0;
        }
        file_name = "beyond_PO" + to_string(i) + "fault1.blif";
        ifstream f_in_1(file_name.c_str());
        if(f_in_1.is_open())
        {
            temp_pair.second = 1;
            check_all_correct++;
        }
        else
        {
            temp_pair.second = 0;
        }
        fault_status.push_back(temp_pair);
        string str = io_names[input_num + i] + "_main";
        wire_name.push_back(str);
        str = io_names[input_num + i] + "_wrong1";
        wire_name.push_back(str);
        str = io_names[input_num + i] + "_wrong0";
        wire_name.push_back(str);
        str = io_names[input_num + i] + "_tks_new_wire";
        wire_name.push_back(str);
    }

    f_out.open("beyond_mix_circuit.v", ios::out);

    if(check_all_correct == 0)
    {
        ifstream f_in_main;
        f_in_main.open("beyond_original_circuit.v", ios::in);
        string str = "";
        getline(f_in_main, str);
        f_out << "module top (" << endl;
        while(getline(f_in_main, str))
        {
            string token_old = "_main";
            string token_new = "";
            int pos = 0;
            while((pos = str.find(token_old,pos)) != string::npos)
            {
                str.replace(pos, token_old.length(),token_new);
            }
            f_out << str << endl;
        }
        f_in_main.close();
    }
    else
    {
        f_out << "module top (" << endl << "\t";
        for(int i = 0; i < input_num + output_num - 1; i++)
        {
            f_out << io_names[i] << ", ";
        }
        f_out << io_names[input_num + output_num - 1] << " );" << endl;
        f_out << "\tinput ";
        f_out << io_names[0];
        for(int i = 1; i < input_num; i++)
        {
            f_out << ", " << io_names[i];
        }
        f_out << ";" << endl;
        f_out << "\toutput ";
        f_out << io_names[input_num];
        for(int i = input_num + 1; i < input_num + output_num; i++)
        {
            f_out  << ", " << io_names[i];
        }
        f_out << ";" << endl;
        f_out << "\twire";
        for(int i = 0; i < output_num; i++)
        {
            if(i == 0)
            {
                if(fault_status[i].first == 0 && fault_status[i].second == 0)
                {
                    f_out << " " << wire_name[i * 4 + 0];
                }
                else if(fault_status[i].first == 1 && fault_status[i].second == 0)
                {
                    f_out << " " << wire_name[i * 4 + 1] << ", " << wire_name[i * 4 + 0];
                }
                else if(fault_status[i].first == 0 && fault_status[i].second == 1)
                {
                    f_out << " " << wire_name[i * 4 + 2] << ", " << wire_name[i * 4 + 0];
                }
                else if(fault_status[i].first == 1 && fault_status[i].second == 1)
                {
                    f_out << " " << wire_name[i * 4 + 3] << ", " << wire_name[i * 4 + 0] << ", " << wire_name[i * 4 + 1] << ", " << wire_name[i * 4 + 2];
                }
            }
            else
            {
                if(fault_status[i].first == 0 && fault_status[i].second == 0)
                {
                    f_out << ", " << wire_name[i * 4 + 0];
                }
                else if(fault_status[i].first == 1 && fault_status[i].second == 0)
                {
                    f_out << ", " << wire_name[i * 4 + 1] << ", " << wire_name[i * 4 + 0];
                }
                else if(fault_status[i].first == 0 && fault_status[i].second == 1)
                {
                    f_out << ", " << wire_name[i * 4 + 2] << ", " << wire_name[i * 4 + 0];
                }
                else if(fault_status[i].first == 1 && fault_status[i].second == 1)
                {
                    f_out << ", " << wire_name[i * 4 + 3] << ", " << wire_name[i * 4 + 0] << ", " << wire_name[i * 4 + 1] << ", " << wire_name[i * 4 + 2];
                }
            }
        }
        f_out << ";" << endl;
        for(int i = 0; i < output_num; i++)
        {
            if(fault_status[i].first == 0 && fault_status[i].second == 0)
            {
                f_out << "buf  ( " << io_names[input_num + i] << ", " << wire_name[i * 4 + 0] << " );" << endl;
            }
            else if(fault_status[i].first == 1 && fault_status[i].second == 0)
            {
                f_out << "or   ( " << io_names[input_num + i] << ", " << wire_name[i * 4 + 0] << ", " << wire_name[i * 4 + 1] << " );" << endl;
            }
            else if(fault_status[i].first == 0 && fault_status[i].second == 1)
            {
                f_out << "and  ( " << io_names[input_num + i] << ", " << wire_name[i * 4 + 0] << ", " << wire_name[i * 4 + 2] << " );" << endl;
            }
            else if(fault_status[i].first == 1 && fault_status[i].second == 1)
            {
                f_out << "or   ( " << wire_name[i * 4 + 3] << ", " << wire_name[i * 4 + 0] << ", " << wire_name[i * 4 + 1] << " );" << endl;
                f_out << "and  ( " << io_names[input_num + i] << ", " << wire_name[i * 4 + 3] << ", " << wire_name[i * 4 + 2] << " );" << endl;
            }
        }
        f_out << "main_circuit ( ";
        for(int i = 0; i < input_num; i++)
        {
            f_out << io_names[i] << ", ";
        }
        for(int i = input_num; i < input_num + output_num - 1; i++)
        {
            f_out << io_names[i] << "_main, ";
        }
        f_out << io_names[input_num + output_num - 1] << "_main );" << endl;
        for(int i = 0; i < output_num; i++)
        {
            if(fault_status[i].first == 1)
            {
                f_out << "submodule" << i << "fault0 ( ";
                for(int j = 0; j < input_num; j++)
                {
                    f_out << io_names[j] << ", ";
                }
                f_out << io_names[input_num + i] << "_wrong1 );" << endl;
            }
            if(fault_status[i].second == 1)
            {
                f_out << "submodule" << i << "fault1 ( ";
                for(int j = 0; j < input_num; j++)
                {
                    f_out << io_names[j] << ", ";
                }
                f_out << io_names[input_num + i] << "_wrong0 );" << endl;
            }
        }
        f_out << "endmodule" << endl;


        ifstream f_in_;
        string verilog_name_ = "beyond_before_fix_circuit.v";
        f_in_.open(verilog_name_.c_str(), ios::in);
        string str_ = "";
        while(getline(f_in_, str_))
        {
            f_out << str_ << endl;
        }
        f_in_.close();

        for(int i = 0; i < output_num; i++)
        {
            if(fault_status[i].first == 1)
            {
                ifstream f_in_fix;
                string verilog_name = "beyond_PO" + to_string(i) + "fault0.v";
                f_in_fix.open(verilog_name.c_str(), ios::in);
                string str = "";
                while(getline(f_in_fix, str))
                {
                    f_out << str << endl;
                }
                f_in_fix.close();
            }
            if(fault_status[i].second == 1)
            {
                ifstream f_in_fix;
                string verilog_name = "beyond_PO" + to_string(i) + "fault1.v";
                f_in_fix.open(verilog_name.c_str(), ios::in);
                string str = "";
                while(getline(f_in_fix, str))
                {
                    f_out << str << endl;
                }
                f_in_fix.close();
            }
        }
    }

    f_out.close();

    int gatecnt = ModifyVerilogFormat(circuit_name, 1);

    ofstream f_out_script;
    f_out_script.open("beyond_mix.script", ios::out);
    f_out_script << "read_verilog beyond_before_flatten_circuit.v" << endl;
    f_out_script << "resyn2" << endl;
    f_out_script << "read_library verify.lib" << endl;
    f_out_script << "map -a" << endl;
    f_out_script << "write_verilog beyond_after_flatten_circuit.v" << endl;
    f_out_script << "quit" << endl;
    f_out_script.close();

    system("./abc -f beyond_mix.script");

}