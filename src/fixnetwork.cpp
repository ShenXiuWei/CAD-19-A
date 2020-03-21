#include "datatype.h"
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include "time.h"
#include <math.h>
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <unordered_map>
using namespace std;


int combinator(int n, int r) {
    int counter = 0;

    vector<bool> v(n);
    std::fill(v.end() - r, v.end(), true);

    do {
        counter++;
    } while (std::next_permutation(v.begin(), v.end()));

    return counter;
}

void printRes(int* a, bool* index, int n, vector<boost::dynamic_bitset<> >& iogen_input_patterns, int output_index, vector< vector<int> > &realPI_to_PO, int input_num, int two_order, unordered_map<string , int> &ppp)
{
    string testpattern_str;
    unordered_map<string, int>::iterator iter;
    boost::dynamic_bitset<> x(input_num);
    boost::dynamic_bitset<> temp(realPI_to_PO[output_index].size());
    for(int i = 0; i < n; i++)
    {
        if(index[i])
        {
            x[realPI_to_PO[output_index][i]] = 1;
            temp[i] = 1;
        }
    }

    to_string(x, testpattern_str);
    if((iter = ppp.find(testpattern_str)) != ppp.end())
    {
    }  
    else
    {
        ppp[testpattern_str] = atoi(testpattern_str.c_str());
        iogen_input_patterns.push_back(x);
    }

    x.flip();

    to_string(x, testpattern_str);
    if((iter = ppp.find(testpattern_str)) != ppp.end())
    {
    }  
    else
    {
        ppp[testpattern_str] = atoi(testpattern_str.c_str());
        iogen_input_patterns.push_back(x);
    }
    

    if(two_order == 1)
    {
        boost::dynamic_bitset<> y(input_num);
        for(int i = 0; i < realPI_to_PO[output_index].size(); i++)
        {
            y[realPI_to_PO[output_index][realPI_to_PO[output_index].size() - 1 - i]] = temp[i];
        }

        to_string(y, testpattern_str);
        if((iter = ppp.find(testpattern_str)) != ppp.end())
        {
        }
        else
        {
            ppp[testpattern_str] = atoi(testpattern_str.c_str());
            iogen_input_patterns.push_back(y);
        }

        y.flip();

        to_string(y, testpattern_str);
        if((iter = ppp.find(testpattern_str)) != ppp.end())
        {
        } 
        else
        {
            ppp[testpattern_str] = atoi(testpattern_str.c_str());
            iogen_input_patterns.push_back(y);
        }
    }

}

bool hasDone(bool* index, int n, int k)
{
    for(int i = n - 1; i >= n - k; i--)
    {
        if(!index[i])
        {
            return false;
        }
    }
    return true;
}

void Comb(int* a, int n, int k, vector<boost::dynamic_bitset<> >& iogen_input_patterns, int pattern_up_limit, int output_index, vector< vector<int> > &realPI_to_PO, int input_num, int two_order, unordered_map<string , int> &ppp)
{
    int pattern_size = 0;
    int break_loop = 0;
    bool *index = new bool[n]();
    for(int i = 0; i < k; i++)
    {
        index[i] = true;
    }
    
    printRes(a, index, n, iogen_input_patterns, output_index, realPI_to_PO, input_num, two_order, ppp);
    pattern_size += 2;
    
    while(!hasDone(index, n, k))
    {
        for(int i = 0; i < n - 1; i++)
        {
            if(index[i] && !index[i + 1])
            {
                index[i] = false;
                index[i + 1] = true;

                int count = 0;
                for(int j = 0; j < i; j++)
                {
                    if(index[j])
                    {
                        index[j] = false;
                        index[count++] = true;
                    }
                }
                if(pattern_size < pattern_up_limit)
                {
                    printRes(a, index, n, iogen_input_patterns, output_index, realPI_to_PO, input_num, two_order, ppp);
                    pattern_size += 2;
                }
                else
                {
                    break_loop = 1;
                }
                break;
            }
        }
        if(break_loop == 1)
        {
            break;
        }
    }
    delete[] index;
}
void CreatePattern(int n, int k, int round, vector<boost::dynamic_bitset<> >& iogen_input_patterns, int pattern_up_limit, int output_index, vector< vector<int> > &realPI_to_PO, int input_num){
    int over_limit = 0;
    unordered_map<string , int> ppp;
    for(int j = 0; j < round; j++)
    {
        long int result;
        if(over_limit == 0)
        {
            result = combinator(n, k);
        }
        
        int two_order = 0;
        if(result > pattern_up_limit)
        {
            two_order = 1;
            over_limit = 1;
        }
        int *a = new int[n]();
        for(int i = 0; i < n; i++)
        {
            a[i] = i + 1;
        }
        Comb(a, n, k, iogen_input_patterns, pattern_up_limit, output_index, realPI_to_PO, input_num, two_order, ppp);
        delete[] a;
        k++;
    }

    //Random extreme pattern
    for(int i = 0; i < pattern_up_limit; i++)
    {
        boost::dynamic_bitset<> z1(input_num);
        for(int j = 0; j < realPI_to_PO[output_index].size(); j++)
        {
            if(rand() % 100 >= 95)
                z1[realPI_to_PO[output_index][j]] = 1;
            else
                z1[realPI_to_PO[output_index][j]] = 0;
        }

        string testpattern_str;
        unordered_map<string, int>::iterator iter;
        to_string(z1, testpattern_str);
        if((iter = ppp.find(testpattern_str)) != ppp.end())
        {
            i--;
        }
        else
        {
            ppp[testpattern_str] = atoi(testpattern_str.c_str());;
            iogen_input_patterns.push_back(z1);
            
            z1.flip();
            to_string(z1, testpattern_str);
            ppp[testpattern_str] = atoi(testpattern_str.c_str());;
            iogen_input_patterns.push_back(z1);
        }
    }

    //Random pattern
    srand(time(NULL));
    for(int i = 0; i < pattern_up_limit; i++)
    {
        boost::dynamic_bitset<> z(input_num);
        for(int j = 0; j < realPI_to_PO[output_index].size(); j++)
        {
            if(rand() % 2 == 1)
                z[realPI_to_PO[output_index][j]] = 1;
            else
                z[realPI_to_PO[output_index][j]] = 0;
        }

        string testpattern_str;
        unordered_map<string, int>::iterator iter;
        to_string(z, testpattern_str);
        if((iter = ppp.find(testpattern_str)) != ppp.end())
        {
            i--;
        }
        else
        {
            ppp[testpattern_str] = atoi(testpattern_str.c_str());;
            iogen_input_patterns.push_back(z);

            z.flip();
            to_string(z, testpattern_str);
            ppp[testpattern_str] = atoi(testpattern_str.c_str());;
            iogen_input_patterns.push_back(z);
        }
    }

    ppp.clear();
}

void FixNetwork(char* io_info, char* iogen, char* circuit_name, int output_index, int input_num, int output_num, vector<string> io_names, int pattern_up_limit, vector< vector<int> > &realPI_to_PO){
    //pattern_up_limit = 5000;
    int num_01_base = 0;
    int cngetm_n = realPI_to_PO[output_index].size();
    int cngetm_m = cngetm_n / 2;
    if(cngetm_m > cngetm_n)
    {
        cngetm_m = cngetm_n;
    }
    //remaining_time = 3600 - remaining_time;

    //  2.  Create pattern
    //  3.  Write fix_in_pat.txt
    //  5.  system("~~~"); --
    //  4.  Write testbench.v
    //  5.  system(~~~~~)
    //  6.  Iogen
    //  7.  Read Read fixnetwork_verify_log.txt
    //  8.  Read fix_io_rel.txt
    //  9.  Compare

    cout << "//--------------------------//" << endl;
    cout << "//       fix network        //" << endl;
    cout << "//--------------------------//" << endl;
    
    //============================================================//
    //                   2.  Create pattern                       //
    //============================================================//

    vector<boost::dynamic_bitset<> > iogen_input_patterns;
    CreatePattern(cngetm_n, num_01_base, cngetm_m, iogen_input_patterns, pattern_up_limit, output_index, realPI_to_PO, input_num);
    long int pattern_num = iogen_input_patterns.size();
    cout << "PO_" << output_index << "  Total number of test pattern: " << pattern_num << endl;    
    cout << "PO_" << output_index << "  Pattern up_limit: " << pattern_up_limit << endl;

    vector<boost::dynamic_bitset<> > iogen_output_patterns(pattern_num, boost::dynamic_bitset<>(output_num));
    vector<boost::dynamic_bitset<> > sis_output_patterns(pattern_num, boost::dynamic_bitset<>(output_num));

    //============================================================//
    //                3.  Write fix_in_pat.txt                    //
    //============================================================//
    ofstream f_out_in_pat;
    f_out_in_pat.open("beyond_fix_in_pat.txt", ios::out);
    f_out_in_pat << input_num << " " << pattern_num << endl;
    for(int i = 0; i < input_num - 1; i++)
    {
        f_out_in_pat << io_names[i] << " ";
    }
    f_out_in_pat << io_names[input_num - 1] << endl;
    for(int i = 0; i < pattern_num; i++)
    {
        for(int j = 0; j < input_num - 1; j++)
        {
            f_out_in_pat << iogen_input_patterns[i][j] << " ";
        }
        f_out_in_pat << iogen_input_patterns[i][input_num - 1] << endl;
    }
    f_out_in_pat.close();
    //============================================================//
    //                       4.  testbench.v                      //
    //============================================================//
    ofstream f_out_fix_network;
    f_out_fix_network.open("beyond_fixnetwork_testbench.v", ios::out);
    f_out_fix_network << "module stimulus_fixnetwork;" << endl;
    f_out_fix_network << "\treg ";
    for(int i = 0; i < input_num - 1; i++)
    {
        f_out_fix_network << io_names[i] << ", ";
    }
    f_out_fix_network << io_names[input_num - 1] << ";" << endl << "\twire ";
    for(int i = input_num; i < input_num + output_num - 1; i++)
    {
        f_out_fix_network << io_names[i] << ", ";
    }
    f_out_fix_network << io_names[input_num + output_num - 1] << ";" << endl << "\treg[" << input_num + output_num - 1 << ":0] tmp;" << endl << "\tinteger fp_r, fp_w, i, temp;" << endl;
    f_out_fix_network << "\tmain_circuit main_1(" << endl;
    for(int i = 0; i < input_num; i++)
    {
        f_out_fix_network << "\t\t." << io_names[i] << "(" << io_names[i] << ")," << endl;
    }
    for(int i = input_num; i < input_num + output_num - 1; i++)
    {
        f_out_fix_network << "\t\t." << io_names[i] << "_main(" << io_names[i] << ")," << endl;
    }
    f_out_fix_network << "\t\t." << io_names[input_num + output_num - 1] << "_main(" << io_names[input_num + output_num - 1] << ")" << endl;
    f_out_fix_network << "\t);" << endl << endl << "\tinitial begin" << endl;
    f_out_fix_network << "\t\tfp_r = $fopen(\"beyond_fix_in_pat.txt\", \"r\");" << endl;
    f_out_fix_network << "\t\tfp_w = $fopen(\"beyond_fixnetwork_verify_log.txt\", \"w\");" << endl;
    f_out_fix_network << "\t\tfor(i = 0; i < 2; i = i + 1) begin" << endl << "\t\t\ttmp = $fgetc(fp_r);" << endl << "\t\t\twhile(tmp != 10) begin" << endl << "\t\t\t\ttmp = $fgetc(fp_r);" << endl << "\t\t\tend" << endl << "\t\tend" << endl;
    f_out_fix_network << "\t\twhile(!$feof(fp_r)) begin" << endl << "\t\t\ttemp = $fscanf(fp_r, \"";
    for(int i = 0; i < input_num; i++)
    {
        f_out_fix_network << "%d ";
    }
    f_out_fix_network << "\"";
    for(int i = 0; i < input_num; i++)
    {
        f_out_fix_network << ", " << io_names[i];
    }
    f_out_fix_network << ");" << endl << "\t\t\t#1;" << endl << "\t\t\t$fwrite(fp_w, \"";
    for(int i = 0; i < output_num; i++)
    {
        f_out_fix_network << "%d ";
    }
    f_out_fix_network << "\\n\"";
    for(int i = input_num; i < input_num + output_num; i++)
    {
        f_out_fix_network << ", " <<io_names[i];
    }
    f_out_fix_network << ");" << endl << "\t\tend" << endl << "\t\t$fclose(fp_r);" << endl << "\t\t$fclose(fp_w);" << endl << "\tend" << endl << "endmodule";
    f_out_fix_network.close();
    //============================================================//
    //     5.  system("~~~");     //
    //============================================================//
    cout<<"step 5"<<endl;
    string ss_ncverilog = "( ncverilog beyond_fixnetwork_testbench.v beyond_before_fix_circuit.v +access+r ) >&beyond_error_before.txt";
    system(ss_ncverilog.c_str());
    //============================================================//
    //                        6.  Iogen                           //
    //============================================================//
    std::string IOGEN(iogen);
    cout<<"starting iogen\n";
    string ss = "./" + IOGEN + " beyond_fix_in_pat.txt beyond_fix_io_rel.txt";
    system(ss.c_str());
    //============================================================//
    //          7.  Read fixnetwork_verify_log.txt                //
    //============================================================//
    ifstream f_in_verify_log;
    f_in_verify_log.open("beyond_fixnetwork_verify_log.txt", ios::in);
    if(!f_in_verify_log){
        cout << ">> ERROR: The input file beyond_fixnetwork_verify_log.txt cannot be open." << endl;
        exit(-1);
    }
    string temp = "";
    for(int i = 0; i < pattern_num; i++)
    {
        for(int j = 0; j < output_num; j++)
        {
            f_in_verify_log >> temp;
            if(temp == "1")
            {
                sis_output_patterns[i][j] = 1;
            }
            else
            {
                sis_output_patterns[i][j] = 0;
            }
        }
    }
    f_in_verify_log.close();

    //============================================================//
    //                  8.  Read fix_io_rel.txt                //
    //============================================================//
    ifstream f_in_fix_io_rel;
    f_in_fix_io_rel.open("beyond_fix_io_rel.txt", ios::in);
    if(!f_in_fix_io_rel){
        cout << ">> ERROR: The input file beyond_fix_io_rel.txt cannot be open." << endl;
        exit(-1);
    }
    f_in_fix_io_rel >> temp >> temp >> temp;
    for(int i = 0; i < input_num + output_num; i++)
    {
        f_in_fix_io_rel >> temp;
    }
    for(int i = 0; i < pattern_num; i++)
    {
        for(int k = 0; k < input_num; k++)
        {
            f_in_fix_io_rel >> temp;
        }
        for(int j = 0; j < output_num; j++)
        {
            f_in_fix_io_rel >> temp;
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
    f_in_fix_io_rel.close();

    //============================================================//
    //                       9.  Compare                          //
    //============================================================//
    vector<boost::dynamic_bitset<> > fault0_pattern;
    vector<boost::dynamic_bitset<> > fault1_pattern;

    for(int i = 0; i < pattern_num; i++)
    {
        if(iogen_output_patterns[i][output_index] == 1 && sis_output_patterns[i][output_index] == 0)
        {
            fault0_pattern.push_back(iogen_input_patterns[i]);
        }
        else if(iogen_output_patterns[i][output_index] == 0 && sis_output_patterns[i][output_index] == 1)
        {
            fault1_pattern.push_back(iogen_input_patterns[i]);
        }    
    }
    cout << "Number of fault0: " << fault0_pattern.size() << endl;
    cout << "Number of fault1: " << fault1_pattern.size() << endl;
    vector<vector<string> > fault0_pattern_str;
    vector<vector<string> > fault1_pattern_str;
    vector<string> temp_fault_pattern_str;
    string temp_fault = "";

    if(fault0_pattern.size() > 0)
    {
        for(int i = 0; i < fault0_pattern.size(); i++)
        {
            temp_fault_pattern_str.clear();
            temp_fault_pattern_str.resize(0);
            for(int j = 0; j < input_num ; j++)
            {
                int check_dc = 0;
                for(int k = 0; k < realPI_to_PO[output_index].size(); k++)
                {
                    if(j == realPI_to_PO[output_index][k])
                    {
                        check_dc++;
                    }
                }
                if(check_dc == 0)
                {
                    temp_fault = "-";
                }
                else
                {
                    if(fault0_pattern[i][j] == 0)
                    {
                        temp_fault = "0";
                    }
                    else if(fault0_pattern[i][j] == 1)
                    {
                        temp_fault = "1";
                    }
                    
                }
                temp_fault_pattern_str.push_back(temp_fault);
            }
            fault0_pattern_str.push_back(temp_fault_pattern_str);
        }
    }

    if(fault1_pattern.size() > 0)
    {
        for(int i = 0; i < fault1_pattern.size(); i++)
        {
            temp_fault_pattern_str.clear();
            temp_fault_pattern_str.resize(0);
            for(int j = 0; j < input_num; j++)
            {
                int check_dc = 0;
                for(int k = 0; k < realPI_to_PO[output_index].size(); k++)
                {
                    if(j == realPI_to_PO[output_index][k])
                    {
                        check_dc++;
                    }
                }
                if(check_dc == 0)
                {
                    temp_fault = "-";
                }
                else
                {
                    if(fault1_pattern[i][j] == 0)
                    {
                        temp_fault = "0";
                    }
                    else if(fault1_pattern[i][j] == 1)
                    {
                        temp_fault = "1";
                    }
                }
                temp_fault_pattern_str.push_back(temp_fault);
            }
            fault1_pattern_str.push_back(temp_fault_pattern_str);
        }
    }

    string fault0_blif_name = "beyond_PO" + to_string(output_index) + "fault0.blif";
    string fault1_blif_name = "beyond_PO" + to_string(output_index) + "fault1.blif";

    if(fault0_pattern.size() > 0)
    {
        ofstream f_out;
        f_out.open(fault0_blif_name.c_str(), ios::out);
        f_out << ".model submodule" << output_index << "fault0" << endl;
        f_out << ".inputs";
        for(int i = 0; i < input_num; i++)
        {
            f_out << " " << io_names[i];
        }
        f_out << endl << ".outputs " << io_names[input_num + output_index] << "_wrong1" << endl;
        f_out << ".names";
        for(int i = 0; i < input_num; i++)
        {
            f_out << " " << io_names[i];
        }
        f_out << " " << io_names[input_num + output_index] << "_wrong1" << endl;
        for(int i = 0; i < fault0_pattern.size(); i++)
        {
            for(int j = 0; j < input_num; j++)
            {
                f_out << fault0_pattern_str[i][j];
            }
            f_out << " 1" << endl;
        }
        f_out << ".end";
        f_out.close();
        ABCOptimization(circuit_name, output_index, 0);
    }
    
    if(fault1_pattern.size() > 0)
    {
        ofstream f_out;
        f_out.open(fault1_blif_name.c_str(), ios::out);
        f_out << ".model submodule" << output_index << "fault1" << endl;
        f_out << ".inputs";
        for(int i = 0; i < input_num; i++)
        {
            f_out << " " << io_names[i];
        }
        f_out << endl << ".outputs " << io_names[input_num + output_index] << "_wrong0" << endl;
        f_out << ".names";
        for(int i = 0; i < input_num; i++)
        {
            f_out << " " << io_names[i];
        }
        f_out << " " << io_names[input_num + output_index] << "_wrong0" << endl;
        for(int i = 0; i < fault1_pattern.size(); i++)
        {
            for(int j = 0; j < input_num; j++)
            {
                f_out << fault1_pattern_str[i][j];
            }
            f_out << " 0" << endl;
        }
        f_out << ".end";
        f_out.close();
        ABCOptimization(circuit_name, output_index, 1);
    }
    
    vector<boost::dynamic_bitset<> > release_memory;
    iogen_input_patterns.swap(release_memory);
    iogen_output_patterns.swap(release_memory);
    sis_output_patterns.swap(release_memory);
    fault0_pattern.swap(release_memory);
    fault1_pattern.swap(release_memory);

}
