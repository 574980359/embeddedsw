/******************************************************************************
*
* Copyright (C) 2019 Xilinx, Inc. All rights reserved.
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
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMANGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*
*
******************************************************************************/


#include "xplmi_scheduler.h"
#include "xplmi_task.h"


static XPlmi_Scheduler_t Sched;

static int XPlimi_IsTask_active(XPlmi_Scheduler_t *SchedPtr, int TaskListIndex)
{
	int ReturnVal = FALSE;

	/* Periodic */
	if ((0U != SchedPtr->TaskList[TaskListIndex].Interval)
		&& (NULL != SchedPtr->TaskList[TaskListIndex].CustomerFunc)
		&& (0U == (SchedPtr->Tick
				% SchedPtr->TaskList[TaskListIndex].Interval))) {
		ReturnVal = TRUE;
	} else if ((0U == SchedPtr->TaskList[TaskListIndex].Interval)
			&& (NULL != SchedPtr->TaskList[TaskListIndex].CustomerFunc)) {
		/* Non-Periodic */
		ReturnVal = TRUE;
	} else {
		/** Do Nothing */
	}

	return ReturnVal;
}

static int XPlmi_IsTask_NonPeriodic(XPlmi_Scheduler_t *SchedPtr, int TaskListIndex)
{
	int ReturnVal = FALSE;

	if ((0U == SchedPtr->TaskList[TaskListIndex].Interval)
		&& (NULL != SchedPtr->TaskList[TaskListIndex].CustomerFunc)) {
		ReturnVal = TRUE;
	} else {
		/** Do Nothing */
	}

	return ReturnVal;
}

int XPlmi_SchedulerInit(void)
{
	int Idx;
	int Status = XST_FAILURE;


	/* Disable all the tasks */
	for (Idx = 0U; Idx < XPLMI_SCHED_MAX_TASK; Idx++) {
		Sched.TaskList[Idx].Interval = 0U;
		Sched.TaskList[Idx].CustomerFunc = NULL;
		Sched.TaskList[Idx].Status = XPLMI_TASK_STATUS_DISABLED;
	}

	Sched.Enabled = FALSE;
	Sched.PitBaseAddr = 0x0;
	Sched.Tick = 0U;

	/* Successfully completed init */
	Status = XST_SUCCESS;
	return Status;
}

int XPlmi_SchedulerStart(XPlmi_Scheduler_t *SchedPtr)
{
	int Status = XST_FAILURE;

	if (SchedPtr == NULL) {
		goto done;
	}
	else
	{
		SchedPtr->Enabled = TRUE;
		Status = XST_SUCCESS;
	}

done:
	return Status;
}

int XPlmi_SchedulerStop(XPlmi_Scheduler_t *SchedPtr)
{
	SchedPtr->Enabled =FALSE;
	return XST_SUCCESS;
}

int XPlmi_SchedulerHandler(void)
{
	int Idx;
	int Status = XST_FAILURE;
	XPlmi_TaskNode *Task;
    /* XPlmi_Printf(DEBUG_GENERAL,"Received :XPlmi_SchedulerHandler\n\r"); */
	XPlmi_UtilRMW(PMC_PMC_MB_IO_IRQ_ACK, PMC_PMC_MB_IO_IRQ_ACK, 0x20);

	/*
        There is a silicon problem where on 2-4% of Versal ES1 S80 devices
        you can get 12A of VCCINT_PL current before CFI housecleaning is run.
        The problem is eliminated when PL Vgg frame housecleaning is run
        so we need to do that ASAP after PLM is loaded.
	It is possible that rail is not up at boot, so keep checking at
	regular interval. Ideally a task should be added and removed once done
	But scheduler is not working as expected hence adding a direct call
	TODO: Remove direct call from here when scheduler is fixed
        */
        XPmPlDomain_HcleanPl();

	for (Idx = 0U; Idx < XPLMI_SCHED_MAX_TASK; Idx++)
	{
		/* Check if the task is triggered and has a valid Callback */
		if (NULL != Sched.TaskList[Idx].CustomerFunc)
		{
			/* Execute the Task */
			Task = XPlmi_TaskCreate(XPLM_TASK_PRIORITY_1, Sched.TaskList[Idx].CustomerFunc, 0U);
			if (Task == NULL)
			{
				Status = XPLMI_UPDATE_STATUS(XPLM_ERR_TASK_CREATE, 0x0);
				goto END;
			}
			XPlmi_TaskTriggerNow(Task);
			}
	}
END:
	return Status;
}

int XPlmi_SchedulerAddTask(XPlmi_Callback_t CallbackFn, int MilliSeconds)
{
	int Idx;
	int Status = XST_FAILURE;

	/* Get the Next Free Task Index */
	for (Idx=0U;Idx < XPLMI_SCHED_MAX_TASK;Idx++) {
		if (NULL == Sched.TaskList[Idx].CustomerFunc){
			break;
		}
	}

	/* Check if we have reached Max Task limit */
	if (XPLMI_SCHED_MAX_TASK == Idx) {
		goto done;
	}

	/* Add Interval as a factor of TICK_MILLISECONDS */
	Sched.TaskList[Idx].Interval = MilliSeconds;
	Sched.TaskList[Idx].OwnerId = 0;
	Sched.TaskList[Idx].CustomerFunc = CallbackFn;
	Status = XST_SUCCESS;

done:
	return Status;
}

int XPlmi_SchedulerRemoveTask(XPlmi_Scheduler_t *SchedPtr, int OwnerId, int MilliSeconds, XPlmi_Callback_t CallbackFn)
{
	int Idx;
	u32 TaskCount = 0U;

	/*Find the Task Index */
	for (Idx = 0U; Idx < XPLMI_SCHED_MAX_TASK; Idx++) {
		if ((CallbackFn == SchedPtr->TaskList[Idx].CustomerFunc) &&
		    (SchedPtr->TaskList[Idx].OwnerId == OwnerId) &&
		    ((SchedPtr->TaskList[Idx].Interval == (MilliSeconds)) ||
				(0U == MilliSeconds))) {
			SchedPtr->TaskList[Idx].Interval = 0U;
			SchedPtr->TaskList[Idx].OwnerId = 0U;
			SchedPtr->TaskList[Idx].CustomerFunc = NULL;
			TaskCount++;
		}
	}

	XPlmi_Printf(DEBUG_DETAILED,"%s: Removed %lu tasks\r\n",
			__func__, TaskCount);

	return ((TaskCount > 0U) ? XST_SUCCESS : XST_FAILURE);
}
