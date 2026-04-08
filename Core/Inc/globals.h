/*
 * globals.h
 *
 *  Created on: 2 Apr 2026
 *      Author: Alan
 */

#ifndef INC_GLOBALS_H_
#define INC_GLOBALS_H_

#define STEPS_PER_REV 200

#define NUM_CHANNELS 3

#define HR_SIZE	7
#define IR_SIZE	4

typedef enum
{
    APP_FC_NOP,
	APP_FC_ROTATE,
	APP_FC_SET_STEP
} app_functioncode_t;

typedef enum
{
    APP_IDLE,
	APP_BUSY
} app_state_t;

typedef enum
{
	STEP_1,
	STEP_1_2,
	STEP_1_4,
	STEP_1_8,
	STEP_1_16,
} step_size_t;

typedef enum
{
	SPEED_1,
	SPEED_2,
	SPEED_3,
	SPEED_4
} speed_t;

typedef enum
{
	DIR_CW,
	DIR_CCW
} direction_t;

typedef enum
{
	HR_FUNCTION,
	HR_ANGLE_LO,
	HR_ANGLE_HI,
	HR_DIR,
	HR_SPEED,
	HR_CHANNEL,
	HR_STEP_SIZE

} holding_registers_t;

#endif /* INC_GLOBALS_H_ */
