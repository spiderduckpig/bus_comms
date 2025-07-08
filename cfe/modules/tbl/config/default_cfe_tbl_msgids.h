/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *   CFE Event Services (CFE_TBL) Application Message IDs
 */
#ifndef CFE_TBL_MSGIDS_H
#define CFE_TBL_MSGIDS_H

#include "cfe_core_api_base_msgids.h"
#include "cfe_tbl_topicids.h"

/*
** cFE Command Message Id's
*/
#define CFE_TBL_CMD_MID     CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_TBL_CMD_MSG     /* 0x1804 */
#define CFE_TBL_SEND_HK_MID CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_TBL_SEND_HK_MSG /* 0x180C */

/*
** CFE Telemetry Message Id's
*/
#define CFE_TBL_HK_TLM_MID  CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_TBL_HK_TLM_MSG  /* 0x0804 */
#define CFE_TBL_REG_TLM_MID CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_TBL_REG_TLM_MSG /* 0x080C */

#endif
