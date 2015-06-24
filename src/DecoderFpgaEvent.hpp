/*
 * DecoderFpgaEvent.hpp
 *
 *  Created on: Feb 20, 2015
 *      Author: gamberini
 */

#ifndef DecoderFpgaEvent_HPP_
#define DecoderFpgaEvent_HPP_

#include "Queue.hpp"

class DecoderFpgaEvent {
	Queue<udp_t> * mPQ;
	Queue<FPGA_EVENT> * mFEQ;
	int type[numOfTrigTypes+1];

	udp_t * mUDP;
	FPGA_EVENT * mFE;

public:
	DecoderFpgaEvent(Queue<udp_t> * PckQue, Queue<FPGA_EVENT> * fpgaEvntQue);
	virtual ~DecoderFpgaEvent();

	void thread();

	void Print();
};

#endif /* DecoderFpgaEvent_HPP_ */
