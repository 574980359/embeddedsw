/******************************************************************************
* Copyright (c) 2018 - 2020 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/


/*****************************************************************************/
/**
*
* @file xplmi_cdo.c
*
* This file contains code to handling the CDO Buffer.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date        Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00  kc   08/23/2018 Initial release
* 1.01  ma   02/03/2020 Change XPlmi_MeasurePerfTime to retrieve Performance
*                       time and print
*
* </pre>
*
* @note
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "xplmi_cdo.h"
#include "xplmi_proc.h"

/************************** Constant Definitions *****************************/
#define XPLMI_CMD_LEN_TEMPBUF		(0x8U)

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/*****************************************************************************/

/*****************************************************************************/
/**
 * @brief	This function will calculate the size of the command. Bits 16 to 23
 * denote the size of the command. If the value is 255, then the word following
 * CmdId would denote the size of the command.
 *
 * @param	Buf is pointer to buffer
 * @param	Len is length of the command that is available in memory to parse
 *
 * @return	Size of the command
 *
 *****************************************************************************/
static u32 XPlmi_CmdSize(u32 *Buf, u32 Len)
{
	u32 Size = 1U;
	if (Len >= Size) {
		u32 CmdId = Buf[0U];
		u32 PayloadLen = (CmdId & XPLMI_CMD_LEN_MASK) >> 16U;
		if (PayloadLen == 255U) {
			Size = 2U;
			if (Len >= Size) {
				PayloadLen = Buf[1U];
			}
		}
		Size += PayloadLen;
	}
	return Size;
}

/*****************************************************************************/
/**
 * @brief	This function will setup the command structure.
 *
 * @param	Cmd is pointer to the command structure
 * @param	Buf is pointer to the command buffer
 * @param	BufLen is length of the buffer. It may not have total Command
 * length if buffer is not available.
 *
 * @return None
 *
 *****************************************************************************/
static void XPlmi_SetupCmd(XPlmi_Cmd * Cmd, u32 *Buf, u32 BufLen)
{
	u32 HdrLen = 1U;

	Cmd->CmdId = Buf[0U];
	Cmd->IpiMask = 0U;
	Cmd->Len = (Cmd->CmdId >> 16U) & 255U;
	Cmd->Payload = Buf + 1U;
	Cmd->ProcessedLen = 0U;
	if (Cmd->Len == 255U) {
		HdrLen = 2U;
		Cmd->Len = Buf[1U];
		Cmd->Payload = Buf + 2U;
	}

	/* Assign the available payloadlen in the buffer */
	if (Cmd->Len > (BufLen - HdrLen)) {
		Cmd->PayloadLen = BufLen - HdrLen;
	} else {
		Cmd->PayloadLen = Cmd->Len;
	}
}

/*****************************************************************************/
/**
 * @brief	This function verifies CDO header.
 *
 * @param	CdoPtr is pointer to the CDO structure
 *
 * @return	XST_SUCCESS on success and error code on failure
 *
 *****************************************************************************/
static int XPlmi_CdoVerifyHeader(XPlmiCdo *CdoPtr)
{
	int Status = XST_FAILURE;
	u32 CheckSum = 0U;
	u32 *CdoHdr = CdoPtr->BufPtr;
	u32 Index;

	if (CdoHdr[1U] != XPLMI_CDO_HDR_IDN_WRD) {
		XPlmi_Printf(DEBUG_GENERAL,
				"CDO Header Identification Failed\n\r");
		Status = XPLMI_UPDATE_STATUS(XPLMI_ERR_CDO_HDR_ID, 0x0U);
		goto END;
	}
	for (Index = 0U; Index < (XPLMI_CDO_HDR_LEN - 1U); Index++) {
		CheckSum += CdoHdr[Index];
	}

	/* Invert checksum */
	CheckSum ^= 0xFFFFFFFFU;
	if (CheckSum != CdoHdr[Index]) {
		XPlmi_Printf(DEBUG_GENERAL,
				"Config Object Checksum Failed\n\r");
		Status = XPLMI_UPDATE_STATUS(XPLMI_ERR_CDO_CHECKSUM, 0x0U);
		goto END;
	} else {
		Status = XST_SUCCESS;
	}

	XPlmi_Printf(DEBUG_INFO,
		"Config Object Version 0x%08x\n\r", CdoHdr[2U]);
	XPlmi_Printf(DEBUG_INFO,
		"Length 0x%08x\n\r", CdoHdr[3U]);

END:
	return Status;
}

/*****************************************************************************/
/**
 * @brief	This function initializes the CDO pointer structure.
 *
 * @param	CdoPtr is pointer to the CDO structure
 *
 * @return	None
 *
 *****************************************************************************/
void XPlmi_InitCdo(XPlmiCdo *CdoPtr)
{
	/* Initialize the CDO structure variables */
	CdoPtr->CopiedCmdLen = 0U;
	CdoPtr->CmdState = XPLMI_CMD_STATE_START;
	CdoPtr->BufLen = 0U;
	CdoPtr->CdoLen = 0U;
	CdoPtr->ProcessedCdoLen = 0U;
	CdoPtr->ImgId = 0U;
	CdoPtr->PrtnId = 0U;
	CdoPtr->DeferredError = FALSE;

	memset(&CdoPtr->Cmd.KeyHoleParams, 0U, sizeof(XPlmi_KeyHoleParams));
	CdoPtr->Cmd.KeyHoleParams.PdiSrc = 0xFFU;
	/* Initialize the CDO buffer user params */
	CdoPtr->CmdEndDetected = FALSE;
	CdoPtr->Cdo1stChunk = TRUE;
}

/*****************************************************************************/
/**
 * @brief	This function will copy the remaining to tempbuf based on the
 * command size.
 *
 * @param	CdoPtr is pointer to the CDO structure
 * @param	BufPtr is pointer to the buffer
 * @param	Size is pointer to the Size of the data copied to tempbuf
 *
 * @return	XST_SUCCESS
 *
 *****************************************************************************/
static int XPlmi_CdoCopyCmd(XPlmiCdo *CdoPtr, u32 *BufPtr, u32 *Size)
{
	int Status = XST_FAILURE;

	/* Copy the remaining cmd data */
	if (CdoPtr->CopiedCmdLen == 1U) {
		/*
		 * To know the size, we need 2nd argument if
		 * length is greater than 255.
		 * Copy the 2nd argument to tempbuf to get the
		 * size correctly
		 */
		CdoPtr->TempCmdBuf[1U] = BufPtr[0U];
		CdoPtr->CopiedCmdLen++;
		BufPtr++;
	}

	/* If size is greater than tempbuf, copy only tempbuf size */
	*Size = XPlmi_CmdSize(CdoPtr->TempCmdBuf, CdoPtr->CopiedCmdLen);
	if (*Size > XPLMI_CMD_LEN_TEMPBUF) {
		*Size = XPLMI_CMD_LEN_TEMPBUF;
	}

	/*
	 * Copy the remaining cmd data to TempCmdBuf only if
	 * command size is greater than copied length
	 */
	if (*Size > CdoPtr->CopiedCmdLen) {
		memcpy(CdoPtr->TempCmdBuf + CdoPtr->CopiedCmdLen, BufPtr,
			(*Size - CdoPtr->CopiedCmdLen) * XPLMI_WORD_LEN);
	}
	CdoPtr->CopiedCmdLen = 0U;
	Status = XST_SUCCESS;

	return Status;
}

/*****************************************************************************/
/**
 * @brief	This function will update the command pointer and resume the
 * command from previous state.
 *
 * @param	CdoPtr Pointer to the CDO structure
 * @param	BufPtr Pointer to the buffer
 * @param	BufLen Len of the buffer
 * @param	Size Pointer to the Size consumed by the command execution
 *
 * @return	XST_SUCCESS in case of success
 *
 *****************************************************************************/
static int XPlmi_CdoCmdResume(XPlmiCdo *CdoPtr, u32 *BufPtr, u32 BufLen, u32 *Size)
{
	int Status = XST_FAILURE;
	XPlmi_Cmd *CmdPtr = &CdoPtr->Cmd;

	/* Update the Payload buffer and length */
	if (CmdPtr->Len > (CmdPtr->ProcessedLen + BufLen)) {
		CmdPtr->PayloadLen = BufLen;
	} else {
		CmdPtr->PayloadLen = CmdPtr->Len - CmdPtr->ProcessedLen;
		CdoPtr->CmdState = XPLMI_CMD_STATE_START;
	}

	/* Copy the image id to cmd subsystem ID */
	CmdPtr->SubsystemId = CdoPtr->ImgId;
	CmdPtr->Payload = BufPtr;
	*Size = CmdPtr->PayloadLen;
	Status = XPlmi_CmdResume(CmdPtr);
	if (Status != XST_SUCCESS) {
		XPlmi_Printf(DEBUG_GENERAL,
			"CMD: 0x%0x Resume failed, Processed Cdo Length 0x%0x\n\r",
			CmdPtr->CmdId, CdoPtr->ProcessedCdoLen);
		goto END;
	}

END:
	return Status;
}

/*****************************************************************************/
/**
 * @brief	This function copies gets the prepares the CMD pointer and
 * executes it.
 *
 * @param	CdoPtr is pointer to the CDO structure
 * @param	BufPtr is pointer to the buffer
 * @param	BufLen is length of the buffer
 * @param	Size is pointer to the Size consumed by the command execution
 *
 * @return	XST_SUCCESS on success and error code on failure
 *
 *****************************************************************************/
static int XPlmi_CdoCmdExecute(XPlmiCdo *CdoPtr, u32 *BufPtr, u32 BufLen, u32 *Size)
{
	int Status = XST_FAILURE;
	XPlmi_Cmd *CmdPtr = &CdoPtr->Cmd;

	/*
	 * Break if CMD says END of commands,
	 * irrespective of the CDO length
	 */
	if (BufPtr[0U] == XPLMI_CMD_END) {
		XPlmi_Printf(DEBUG_INFO, "CMD END detected \n\r");
		CdoPtr->CmdEndDetected = TRUE;
		Status = XST_SUCCESS;
		goto END;
	}

	*Size = XPlmi_CmdSize(BufPtr, BufLen);
	CmdPtr->Len = *Size;
	/*
	 * Check if Cmd payload is less than buffer size,
	 * then copy to temporary buffer
	 */
	if ((*Size > BufLen) && (BufLen < XPLMI_CMD_LEN_TEMPBUF)) {
		/* Copy Cmd to temporary buffer */
		memcpy(CdoPtr->TempCmdBuf, BufPtr, BufLen * XPLMI_WORD_LEN);
		CdoPtr->CopiedCmdLen = BufLen;
		*Size = BufLen;
		Status = XST_SUCCESS;
		goto END;
	}

	/*
	 * If size is greater than tempbuf, execute partially
	 * and resume the cmd in next iteration
	 */
	if (*Size > BufLen) {
		*Size = BufLen;
		CdoPtr->CmdState = XPLMI_CMD_STATE_RESUME;
	}
	/* Copy the image id to cmd subsystem ID */
	CmdPtr->SubsystemId = CdoPtr->ImgId;

	/* Execute the command */
	XPlmi_SetupCmd(CmdPtr, BufPtr, *Size);
	CmdPtr->DeferredError = FALSE;
	Status = XPlmi_CmdExecute(CmdPtr);
	if (Status != XST_SUCCESS) {
		XPlmi_Printf(DEBUG_GENERAL,
			"CMD: 0x%0x execute failed, Processed Cdo Length 0x%0x\n\r",
			CmdPtr->CmdId,
			CdoPtr->ProcessedCdoLen + CdoPtr->BufLen - BufLen);
		goto END;
	}
	if(CmdPtr->Len == CmdPtr->PayloadLen - 1U) {
		CdoPtr->ProcessedCdoLen +=  CdoPtr->Cmd.KeyHoleParams.ExtraWords;
		CmdPtr->PayloadLen = CmdPtr->Len;
		CdoPtr->CmdState = XPLMI_CMD_STATE_START;
		CdoPtr->CopiedCmdLen = 0U;
	}

END:
	return Status;
}

/*****************************************************************************/
/**
 * @brief	This function process the CDO file.
 *
 * @param	CdoPtr is pointer to the CDO structure
 *
 * @return	XST_SUCCESS on success and error code on failure
 *
 *****************************************************************************/
int XPlmi_ProcessCdo(XPlmiCdo *CdoPtr)
{
	int Status = XST_FAILURE;
	u32 Size = 0U;
	u32 CopiedCmdLen = CdoPtr->CopiedCmdLen;
	u32 *BufPtr = CdoPtr->BufPtr;
	u32 BufLen = CdoPtr->BufLen;
#ifdef PLM_PRINT_PERF_CDO_PROCESS
	u64 ProcessTime = XPlmi_GetTimerValue();
	XPlmi_PerfTime PerfTime = {0U};
#endif

	/* Verify the header for the first chunk of CDO */
	if (CdoPtr->Cdo1stChunk == TRUE) {
		Status = XPlmi_CdoVerifyHeader(CdoPtr);
		if (Status != XST_SUCCESS) {
			goto END;
		}
		CdoPtr->Cdo1stChunk = FALSE;
		CdoPtr->CdoLen = BufPtr[3U];

		BufPtr += XPLMI_CDO_HDR_LEN;
		BufLen -= XPLMI_CDO_HDR_LEN;
		CdoPtr->BufLen -= XPLMI_CDO_HDR_LEN;
	}

	/*
	 * Check if BufLen is greater than CdoLen
	 * This is required if more buffer is copied than CDO len.
	 * Mainly for PLM CDO where BufLen is not present and is
	 * given as Max PRAM len.
	 */
	if ((BufLen + CdoPtr->ProcessedCdoLen) > (CdoPtr->CdoLen)) {
		BufLen = CdoPtr->CdoLen - CdoPtr->ProcessedCdoLen;
		CdoPtr->BufLen = BufLen;
	}

	/*
	 * In case CmdEnd is detected in previous iteration,
	 * it just returns
	 */
	if (CdoPtr->CmdEndDetected == TRUE) {
		Status = XST_SUCCESS;
		goto END;
	}

	XPlmi_Printf(DEBUG_INFO,
			"Processing CDO, Chunk Len 0x%08x\n\r", BufLen);
	/*
	 * Check if cmd data is copied
	 * partially during the last iteration
	 */
	if (CopiedCmdLen != 0U) {
		Status = XPlmi_CdoCopyCmd(CdoPtr, BufPtr, &Size);
		if (Status != XST_SUCCESS) {
			goto END;
		}
		BufPtr = CdoPtr->TempCmdBuf;
		BufLen = Size;
	}

	/* Execute the commands in the Cdo Buffer */
	while (BufLen > 0U) {
		/* Check if cmd has to be resumed */
		if (CdoPtr->CmdState == XPLMI_CMD_STATE_RESUME) {
			Status =
				XPlmi_CdoCmdResume(CdoPtr, BufPtr, BufLen, &Size);
		} else {
			Status =
				XPlmi_CdoCmdExecute(CdoPtr, BufPtr, BufLen, &Size);
		}
		CdoPtr->DeferredError |= CdoPtr->Cmd.DeferredError;
		/*
		 * If command end is detected, or in case of any error,
		 * exit the loop
		 */
		if ((Status != XST_SUCCESS) ||
			(CdoPtr->CmdEndDetected == TRUE)) {
			goto END;
		}

		/* Update the parameters for next iteration */
		if (CopiedCmdLen != 0U) {
			BufPtr = CdoPtr->BufPtr + (Size - CopiedCmdLen);
			BufLen = CdoPtr->BufLen - (Size - CopiedCmdLen);
			CopiedCmdLen = 0U;
		} else {
			BufPtr += Size;
			BufLen -= Size;
		}
	}

	CdoPtr->ProcessedCdoLen += CdoPtr->BufLen;
	Status = XST_SUCCESS;

END:
#ifdef PLM_PRINT_PERF_CDO_PROCESS
	XPlmi_MeasurePerfTime(ProcessTime, &PerfTime);
	XPlmi_Printf(DEBUG_PRINT_PERF,
			"%u.%u ms Cdo Processing time\n\r",
			(u32)PerfTime.TPerfMs, (u32)PerfTime.TPerfMsFrac);
#endif
	return Status;
}
