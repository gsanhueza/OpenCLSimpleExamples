all: info original vector minimal

info:
	g++ device_info.cpp -o device_info -lOpenCL -std=c++11

original:
	g++ main_original.cpp -o main_original -lOpenCL -std=c++11

vector:
	g++ main_vector.cpp -o main_vector -lOpenCL -std=c++11

minimal:
	g++ main_minimal.cpp -o main_minimal -lOpenCL -std=c++11

clean:
	rm -vf main_original main_vector main_minimal device_info
