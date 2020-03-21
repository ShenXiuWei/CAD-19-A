#include "datatype.h"
#include "satopt.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <utility>
#include <unordered_map>
#include <map>
#include <math.h>
using namespace::std;

#define DEBUG 0
#define DEBUG_DUMMY 0

string Int2Binary(int ConverInt){
    string ConverString = "";
    while(ConverInt != 0){
        ConverString = (ConverInt % 2 == 0 ? "0" : "1") + ConverString;
        ConverInt /= 2;
    }
    return ConverString;
}
bool sort_func(pair<int, int> i, pair<int, int> j){  return (i.second > j.second); }


string Order_back(vector<pair <int,int> > &inputOrder, string pat){
    string re_str(pat);
    
    int input_size = pat.size();

    for(int i = 0; i < input_size; i++){
      re_str[inputOrder[i].first] = pat[i];
    }
    #if DEBUG
    cout<<"cur ordered : "<<endl;
    for(int i = 0; i < input_size; i++){
      cout<<inputOrder[i].first<<" ";
    }
    cout<<endl;
    cout<<"og_re : "<<pat<<endl;
    cout<<"ba : "<<re_str<<endl;
    #endif
    
    return re_str;
  
}

string Reorder(vector<pair <int,int> > &inputOrder, string pat){
    int input_size = pat.size();
	
    #if DEBUG
    cout<<"og order : "<<endl;
    for(int i = 0; i < input_size; i++){
      cout<<inputOrder[i].first<<" ";
    }
    cout<<endl;
    for(int i = 0; i < input_size; i++){
      cout<<inputOrder[i].second<<" ";
    }
    cout<<endl;
    #endif
	
    for(int i = 0; i < input_size; i++){
      if(pat[inputOrder[i].first] == '1')
        inputOrder[i].second = -inputOrder[i].second;
    }
    sort(inputOrder.begin(),inputOrder.end(),sort_func);
    string re_str = "";
    
    
    for(int i = 0; i < input_size; i++){
      re_str += pat[inputOrder[i].first];
    }
    
    #if DEBUG
    cout<<"re-ordered : "<<endl;
    for(int i = 0; i < input_size; i++){
      cout<<inputOrder[i].first<<" ";
    }
    cout<<endl;
    for(int i = 0; i < input_size; i++){
      cout<<inputOrder[i].second<<" ";
    }
    cout<<endl;
    cout<<"og : "<<pat<<endl;
    cout<<"re : "<<re_str<<endl;
    #endif
    
    //Turn back the count since it won't be used again 
    for(int i = 0; i < input_size; i++){
      if(pat[inputOrder[i].first] == '1')
        inputOrder[i].second = -inputOrder[i].second;
    }
    
    return re_str;

}

string SATOptimization(string infile, vector<int> dummyPI, int output_index){
    cout << "//--------------------------//" << endl;
    cout << "//       Optimization       //" << endl;
    cout << "//--------------------------//" << endl;

    #if DEBUG_DUMMY
    cout << "Dummy PI:" << endl;
    for(int i = 0; i < dummyPI.size(); i++){
        cout << dummyPI[i] << " ";
    }
    cout << "\n" << endl;
    #endif

    /////////////////////////////////////
    //    read file from simulation    //
    /////////////////////////////////////

    ifstream f_in;
    int input_num = 0;
    int output_num = 0;
    int pattern_num = 0;
    f_in.open(infile.c_str(), ios::in);
    if(!f_in){
        cout << ">> ERROR: The input file " << infile.c_str() << " cannot be open." << endl;
        exit(-1);
    }

    f_in >> input_num >> output_num >> pattern_num;
    vector<string> io_names;
    io_names.clear();
    io_names.resize(0);

    // unordered_map for data
    unordered_map<string, string> patterns(pattern_num);
    
    string temp = "";
    for(int i = 0; i < input_num + output_num; i++){
        f_in >> temp;
        if (find(dummyPI.begin(), dummyPI.end(), i) == dummyPI.end()){
            io_names.push_back(temp);
        }
    }

    // zeros and ones for reordering
    int real_PI_num = input_num - dummyPI.size();
    vector<int> ones_in_onset(real_PI_num,0);
    vector<int> ones_in_offset(real_PI_num,0);
    vector<int> zeros_in_onset(real_PI_num,0);
    vector<int> zeros_in_offset(real_PI_num,0);
    vector<pair <int,int> > inputOrder;
    
    for(int i = 0; i < pattern_num; i++){
        string temp2 = "";
        string temp_inputpat = "";
        int k = 0;
        for(int j = 0; j < input_num + output_num; j++){
            f_in >> temp2;

            // collect the input_pattern
            if (find(dummyPI.begin(), dummyPI.end(), j) == dummyPI.end() && j < input_num){
                temp_inputpat += temp2;
                //patterns[i][k++] = temp2;
            }
            // collect the output bit, and insert the input and output pattern into the map
            if (j == input_num + output_index){
                patterns.insert(make_pair(temp_inputpat, temp2));
                for(int id = 0; id < real_PI_num; id++){
                    if(temp2.compare("1") == 0){//onset
                      if(temp_inputpat[id] == '1')
                        ones_in_onset[id]++;
                      else if (temp_inputpat[id] == '0')
                        zeros_in_onset[id]++;
                    }
                    else{//offset
                      if(temp_inputpat[id] == '1')
                        ones_in_offset[id]++;
                      else if (temp_inputpat[id] == '0')
                        zeros_in_offset[id]++;
                    
                    }
                }
            }
                
        }
    }
    f_in.close();
    for(int id = 0; id < real_PI_num ;id++)
        inputOrder.push_back(make_pair(id,(ones_in_onset[id] + zeros_in_offset[id]) - (zeros_in_onset[id] + ones_in_offset[id])));

    input_num -= dummyPI.size();

    // if (output_index == 2) {
    //     ofstream ffout;
    //     ffout.open("test_for_patttern", ios::out);
    //     for (int i = 0; i < io_names.size(); i++){
    //         ffout << io_names[i] << " ";
    //     }
    //     ffout << endl;
    //     for (auto& pattern : patterns){
    //         ffout << pattern.first << " " << pattern.second << endl;
    //     }
    //     ffout.close();
    // }

    /////////////////////
    //     MINIMIZE    //
    /////////////////////
    vector<string> result_patterns;
    unordered_set<string> check_redundant_patterns;
    for (auto& pattern : patterns){
        string result_temp = "";
        
        //  for each pattern that the output bit of it is "1"
        if (pattern.second == "1"){
            /**  speed up  **/
            bool skip = false;    // use to detect: if this pattern foramt has already appear in the results, needn't to handle it
            for (auto& r_pattern : result_patterns){
                skip = true;
                for (int j = 0; j < r_pattern.size()-1; j++){
                    if (r_pattern[j] != '-' && r_pattern[j] != pattern.first[j]){
                        skip = false;
                        break;
                    }
                }
                if (skip)
                    break;
            }
            if (skip){
                //cout << "DETECTDETECT !!" << endl;
                continue;
            }

            
            //string input_pat(pattern.first);
            //string dc_string(pattern.first);
            // reordering
            string input_pat(pattern.first);
			if (real_PI_num != 0) 
				input_pat = Reorder(inputOrder, pattern.first);
            string dc_string(input_pat);
            vector<int> dc_pos;
            int dc_count = 0;
            char record_bit;
            
            for (int i = 0; i < input_pat.size(); i++){
                record_bit = dc_string[i];  // for speed up
                dc_string[i] = '-';

                /** scan through the dc_string, count the dc and record their positions **/
                for (int j = 0; j < dc_string.size(); j++){
                    if (dc_string[j] == '-'){
                        dc_count++;
                        dc_pos.push_back(j);    // record the position of dc_bit
                    } 
                }
                    
                /** replace th right position of a bit to the right value(0 or 1) **/ 
                long long int dc_range = pow(2, dc_count);  // dc_range : whole search space for possibie dc bits, ex: 2-bits dc position -> 4 possibilities (0, 1, 2, 3)
                bool fail = false;
                for (long long int j = 0; j < dc_range; j++){
                    /**  for speed up  **/
                    if (record_bit == '0' && j % 2  == 0){
                        continue;
                    }            
                    if (record_bit == '1' && j % 2  == 1){
                        continue;
                    }

                    string bin(Int2Binary(j));  // Integer -> Binary
                    while(bin.size() < dc_count){
                        bin = "0" + bin;    // ex: 0's binary is 0, but sometimes we want to a format like "00" or "000" 
                    }

                    int cnt = 0;    // index for selecting a bit in Bin string    
                    string query_pat(dc_string);  // query_pat will be the combination of (dc_string + replaced possible dc bits)
                                                  // ex : -10-0 -> 01000 or 01010 or 11000 or 11010 (four query_pat possibilities)

                    for (int k = 0; k < dc_string.size(); k++){
                        int dc = dc_pos[cnt]; // indicate the positions of the dc-bits (the places that possibly can be minimize)  
                        if (k == dc && cnt < bin.size()){
                            query_pat[k] = bin[cnt];
                            cnt++;
                        }
                    }
                    
					string re_query_pat(query_pat);
					if (real_PI_num != 0){
						re_query_pat = Order_back(inputOrder, query_pat);
                    }
					unordered_map<string, string>::const_iterator got = patterns.find(re_query_pat);
                    
					
                    
                    //unordered_map<string, string>::const_iterator got = patterns.find(query_pat);
                    if (got != patterns.end() && got->second == "0"){
                        fail = true;
                        break;             
                    }
                }              
                dc_count = 0;
                dc_pos.clear();
 
                /** if fail, dc_string must recover to its original value(ex: -0101111 -> 00101111) **/
                if (fail){
                    dc_string[i] = input_pat[i];
                    continue;
                }
            }
			if(real_PI_num != 0){
				string re_dc_string = Order_back(inputOrder, dc_string);
				result_temp = re_dc_string + "1";
			}else{
				result_temp = dc_string + "1";
			}
            // if (!check_redundant_patterns.count(result_temp)){
            //     check_redundant_patterns.insert(result_temp);
                result_patterns.push_back(result_temp);
            // }
        }
    }
    
    // if (output_index == 2){
    //     ofstream fffout;
    //     fffout.open("test_for_result_patttern", ios::out);
    //     for (int i = 0; i < io_names.size(); i++){
    //         fffout << io_names[i] << " ";
    //     }
    //     fffout << endl;
    //     for (auto& result : result_patterns){
    //         fffout << result << endl;
    //     }
    //     fffout.close();
    // }
    

    ///////////////////////////////
    //        write file         //
    ///////////////////////////////
    ofstream f_out;
    string outfile = "beyond_to_abcOpt.txt";
    f_out.open(outfile.c_str(), ios::out);
    if(!f_out){
        cout << ">> ERROR: The output file " << outfile << " cannot be open." << endl;
        exit(-1);
    }
    if (input_num == 0 || output_num == 0)
        return outfile;
    else {
        f_out << input_num << " " << 1 << " " << result_patterns.size() << endl;
        for (int i = 0; i < input_num; i++) {
            f_out << io_names[i] << " ";
        }
        f_out << io_names[input_num + output_index] << endl;

        //  each patterns
        for (int i = 0; i < result_patterns.size(); i++) {
            for (int j = 0; j < input_num + 1; j++) {
                f_out << result_patterns[i][j];
                if (j != input_num)
                    f_out << " ";
            }
            f_out << endl;
        }
        f_out.close();
        return outfile;
    }
}
