
#include <iostream>
#include <opencv2/highgui/highgui.hpp>


int main(int argc, char** argv) {

	const char* file_name = "data/videos/720.mp4";

    cv::VideoCapture cap(file_name);

    if(!cap.isOpened()) {
		std::cout << "file not found:" << file_name << std::endl;
	return -1;
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

*/

/*
	open video
	foreach frame {
		calc flow();
		if (step = N) {
			save flow double side;
		}
	}

*/

	std::cout << "hello flow" << std::endl;	
}
