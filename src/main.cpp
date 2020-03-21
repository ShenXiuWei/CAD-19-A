#include "datatype.h"
#include<iostream>
#include<string>
#include<vector>
#include<time.h>
#include<fstream>
#include<utility>
#include<iostream>
#include<bits/stdc++.h>
using namespace std;

int main(int argc, char * argv[]) {

    time_t start_clk, stop_clk, before_fix_stop_clk, dummy_stop_clk, build_circuit_stop_clk, fixnetwork_stop_clk, abc_flatten_stop_clk;
    vector< vector<int> > realPI_to_PO;
    vector<int> dummyPI, classifyPO, up_limit, not_truth_table_PO, realPI;
    vector<string> io_names;    
    vector< pair<int,int> > dummy_two_00, dummy_two_01, dummy_two_both;
    vector<int> dummy_two_none;
    int input_num = 0, output_num = 0, pattern_per_min = 0;
    string simResult, optResult;
    
    start_clk = time(NULL);
    BuildDummyInfo(argv[1], argv[2], io_names, input_num, output_num, pattern_per_min);
    cout<<"Patterns Per Min : "<<pattern_per_min<<endl;

    CheckConstOutput(argv[2], input_num, output_num, io_names, classifyPO);
    WriteBlifHeader(input_num, output_num, io_names, argv[3]);
    DecideUpLimit(input_num, output_num, up_limit, not_truth_table_PO, pattern_per_min, classifyPO);
    dummy_stop_clk = time(NULL);

    sort(not_truth_table_PO.begin(), not_truth_table_PO.end());
    cout << "Not truth table: ";
    for(int i = 0; i < not_truth_table_PO.size(); i++)
    {
        cout << not_truth_table_PO[i] << " ";
    }
    cout << endl;

    for(int i = 0; i < output_num; i++)
    {
        if(classifyPO[i] == 0)
        {
            WriteBlif_Constant(input_num, output_num, io_names, i, 0, argv[3]);
            realPI.clear();
            realPI.resize(0);
            for(int j = 0; j < input_num; j++)
            {
                realPI.push_back(j);
            }
            realPI_to_PO.push_back(realPI);
        }
        else if(classifyPO[i] == 1)
        {
            WriteBlif_Constant(input_num, output_num, io_names, i, 1, argv[3]);
            realPI.clear();
            realPI.resize(0);
            for(int j = 0; j < input_num; j++)
            {
                realPI.push_back(j);
            }
            realPI_to_PO.push_back(realPI);
        }
        else
        {   
            simResult = PatternsGeneration(argv[2],dummy_two_00, dummy_two_01, dummy_two_both, dummy_two_none, realPI, dummyPI, io_names, input_num, output_num, i, up_limit[i]);
            realPI_to_PO.push_back(realPI);
            optResult = SATOptimization(simResult, dummyPI, i);
            WriteBlif(optResult, argv[3]);
        }
    }


    WriteBlifEnd(argv[3]);
    ABCOptimization(argv[3], -1, -1);
    build_circuit_stop_clk = time(NULL);
    int gatecnt0 = ModifyVerilogFormat(argv[3], 0);
	before_fix_stop_clk = time(NULL);
    int before_fix_sec = difftime(before_fix_stop_clk, start_clk);
	cout<<"before fix time: "<< before_fix_sec<<endl;
	vector<int> timing_info(3,0);
	for(int i = 0; i < not_truth_table_PO.size(); i++){
		timing_info[0] += realPI_to_PO[not_truth_table_PO[i]].size();
		cout<<not_truth_table_PO[i]<<"   "<<realPI_to_PO[not_truth_table_PO[i]].size()<<endl;
	}
	timing_info[2] = pattern_per_min;
	cout<<"pattern_per_min: "<<pattern_per_min<<endl;
    for(int i = 0; i < output_num; i++)
    {
        for(int j = 0; j < not_truth_table_PO.size(); j++)
        {
            if(i == not_truth_table_PO[j])
            {
				cout<<"PI num: "<<realPI_to_PO[i].size()<<endl;
				timing_info[1] = realPI_to_PO[i].size();
				long int pattern_up_limit = Timing_function(i, not_truth_table_PO.size(), start_clk, timing_info, gatecnt0);
                time_t before_fix = time(NULL);
                FixNetwork(argv[1], argv[2], argv[3], i, input_num, output_num, io_names, pattern_up_limit, realPI_to_PO);
                cout<<"Fix time: "<<difftime(time(NULL), before_fix)<<endl;
                break;
            }
        }
    }
    

    fixnetwork_stop_clk = time(NULL);
    MixVerilog(argv[3], io_names, input_num, output_num, realPI_to_PO);
    abc_flatten_stop_clk = time(NULL);
    int gatecnt = ModifyVerilogFormat(argv[3], 2);
    stop_clk = time(NULL);

    cout << "Find dummy time: " << difftime(dummy_stop_clk, start_clk) << "(s)" << endl;
    cout << "Build circuit time: " << difftime(build_circuit_stop_clk, dummy_stop_clk) << "(s)" << endl;
    cout << "Fix network time: " << difftime(fixnetwork_stop_clk, before_fix_stop_clk) << "(s)" << endl;
    cout << "ABC flatten time: " << difftime(abc_flatten_stop_clk, fixnetwork_stop_clk) << "(s)" << endl;
    cout << "Total time: " << difftime(stop_clk, start_clk) << "(s)" << endl;
    cout << "Gate count: " << gatecnt << endl;
	system("rm beyond_*");
    return 0; 
}
