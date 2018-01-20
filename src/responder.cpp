
#include "responder.h"

namespace dslam {


  CameraPrameterResponderObj::CameraPrameterResponderObj(FlowFrame frame) {

  }


  Responder createCameraPrameterResponder(FlowFrame frame) {
  	return new CameraPrameterResponderObj(frame);
  }

  void releaseResponder(Responder responder) {
  	delete responder;
  }




} // namespace dslam
