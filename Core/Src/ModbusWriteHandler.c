#include "ModbusWriteHandler.h"

void handleRegisterWrite(uint16_t address)
{

	// for odd number registers toggle pin, for even - flash
	if (address % 2)
	{
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	}
	else
	{
		for (int i=0; i < 2; i++)
		{
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
			osDelay(100);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
			osDelay(100);
		}
	}

}
