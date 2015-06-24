/*
 * Decoder.hpp
 *
 *  Created on: Feb 20, 2015
 *      Author: gamberini
 */

#ifndef DECODER_HPP_
#define DECODER_HPP_

#include "Queue.hpp"
#include "EobDataHandler.hpp"

class Decoder {
	Queue<udp_t> * mPQ;
	Queue<CPU_EVENT> * mCEQ;
	EobDataHandler * mEDH;
	int type[numOfTrigTypes+1];

	udp_t * mUDP;
	LPE_HDR * mLH;
	EVNT_HDR * mEH;
	EVNT_FROM_CPU * mEFC;
	CPU_EVENT * mCE;

public:
	Decoder(Queue<udp_t> * PckQue, Queue<CPU_EVENT> * cpuEvntQue, EobDataHandler * eobDatHand);
	virtual ~Decoder();

	void thread();

	void Print();
};

#endif /* DECODER_HPP_ */
