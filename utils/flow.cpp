#include <stdio.h>							// sprintf
#include <iostream>							// std::cout
#include <opencv2/highgui/highgui.hpp>		// cv::VideoCapture
#include <opencv2/imgproc/imgproc.hpp>		// cv::cvtColor
#include <opencv2/video/video.hpp>			// cv::calcOpticalFlowFarneback
#include <unistd.h> 						// getcwd()


void save_flow(cv::Mat& flow, int prev_id, int next_id) {


	char file_name[256];
	sprintf(file_name, "temp/flow/flow_%d_%d.flw", prev_id, next_id);

	FILE* file = NULL;
	file = fopen(file_name, "wb");
	if (!file) {
		perror("Error");
		std::cout << "Unable to open file:" << file_name << std::endl;
		return;
	}
	int size_type[3] = { flow.cols, flow.rows, flow.type() };
	fwrite(size_type, sizeof(size_type), 1, file);
	for (int i = 0; i < flow.rows; i++) {
		fwrite(flow.ptr(i), flow.step[0], 1, file);
	}
	fclose(file);
	std::cout << "Write flow to file:" << file_name << std::endl;

}


int main(int argc, char** argv) {

	char buffer[1024];
	if (getcwd(buffer, 1024)) {
		std::cout << "cwd:" << buffer << std::endl;
	}
	std::cout << argv[0] << std::endl;
	if (argc < 2) { 
		std::cout << "Please specify a video file.\n";
		return -1;
	}

    cv::VideoCapture capture(argv[1]);
    if(!capture.isOpened()) {
		std::cout << "file not found:" << argv[1] << std::endl;
		return -1;
	}

	cv::Mat prev, next, frame, gray;
	cv::Mat flow;
	int times = 0;
	while(capture.read(frame) && times < 5) {

		cv::cvtColor(frame, gray, CV_BGR2GRAY);
		if (prev.empty()) {
			prev = gray;
		}
		else {
			next = gray;
			cv::calcOpticalFlowFarneback(prev, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
			save_flow(flow, times-1, times);
		}
		times++;
	}
	

	std::cout << "hello flow" << std::endl;	
}

/*

	
			//write_ptr = fopen("test.bin","wb");  // w for write, b for binary
			//fwrite(buffer,sizeof(buffer),1,write_ptr); // write 10 bytes from our buffer

			//std::cout << "calcOpticalFlowFarneback(" << times << ")" << std::endl;
			//save(flow);

	for (int i = 0; i < 5; i++) {


    Mat edges;
    namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        imshow("edges", edges);
        if(waitKey(30) >= 0) break;
    }


	open video
	foreach frame {
		calc flow();
		if (step = N) {
			save flow double side;
		}
	}


	const char* file_name = NULL;
	if (argc > 1) {
		file_name = ;
	}
	else {
		file_name = "data/videos/720.mp4";
	}


	char buffer[1024];
	if (getcwd(buffer, 1024)) {
		std::cout << "cwd:" << buffer << std::endl;
	}

*/






