#include "datatype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <set>
#include <bitset>
#include <fstream>
#include <sstream>
#include <iostream>
#include "time.h"
#include <math.h>
#include <iomanip>
#include <boost/dynamic_bitset.hpp>
#include <unordered_map>
using namespace::std;

#define OUT_THREDHOLD 0.002
#define PATTERN_NUMS 200000
double pattern_num = PATTERN_NUMS;

void CheckConstOutput(char* iogen, int &input_num, int &output_num, vector<string> &io_names, vector<int> &classifyPO){
    
    //============================================================//
    //           Write beyond_classify_constant_in_pat.txt        //
    //============================================================//
    string testpattern_str;
    unordered_map<string , int> ppp;
    unordered_map<string, int>::iterator iter;
    vector<boost::dynamic_bitset<> > iogen_input_patterns(pattern_num, boost::dynamic_bitset<>(input_num));

    srand(time(NULL));
    for(double i = 0; i < pattern_num; i++){
        for(double j = 0; j < input_num; j++)
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

    ofstream f_out_in_pat;
    f_out_in_pat.open("beyond_classify_constant_in_pat.txt", ios::out);
    f_out_in_pat << input_num << " " << PATTERN_NUMS << endl;
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

    string _IOGEN;
    _IOGEN.assign(iogen);
    string ss = "./" + _IOGEN + " beyond_classify_constant_in_pat.txt beyond_classify_constant_io_rel.txt";
    system(ss.c_str());
    //============================================================
    //                   Read output_io_rel.txt                    
    //============================================================
    
    vector<double> ratio_1;
    ratio_1.clear();
    ratio_1.resize(0);

    ifstream f_in_output_io_rel;
    f_in_output_io_rel.open("beyond_classify_constant_io_rel.txt", ios::in);
    if(!f_in_output_io_rel){
        cout << ">> ERROR: The input file beyond_classify_constant_io_rel.txt cannot be open." << endl;
        exit(-1);
    }
    string temp = "";
    f_in_output_io_rel >> temp >> temp >> temp;
    vector<boost::dynamic_bitset<> > iogen_output_patterns(output_num, boost::dynamic_bitset<>(pattern_num));
    for(int i = 0; i < input_num + output_num; i++)
    {
        f_in_output_io_rel >> temp;
    }
    for(int i = 0; i < pattern_num; i++)
    {
        for(int k = 0; k < input_num; k++)
        {
            f_in_output_io_rel >> temp;
        }
        for(int j = 0; j < output_num; j++)
        {
            f_in_output_io_rel >> temp;
            if(temp == "1")
            {
                iogen_output_patterns[j][i] = 1;
            }
            else
            {
                iogen_output_patterns[j][i] = 0;
            }
        }
    }
    f_in_output_io_rel.close();
    //============================================================
    //                        Classify                   
    //============================================================
    int output_type_counter[3] = {0};
    for(int i = 0; i < output_num; i++)
    {
        ratio_1.push_back(iogen_output_patterns[i].count() / pattern_num);
        if(iogen_output_patterns[i].none() || (ratio_1[i] < 0 + OUT_THREDHOLD))
		{
			classifyPO.push_back(0);
            output_type_counter[0]++;
		}
		else if(iogen_output_patterns[i].count() == iogen_output_patterns[i].size() || (ratio_1[i] > 1 - OUT_THREDHOLD))
		{
			classifyPO.push_back(1);
            output_type_counter[1]++;
		}
		else
        {
            classifyPO.push_back(2);
            output_type_counter[2]++;
        }
    }
    //==================================================================================
    //      						Print output info
    //==================================================================================
	cout << "//--------------------------//" << endl;
    cout << "//     Output Classify      //" << endl;
    cout << "//--------------------------//" << endl;
    cout << "Output info:" << endl;
	cout << "Number of output(s):\t" << output_num << endl;
    cout << "Number of constant 0:\t" << output_type_counter[0] << endl;
    cout << "Number of constant 1:\t" << output_type_counter[1] << endl;
    cout << "Number of non-constant:\t" << output_type_counter[2] << endl;
    for(int i = 0; i < output_num; i++)
    {
        cout << "Bit " << i << " Ratio (1): " << ratio_1[i] << endl;
    }
    vector<boost::dynamic_bitset<> >().swap(iogen_output_patterns);	//	release memorys
}
