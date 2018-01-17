#ifndef __DSLAM_HEADER__
#define __DSLAM_HEADER__

namespace dslam {


struct FlowHeader {
	int cols;
	int rows;
	int type;
	int step;
};

struct Status {
	const char* flowDir;
};



void makeFlowFromVideo(const char* video_name);


}

#endif // __DSLAM_HEADER__
