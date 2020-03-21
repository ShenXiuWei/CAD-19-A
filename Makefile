all:
	/usr/local/bin/g++ -O3 -lz -o ./lrg glucose/core/*.cc glucose/utils/*.cc glucose/simp/SimpSolver.cc src/dummyinfo.cpp src/pattern.cpp src/classifyoutput.cpp src/satopt.cpp src/abcopt.cpp src/main.cpp src/minimize.cpp src/modifyverilog.cpp  src/writeblif.cpp src/fixnetwork.cpp src/mixverilog.cpp src/TimingFunc.cpp
	/usr/local/bin/g++ -O3 -lz -o ./verify_ncverilog src/verify_ncverilog.cpp	
clean:
	rm -f ./lrg ./verify ./verify1 ./verify_ncverilog
rm:
	rm -f beyond_*
