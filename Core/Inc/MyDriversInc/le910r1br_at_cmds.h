/*
 * le910r1br_at_cmds.h
 *
 *  Created on: Oct 5, 2024
 *      Author: lucas
 */

#ifndef INC_MYDRIVERSINC_LE910R1BR_AT_CMDS_H_
#define INC_MYDRIVERSINC_LE910R1BR_AT_CMDS_H_

const char *AT_RSP_OK = "OK\r\n";
const char *AT_RSP_ERROR = "ERROR\r\n";

const char *AT_CMD_AT = "AT";
const char *AT_CMD_ATZ = "ATZ";
const char *AT_CMD_REPEAT = "A/";

const char *AT_CMD_ATE = "ATE";

const char *AT_CMD_CPIN = "+CPIN";
const char *AT_PRM_CPIN_PIN = "=1234";
const char *AT_PRM_CPIN_NEWPIN = ",1234";

const char *AT_CMD_CFUN = "+CFUN";
const char *AT_CMD_COPS = "+COPS";
const char *AT_CMD_BND = "#BND";
const char *AT_CMD_CSQ = "+CSQ";
const char *AT_CMD_REBOOT = "#REBOOT";
const char *AT_CMD_SHDN = "#SHDN";
const char *AT_CMD_CGATT = "+CGATT";
const char *AT_CMD_CGACT = "+CGACT";

const char *AT_PRM_ASK = "?";

const char *AT_PRM_NUM_0 = "0";
const char *AT_PRM_NUM_1 = "1";
const char *AT_PRM_NUM_2 = "2";
const char *AT_PRM_NUM_3 = "3";
const char *AT_PRM_NUM_4 = "4";
const char *AT_PRM_NUM_5 = "5";
const char *AT_PRM_NUM_6 = "6";
const char *AT_PRM_NUM_7 = "7";
const char *AT_PRM_NUM_8 = "8";
const char *AT_PRM_NUM_9 = "9";
const char *AT_PRM_NUM_10 = "10";
const char *AT_PRM_NUM_11 = "11";
const char *AT_PRM_NUM_12 = "12";

#endif /* INC_MYDRIVERSINC_LE910R1BR_AT_CMDS_H_ */
