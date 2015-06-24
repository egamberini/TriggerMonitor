/*
 * Types.hpp
 *
 *  Created on: Feb 20, 2015
 *      Author: gamberini
 */

#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <cstdint>

#include "Config.hpp"

struct LPE_HDR {
	uint32_t firstEventNum :24;
	uint8_t sourceID;
	uint16_t mepLength;
	uint8_t numEvents;
	uint8_t sourceSubID;
};

struct EVNT_HDR {
	uint16_t eventLen;
	uint8_t eventNumLSB;
	uint8_t eventFlags;
	uint32_t eventTimestamp;
};

struct udp_t {
	u_int len;
	u_char buf[udpPayload];
};

struct event_t {
	u_char event[eventLength];
};

struct EVNT_FROM_CPU {
	uint8_t fineTime;
	uint8_t poo;
	uint16_t pIdA;
	uint16_t pIdB;
	uint16_t pIdC;
	uint16_t pIdD;
	uint16_t pIdE;
	uint16_t pIdF;
	uint16_t pIdG;
};

struct CPU_EVENT {
	uint32_t timeStamp;
	uint8_t fineTime;
	uint8_t type;
	uint16_t pIdA;
	uint16_t pIdB;
	uint16_t pIdC;
	uint16_t pIdD;
	uint16_t pIdE;
	uint16_t pIdF;
	uint16_t pIdG;
};

struct FPGA_EVENT {
	uint32_t triggerNumber :24;
	uint8_t triggerWord;
	uint32_t timeStamp;
};

struct EVNT_TO_PCF {
	uint8_t fineTime;
	uint8_t dataType;
	uint16_t pIdA;
	uint16_t pIdB;
	uint16_t pIdC;
	uint16_t pIdD;
	uint16_t pIdE;
	uint16_t pIdF;
	uint16_t pIdG;
	uint32_t prevTimeStamp;
	uint8_t trigWord;
	uint8_t prevTrigWord;
	uint16_t evntWord;
//	uint32_t endOfEvnt :24;
};

struct EOB_INFO_PER_SRC { // 8 bytes
	uint32_t analyzedPrimitive;
	uint8_t timeWindow;
	uint8_t sourceID;
	uint16_t pad;
};

struct EOB_INFO_PER_TRIGTYPE { //28 bytes
	uint16_t lutMask[numOfSources];
	uint16_t pad0;
	uint32_t matchedTrig;
	uint32_t lostTrig;
	uint32_t downscal;
};

struct EOB_EVNT {
	EOB_INFO_PER_SRC source[numOfSources];			// 7 *  8 =  56 bytes
	uint32_t nTrigTypes;							// 1 *  4 =   4
	EOB_INFO_PER_TRIGTYPE trigType[numOfTrigTypes];	// 8 * 28 = 224
};													//    TOT = 284

#endif /* TYPES_HPP_ */
