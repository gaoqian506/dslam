#ifndef __COMMON_HEADER__
#define __COMMON_HEADER__


namespace dslam {
	/**
	@brief A config and status indicator structure.
	*/
	struct Status {
		/** Default directory which optical flow file will read and write. */
		const char* flowDir;

		/** A inhabit buffer for temporary file name of optical flow. */
		char tempFlowName[256];
	};





} // namespace


#endif // __COMMON_HEADER__
