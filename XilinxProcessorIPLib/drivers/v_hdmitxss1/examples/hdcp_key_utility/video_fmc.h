/******************************************************************************
*
* Copyright (C) 2018 – 2019 Xilinx, Inc.  All rights reserved.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*
******************************************************************************/

/*****************************************************************************/
/**
*
* @file si5324drv.h
*
* This file contains definitions for low-level driver functions for
* controlling the SiliconLabs Si5324 clock generator as mounted on the KC705
* demo board.
* The user should refer to the hardware device specification for more details
* of the device operation.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date        Changes
* ----- --- ----------   -----------------------------------------------
*           dd/mm/yyyy
* ----- --- ----------   -----------------------------------------------
* 1.00  gm  12/05/2018   Initial release
* </pre>
*
******************************************************************************/

#ifndef VIDEO_FMC_H_
#define VIDEO_FMC_H_

#include "xparameters.h"
#include "idt_8t49n24x.h"
#include "ti_lmk03318.h"

#define VFMC_GPIO_TX_CH4_DATASRC_SEL_MASK	0x00000004

#define VFMC_GPIO_RX_CH4_DATASRC_SEL_MASK	0x00040000

typedef enum {
	VFMC_GPIO_TX_LED0 	    = 0x00000001,
	VFMC_GPIO_TX_LED1 	    = 0x00000002,
	VFMC_GPIO_RX_LED0 	    = 0x00010000,
	VFMC_GPIO_RX_LED1 	    = 0x00020000,
} XVfmc_Gpio_Led;

typedef enum {
	VFMC_GPIO_TX_CH4_As_DataAndClock,
	VFMC_GPIO_TX_CH4_As_ClockOut,
	VFMC_GPIO_RX_CH4_As_Data,
	VFMC_GPIO_RX_CH4_As_Clock,
} XVfmc_Gpio_Ch4_DataClkSel;


int Vfmc_I2cMuxSelect(void *IicPtr);
int Vfmc_HdmiInit(u16 GpioDeviceId, void *IicPtr);
int Vfmc_PowerDownTiLMK03318(void *IicPtr, u8 Powerdown);
void Vfmc_Gpio_Led_On(XVfmc_Gpio_Led Led, u8 On);
void Vfmc_Gpio_Ch4_DataClock_Sel(XVfmc_Gpio_Ch4_DataClkSel DataClkSel);


#endif /* VIDEO_FMC_H_ */
