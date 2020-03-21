#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<iostream>
#include <iomanip>
#include<stdlib.h>
#include<string.h>
#include <sstream>
#include <string>
#include "time.h"
#include <math.h>
#include <boost/dynamic_bitset.hpp>
#include <unordered_map>
using namespace::std;

#define TEST_PATTERNS_0 10000
#define TEST_PATTERNS_1 10000
#define RANDOM_PROBABILITY_01 90

template <class T> 
std::string ConvertToString(T);
int main(int argc, char * argv[]){
    //  1.  Read io_info.txt
    //  2.  Create pattern
    //  3.  Write verify_in_pat.txt
    //  4.  Write before_fix_testbench.v for temp_circuit.v
    //  5.  Write after_fix_testbench.v for circuit.v
    //  6.  system("ncverilog before_fix_testbench.v temp_circuit.v +access+r");
    //  7.  system("ncverilog after_fix_testbench.v circuit.v +access+r");
    //  8.  Iogen
    //  9.  Read before_fix_verify_log.txt
    //  10.  Read after_fix_verify_log.txt
    //  11.  Read verify_io_rel.txt
    //  12.  Compare

    double pattern_num = atoi(argv[4]);
    int input_num = 0;
    int output_num = 0;
    vector<string> io_names;
    io_names.clear();
    io_names.resize(0);
    cout << "//--------------------------//" << endl;
    cout << "//       Verification       //" << endl;
    cout << "//--------------------------//" << endl;

    //============================================================//
    //                  1.  Read io_info.txt                      //
    //============================================================//
    string temp_io_name = "";
    ifstream f_in_io_info;
    f_in_io_info.open(argv[2], ios::in);
    if (!f_in_io_info) 
    {
		cout << "ERROR: The input file " << argv[2] << " cannot be open." << endl;
		exit(-1);
	}
    f_in_io_info >> input_num >> output_num;
    for(int i = 0; i < input_num + output_num; i++)
    {
        f_in_io_info >> temp_io_name;
        io_names.push_back(temp_io_name);
    }
    f_in_io_info.close();

    //============================================================//
    //                   2.  Create pattern                       //
    //============================================================//
    vector<boost::dynamic_bitset<> > iogen_input_patterns(pattern_num, boost::dynamic_bitset<>(input_num));
    vector<boost::dynamic_bitset<> > iogen_output_patterns(pattern_num, boost::dynamic_bitset<>(output_num));
    vector<boost::dynamic_bitset<> > before_fix_output_patterns(pattern_num, boost::dynamic_bitset<>(output_num));
    vector<boost::dynamic_bitset<> > after_fix_output_patterns(pattern_num, boost::dynamic_bitset<>(output_num));

    string testpattern_str;
    unordered_map<string , int> ppp;
    unordered_map<string, int>::iterator iter;

    srand(time(NULL));
    for(int i = 0; i < pattern_num - TEST_PATTERNS_0 - TEST_PATTERNS_1; i++){
        for(int j = 0; j < input_num; j++)
    	{
    		if(rand() % 2 == 1)
    			iogen_input_patterns[i][j] = 1;
    		else
    			iogen_input_patterns[i][j] = 0;
    	}

        to_string(iogen_input_patterns[i], testpattern_str);
        if((iter = ppp.find(testpattern_str)) != ppp.end())
            i--;   
        else
            ppp[testpattern_str] = i;
    }
    for(int i = pattern_num - TEST_PATTERNS_0 - TEST_PATTERNS_1; i < pattern_num - TEST_PATTERNS_0; i++)
    {
        for(int j = 0; j < input_num; j++)
        {
            if(rand() % 100 >= RANDOM_PROBABILITY_01)
                iogen_input_patterns[i][j] = 0;
            else
                iogen_input_patterns[i][j] = 1;
        }

        to_string(iogen_input_patterns[i], testpattern_str);
        if((iter = ppp.find(testpattern_str)) != ppp.end())
            i--;
        else
            ppp[testpattern_str] = i;
    }
    for(int i = pattern_num - TEST_PATTERNS_0; i < pattern_num; i++)
    {
        for(int j = 0; j < input_num; j++)
        {
            if(rand() % 100 >= RANDOM_PROBABILITY_01)
                iogen_input_patterns[i][j] = 1;
            else
                iogen_input_patterns[i][j] = 0;
        }

        to_string(iogen_input_patterns[i], testpattern_str);
        if((iter = ppp.find(testpattern_str)) != ppp.end())
            i--;
        else
            ppp[testpattern_str] = i;
    }

    //============================================================//
    //                3.  Write verify_in_pat.txt                 //
    //============================================================//
    string pattern_num_s = argv[4];
    ofstream f_out_in_pat;
    f_out_in_pat.open("beyond_verify_in_pat.txt", ios::out);
    f_out_in_pat << input_num << " " << pattern_num_s << endl;
    for(int i = 0; i < input_num - 1; i++)
    {
        f_out_in_pat << io_names[i] << " ";
    }
    f_out_in_pat << io_names[input_num - 1] << endl;
    for(int i = 0; i < pattern_num; i++)
    {
        for(int j = input_num - 1; j >= 1; j--)
        {
            f_out_in_pat << iogen_input_patterns[i][j] << " ";
        }
        f_out_in_pat << iogen_input_patterns[i][0] << endl;
    }
    f_out_in_pat.close();

    //============================================================//
    //     4.  Write before_fix_testbench.v for temp_circuit.v    //
    //        5.  Write after_fix_testbench.v for circuit.v       //
    //============================================================//
    ofstream f_out_before_fix_testbench;
    ofstream f_out_after_fix_testbench;
    f_out_before_fix_testbench.open("beyond_before_fix_testbench.v", ios::out);
    f_out_after_fix_testbench.open("beyond_after_fix_testbench.v", ios::out);
    f_out_before_fix_testbench << "module stimulus_before;" << endl;
    f_out_after_fix_testbench << "module stimulus_after;" << endl;
    f_out_before_fix_testbench << "\treg ";
    f_out_after_fix_testbench << "\treg ";
    for(int i = 0; i < input_num - 1; i++)
    {
        f_out_before_fix_testbench << io_names[i] << ", ";
        f_out_after_fix_testbench << io_names[i] << ", ";
    }
    f_out_before_fix_testbench << io_names[input_num - 1] << ";" << endl << "\twire ";
    f_out_after_fix_testbench << io_names[input_num - 1] << ";" << endl << "\twire ";
    for(int i = input_num; i < input_num + output_num - 1; i++)
    {
        f_out_before_fix_testbench << io_names[i] << ", ";
        f_out_after_fix_testbench << io_names[i] << ", ";
    }
    f_out_before_fix_testbench << io_names[input_num + output_num - 1] << ";" << endl << "\treg[" << input_num + output_num - 1 << ":0] tmp;" << endl << "\tinteger fp_r, fp_w, i, temp;" << endl;
    f_out_after_fix_testbench << io_names[input_num + output_num - 1] << ";" << endl << "\treg[" << input_num + output_num - 1 << ":0] tmp;" << endl << "\tinteger fp_r, fp_w, i, temp;" << endl;
    f_out_before_fix_testbench << "\tmain_circuit main_1(" << endl;
    f_out_after_fix_testbench << "\ttop top_1(" << endl;
    for(int i = 0; i < input_num; i++)
    {
        f_out_before_fix_testbench << "\t\t." << io_names[i] << "(" << io_names[i] << ")," << endl;
        f_out_after_fix_testbench << "\t\t." << io_names[i] << "(" << io_names[i] << ")," << endl;
    }
    for(int i = input_num; i < input_num + output_num - 1; i++)
    {
        f_out_before_fix_testbench << "\t\t." << io_names[i] << "_main(" << io_names[i] << ")," << endl;
        f_out_after_fix_testbench << "\t\t." << io_names[i] << "(" << io_names[i] << ")," << endl;
    }
    f_out_before_fix_testbench << "\t\t." << io_names[input_num + output_num - 1] << "_main(" << io_names[input_num + output_num - 1] << ")" << endl;
    f_out_after_fix_testbench << "\t\t." << io_names[input_num + output_num - 1] << "(" << io_names[input_num + output_num - 1] << ")" << endl;
    f_out_before_fix_testbench << "\t);" << endl << endl << "\tinitial begin" << endl;
    f_out_after_fix_testbench << "\t);" << endl << endl << "\tinitial begin" << endl;
    f_out_before_fix_testbench << "\t\tfp_r = $fopen(\"beyond_verify_in_pat.txt\", \"r\");" << endl;
    f_out_after_fix_testbench << "\t\tfp_r = $fopen(\"beyond_verify_in_pat.txt\", \"r\");" << endl;
    f_out_before_fix_testbench << "\t\tfp_w = $fopen(\"beyond_before_fix_verify_log.txt\", \"w\");" << endl;
    f_out_after_fix_testbench << "\t\tfp_w = $fopen(\"beyond_after_fix_verify_log.txt\", \"w\");" << endl;
    f_out_before_fix_testbench << "\t\tfor(i = 0; i < 2; i = i + 1) begin" << endl << "\t\t\ttmp = $fgetc(fp_r);" << endl << "\t\t\twhile(tmp != 10) begin" << endl << "\t\t\t\ttmp = $fgetc(fp_r);" << endl << "\t\t\tend" << endl << "\t\tend" << endl;
    f_out_after_fix_testbench << "\t\tfor(i = 0; i < 2; i = i + 1) begin" << endl << "\t\t\ttmp = $fgetc(fp_r);" << endl << "\t\t\twhile(tmp != 10) begin" << endl << "\t\t\t\ttmp = $fgetc(fp_r);" << endl << "\t\t\tend" << endl << "\t\tend" << endl;
    f_out_before_fix_testbench << "\t\twhile(!$feof(fp_r)) begin" << endl << "\t\t\ttemp = $fscanf(fp_r, \"";
    f_out_after_fix_testbench << "\t\twhile(!$feof(fp_r)) begin" << endl << "\t\t\ttemp = $fscanf(fp_r, \"";
    for(int i = 0; i < input_num; i++)
    {
        f_out_before_fix_testbench << "%d ";
        f_out_after_fix_testbench << "%d ";
    }
    f_out_before_fix_testbench << "\"";
    f_out_after_fix_testbench << "\"";
    for(int i = 0; i < input_num; i++)
    {
        f_out_before_fix_testbench << ", " << io_names[i];
        f_out_after_fix_testbench << ", " << io_names[i];
    }
    f_out_before_fix_testbench << ");" << endl << "\t\t\t#1;" << endl << "\t\t\t$fwrite(fp_w, \"";
    f_out_after_fix_testbench << ");" << endl << "\t\t\t#1;" << endl << "\t\t\t$fwrite(fp_w, \"";
    for(int i = 0; i < output_num; i++)
    {
        f_out_before_fix_testbench << "%d ";
        f_out_after_fix_testbench << "%d ";
    }
    f_out_before_fix_testbench << "\\n\"";
    f_out_after_fix_testbench << "\\n\"";
    for(int i = input_num; i < input_num + output_num; i++)
    {
        f_out_before_fix_testbench << ", " <<io_names[i];
        f_out_after_fix_testbench << ", " <<io_names[i];
    }
    f_out_before_fix_testbench << ");" << endl << "\t\tend" << endl << "\t\t$fclose(fp_r);" << endl << "\t\t$fclose(fp_w);" << endl << "\tend" << endl << "endmodule";
    f_out_after_fix_testbench << ");" << endl << "\t\tend" << endl << "\t\t$fclose(fp_r);" << endl << "\t\t$fclose(fp_w);" << endl << "\tend" << endl << "endmodule";

    f_out_before_fix_testbench.close();
    f_out_after_fix_testbench.close();

    //================================================================================//
    //     6.  system("ncverilog before_fix_testbench.v temp_circuit.v +access+r");   //
    //     7.  system("ncverilog after_fix_testbench.v circuit.v +access+r");         //
    //================================================================================//
    std::string CASE_NAME(argv[3]);

    // string ss_before = "( ./icarus/bin/iverilog -o test.vvp before_fix_testbench.v verify_" + CASE_NAME + " ) >&error.txt";
    // system(ss_before.c_str());
    // string ss_before_vvp = "( ./icarus/bin/vvp test.vvp ) >&error.txt";
    // system(ss_before_vvp.c_str());

    // string ss_after = "( ./icarus/bin/iverilog -o test.vvp after_fix_testbench.v " + CASE_NAME + " ) >&error.txt";
    // system(ss_after.c_str());
    // string ss_after_vvp = "( ./icarus/bin/vvp test.vvp ) >&error.txt";
    // system(ss_after_vvp.c_str());
    
    string ss_before = "( ncverilog beyond_before_fix_testbench.v beyond_before_fix_circuit.v +access+r ) >&beyond_error.txt";
    string ss_after = "( ncverilog beyond_after_fix_testbench.v " + CASE_NAME + " +access+r ) >&beyond_error.txt";
    system(ss_before.c_str());
    system(ss_after.c_str());

    //============================================================//
    //                        8.  Iogen                           //
    //============================================================//
    std::string IOGEN(argv[1]);
    string ss = "./" + IOGEN + " beyond_verify_in_pat.txt beyond_verify_io_rel.txt";
    system(ss.c_str());

    //============================================================//
    //           9.  Read before_fix_verify_log.txt               //
    //            10.  Read after_fix_verify_log.txt              //
    //============================================================//
    ifstream f_in_before_fix_verify_log;
    ifstream f_in_after_fix_verify_log;
    f_in_before_fix_verify_log.open("beyond_before_fix_verify_log.txt", ios::in);
    f_in_after_fix_verify_log.open("beyond_after_fix_verify_log.txt", ios::in);
    if(!f_in_before_fix_verify_log){
        cout << ">> ERROR: The input file beyond_before_fix_verify_log.txt cannot be open." << endl;
        exit(-1);
    }
    if(!f_in_after_fix_verify_log){
        cout << ">> ERROR: The input file beyond_after_fix_verify_log.txt cannot be open." << endl;
        exit(-1);
    }
    string before_fix_temp = "";
    string after_fix_temp = "";
    for(int i = 0; i < pattern_num; i++)
    {
        for(int j = 0; j < output_num; j++)
        {
            f_in_before_fix_verify_log >> before_fix_temp;
            f_in_after_fix_verify_log >> after_fix_temp;
            if(before_fix_temp == "1")
            {
                before_fix_output_patterns[i][j] = 1;
            }
            else
            {
                before_fix_output_patterns[i][j] = 0;
            }
            if(after_fix_temp == "1")
            {
                after_fix_output_patterns[i][j] = 1;
            }
            else
            {
                after_fix_output_patterns[i][j] = 0;
            }
        }
    }
    f_in_before_fix_verify_log.close();
    f_in_after_fix_verify_log.close();

    //============================================================//
    //                 11.  Read verify_io_rel.txt                //
    //============================================================//
    ifstream f_in_verify_io_rel;
    string temp = "";
    f_in_verify_io_rel.open("beyond_verify_io_rel.txt", ios::in);
    if(!f_in_verify_io_rel){
        cout << ">> ERROR: The input file beyond_verify_io_rel.txt cannot be open." << endl;
        exit(-1);
    }
    f_in_verify_io_rel >> temp >> temp >> temp;
    for(int i = 0; i < input_num + output_num; i++)
    {
        f_in_verify_io_rel >> temp;
    }
    for(int i = 0; i < pattern_num; i++)
    {
        for(int k = 0; k < input_num; k++)
        {
            f_in_verify_io_rel >> temp;
        }
        for(int j = 0; j < output_num; j++)
        {
            f_in_verify_io_rel >> temp;
            if(temp == "1")
            {
                iogen_output_patterns[i][j] = 1;
            }
            else
            {
                iogen_output_patterns[i][j] = 0;
            }
        }
    }
    f_in_verify_io_rel.close();

    //============================================================//
    //                       12.  Compare                         //
    //============================================================//
    double before_fix_bits_accuracy[output_num] = {0};
    double before_fix_bits_correct_result[output_num] = {0};
    double after_fix_bits_accuracy[output_num] = {0};
    double after_fix_bits_correct_result[output_num] = {0};
    for (int i = 0; i < pattern_num; i++)
    {
        for(int j = 0; j < output_num; j++)
        {
            if(before_fix_output_patterns[i][j] == iogen_output_patterns[i][j])
            {
                before_fix_bits_correct_result[j]++;
            }
            if(after_fix_output_patterns[i][j] == iogen_output_patterns[i][j])
            {
                after_fix_bits_correct_result[j]++;
            }
        }
    }
    double before_fix_accuracy = 0;
    double before_fix_correct_result = 0;
    double after_fix_accuracy = 0;
    double after_fix_correct_result = 0;
    for(int i = 0; i < pattern_num; i++){
        if(before_fix_output_patterns[i] == iogen_output_patterns[i])
        {
            before_fix_correct_result++;
        }
        if(after_fix_output_patterns[i] == iogen_output_patterns[i])
        {
            after_fix_correct_result++;
        }
    }
    before_fix_accuracy = before_fix_correct_result / pattern_num;
    after_fix_accuracy = after_fix_correct_result / pattern_num;
    for(int i = 0; i < output_num; i++)
    {
        before_fix_bits_accuracy[i] = before_fix_bits_correct_result[i] / pattern_num;
        after_fix_bits_accuracy[i] = after_fix_bits_correct_result[i] / pattern_num;
    }

    vector<string> before_fix_print_str;
    before_fix_print_str.clear();
    before_fix_print_str.resize(0);
    vector<string> after_fix_print_str;
    after_fix_print_str.clear();
    after_fix_print_str.resize(0);
    string sss = "";
    sss = "|   Correct: " + ConvertToString(before_fix_correct_result);
    before_fix_print_str.push_back(sss);
    sss = "|   Correct: " + ConvertToString(after_fix_correct_result);
    after_fix_print_str.push_back(sss);
    sss = "|   Inorrect: " + ConvertToString((pattern_num - before_fix_correct_result));
    before_fix_print_str.push_back(sss);
    sss = "|   Inorrect: " + ConvertToString((pattern_num - after_fix_correct_result));
    after_fix_print_str.push_back(sss);
    for(int i = 0; i < output_num; i++)
    {
        sss = "|   Bit " + ConvertToString(i) + "  Accuracy: " + ConvertToString((before_fix_bits_accuracy[i] * 100));
        before_fix_print_str.push_back(sss);
        sss = "|   Bit " + ConvertToString(i) + "  Accuracy: " + ConvertToString((after_fix_bits_accuracy[i] * 100));
        after_fix_print_str.push_back(sss);
    }
    sss = "|   Circuit accuracy: " + ConvertToString(before_fix_accuracy * 100) + "%";
    before_fix_print_str.push_back(sss);
    sss = "|   Circuit accuracy: " + ConvertToString(after_fix_accuracy * 100) + "%";
    after_fix_print_str.push_back(sss);

    cout << "-------------------------------------------------------------------" << endl;
    cout << "|                     Random " << pattern_num_s << " patterns                      |" << endl;
    cout << "-------------------------------------------------------------------" << endl;
    cout << setiosflags(ios::left) << setw(33) << "|   Before fix network" << setw(33) << "|   After fix network" << setw(5) << "|" << resetiosflags(ios::left) << endl;
    
    cout << "-------------------------------------------------------------------" << endl;
    for(int i = 0; i < output_num + 3; i++)
    {
        cout << setiosflags(ios::left) << setw(33) << before_fix_print_str[i] << setw(33) << after_fix_print_str[i] << setw(5) << "|"<< resetiosflags(ios::left) << endl;
    }
    cout << "-------------------------------------------------------------------" << endl;

    //system("rm *.blif* *testbench* *.script *.txt *.v");
}

template <class T> 
    std::string ConvertToString(T value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}
