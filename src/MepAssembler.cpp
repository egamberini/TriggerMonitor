/*
 * MepAssembler.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: gamberini
 */

#include "MepAssembler.hpp"

MepAssembler::MepAssembler(Queue<event_t> * evntQue, TxSocket * txSoc, EobDataHandler * eobDatHand)
: mEQ(evntQue),
  mTS(txSoc),
  mEDH(eobDatHand),
  mEvntCntr(0)
{
	mUDP = new udp_t;

}

MepAssembler::~MepAssembler() {
}

void MepAssembler::thread() {
	int checkEventNum = 0;
	bool eob = false;

	while(1) {
		checkEventNum++;
		this->FillMepHeader();
		if(mEvntCntr != checkEventNum-1) {
			std::cout<<"ERROR Mep Event Number:"<<std::endl;
			printf("mEvntCntr			%08x\n", mEvntCntr);
			printf("checkEventNum-1		%08x\n", checkEventNum-1);
		}
		for(int i=0; i<MepFactor; i++) {
			memcpy(mUDP->buf+8+i*eventLength, (mEQ->Deque())->event, sizeof(event_t));

			mUDP->len += eventLength;

			/* if the event is of EoB type append the EobData */
			if((mEQ->Deque())->event[28] == 35) {
				memcpy(mUDP->buf+8+(i+1)*eventLength, mEDH->getEobData(), mEDH->getEobDataLen());
				mUDP->len += mEDH->getEobDataLen();
				eob = true;
			}
			mEQ->Pop();

			//DEBUG check
			if(mUDP->buf[10] != mUDP->buf[0]) {
				printf("ERROR in MEP HEADER\n"
						"FirstEventLsb:  (event header) %02x | (mep header) %02x\n", mUDP->buf[10],
						mUDP->buf[0]);
//				sleep(1);
			}
			mEvntCntr++;
		}

		//DEBUG
//		this->PrintMep();

		mTS->SendTo(mUDP->buf, mUDP->len);

		if(eob) {
//			std::cout<<"[DBG] MepAssembler EoB"<<std::endl;
			mEvntCntr = 0;
			checkEventNum = 0;
			mTS->ResetRoundRobin();
			eob = false;
		}
	}
}

void MepAssembler::FillMepHeader() {
	LPE_HDR * header = reinterpret_cast<LPE_HDR *>(mUDP->buf);
	header->firstEventNum = mEvntCntr & 0x00FFFFFF;
	header->sourceID = 0x40;
	header->mepLength = 8+MepFactor*eventLength;
	header->numEvents = MepFactor;
	header->sourceSubID = 0;
	mUDP->len = 8;
}

void MepAssembler::PrintMep() {
	std::cout<<"[DBG] MepAssembler -> mUDP.len = "<<std::dec<<mUDP->len<<std::endl;
	for(unsigned int j=0; j<mUDP->len; j+=4) {
		printf("%02x %02x %02x %02x\n", mUDP->buf[j], mUDP->buf[j+1], mUDP->buf[j+2], mUDP->buf[j+3]);
	}
}
