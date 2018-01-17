
#include "dslam.h"
#include <iostream>



int main(int argc, char** argv) {

	std::cout << "hello local" << std::endl;

	//dslam::FlowGraph graph = 
	//	dslam::createFlowGraph();
	
	cv::Mat flow;
	//while (true) {
	for (int i = 0; i < 4; i++) {

		const char* name = 
			dslam::queryFlowName(i, i+1);
		dslam::readFlow(name, flow);

		//dslam::flowGraphAddFlow(graph, flow);
		//dslam::localOptimization(graph);
		//dslam::cullRedundantFlow(graph);
	}

	//dslam::releaseFlowGraph(graph);
	

}
