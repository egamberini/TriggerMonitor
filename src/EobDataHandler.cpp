/*
 * EobDataHandler.cpp
 *
 *  Created on: Apr 21, 2015
 *      Author: gamberini
 */

#include <iostream>
#include <cstring>

#include "EobDataHandler.hpp"

EobDataHandler::EobDataHandler()
{
	mEOB = new EOB_EVNT;
}

EobDataHandler::~EobDataHandler() {
	// TODO Auto-generated destructor stub
}

void EobDataHandler::setNumOfAnalyzedPrim(unsigned char * buf_p) {
	uint32_t analyzedPrim[numOfSources];
	memcpy(analyzedPrim, buf_p, sizeof(analyzedPrim));
	for(int i=0; i<numOfSources; i++) {
		std::cout<<analyzedPrim[i]<<std::endl;
		mEOB->source[i].analyzedPrimitive =	analyzedPrim[i];
	}
}

void EobDataHandler::setMatchedTriggers(int * matchTrig_p) {
	int matchedTrigger[numOfTrigTypes+1];
	memcpy(matchedTrigger, matchTrig_p, sizeof(matchedTrigger));
	for(int i=0; i<numOfTrigTypes; i++) {
		std::cout<<matchedTrigger[i+1]<<std::endl;
		mEOB->trigType[i].matchedTrig = matchedTrigger[i+1];
	}
}

EOB_EVNT * EobDataHandler::getEobData() {
	return mEOB;
}

unsigned int EobDataHandler::getEobDataLen() {
	return sizeof(mEOB);
}
