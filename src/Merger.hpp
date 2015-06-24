/*
 * Merger.hpp
 *
 *  Created on: Mar 10, 2015
 *      Author: gamberini
 */

#ifndef MERGER_HPP_
#define MERGER_HPP_

#include "Queue.hpp"
#include "EobDataHandler.hpp"

class Merger {
	Queue<FPGA_EVENT> * mFEQ;
	Queue<CPU_EVENT> * mCEQ;
	Queue<event_t> * mEQ;
	EobDataHandler * mEDH;
	uint32_t prevTimeStamp;
	uint8_t prevTrigWord;
	int matchedType[numOfTrigTypes+1];

	FPGA_EVENT * mFE;
	CPU_EVENT * mCE;
	event_t * mEVNT;

public:
	Merger(Queue<FPGA_EVENT> * fpgaEvntQue, Queue<CPU_EVENT> * cpuEvntQue, Queue<event_t> * evntQue, EobDataHandler * eobDatHand);
	virtual ~Merger();

	void thread();

	void FillEventHeader();

	void FillSpecialEventHeader();

	void FillEobEventHeader();

	void FillEvent();

	void Print();

	void FillSpecialEvent();
};

#endif /* MERGER_HPP_ */
