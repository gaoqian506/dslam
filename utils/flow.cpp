
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <unistd.h>



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

	cv::Mat frame;
	while(capture.read(frame)) {
		// calc flow
		// save flow with fixed step
	}
	

	std::cout << "hello flow" << std::endl;	
}

/*




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






