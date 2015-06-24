/*
 * MepAssembler.hpp
 *
 *  Created on: Mar 16, 2015
 *      Author: gamberini
 */

#ifndef MEPASSEMBLER_HPP_
#define MEPASSEMBLER_HPP_

#include "TxSocket.hpp"
#include "Queue.hpp"
#include "EobDataHandler.hpp"

class MepAssembler {
	Queue<event_t> * mEQ;
	TxSocket * mTS;
	EobDataHandler * mEDH;

	int mEvntCntr;

	udp_t * mUDP;

public:
	MepAssembler(Queue<event_t> * evntQue, TxSocket * txSoc, EobDataHandler * eobDatHand);
	virtual ~MepAssembler();

	void thread();

	void FillMepHeader();

	void PrintMep();
};

#endif /* MEPASSEMBLER_HPP_ */
