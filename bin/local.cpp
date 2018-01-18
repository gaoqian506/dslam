
#include "dslam.h"
#include <iostream>



int main(int argc, char** argv) {

	std::cout << "hello local" << std::endl;

	dslam::FlowGraph graph =
		dslam::createEmptyFlowGraph();

	dslam::Flow flow1, flow2;
	//while (true) {
	for (int i = 0; i < 4; i++) {

		const char* name1 = dslam::queryFlowName(i, i+1);
		flow1 = dslam::createFlowFromFile(name1);

		const char* name2 = dslam::queryFlowName(i+1, i);
		flow2 = dslam::createFlowFromFile(name2);

		dslam::flowGraphAddFrameWithFlows(graph, flow1, flow2);
		dslam::flowGraphLocalOptimize(graph);
		dslam::flowGraphCullRedundantFlow(graph);
	}

	dslam::releaseFlowGraph(graph);


}
