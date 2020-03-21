NUMS="1 2 3 4 5 6 7 8 9 10"

for case_num in $NUMS
do
	echo "-------case_num = $case_num-------"
	./run.sh $case_num
	make rm
done
