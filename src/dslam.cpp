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
	cv::Mat flow_mat;
	Flow flow;
	int times = 0;
	while(capture.read(frame) && times < 5) {

		cv::cvtColor(frame, gray, CV_BGR2GRAY);
		if (prev.empty()) {
			prev = gray;
		}
		else {
			next = gray;
			cv::calcOpticalFlowFarneback(prev, next, flow_mat, 0.5, 3, 15, 3, 5, 1.2, 0);
			flow = createFlowFromMat(flow_mat);
			saveFlow(flow, queryFlowName(times-1, times));
			releaseFlow(flow);

			cv::calcOpticalFlowFarneback(next, prev, flow_mat, 0.5, 3, 15, 3, 5, 1.2, 0);
			flow = createFlowFromMat(flow_mat);
			saveFlow(flow, queryFlowName(times, times-1));
			releaseFlow(flow);
		}
		times++;
	}
}
Flow createEmptyFlow() {
	return new FlowObj();
}
Flow createFlowFromFile(const char* name) {

	FILE* file = NULL;
	file = fopen(name, "rb");
	if (!file) {
		perror(name);
		return NULL;
	}
	FlowHeader header;
	fread(&header, sizeof(header), 1, file);
	Flow flow = createEmptyFlow();
	flow->mat = cv::Mat(header.rows, header.cols, header.type);
	fread(flow->mat.data, flow->mat.cols*flow->mat.step[1], 1, file);
	std::cout << "Read flow from file:" << name << std::endl;
	fclose(file);
	return flow;
}
Flow createFlowFromMat(const cv::Mat& mat) {
	Flow flow = createEmptyFlow();
	mat.copyTo(flow->mat);
	return flow;
}
void releaseFlow(Flow flow) {
	delete flow;
}
void saveFlow(Flow flow, const char* name) {

	FILE* file = NULL;
	file = fopen(name, "wb");
	if (!file) {
		perror(name);
		return;
	}

	FlowHeader header = {
		flow->mat.cols,
		flow->mat.rows,
		flow->mat.type(),
		flow->mat.step[1]
	};

	fwrite(&header, sizeof(header), 1, file);
	assert(flow->mat.isContinuous());
	fwrite(flow->mat.data, header.rows*header.step, 1, file);
	fclose(file);
	std::cout << "Write flow to file:" << name << std::endl;
}

const char* queryFlowName(int from, int to) {

	sprintf(status.tempFlowName, "%s/flow_%d_%d.flw",
		status.flowDir, from, to);
	return status.tempFlowName;
}

FlowGraph createEmptyFlowGraph() {

	return new FlowGraphObj();

}
void releaseFlowGraph(FlowGraph graph) {
	for(std::vector<Frame>::iterator it = graph->frameVector.begin(); it != graph->frameVector.end(); it++) {
		releaseFrame(*it);
	}
	delete graph;
}
void flowGraphAddFrameWithFlows(FlowGraph graph, Flow flow01, Flow flow10) {
	Frame frame = createEmptyFrame();
	Frame latest = flowGraphGetLatestFrame(graph);
	if (latest) {
		FrameNeighbor neighbor01 = createEmptyFrameNeighbor();
		frameNeighborSetFrame(neighbor01, frame);
		frameNeighborSetFlow(neighbor01, flow01);
		frameAddNeighbor(latest, neighbor01);

		FrameNeighbor neighbor10 = createEmptyFrameNeighbor();
		frameNeighborSetFrame(neighbor10, latest);
		frameNeighborSetFlow(neighbor10, flow10);
		frameAddNeighbor(frame, neighbor10);
	}
	flowGraphAddFrame(graph, frame);

}
void flowGraphLocalOptimize(FlowGraph graph) {
	// for last frame
	// estimate the rotation firstly
	// determine the translation secondly
	// and then update depth
	// as to this, local optimaize completed

}
void flowGraphCullRedundantFlow(FlowGraph graph) {

}

Frame flowGraphGetLatestFrame(FlowGraph graph) {
	if (graph->frameVector.empty()) { return NULL; }
	else { return graph->frameVector.back(); }
}

void flowGraphAddFrame(FlowGraph graph, Frame frame) {
	graph->frameVector.push_back(frame);

}


Frame createEmptyFrame() {
	return new FrameObj();
}

void releaseFrame(Frame frame) {
	for(std::vector<FrameNeighbor>::iterator it = frame->neighborVector.begin(); it != frame->neighborVector.end(); it++) {
		releaseFrameNeighbor(*it);
	}
	delete frame;
}

void frameAddNeighbor(Frame frame, FrameNeighbor neighbor) {
	frame->neighborVector.push_back(neighbor);
}

FrameNeighbor createEmptyFrameNeighbor() {
	return new FrameNeighborObj();
}
void releaseFrameNeighbor(FrameNeighbor neighbor) {
	releaseFlow(neighbor->flow);
	delete neighbor;
}

void frameNeighborSetFrame(FrameNeighbor neighbor, Frame nFrame) {
	neighbor->frame = nFrame;

}
void frameNeighborSetFlow(FrameNeighbor neighbor, Flow flow) {
	neighbor->flow = flow;
}


}
