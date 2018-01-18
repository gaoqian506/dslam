#include <stdio.h>		// perror
#include <iostream>		// std::cout
#include <unistd.h> 	// getcwd()


#include "dslam.h"


int main(int argc, char** argv) {

	char buffer[1024];
	if (getcwd(buffer, 1024)) {
		std::cout << "cwd:" << buffer << std::endl;
	}
	std::cout << argv[0] << std::endl;
	if (argc < 2) {
		perror("Please specify a video file");
		return -1;
	}

	dslam::makeFlowFromVideo(argv[1]);

	std::cout << "hello flow" << std::endl;
}
