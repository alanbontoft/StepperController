#include "ModbusWriteHandler.h"

extern uint16_t HoldingRegs[];

void handleRegisterWrite(uint16_t address)
{

	if (address == HR_FUNCTION)
	{
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

		switch(HoldingRegs[HR_FUNCTION])
		{
			case APP_FC_ROTATE:
				rotate();
				break;
			case APP_FC_SET_STEP:
				setStepSize();
				break;
		}
	}

}

/**
 * @brief
 * This method validates slave incoming data written to holding registers
 *
 * @return 0 if OK, EXCEPTION if anything fails
 * @ingroup modH Modbus handler
 */
uint8_t validateWrite(modbusHandler_t *modH)
{
	bool valid = true;
	    // check start address & nb range
	    uint16_t u16AdRegs = 0;
	    uint16_t u16Value = 0;
	    // uint16_t u16NRegs = 0;

	//uint8_t u8regs;
	switch ( modH->u8Buffer[ FUNC ] )
	{
		case MB_FC_WRITE_REGISTER:
			u16AdRegs = word( modH->u8Buffer[ ADD_HI ], modH->u8Buffer[ ADD_LO ]);

			// for single register write, value follows immediately after address values
			// hence name of registers (NB_HI, NB_LO) which are used for multi-register writes
			u16Value = word( modH->u8Buffer[ NB_HI ], modH->u8Buffer[ NB_LO ]);

			switch (u16AdRegs)
			{
				case HR_FUNCTION:
					valid = (u16Value <= APP_FC_SET_STEP);
					break;
				/*
				case HR_DIR:
					valid = (u16Value <= DIR_CCW);
					break;*/
				case HR_SPEED:
					valid = (u16Value <= SPEED_4);
					break;
				case HR_CHANNEL:
					valid = (u16Value < NUM_CHANNELS);
					break;
				case HR_STEP_SIZE:
					valid = (u16Value <= STEP_1_16);
					break;

				case HR_ANGLE_HI:
				case HR_ANGLE_LO:
				default:
					break;
			}

			break;

		case MB_FC_WRITE_MULTIPLE_REGISTERS:
			/*u16AdRegs = word( modH->u8Buffer[ ADD_HI ], modH->u8Buffer[ ADD_LO ]);
			u16NRegs = word( modH->u8Buffer[ NB_HI ], modH->u8Buffer[ NB_LO ]);
			if ((u16AdRegs + u16NRegs) > modH->u16HoldingRegSize) return EXC_ADDR_RANGE;

			//verify answer frame size in bytes
			u16NRegs = u16NRegs*2 + 5; // adding the header  and CRC
			if ( u16NRegs > 256 ) return EXC_REGS_QUANT;*/
			break;
	}

	if (!valid) return EXC_ILLEGAL_DATA;

	return 0; // OK, no exception code thrown
}
