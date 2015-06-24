/*
 * Merger.cpp
 *
 *  Created on: Mar 10, 2015
 *      Author: gamberini
 */

#include "Merger.hpp"

Merger::Merger(Queue<FPGA_EVENT> * fpgaEvntQue, Queue<CPU_EVENT> * cpuEvntQue, Queue<event_t> * evntQue, EobDataHandler * eobDatHand)
: mFEQ(fpgaEvntQue),
  mCEQ(cpuEvntQue),
  mEQ(evntQue),
  mEDH(eobDatHand),
  prevTimeStamp(0),
  prevTrigWord(0),
  matchedType()
{
	mFE = new FPGA_EVENT;
	mCE = new CPU_EVENT;
	mEVNT = new event_t;
}

Merger::~Merger() {
	// TODO Auto-generated destructor stub
}

void Merger::thread() {
	bool analyseNormTrig = false;
	int skippedCpuEvents = 0;
	int skippedFpgaEvents = 0;
	int matchedEvents = 0;
	int ltuDelay = 0xe;
	bool matchCondition;
	bool skipFPGA, skipCPU;
	unsigned int periodicTrig = 0;

	while(1) {
		cpuEob:
		memset(mEVNT, 0, sizeof(event_t));

		// deque FPGA EVENT
//		std::cout<<"[DBG] Merger: deque Fpga event"<<std::endl;

//		usleep(1000);
//		std::cout<<"Merger: deque Fpga event"<<std::endl;
		memcpy(mFE, mFEQ->Deque(), sizeof(FPGA_EVENT));
		mFEQ->Pop();

		// Sob trigger
		if(mFE->triggerWord == 34) {
//			std::cout<<"[DBG] Merger->SoB"<<std::endl;
			this->FillSpecialEventHeader();
			mEQ->Enque(*mEVNT);
			prevTimeStamp = mFE->timeStamp;
			prevTrigWord = mFE->triggerWord;
			analyseNormTrig = true;
			continue;
		}

		// Eob trigger
		if(mFE->triggerWord == 35) {
//			std::cout<<"[DBG] Merger->EoB"<<std::endl;
			this->FillEobEventHeader();
			this->FillSpecialEvent();
			mEQ->Enque(*mEVNT);
			prevTimeStamp = 0;
			prevTrigWord = 0;

			// empty the CPU event queue looking for the Eob packet
			usleep(100);
			if(!mCEQ->IsEmpty()) {
				do {
					memcpy(mCE, mCEQ->Deque(), sizeof(CPU_EVENT));
					mCEQ->Pop();
				} while(mCE->type != 35);
			}
			analyseNormTrig = false;
			std::cout<<"Merger: matchedEvents:		"<<std::dec<<matchedEvents<<std::endl;
			matchedEvents = 0;
			/* fill the EobData structure */
			mEDH->setMatchedTriggers(matchedType);
			this->Print();
			std::cout<<"Merger: skippedCpuEvents:	"<<std::dec<<skippedCpuEvents<<std::endl;
			skippedCpuEvents = 0;
			std::cout<<"Merger: skippedFpgaEvents:	"<<std::dec<<skippedFpgaEvents<<std::endl;
			skippedFpgaEvents = 0;
			std::cout<<"Merger: periodicTrigger:	"<<std::dec<<periodicTrig<<std::endl;
			periodicTrig = 0;
			continue;
		}

		if(mFE->triggerWord == 1) { // 32 for synch trigger
			this->FillSpecialEventHeader();
			this->FillSpecialEvent();
			mEQ->Enque(*mEVNT);
			prevTimeStamp = mFE->timeStamp;
			prevTrigWord = mFE->triggerWord;
			periodicTrig++;
			continue;
		}

		// normal trigger
		if(analyseNormTrig) {
			this->FillEventHeader();
			// search the corresponding cpu event (equal timestamp)
//			std::cout<<"[DBG] Merger -> analyzing normal trigger"<<std::endl;
			do {
				memcpy(mCE, mCEQ->Deque(), sizeof(CPU_EVENT));
				mCEQ->Pop();
				if(mCE->type == 35) {
//					std::cout<<"[DBG] Merger->CPU EoB detected!"<<std::endl;
					analyseNormTrig = false;
					goto cpuEob;
				}
				skipFE:
				skipFPGA = mCE->timeStamp > mFE->timeStamp-fixedLatency-ltuDelay+1;
				skipCPU = mCE->timeStamp < mFE->timeStamp-fixedLatency-ltuDelay-1;
				matchCondition = !skipFPGA && !skipCPU;
				if(!matchCondition) {
					//DEBUG
					std::cout<<"[DBG] Merger->CPU TS skipped->no correspondence in FPGA TS"<<std::endl;
//					printf("CPU TS:		%08x\n", mCE->timeStamp);
//					printf("FPGA TS-FL:	%08x\n", mFE->timeStamp-fixedLatency-ltuDelay);
//					printf("FPGA TS:	%08x\n", mFE->timeStamp);
//					std::cout<<"skipFPGA "<<skipFPGA<<std::endl;
//					std::cout<<"skipCPU "<<skipCPU<<std::endl;
//					usleep(100000);
					if(skipCPU) {
						std::cout<<"[DBG] skip CPU"<<std::endl;
						skippedCpuEvents++;
					}
					if(skipFPGA) {
						memcpy(mFE, mFEQ->Deque(), sizeof(FPGA_EVENT));
						mFEQ->Pop();
						skippedFpgaEvents++;
						goto skipFE;
					}
				}
				//DEBUG
//				printf("CPU TS:		%08x\n", mCE->timeStamp);
//				printf("FPGA TS-FL:	%08x	//	FPGA TS:	%08x\n", mFE->timeStamp-fixedLatency-ltuDelay, mFE->timeStamp);
//				usleep(10000);
			} while(!matchCondition);
//			std::cout<<"[DBG] Merger->Timestamp matched!"<<std::endl;
			matchedType[mCE->type]++;
			matchedEvents++;
			this->FillEvent();
			mEQ->Enque(*mEVNT);
			prevTimeStamp = mFE->timeStamp-fixedLatency;
			prevTrigWord = mFE->triggerWord;
		}
	}
}

void Merger::FillEventHeader() {
	EVNT_HDR * header = reinterpret_cast<EVNT_HDR *>(mEVNT->event);
	header->eventLen = eventLength & 0x0000FFFF;
	header->eventNumLSB = mFE->triggerNumber & 0x000000FF;
	header->eventFlags = 0;
	header->eventTimestamp = mFE->timeStamp-fixedLatency;
}

void Merger::FillSpecialEventHeader() {
	EVNT_HDR * header = reinterpret_cast<EVNT_HDR *>(mEVNT->event);
	header->eventLen = eventLength & 0x0000FFFF;
	header->eventNumLSB = mFE->triggerNumber & 0x000000FF;
	header->eventFlags = 0;
	header->eventTimestamp = mFE->timeStamp;
}

void Merger::FillEobEventHeader() {
	EVNT_HDR * header = reinterpret_cast<EVNT_HDR *>(mEVNT->event);
	header->eventLen = eventLength & 0x0000FFFF;
	header->eventNumLSB = mFE->triggerNumber & 0x000000FF;
	header->eventFlags = 0x80;
	header->eventTimestamp = mFE->timeStamp;
}

void Merger::FillEvent() {
	EVNT_TO_PCF * event = reinterpret_cast<EVNT_TO_PCF *>(mEVNT->event+8);
	event->fineTime = mCE->fineTime;
	event->dataType = 1;
	event->pIdA = mCE->pIdA;
	event->pIdB = mCE->pIdB;
	event->pIdC = mCE->pIdC;
	event->pIdD = mCE->pIdD;
	event->pIdE = mCE->pIdE;
	event->pIdF = mCE->pIdF;
	event->pIdG = mCE->pIdG;
	event->prevTimeStamp = prevTimeStamp;
	event->trigWord = mFE->triggerWord;
	event->prevTrigWord = prevTrigWord;
//	event->endOfEvnt = mCE->timeStamp & 0x00FFFFFF;
}

void Merger::Print() {
	std::cout<<"Merger->Matched triggers:"<<std::endl;
	std::cout<<"Trigger mask 1:	"<<std::dec<<matchedType[1]<<std::endl;
	std::cout<<"Trigger mask 2:	"<<std::dec<<matchedType[2]<<std::endl;
	std::cout<<"Trigger mask 3:	"<<std::dec<<matchedType[3]<<std::endl;
	std::cout<<"Trigger mask 4:	"<<std::dec<<matchedType[4]<<std::endl;
	std::cout<<"Trigger mask 5:	"<<std::dec<<matchedType[5]<<std::endl;
	std::cout<<"Trigger mask 6:	"<<std::dec<<matchedType[6]<<std::endl;
	std::cout<<"Trigger mask 7:	"<<std::dec<<matchedType[7]<<std::endl;
	std::cout<<"Trigger mask 8:	"<<std::dec<<matchedType[8]<<std::endl;
	memset(matchedType, 0, sizeof(matchedType));
}

void Merger::FillSpecialEvent() {
	EVNT_TO_PCF * event = reinterpret_cast<EVNT_TO_PCF *>(mEVNT->event+8);
	event->dataType = 2;
	event->prevTimeStamp = prevTimeStamp;
	event->trigWord = mFE->triggerWord;
	event->prevTrigWord = prevTrigWord;
}
