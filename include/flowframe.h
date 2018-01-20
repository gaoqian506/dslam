#ifndef __FLOW_FRAME_HEADER__
#define __FLOW_FRAME_HEADER__

#include "common.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>

namespace dslam {


  /**
  @brief A otpical flow file header which define the basic infomation needed to read and write.
  */
  struct FlowHeader {
  	int cols;	/**< columns of optical flow */
  	int rows;	/**< rows of optical flow */
  	int type;	/**< type of optical flow */
  	int step;	/**< step of optical flow, line with in bytes */
  };

  /**
  @brief A optical flow object which store and maintain the optical flow data.
  */
  struct FlowObj {

  	/** Optical flow data structure, maintained thanks opencv's support. */
  	cv::Mat mat;
  };

  /**
  @brief Camera object that descript the camera situation including translation, rotation, intrinsics and distortions.
  */
  struct CameraObj {
  	/** Camera parameters, they are stored in one data array, consists of translate, rotation, focal length, principal point and distortion coefficients. */
  	double paras[9];

  };

  struct FlowFrameNeighborObj;
  /**
  @brief A frame object that has a list of neighbors, camera and etc..
  */
  struct FlowFrameObj {
  	/** Camera object represent the frame's pose and projecting progress */
  	CameraObj* camera;

  	/** Vector of neighbors */
  	std::vector<FlowFrameNeighborObj*> neighborVector;
  };

  /**
  @brief A neighbor of frame, they are binded with optical flow.
  */
  struct FlowFrameNeighborObj {
  	/** The neighbor frame. */
  	FlowFrameObj* frame;

  	/** The binding optical flow with its neighbor. */
  	FlowObj* flow;
  };

  typedef FlowFrameObj* FlowFrame;
  typedef FlowFrameNeighborObj* FlowFrameNeighbor;
  typedef FlowObj* Flow;
  typedef CameraObj* Camera;


  /**
  @brief Exact bidirectional optical flow from a video file.
  Exacted optical flow will write in the directory
  define with Status::flowdir (default = temp/flow).

  @param video_name path to video file
  @return No
  @see Status
  @note invalid video path is undefined.
  */
  void makeFlowFromVideo(const char* video_name);

  /**
  @brief The flow initialization. Which would be called paired with releaseFlow();

  @return A empty flow instance.
  @see releaseFlow()
  @note
  */
  Flow createEmptyFlow();

  /**

  @brief Initialing a flow instance from file. Which would be called paired with releaseFlow();

  @param name Flow file name, should be in the directory defined with Status:
  @return Flow instance from file.
  @see createEmptyFlow() releaseFlow()
  @note Invalid flow path is undefined.
  */
  Flow createFlowFromFile(const char* name);
  Flow createFlowFromMat(const cv::Mat& mat);
  void releaseFlow(Flow flow);
  void saveFlow(Flow flow, const char* name);
  const char* queryFlowName(int from, int to);



  FlowFrame createEmptyFlowFrame();
  void releaseFlowFrame(FlowFrame frame);
  void flowFrameAddNeighbor(FlowFrame frame, FlowFrameNeighbor neighbor);

  /**
  @brief Optimize camera's parameters of specified frame, the parameters consist of externals, intrinsics and distortions.

  @param frame Which's camera will process.
  @return NO.
  @see flowGraphLocalOptimize()
  @note No.
  */


  void flowFrameOptimizeCameraParameters(FlowFrame frame);



  /**
  @brief Optimize frame's optical flow and depth map according to camera motion and neiborhood circumstance.

  @param frame Which's optical flow and depth map will process.
  @return NO.
  @see flowGraphLocalOptimize()
  @note No.
  */
  void flowFrameOptimizeFlowAndDepth(FlowFrame frame);

  /**
  @brief Cull redundant neighbor according camera pose and optical flow status, to get a effective graph hierarchy.

  @param frame Which's redundant neighbor will be culled..
  @return NO.
  @see flowGraphLocalOptimize()
  @note No.
  */
  void flowFrameCullRedundantNeighbor(FlowFrame frame);

  /**
  @brief Query frame's camera object.

  @param frame Which frame's camera to query.
  @return Camera object.
  @see CameraObj
  @note No.
  */
  Camera flowFrameGetCamera(FlowFrame frame);

  /**
  @brief Query all of frame's neighbor cameras.

  @param frame Which frame is to query.
  @return Vector of neighbors' cameras.
  @see CameraObj
  @note No.
  */
  std::vector<Camera> flowFrameGetNeighborCameraVector(FlowFrame frame);

  /**
  @brief Query all of frame's neighbor flows.

  @param frame Which frame is to query.
  @return Vector of neighbors' flows.
  @see FlowObj
  @note No.
  */
  std::vector<Flow> flowFrameGetNeighborFlowVector(FlowFrame frame);



  /**
  @brief Query all of frame's neighbor vector.

  @param frame Which frame is to query.
  @return Vector of neighbors.
  @see FlowFrameNeighborObj
  @note No.
  */
  std::vector<FlowFrameNeighbor>& flowFrameGetNeighborVector(FlowFrame frame);



  FlowFrameNeighbor createEmptyFlowFrameNeighbor();
  void releaseFlowFrameNeighbor(FlowFrameNeighbor neighbor);
  void flowFrameNeighborSetFrame(FlowFrameNeighbor neighbor, FlowFrame nFrmae);
  void flowFrameNeighborSetFlow(FlowFrameNeighbor neighbor, Flow flow);





  /**
  @brief Create an original camera object.

  @return A camera object.
  @see releaseCamera().
  @note No.
  */
  Camera createOriginalCamera();

  /**
  @brief Release an camera object.

  @param camera Camera object to be released.
  @return No.
  @see createOriginalCamera().
  @note No.
  */
  void releaseCamera(Camera camera);



} // namespace


#endif // __FLOW_FRAME_HEADER__
