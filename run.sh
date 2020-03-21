case_num=$1
run_num=$2

if [ -z $1 ]; then
    case_num="1"
fi
if [ -z $2 ]; then
    run_num="1"
fi
for i in $(seq 1 $run_num);
do
    time ./lrg benchmark/beta_publish/case$case_num/io_info.txt benchmark/beta_publish/case$case_num/iogen result/circuit$case_num.v
#    time ./verify1 benchmark/beta_publish/case$case_num/iogen benchmark/beta_publish/case$case_num/io_info.txt circuit$case_num.v.blif 100000
#	time ./verify benchmark/beta_publish/case$case_num/iogen benchmark/beta_publish/case$case_num/io_info.txt circuit$case_num.v 1000
	time ./verify_ncverilog benchmark/beta_publish/case$case_num/iogen benchmark/beta_publish/case$case_num/io_info.txt result/circuit$case_num.v 100000
done

