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



void makeFlowFromVideo(const char* video_name);
void saveFlow(cv::Mat& flow, const char* name);
cv::Mat loadFlow(const char* name);
const char* queryFlowName(int from, int to);


}

#endif // __DSLAM_HEADER__
