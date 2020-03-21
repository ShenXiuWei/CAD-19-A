#ifndef _DATATYPE_
#define _DATATYPE_
#include"../glucose/core/glucose.h"
#include<string>
#include<vector>
using namespace std;
void DecideUpLimit(int, int, vector<int> &, vector<int> &, int, vector<int>);
void BuildDummyInfo(char*, char*, vector<string> &, int &, int &, int &);
void CheckConstOutput(char*, int &, int &, vector<string> &, vector<int> &);
string PatternsGeneration(char*,vector< pair<int,int> >&,vector< pair<int,int> >&,vector< pair<int,int> >&,vector<int>&, vector<int> &, vector<int> &, vector<string>, int, int, int, int);
string SATOptimization(string, vector<int>, int);
void ABCOptimization(char*, int, int);
int  ModifyVerilogFormat(char*, int);
void WriteBlifHeader(int, int, vector<string>, char*);
void WriteBlifEnd(char*);
void WriteBlif_Constant(int, int, vector<string>, int, int, char*);
void WriteBlif(string, char*);
void FixNetwork(char*, char*, char*, int, int, int, vector<string>, int, vector< vector<int> > &);
void MixVerilog(char*, vector<string>, int, int, vector< vector<int> > &);
long int Timing_function(int , int , time_t, vector<int> &, int);
#endif
