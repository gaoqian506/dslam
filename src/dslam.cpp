#include "dslam.h"

#include <stdio.h>						// sprintf
#include <iostream>						// std::cout
#include <opencv2/highgui/highgui.hpp>	// cv::VideoCapture
#include <opencv2/imgproc/imgproc.hpp>	// cv::cvtColor
#include <opencv2/video/video.hpp>		// cv::calcOpticalFlowFarneback

namespace dslam {

Status status = {
	"temp/flow"	// flowDir
};



void makeFlowFromVideo(const char* video_name) {

    cv::VideoCapture capture(video_name);
    if(!capture.isOpened()) {
		perror("Error");
		return;
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
			saveFlow(flow, queryFlowName(times-1, times));
		}
		times++;
	}
}

void saveFlow(cv::Mat& flow, const char* name) {

	FILE* file = NULL;
	file = fopen(name, "wb");
	if (!file) {
		perror("Unable to open file");
		return;
	}

	FlowHeader header = {
		flow.cols, 
		flow.rows,
		flow.type(),
		flow.step[1]
	};
	
	fwrite(&header, sizeof(header), 1, file);
	assert(flow.isContinuous());
	fwrite(flow.data, flow.step[0]*flow.rows, 1, file);
	fclose(file);
	std::cout << "Write flow to file:" << name << std::endl;
}

void readFlow(const char* name, cv::Mat& flow) {


	FILE* file = NULL;
	file = fopen(name, "rb");
	if (!file) {
		perror(name);
		return;
	}
	FlowHeader header;
	fread(&header, sizeof(header), 1, file);

	if (flow.cols != header.cols ||
		flow.rows != header.rows ||
		flow.type() != header.type ||
		flow.step[1] != header.step ||
		!flow.isContinuous()) {
		flow = cv::Mat(header.rows, header.cols,
			header.type);
	}

	fread(flow.data, flow.cols*flow.step[1],
		1, file);
	std::cout << "Read flow from file:" << name << std::endl;


	fclose(file);

}

const char* queryFlowName(int from, int to) {

	sprintf(status.tempFlowName, "%s/flow_%d_%d.flw",
		status.flowDir, from, to);
	return status.tempFlowName;

}

}
