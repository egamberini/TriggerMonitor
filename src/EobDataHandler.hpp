/*
 * EobDataHandler.hpp
 *
 *  Created on: Apr 21, 2015
 *      Author: gamberini
 */

#ifndef EOBDATAHANDLER_HPP_
#define EOBDATAHANDLER_HPP_

#include <cstdint>

#include "Types.hpp"

class EobDataHandler {
	EOB_EVNT * mEOB;

public:
	EobDataHandler();
	~EobDataHandler();

	void setNumOfAnalyzedPrim(unsigned char * buf_p);

	void setMatchedTriggers(int * matchTrig_p);

	EOB_EVNT * getEobData();

	unsigned int getEobDataLen();
};

#endif /* EOBDATAHANDLER_HPP_ */
