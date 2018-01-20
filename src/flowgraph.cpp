#include "flowgraph.h"

namespace dslam {


  FlowGraph createEmptyFlowGraph() {
  	return new FlowGraphObj();
  }


  void flowGraphAddFlowFrameWithFlows(FlowGraph graph, Flow flow01, Flow flow10) {

  	FlowFrame frame = createEmptyFlowFrame();
  	FlowFrame latest = flowGraphGetLatestFlowFrame(graph);
  	if (latest) {
  		FlowFrameNeighbor neighbor01 = createEmptyFlowFrameNeighbor();
  	 	flowFrameNeighborSetFrame(neighbor01, frame);
  	 	flowFrameNeighborSetFlow(neighbor01, flow01);
  	 	flowFrameAddNeighbor(latest, neighbor01);

  		FlowFrameNeighbor neighbor10 = createEmptyFlowFrameNeighbor();
  		flowFrameNeighborSetFrame(neighbor10, latest);
  		flowFrameNeighborSetFlow(neighbor10, flow10);
  		flowFrameAddNeighbor(frame, neighbor10);
  	}
  	 flowGraphAddFlowFrame(graph, frame);


  }
  void flowGraphLocalOptimize(FlowGraph graph) {

  	FlowFrame frame = flowGraphGetLatestFlowFrame(graph);
  	flowFrameOptimizeCameraParameters(frame);
  	flowFrameOptimizeFlowAndDepth(frame);
  	flowFrameCullRedundantNeighbor(frame);
  }

  void flowGraphCullRedundantFlow(FlowGraph graph) {

  }

  FlowFrame flowGraphGetLatestFlowFrame(FlowGraph graph) {
  	if (graph->frameVector.empty()) { return NULL; }
  	else { return graph->frameVector.back(); }
  }

  void flowGraphAddFlowFrame(FlowGraph graph, FlowFrame frame) {
  	graph->frameVector.push_back(frame);

  }


  void releaseFlowGraph(FlowGraph graph) {
  	for(std::vector<FlowFrame>::iterator it = graph->frameVector.begin(); it != graph->frameVector.end(); it++) {
  		releaseFlowFrame(*it);
  	}
  	delete graph;
  }


} // namespace
