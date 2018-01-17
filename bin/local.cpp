
#include <iostream>
#include <opencv2/core/core.hpp>
#include <stdio.h>

cv::Mat read_flow(const char* file_name) {

	FILE* file = NULL;
	file = fopen(file_name, "rb");
	if (!file) {
		perror("Error");
		std::cout << "Unable to open file:" << file_name << std::endl;
		return;
	}
	int size_type[3];

	fread(size_type, sizeof(size_type), 1, file);

	cv::Mat flow = cv::Mat(size_type[0], 
		size_type[1], size_type[2]);
	fread(size_type, sizeof(size_type), 1, file);

	 = { flow.cols, flow.rows, flow.type() };
	fwrite(size_type, sizeof(size_type), 1, file);
	for (int i = 0; i < flow.rows; i++) {
		fwrite(flow.ptr(i), flow.step[0], 1, file);
	}
	fclose(file);

	return cv::Mat();

}


int main(int argc, char** argv) {

	std::cout << "hello local" << std::endl;	
	//while (true) {
	
	char file_name[256];
	for (int i = 0; i < 4; i++) {

		sprintf(file_name, "temp/flow/flow_%d_%d.flw", 
			i, i+1);
		cv::Mat frame = read_flow(file_name);
		
		// read the frame
		// add into frame graph
		// local BA
		// optimize graph

	}
	

}
