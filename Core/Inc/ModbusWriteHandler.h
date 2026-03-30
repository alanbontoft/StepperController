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

void handleRegisterWrite(uint16_t address);

#endif /* INC_MODBUSWRITEHANDLER_H_ */
