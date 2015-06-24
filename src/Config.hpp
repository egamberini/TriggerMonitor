/*
 * Config.hpp
 *
 *  Created on: Feb 19, 2015
 *      Author: gamberini
 */

#ifndef TRIGGERMONITOR_V0_SRC_CONFIG_HPP_
#define TRIGGERMONITOR_V0_SRC_CONFIG_HPP_

#include <string>
#include <cstdint>

/* segesta CPU event reception */
const std::string localIP1("192.168.2.2");
const uint16_t localPort1 = 0xE621;
const std::string remoteIP1("192.168.2.192");
const uint16_t remotePort1 = 0xD621;

/* LTU monitoring */
//const std::string localIP2("192.168.1.192");
//const uint16_t localPort2 = 0xE624;
//const std::string remoteIP2("192.168.1.9");
//const uint16_t remotePort2 = 0xD622;

/* TTC monitoring */
const std::string localIP2("192.168.1.192");
const uint16_t localPort2 = 0xE623;
const std::string remoteIP2("192.168.1.8");
const uint16_t remotePort2 = 0xD622;

/* PC farm MEP transmission */
const std::string localPcFarmIP("10.194.12.30");
const uint16_t localPcFarmPort = 0xD621;
const uint16_t remotePcFarmPort = 0xE621;

const int udpPayload = 1472;

const int eventLength = 24 + 8; //event + header

const int fixedLatency = 0x8FB0;

const int MepFactor = 1;

const int numOfSources = 7;

const int numOfTrigTypes = 8;

#endif /* TRIGGERMONITOR_V0_SRC_CONFIG_HPP_ */
