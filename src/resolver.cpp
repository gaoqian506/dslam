#include "resolver.h"

namespace dslam {

  Resolver createEmptyResolver() {
  	return new ResolverObj();
  }

  void resolverSetResponder(Resolver resolver, Responder responder) {

  	resolver->responder = responder;
  }

  void resolve(Resolver resolver) {

  	// get target dimensions
  	// get source dimensions

  	// foreach tDim in target dimensions {
  		// err, jacobin
  		// foreach sDim in source dimensions {
  			// responder->calculateErrorJacobin(tDim, sDim, err, jacobin);
  			// accumulate(err jacobin);
  		// }
  		// set optimizing result at tDim
  	// }

  }

  void releaseResolver(Resolver resovler) {
  	delete resovler;
  }


} // namespace
