#ifndef __RESPONDER_HEADER__
#define __RESPONDER_HEADER__

#include "common.h"
#include "flowframe.h"


namespace dslam {

class ResponderObj {

};


class CameraPrameterResponderObj : public ResponderObj {

public:
	CameraPrameterResponderObj(FlowFrame frame);

};


typedef ResponderObj* Responder;

Responder createCameraPrameterResponder(FlowFrame frame);

void releaseResponder(Responder responder);



} // namespace dslam


#endif // __RESPONDER_HEADER__
