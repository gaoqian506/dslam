#ifndef __DSLAM_HEADER__
#define __DSLAM_HEADER__

#include <opencv2/core/core.hpp>

namespace dslam {


struct FlowHeader {
	int cols;
	int rows;
	int type;
	int step;
};

struct Status {
	const char* flowDir;
	char tempFlowName[256];
};

struct FlowObj {
	cv::Mat mat;
};

struct FrameNeighborObj;
struct FrameObj {
	std::vector<FrameNeighborObj*> neighborVector;
};

struct FrameNeighborObj {
	FrameObj* frame;
	FlowObj* flow;
};

struct FlowGraphObj {
	std::vector<FrameObj*> frameVector;
};


typedef FrameObj* Frame;
typedef FrameNeighborObj* FrameNeighbor;
typedef FlowGraphObj* FlowGraph;
typedef FlowObj* Flow;


void makeFlowFromVideo(const char* video_name);
Flow createEmptyFlow();
Flow createFlowFromFile(const char* name);
Flow createFlowFromMat(const cv::Mat& mat);
void releaseFlow(Flow flow);
void saveFlow(Flow flow, const char* name);
const char* queryFlowName(int from, int to);


FlowGraph createEmptyFlowGraph();
void releaseFlowGraph(FlowGraph graph);
void flowGraphAddFrameWithFlows(FlowGraph graph, const Flow flow01, const Flow flow10);
void flowGraphLocalOptimize(FlowGraph graph);
void flowGraphCullRedundantFlow(FlowGraph graph);
Frame flowGraphGetLatestFrame(FlowGraph graph);
void flowGraphAddFrame(FlowGraph graph, Frame frame);

Frame createEmptyFrame();
void releaseFrame(Frame frame);
void frameAddNeighbor(Frame frame, FrameNeighbor neighbor);

FrameNeighbor createEmptyFrameNeighbor();
void releaseFrameNeighbor(FrameNeighbor neighbor);
void frameNeighborSetFrame(FrameNeighbor neighbor, Frame nFrmae);
void frameNeighborSetFlow(FrameNeighbor neighbor, Flow flow);






}

#endif // __DSLAM_HEADER__
