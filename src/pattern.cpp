#include "datatype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstdlib> 
#include <sstream> 
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
using namespace::std;


#define NOT_DUMMY_UP_LIMIT 30
#define LAST_GENERATION_NUMBER   500000
#define BASE_NUMBER 1000
#define DEBUG 0

string  IOGEN;
string  dummy_info;

vector<string> io_names;
int            input_num;
int            output_num;
int            my_output_index;

int            unsure_count;
int            up_limit;
vector<bool>   PI_state;        // 0 not dummy  1 dummy
vector<bool>   GATE_state;
vector<int>    PI_state_count;  // counter of the times of being different from base
vector<string> last_vs;
vector<int>    dummy_PI;
vector<int>    real_PI;
int dummy_two_origin_size;


vector< pair<int, int> > dummy_two_00;
vector< pair<int, int> > dummy_two_01;
vector< pair<int, int> > dummy_two_origin;

struct PI
{
    int index;
    double ratio;
};
struct Pattern
{
    boost::dynamic_bitset<> input;
    boost::dynamic_bitset<> output;
};
struct Group
{
    vector<struct Pattern> member;      // v[0] base of the group
};
struct ID_PAIR
{   
    bool alive;
    pair<int, int> pa;
};
bool operator<(const  PI &a, const PI &b)
{   
    if(a.ratio != b.ratio)
        return a.ratio > b.ratio;
    else
        return a.index > b.index;
}
string toBinary(int n)
{
    std::string r;
    while(n!=0)
    {
        r=(n%2==0 ?"0":"1")+r; n/=2;
    }
    return r;
}
void read_info()
{   
    int pi_count = 0;
    int gate_count = 0;
    ifstream fin("beyond_DummyInfo.log");
    if(!fin)
        cout << "open fail." << endl;
    else
    {   
        set<struct PI> S; 
        S.clear();
        string str_buf;
        int i ,j;
        for(i = 0; i != output_num + 1; ++i)
        {   
            getline(fin, str_buf);
            if(i!=0 && i == my_output_index + 1)
            {
                istringstream stream(str_buf);
                for(j=0; j!= input_num + 2; j++)
                {   
                    if(j == 0)
                    {
                        getline(stream, str_buf, ',');
                        //cout<<"0 : "<<str_buf<<endl;
                    }
                    if(j >= 1 && j != input_num + 1)
                    {
                        PI p;
                        getline(stream, str_buf, ',');
                        p.index = j-1;
                        p.ratio = atof(str_buf.c_str());
                        if(p.ratio > 0)
                        {
                            S.insert(p);
                        }
                        //cout<<j<<" : "<<str_buf<<endl;
                    }
                    if(j == input_num + 1)
                    {
                        getline(stream, str_buf, ',');
                        cout<<"num of RealPI : "<<str_buf<<endl;
                    }
                }
            }
        }

        fin.close(); 

        cout<<"up_limit : "<<up_limit<<endl;   

        std::set<struct PI>::iterator iter1 = S.begin();
        if(S.size() > up_limit)
        {   
            advance(iter1, up_limit); 
            std::set<struct PI>::iterator iter = S.erase(iter1, S.end());
        }     

        for(int i=0; i!=input_num; ++i)
        {
            for(std::set<struct PI>::iterator it = S.begin(); it != S.end(); ++it)
            {
                if(i == it->index)
                {
                    PI_state[i] = 0;
                    pi_count++;
                }   
            }
        }
    }

    ifstream fin_ex("beyond_DummyInfo_ex.log");
    if(!fin_ex)
        cout << "open fail." << endl;
    else
    {   
        set<struct PI> S; 
        S.clear();
        string str_buf;
        int i ,j;
        for(i = 0; i != output_num + 1; ++i)
        {   
            getline(fin_ex, str_buf);
            if(i!=0 && i == my_output_index + 1)
            {
                istringstream stream(str_buf);
                for(j=0; j!= input_num + 2; j++)
                {   
                    if(j == 0)
                    {
                        getline(stream, str_buf, ',');
                        //cout<<"0 : "<<str_buf<<endl;
                    }
                    if(j >= 1 && j != input_num + 1)
                    {
                        PI p;
                        getline(stream, str_buf, ',');
                        p.index = j-1;
                        p.ratio = atof(str_buf.c_str());
                        if(p.ratio > 0)
                        {
                            S.insert(p);
                        }
                        //cout<<j<<" : "<<str_buf<<endl;
                    }
                    if(j == input_num + 1)
                    {
                        getline(stream, str_buf, ',');
                        cout<<"num of RealPI ex: "<<str_buf<<endl;
                    }
                }
            }
        }

        fin_ex.close(); 

        for(std::set<struct PI>::iterator it = S.begin(); it != S.end(); ++it)
        {
            real_PI.push_back(it->index);
        }
        sort(real_PI.begin(), real_PI.end());
    }
}
void Last_Generation(int seed)
{
    //==================================================================================
    //      Generate last input patterns and write them to in_pat.txt
    //==================================================================================
    vector<string> v_s;
    v_s.clear();
    v_s.resize(0);
    dummy_PI.clear();

    for(int i = 0; i < input_num; i++)
    {   
        if(PI_state[i] == 1)
            dummy_PI.push_back(i);
    }

    cout<<"my_output_index : "<<my_output_index<<"\t";
    cout<<"Real  PI : "<<up_limit<<"\t";
    cout<<"Dummy PI : "<<input_num - up_limit<<endl;

    ofstream f_out_in_pat;
    f_out_in_pat.open("beyond_in_pat.txt", ios::out);
    if (!f_out_in_pat)
    {
        cout << "ERROR: The input file in_pat.txt cannot be open." << endl;
        exit(-1);
    }
    if( up_limit < NOT_DUMMY_UP_LIMIT)
    {
        if(DEBUG)cout<< "WHOLE TRUTH TABLE"<<endl;

        unsigned long long int max_value = pow(2, up_limit);
        for(unsigned long long int i = 0 ; i < max_value ; i++)
        {
            string s = toBinary(i);

            int need = up_limit - s.length();
            for(int j = 0; j < need; j++ )
            {
                s = "0" + s;
            }

            string temp_s = "";
            int index = up_limit - 1;
            for(int j = 0; j < input_num; j++ )
            {  
                if(PI_state[j] == 1)
                {
                    temp_s = temp_s + "1";
                }
                else
                {
                    temp_s = temp_s + s[index];
                    index--;
                }
                if(j != input_num - 1)
                    temp_s = temp_s + " ";
            }
            v_s.push_back(temp_s);
         }

        cout<<"SIZE : "<<v_s.size()<<endl;
        f_out_in_pat << input_num << " " << v_s.size() << endl;
        for(int i = 0; i < input_num; i++)
        {
            f_out_in_pat << io_names[i];
            if(i != input_num - 1)
                f_out_in_pat<<" ";
            else
                f_out_in_pat<<endl;
        }

        for(int i = 0; i< v_s.size(); i++)
        {
            f_out_in_pat<< v_s[i] <<endl;
        }
        f_out_in_pat.close();
    }
    
}
string PatternsGeneration(char* iogen, vector<pair<int, int>> &main_dummy_two_00, vector<pair<int, int>> &main_dummy_two_01, vector<pair<int, int>> &main_dummy_two_both, vector<int> &main_dummy_two_none, vector<int> &main_realPI, vector<int> &main_dummyPI, vector<string> main_io_names, int main_input_num, int main_output_num, int main_output_index, int main_up_limit)
{
    main_dummyPI.clear();
    main_dummyPI.resize(0);
    main_realPI.clear();
    main_realPI.resize(0);
    real_PI.clear();
    real_PI.resize(0);
    PI_state.clear();
    PI_state.resize(0);
    GATE_state.clear();
    GATE_state.resize(0);
    PI_state_count.clear();
    PI_state_count.resize(0);
    dummy_two_00.clear();
    dummy_two_00.resize(0);
    dummy_two_01.clear();
    dummy_two_01.resize(0);
    dummy_two_origin.clear();
    dummy_two_origin.resize(0);
    main_dummy_two_00.clear();
    main_dummy_two_00.resize(0);
    main_dummy_two_01.clear();
    main_dummy_two_01.resize(0);
    main_dummy_two_both.clear();
    main_dummy_two_both.resize(0);
    main_dummy_two_none.clear();
    main_dummy_two_none.resize(0);
    IOGEN.assign(iogen);
    io_names = main_io_names;
    input_num = main_input_num;
    output_num = main_output_num;
    my_output_index = main_output_index;
    up_limit = main_up_limit;

    cout<<"//--------------------------//"<<endl<<"//       Simulation         //"<<endl<<"//--------------------------//"<<endl;
    if(input_num > 0)
    {
        for(int i = 0; i < input_num; i++)
        {
            PI_state.push_back(true);
            GATE_state.push_back(true);
            PI_state_count.push_back(0);
        }
        srand( time(NULL) );

        read_info();

        Last_Generation(rand());

        string ss =  "./" + IOGEN + " beyond_in_pat.txt beyond_to_satOpt.txt" ;
        system(ss.c_str());


        main_dummyPI = dummy_PI;
        main_realPI = real_PI;
    }
    
    return "beyond_to_satOpt.txt";
}