/******************************************************************************
 *
 * Copyright (C) 2020 Xilinx, Inc.  All rights reserved.
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
 * Use of the Software is limited solely to applications:
 * (a) running on a Xilinx device, or
 * (b) that interact with a Xilinx device through a bus or interconnect.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of the Xilinx shall not be used
 * in advertising or otherwise to promote the sale, use or other dealings in
 * this Software without prior written authorization from Xilinx.
 *
 ******************************************************************************/
/*****************************************************************************/
/**
 *
 * @file xusbps_class_audio.h
 *
 * This file contains the implementation of chapter 9 specific code for
 * the example.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who	Date     Changes
 * ----- ---- -------- -------------------------------------------------------
 * 1.0   pm	20/02/20 First release
 *
 * </pre>
 *
 *****************************************************************************/
#ifndef  XUSB_CLASS_AUDIO_H
#define  XUSB_CLASS_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files **********************************/
#include "xusbps_ch9.h"
#include "xusbps_ch9_audio.h"
#include "xparameters.h"

/************************** Constant Definitions ******************************/
#ifdef XUSBPS_UAC1

/* Audio Class-Specific Request Codes */
#define UAC1_SET_CUR				0x01
#define UAC1_GET_CUR				0x81
#define UAC1_GET_MIN				0x82
#define UAC1_GET_MAX				0x83
#define UAC1_GET_RES				0x84

#else	/* XUSBPS_UAC2 */

/* A.14 Audio Class-Specific Request Codes */
#define UAC2_CS_CUR				0x01
#define UAC2_CS_RANGE				0x02

/* A.17 Control Selector Codes */
/* A.17.1 Clock Source Control Selectors */
#define UAC2_CS_CONTROL_UNDEFINED		0x00
#define UAC2_CS_CONTROL_SAM_FREQ		0x01
#define UAC2_CS_CONTROL_CLOCK_VALID		0x02

/* A.17.2 Clock Selector Control Selectors */
#define UAC2_CX_CONTROL_UNDEFINED		0x00
#define UAC2_CX_CONTROL_CLOCK_SEL		0x01

/* A.17.7 Feature Unit Control Selectors */
#define UAC2_FU_CONTROL_UNDEFINED		0x00
#define UAC2_FU_MUTE_CONTROL			0x01
#define UAC2_FU_VOLUME_CONTROL			0x02

#endif

/************************** Variable Definitions ******************************/

/************************** Function Prototypes *******************************/
void XUsbPs_ClassReq(XUsbPs *InstancePtr,
		XUsbPs_SetupData *SetupData);

#ifdef __cplusplus
}
#endif

#endif /* XUSB_CLASS_AUDIO_H */
