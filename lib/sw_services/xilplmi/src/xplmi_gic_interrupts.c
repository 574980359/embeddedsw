/******************************************************************************
*
* Copyright (C) 2019 Xilinx, Inc.  All rights reserved.
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
* @file xplmi_gic_interrupts.c
*
* This file is to handle the GIC interrupts
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date        Changes
* ====  ==== ======== ======================================================-
* 1.00  mg   10/08/2018 Initial release
*
* </pre>
*
* @note
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "xplmi_gic_interrupts.h"
#include "xplmi_hw.h"
#include "xplmi_util.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/
static struct GicIntrHandlerTable
   g_GicPInterruptTable[XPLMI_GICP_SOURCE_COUNT][XPLMI_NO_OF_BITS_IN_REG] = {
#ifdef XPAR_XIPIPSU_0_DEVICE_ID
	[XPLMI_PMC_GIC_IRQ_GICP0][XPLMI_GICP0_SRC27] =
		{(void *)0U, XPlmi_IpiDispatchHandler},
#endif
};

/*****************************************************************************/
/**
 * @brief This will register the GIC handler
 *
 * @param	None
 * @return	None
 *****************************************************************************/
void XPlmi_GicRegisterHandler(u32 PlmIntrId, Function_t Handler, void *Data)
{
	u32 GicPVal;
	u32 GicPxVal;

	/** Get the GicP mask */
	GicPVal = (PlmIntrId & XPLMI_GICP_MASK)>>8U;
	GicPxVal = (PlmIntrId & XPLMI_GICPX_MASK)>>16U;

	/** Register Handler */
	g_GicPInterruptTable[GicPVal][GicPxVal].GicHandler = Handler;
	g_GicPInterruptTable[GicPVal][GicPxVal].Data = Data;

}

/*****************************************************************************/
/**
 * @brief This will enable the GIC interrupt
 *
 * @param	None
 * @return	None
 *****************************************************************************/
void XPlmi_GicIntrEnable(u32 PlmIntrId)
{
	u32 GicPVal;
	u32 GicPxVal;

	/** Get the GicP mask */
	GicPVal = (PlmIntrId & XPLMI_GICP_MASK)>>8U;
	GicPxVal = (PlmIntrId & XPLMI_GICPX_MASK)>>16U;

	/* Enable interrupt */
	XPlmi_UtilRMW(PMC_GLOBAL_GICP_PMC_IRQ_ENABLE,
		1U<<GicPVal,
		1U<<GicPVal);

	XPlmi_UtilRMW(PMC_GLOBAL_GICP0_IRQ_ENABLE + (GicPVal*0x14),
		1U<<GicPxVal,
		1U<<GicPxVal);
}

/*****************************************************************************/
/**
 * @brief This will disable the GIC interrupt
 *
 * @param	None
 * @return	None
 *****************************************************************************/
void XPlmi_GicIntrDisable(u32 PlmIntrId)
{
	u32 GicPVal;
	u32 GicPxVal;

	/** Get the GicP mask */
	GicPVal = (PlmIntrId & XPLMI_GICP_MASK)>>8U;
	GicPxVal = (PlmIntrId & XPLMI_GICPX_MASK)>>16U;

	/* Disable interrupt */
	XPlmi_UtilRMW(PMC_GLOBAL_GICP0_IRQ_DISABLE + (GicPVal*0x14),
		1U<<GicPxVal,
		1U<<GicPxVal);
}

/*****************************************************************************/
/**
 * @brief This is handler for GIC interrupts
 *
 * @param	CallbackRef is presently the interrupt number that is received
 *
 * @return	void
 *
 *****************************************************************************/
void XPlmi_GicIntrHandler(void *CallbackRef)
{
	u32 GicPIntrStatus;
	u32 GicPNIntrStatus;
	u32 GicIndex;
	u32 GicPIndex;

	(void )CallbackRef;

	GicPIntrStatus = XPlmi_In32(XPLMI_GICP_IRQ_STATUS);
	XPlmi_Printf(DEBUG_DETAILED,
		     "GicPIntrStatus: 0x%x\r\n", GicPIntrStatus);

	for (GicIndex = 0U; GicIndex < XPLMI_GICP_SOURCE_COUNT; GicIndex++) {

		if (GicPIntrStatus & (1U << GicIndex)) {
			GicPNIntrStatus =
			  XPlmi_In32(XPLMI_GICP0_IRQ_STATUS + (GicIndex*0x14));
			XPlmi_Printf(DEBUG_DETAILED,
				     "GicP%d Intr Status: 0x%x\r\n",
				     GicIndex, GicPNIntrStatus);

			for (GicPIndex = 0U; GicPIndex <
			     XPLMI_NO_OF_BITS_IN_REG; GicPIndex++) {

				if (GicPNIntrStatus & (1U<<GicPIndex)) {
					if(g_GicPInterruptTable[GicIndex][GicPIndex].GicHandler != NULL) {
						g_GicPInterruptTable[GicIndex][GicPIndex].GicHandler(
							g_GicPInterruptTable[GicIndex][GicPIndex].Data);
					}
					else {
						XPlmi_Printf(DEBUG_INFO, "%s: Error: Unhandled GIC interrupt received\n\r", __func__);
					}

					XPlmi_Out32((XPLMI_GICP0_IRQ_STATUS +
						    (GicIndex*0x14U)),
						    (1U<<GicPIndex));

				}
			}
			XPlmi_Out32(XPLMI_GICP_IRQ_STATUS, (1U << GicIndex));
		}
	}

}
