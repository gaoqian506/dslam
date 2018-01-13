
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
//#include <unistd.h> // getcwd()



int main(int argc, char** argv) {

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
	while(capture.read(frame) && times++ < 5) {

		cv::cvtColor(frame, gray, CV_BGR2GRAY);
		if (prev.empty()) {
			prev = gray;
		}
		else {
			next = gray;
			cv::calcOpticalFlowFarneback(prev, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
			std::cout << "calcOpticalFlowFarneback(" << times << ")" << std::endl;	
			//save(flow);
		}
	}
	

	std::cout << "hello flow" << std::endl;	
}

/*

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






