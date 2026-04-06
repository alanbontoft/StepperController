/*
 * ModbusWriteHandler.h
 *
 *  Created on: Sep 19, 2024
 *      Author: 103019535
 */

#ifndef INC_MODBUSWRITEHANDLER_H_
#define INC_MODBUSWRITEHANDLER_H_

#include <stdint.h>
#include "main.h"
#include "cmsis_os.h"
#include "modbus.h"
#include "globals.h"


void handleRegisterWrite(uint16_t address);
uint8_t validateWrite(modbusHandler_t *modH);

#endif /* INC_MODBUSWRITEHANDLER_H_ */
