/************************************************************************
 * Bus Communications App - messages
 ************************************************************************/
#ifndef BUS_COMMS_MSG_H
#define BUS_COMMS_MSG_H

#include "cfe_msg.h"

#define BUS_COMMS_NOOP_CC           0
#define BUS_COMMS_RESET_COUNTERS_CC 1

typedef struct
{
    CFE_MSG_TelemetryHeader_t TelemetryHeader;
    struct {
        uint8 CommandCounter;
        uint8 CommandErrorCounter;
        uint8 Spare[2];
    } Payload;
} BUS_COMMS_HkTlm_t;

#endif /* BUS_COMMS_MSG_H */
