#ifndef __FLOW_GRAPH_HEADER__
#define __FLOW_GRAPH_HEADER__

#include "common.h"
#include "flowframe.h"

namespace dslam {

  /**
  @brief A flow graph (network) used to optimization.
  */
  struct FlowGraphObj {
  	std::vector<FlowFrameObj*> frameVector;
  };

  typedef FlowGraphObj* FlowGraph;



  FlowGraph createEmptyFlowGraph();

  void flowGraphAddFlowFrameWithFlows(FlowGraph graph, const Flow flow01, const Flow flow10);

  /**
  @brief Optimize the last frame. Firstly, camera parameters will estimazied including translate, rotation, intrinsics and distortion coefficients. Secendly flow and depth map will optimized jointly. Lastly, the rebundant frame bill be culled.

  @param graph Which graph's last frame will be optimizaed.
  @return No
  @see flowGraphCullRedundantFlow()
  @note No.
  */
  void flowGraphLocalOptimize(FlowGraph graph);


  /**
  @brief Cull redundant flow from flow graph.

  @param graph Which graph's flow will be culled.
  @return No
  @see flowGraphLocalOptimize()
  @note No.
  */
  void flowGraphCullRedundantFlow(FlowGraph graph);

  /**
  @brief Query the latest frame from graph.

  @param graph Which graph to query.
  @return latest frame if at least one frame in graph.
  @return NULL if there is no frame in graph.
  @see flowGraphAddFlowFrame() frameAddNeighbor()
  @note No.
  */
  FlowFrame flowGraphGetLatestFlowFrame(FlowGraph graph);
  void flowGraphAddFlowFrame(FlowGraph graph, FlowFrame frame);

  void releaseFlowGraph(FlowGraph graph);

} // namespace

#endif //__FLOW_GRAPH_HEADER__
