#ifndef __DSLAM_HEADER__
#define __DSLAM_HEADER__

#include <opencv2/core/core.hpp>

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
@brief A config and status indicator structure.
*/
struct Status {
	/** Default directory which optical flow file will read and write. */
	const char* flowDir;
	
	/** A inhabit buffer for temporary file name of optical flow. */
	char tempFlowName[256];
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


/**
@brief Camera parameter suggestion object that descript the increment of camera parameters that will achieve a better consistent respect of optical flow.
*/
struct CameraParameterSuggestionObj {
	/** Suggestion of increment of camera parameters. */
	double suggestion[9];
};



struct FrameNeighborObj;
/**
@brief A frame object that has a list of neighbors, camera and etc..
*/
struct FrameObj {
	/** Camera object represent the frame's pose and projecting progress */
	CameraObj* camera;

	/** Vector of neighbors */
	std::vector<FrameNeighborObj*> neighborVector;
};

/**
@brief A neighbor of frame, they are binded with optical flow.
*/
struct FrameNeighborObj {
	/** The neighbor frame. */
	FrameObj* frame;

	/** The binding optical flow with its neighbor. */
	FlowObj* flow;
};

/**
@brief A flow graph (network) used to optimization.
*/
struct FlowGraphObj {
	std::vector<FrameObj*> frameVector;
};

/**
@brief A 4D int array that descrip the iterate time for a solver optimization.
*/
struct Space4 {
	int space[4];
};


/**
@brief A 4D index indicate iterate position when performing a optimizing.
*/
struct Indics4 {
	int index[4];
};


/**
@brief A definition of error-jacobin callback (function pointor), which will called by a resolver many times to collect optimizing needed infomations.
*/
typedef void (*ErrorJacobinCallback)(void* constrains, void* error, void* yacobin, Indics4 index);

/**
@brief A resolver object or optimization.
*/
struct ResolverObj {
	/** Constrain vector will parsed by specified error-yacobin callback */
	void* constrtains[10];

	/** Callback will be called indexCount times for collecting optimizing infomation. */
	ErrorJacobinCallback ejCallback;

	/** The times will callback will be called. The index passes to callback will interate in this variable. */
	Space4 space;

};





typedef FrameObj* Frame;
typedef FrameNeighborObj* FrameNeighbor;
typedef FlowGraphObj* FlowGraph;
typedef FlowObj* Flow;
typedef CameraObj* Camera;
typedef CameraParameterSuggestionObj* CameraParameterSuggestion;
typedef ResolverObj* Resolver;


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


FlowGraph createEmptyFlowGraph();
void releaseFlowGraph(FlowGraph graph);
void flowGraphAddFrameWithFlows(FlowGraph graph, const Flow flow01, const Flow flow10);

/**
@brief Optimize the last frame. Firstly, camera parameters will estimazied including translate, rotation, intrinsics and distortion coefficients. Secendly flow and depth map will optimized jointly. Lastly, the rebundant frame bill be culled.

@param graph Which graph's last frame will be optimizaed.
@return No
@see flowGraphCullRedundantFlow()
@note No.
*/
void flowGraphLocalOptimize(FlowGraph graph);

void flowGraphCullRedundantFlow(FlowGraph graph);

/**
@brief Query the latest frame from graph.

@param graph Which graph to query.
@return latest frame if at least one frame in graph.
@return NULL if there is no frame in graph.
@see flowGraphAddFrame() frameAddNeighbor()
@note No.
*/
Frame flowGraphGetLatestFrame(FlowGraph graph);
void flowGraphAddFrame(FlowGraph graph, Frame frame);

Frame createEmptyFrame();
void releaseFrame(Frame frame);
void frameAddNeighbor(Frame frame, FrameNeighbor neighbor);

/**
@brief Optimize camera's parameters of specified frame, the parameters consist of externals, intrinsics and distortions.

@param frame Which's camera will process.
@return NO.
@see flowGraphLocalOptimize()
@note No.
*/
void frameOptimizeCameraParameters(Frame frame);

/**
@brief Optimize frame's optical flow and depth map according to camera motion and neiborhood circumstance.

@param frame Which's optical flow and depth map will process.
@return NO.
@see flowGraphLocalOptimize()
@note No.
*/
void frameOptimizeFlowAndDepth(Frame frame);

/**
@brief Cull redundant neighbor according camera pose and optical flow status, to get a effective graph hierarchy.

@param frame Which's redundant neighbor will be culled..
@return NO.
@see flowGraphLocalOptimize()
@note No.
*/
void frameCullRedundantNeighbor(Frame frame);

/**
@brief Query frame's camera object.

@param frame Which frame's camera to query.
@return Camera object.
@see CameraObj
@note No.
*/
Camera frameGetCamera(Frame frame);

/**
@brief Query all of frame's neighbor cameras.

@param frame Which frame is to query.
@return Vector of neighbors' cameras.
@see CameraObj
@note No.
*/
std::vector<Camera> frameGetNeighborCameraVector(Frame frame);

/**
@brief Query all of frame's neighbor flows.

@param frame Which frame is to query.
@return Vector of neighbors' flows.
@see FlowObj
@note No.
*/
std::vector<Flow> frameGetNeighborFlowVector(Frame frame);



/**
@brief Query all of frame's neighbor vector.

@param frame Which frame is to query.
@return Vector of neighbors.
@see FrameNeighborObj
@note No.
*/
std::vector<FrameNeighbor>& frameGetNeighborVector(Frame frame);



FrameNeighbor createEmptyFrameNeighbor();
void releaseFrameNeighbor(FrameNeighbor neighbor);
void frameNeighborSetFrame(FrameNeighbor neighbor, Frame nFrmae);
void frameNeighborSetFlow(FrameNeighbor neighbor, Flow flow);

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

/**
@brief Create a empty camera parameter optimizing suggestion, that the increments are set to zeros.

@return A empty camera parameter suggestion object.
@see CameraParameterSuggestionObj
@note No.
*/
CameraParameterSuggestion createEmptyCameraParameterSuggestion();


/**
@brief Create a empty camera parameter optimizing suggestion from a memory buffer.

@param data The start address of data buffer in memory.
@return A serialized camera parameter suggestion object.
@see createEmptyCameraParameterSuggestion
@note No.
*/
CameraParameterSuggestion createCameraParameterSuggestionFromMemory(double* data);

/**
@brief Update the camera parameters with sugesstion, this will consider the manifold phenomenon.

@param camera Which's parameters will be updated.
@param suggestion The updating suggestion.
@return No.
@see CameraParameterSuggestionObj
@note No.
*/
void incrementCameraParameterWithSuggestion(Camera camera, CameraParameterSuggestion suggestion);

/**
@brief Check whether the optimization is achieved.

@param camera Camera that will be evalated.
@param suggestion Suggestion updating suggestion.
@return true If check passed.
@return false If check failure.
@see CameraParameterSuggestionObj
@note No.
*/
bool cameraParameterOptimizationSatisfied(Camera camera, CameraParameterSuggestion suggestion);


/**
@brief Extract the relative relationship between tow (neighboring) cameras.

@param fromCamera Basis camera.
@param toCamera Target camera the relative information would be extract.
@return Relative camera object.
@see resolveCameraparameterSuggestion().
@note No.
*/
Camera extractRelativeCamera(Camera fromCamera, Camera toCamera);

/**
@brief Release camera parameter suggestion object.

@param suggestion Which object to be released.
@return void.
@see CameraParameterSuggestionObj
@note No.
*/
void releaseCameraParameterSuggestion(CameraParameterSuggestion suggestion);



/**
@brief Optimize camera parameters by the optcal flow constrains, this is to do a non-linear optimizing task, until optimization task is satisfied.

@param camera The optimizing target.
@param neighborCameras The camera vector of neighbors.
@param neighborFlows The optical flow vector of neighbors.
@return NO
@see frameOptimizeCameraParameters() flowGraphLocalOptimize()
@note No.
*/
void resolveCameraParameters(Camera camera, const std::vector<Camera>& neighborCameras, const std::vector<Flow>& neighborFlows);

/**
@brief Optimize camera parameters, by the optcal flow constrains, for one step. Firstly, each neibor give a suggestion, and then, they are fused to a optimized result. If not satisfy terminate condition, then otpimize the parameters again.

@param camera The optimizing target.
@param neighborCameras The camera vector of neighbors.
@param neighborFlows The optical flow vector of neighbors.
@return NO
@see frameOptimizeCameraParameters() flowGraphLocalOptimize()
@note No.
*/
CameraParameterSuggestion resolveCameraparameterSuggestion(Camera camera, const std::vector<Camera>& neighborCameras, const std::vector<Flow>& neighborFlows);

/**
@brief A signle contrain version to optimize camera parameters, by neibor camera and flow. First, extract relative camera between two neighboring cameras, and resovle the suggestion for the relative camera.

@param camera The optimizing target.
@param neighborCamera Neighbor's camera.
@param neighborFlow Neighbor's flow.
@return A camera parameter optimize suggestion object.
@see frameOptimizeCameraParameters(); flowGraphLocalOptimize(); resolveCameraparameterSuggestion().
@note No.
*/
CameraParameterSuggestion resolveCameraparameterSuggestion(Camera camera, const Camera neighborCamera, const Flow neighborFlow);


/**
@brief Resolve camera parameter optimize suggestion via optical flow.

@param camera The optimizing target object, from its initial states.
@param flow The optimizing contrains, to be reconciled by camera adjustment.
@return A camera parameter optimize suggestion object.
@see frameOptimizeCameraParameters(), flowGraphLocalOptimize(), resolveCameraparameterSuggestion(), extractRelativeCamera().
@note No.
*/
CameraParameterSuggestion resolveCameraparameterSuggestion(const Camera camera, const Flow flow);

/**
@brief Fuse several camera parameter optimize suggestions into a one.

@param suggestionVector The vector of camera parameter optimize suggestion, which will be fused.
@return A fused camera parameter optimize suggestion.
@see frameOptimizeCameraParameters(); flowGraphLocalOptimize(); resolveCameraparameterSuggestion().
@note No.
*/
CameraParameterSuggestion fuseCameraParameterSuggestions(const std::vector<CameraParameterSuggestion>& suggestionVector);

/**
@brief Resolver creation method.

@param resovler Which resolver object will be released.

@return A Resolver object.
@see releaseResolver().
@note No.
*/
Resolver createEmptyResolver(Space4 space);

/**
@brief Add a constrain to resolver, which will transmit to error-jacobin callback method. The constrain order will in accord with calling sequence.

@param resovler Which resolver object will add constrain to.
@param constrain A constrain element that will transmit to error-jacobin callback method.

@return No.
@see No.
@note No.
*/
void resolverAddConstrain(Resolver resolver, void* constrain);


/**
@brief Set error and jacobin callback for a resolver object.

@param resovler Which resolver object will be sed.
@param callback A error and jacobin callback (function pointor.

@return No.
@see No.
@note No.
*/
void resolverSetErrorJacobinCallback(Resolver resolver, ErrorJacobinCallback callback);

/**
@brief Launch a optimizing process. 

@param resovler Which resolver's optimation will be launched.

@return No.
@see resolverAddConstrain(), resolverSetErrorJacobinCallback().
@note No.
*/
void resolve(Resolver resolver);

/**
@brief Retrieve optimizing result from resolver object.

@param resovler Which from resolver's result will be retrieved.
@return A pointor to memory buffer storing resolver's optimizing result.
@see resolve().
@note No.
*/
double* resolverGetOptimizedResult(Resolver resolver);

/**
@brief Release a resolver object.

@return No.
@see createResolver().
@note No.
*/
void releaseResolver(Resolver resovler);


void cameraFlowErrorJacobinResolverCallback(void* constrains, void* error, void* yacobin, Indics4 index);




}

#endif // __DSLAM_HEADER__
