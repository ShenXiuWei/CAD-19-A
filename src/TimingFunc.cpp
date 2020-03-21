#include<iostream>
#include<vector>
#include<time.h>


#define DEBUG 1
using namespace std;

long int Timing_function(int output_index, int not_truth_PO_num, time_t start_clk, vector<int> &timing_info, int gatecnt)
{
	
	int TIME_LIMIT = 2000;
	int alt_time_limit = 2500;
	double PI_num_ratio = (double)timing_info[1]/(double)timing_info[0];	 // get this output should get how much ratio of time
	#if DEBUG 
	cout<<"timing_info[0]: "<<timing_info[0]<<endl;
	cout<<"timing_info[1]: "<<timing_info[1]<<endl;
	cout<<"PI num ratio: "<<PI_num_ratio<<endl;
	#endif
	timing_info[0] = timing_info[0] - timing_info[1]; // update combined total number of real PI
	int remaining_time = 0;
	if (gatecnt > 200){
		remaining_time = TIME_LIMIT - difftime(time(NULL), start_clk); //remaining_time
	}else {
		remaining_time = alt_time_limit - difftime(time(NULL), start_clk); //remaining_time
	}
	double mag = 1;
	if(gatecnt < 60 && PI_num_ratio == 1 && timing_info[1] < 40){
		mag = 2;
		cout << " Mag "<<endl;
	}
		
	
	if (remaining_time < 0) {
		cout<<"running out of time: "<<remaining_time<<endl;
		return 0;
	}
	long int full_capacity = remaining_time * PI_num_ratio * ((timing_info[2] *166) /(timing_info[2] / 60 + 10000) * mag) ; // how much patterns could this output get
	
	if (full_capacity < 0 ){//exceed long int
		full_capacity = 2000000000;
		cout<<"exceeded\n";
	}
	
	long int up_limit = full_capacity*0.8 / (timing_info[1]/2 + 2) / 2;
	
	
	if (PI_num_ratio == 1 && timing_info[1] > 69 && timing_info[1] < 72 && up_limit > 110000){
		up_limit = 110000;
	}
	#if DEBUG 
	cout<<"remaining time: "<<remaining_time<<endl;
	cout<<"full_capacity: "<<full_capacity<<endl;
	cout<<"UP limit: "<<up_limit<<endl;
	#endif
	return up_limit;
	
	
}
