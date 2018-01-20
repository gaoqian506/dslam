#ifndef __RESOLVER_HEADER__
#define __RESOLVER_HEADER__

#include "common.h"
#include "responder.h"

namespace dslam {


  class ResponderObj;
  /**
  @brief A resolver object or optimization.
  */
  struct ResolverObj {
  	ResponderObj* responder; /**< Associat delegate for optimization */

  };


  typedef ResolverObj* Resolver;

/**
@brief Resolver creation method.

@return A Resolver object.
@see releaseResolver().
@note No.
*/
Resolver createEmptyResolver();

/**
@brief Add a responder to resolver.

@param resovler Which resolver object will add constrain to.
@param responder A responder object.

@return No.
@see createEmptyResolver().
@note No.
*/
void resolverSetResponder(Resolver resolver, Responder responder);


/**
@brief Launch a optimizing process.

@param resovler Which resolver's optimation will be launched.

@return No.
@see resolverAddConstrain(), resolverSetErrorJacobinCallback().
@note No.
*/
void resolve(Resolver resolver);


/**
@brief Release a resolver object.

@return No.
@see createResolver().
@note No.
*/
void releaseResolver(Resolver resovler);

/**
@brief Create a responder for optimizing camera parameters.

@param camera. Initializing argument for responder.
@param flow. Initializing argument for responder.
@return A base responder object.
@see Responder.
@note No.
*/



}

#endif // __RESOLVER_HEADER__
