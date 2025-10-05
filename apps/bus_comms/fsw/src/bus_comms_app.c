/************************************************************************
 * Bus Communications App - minimal skeleton based on sample_app
 ************************************************************************/

#include "bus_comms_app.h"
#include "bus_comms_events.h"
#include "bus_comms_version.h"
#include "bus_comms_table.h"

#include <string.h>

BUS_COMMS_AppData_t BUS_COMMS_AppData;

void BUS_COMMS_AppMain(void)
{
    int32            status;
    CFE_SB_Buffer_t *SBBufPtr;

    CFE_ES_PerfLogEntry(BUS_COMMS_APP_PERF_ID);

    status = BUS_COMMS_AppInit();
    if (status != CFE_SUCCESS)
    {
        BUS_COMMS_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    while (CFE_ES_RunLoop(&BUS_COMMS_AppData.RunStatus) == true)
    {
        CFE_ES_PerfLogExit(BUS_COMMS_APP_PERF_ID);

        status = CFE_SB_ReceiveBuffer(&SBBufPtr, BUS_COMMS_AppData.CmdPipe, CFE_SB_PEND_FOREVER);

        CFE_ES_PerfLogEntry(BUS_COMMS_APP_PERF_ID);

        if (status == CFE_SUCCESS)
        {
            BUS_COMMS_ProcessCommandPacket(SBBufPtr);
        }
        else
        {
            CFE_EVS_SendEvent(BUS_COMMS_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                              "BUS_COMMS: SB Pipe Read Error, App Will Exit");
            BUS_COMMS_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    CFE_ES_PerfLogExit(BUS_COMMS_APP_PERF_ID);
    CFE_ES_ExitApp(BUS_COMMS_AppData.RunStatus);
}

int32 BUS_COMMS_AppInit(void)
{
    int32 status;

    BUS_COMMS_AppData.RunStatus = CFE_ES_RunStatus_APP_RUN;
    BUS_COMMS_AppData.CmdCounter = 0;
    BUS_COMMS_AppData.ErrCounter = 0;

    BUS_COMMS_AppData.PipeDepth = BUS_COMMS_APP_PIPE_DEPTH;
    strncpy(BUS_COMMS_AppData.PipeName, "BUS_COMMS_CMD_PIPE", sizeof(BUS_COMMS_AppData.PipeName));
    BUS_COMMS_AppData.PipeName[sizeof(BUS_COMMS_AppData.PipeName) - 1] = 0;

    status = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("BUS_COMMS: Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    CFE_MSG_Init(CFE_MSG_PTR(BUS_COMMS_AppData.HkTlm.TelemetryHeader), CFE_SB_ValueToMsgId(BUS_COMMS_HK_TLM_MID),
                 sizeof(BUS_COMMS_AppData.HkTlm));

    status = CFE_SB_CreatePipe(&BUS_COMMS_AppData.CmdPipe, BUS_COMMS_AppData.PipeDepth, BUS_COMMS_AppData.PipeName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("BUS_COMMS: Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BUS_COMMS_SEND_HK_MID), BUS_COMMS_AppData.CmdPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("BUS_COMMS: Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BUS_COMMS_CMD_MID), BUS_COMMS_AppData.CmdPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("BUS_COMMS: Error Subscribing to CMD, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    CFE_EVS_SendEvent(BUS_COMMS_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION, "BUS_COMMS App Initialized. %s",
                      BUS_COMMS_VERSION_STRING);

    return CFE_SUCCESS;
}

void BUS_COMMS_ProcessCommandPacket(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_SB_MsgId_t MsgId = CFE_SB_INVALID_MSG_ID;

    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);

    switch (CFE_SB_MsgIdToValue(MsgId))
    {
        case BUS_COMMS_CMD_MID:
            BUS_COMMS_ProcessGroundCommand(SBBufPtr);
            break;
        case BUS_COMMS_SEND_HK_MID:
            BUS_COMMS_ReportHousekeeping((CFE_MSG_CommandHeader_t *)SBBufPtr);
            break;
        default:
            CFE_EVS_SendEvent(BUS_COMMS_INVALID_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "BUS_COMMS: invalid command packet, MID = 0x%x",
                              (unsigned int)CFE_SB_MsgIdToValue(MsgId));
            break;
    }
}

void BUS_COMMS_ProcessGroundCommand(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_MSG_FcnCode_t CommandCode = 0;
    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &CommandCode);

    switch (CommandCode)
    {
        case BUS_COMMS_NOOP_CC:
            // TODO: implement NOOP
            BUS_COMMS_AppData.CmdCounter++;
            CFE_EVS_SendEvent(BUS_COMMS_COMMANDNOP_INF_EID, CFE_EVS_EventType_INFORMATION,
                              "BUS_COMMS: NOOP command %s", BUS_COMMS_VERSION_STRING);
            break;
        case BUS_COMMS_RESET_COUNTERS_CC:
            BUS_COMMS_AppData.CmdCounter = 0;
            BUS_COMMS_AppData.ErrCounter = 0;
            CFE_EVS_SendEvent(BUS_COMMS_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION,
                              "BUS_COMMS: RESET command");
            break;
        default:
            CFE_EVS_SendEvent(BUS_COMMS_COMMAND_ERR_EID, CFE_EVS_EventType_ERROR,
                              "BUS_COMMS: Invalid ground command code: CC = %d", CommandCode);
            BUS_COMMS_AppData.ErrCounter++;
            break;
    }
}

int32 BUS_COMMS_ReportHousekeeping(const CFE_MSG_CommandHeader_t *Msg)
{
    BUS_COMMS_AppData.HkTlm.Payload.CommandErrorCounter = BUS_COMMS_AppData.ErrCounter;
    BUS_COMMS_AppData.HkTlm.Payload.CommandCounter      = BUS_COMMS_AppData.CmdCounter;

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BUS_COMMS_AppData.HkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(BUS_COMMS_AppData.HkTlm.TelemetryHeader), true);

    return CFE_SUCCESS;
}
