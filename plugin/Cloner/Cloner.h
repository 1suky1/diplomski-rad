/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

/*
	Cloner.h
*/

#pragma once

#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096

#define PF_DEEP_COLOR_AWARE 1	// make sure we get 16bpc pixels; 
								// AE_Effect.h checks for this.

#include "AEConfig.h"

#ifdef AE_OS_WIN
	typedef unsigned short PixelType;
	#include <Windows.h>
#endif

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "AE_EffectCBSuites.h"
#include "AE_GeneralPlug.h"
#include "AEFX_ChannelDepthTpl.h"
#include "AEGP_SuiteHandler.h"

#include <string>


const std::string
	StrName{ "Cloner" },
	StrDescription{ "Create clones." },
	StrNumX_Param_Name{ "X Clones" },
	StrNumY_Param_Name{ "Y Clones" },
	StrSize_Param_Name{ "Size X" },
	StrSizeY_Param_Name{ "Size Y" },
	StrDistance_Param_Name{ "Distance X" },
	StrDistanceY_Param_Name{ "Distance Y" },
	StrCenterPoint{ "Origin" },
	StrLayer{ "Layer" };

/* Versioning information */

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1


/* Parameter defaults */

const double
	CLONER_DISTANCE_MIN = 0.0,
	CLONER_DISTANCE_MAX = 2000.0,
	CLONER_DISTANCE_DEF = 100.0,
	CLONER_DISTANCEY_MIN = 0.0,
	CLONER_DISTANCEY_MAX = 2000.0,
	CLONER_DISTANCEY_DEF = 100.0,
	CLONER_SIZE_MIN = 0.0,
	CLONER_SIZE_MAX = 300.0,
	CLONER_SIZE_DEF = 100.0,
	CLONER_SIZEY_MIN = 0.0,
	CLONER_SIZEY_MAX = 300.0,
	CLONER_SIZEY_DEF = 100.0;

const int
	CLONER_NUMX_MIN = 1,
	CLONER_NUMX_MAX = 1000,
	CLONER_NUMX_DEF = 3,
	CLONER_NUMY_MIN = 1,
	CLONER_NUMY_MAX = 1000,
	CLONER_NUMY_DEF = 3;

enum {
	CLONER_INPUT = 0,
	CLONER_CENTER,
	CLONER_NUMX,
	CLONER_NUMY,
	CLONER_DISTANCE,
	CLONER_DISTANCEY,
	CLONER_SIZE,
	CLONER_SIZEY,
	CLONER_LAYER,
	CLONER_NUM_PARAMS
};

enum {
	CENTER_ID = 1,
	NUMX_DISK_ID,
	NUMY_DISK_ID,
	DISTANCE_DISK_ID,
	DISTANCEY_DISK_ID,
	SIZE_DISK_ID,
	SIZEY_DISK_ID,
	LAYER_ID
};

typedef struct ClonerInfo{
	PF_FpLong	numxF;
	PF_FpLong	numyF;
	PF_FpLong	distF;
	PF_FpLong	sizeF;
	A_long		centerX;
	A_long		centerY;
	A_long		xID;
	A_long		yID;
	A_long		width;
	A_long		height;
} ClonerInfo;


extern "C" {

	DllExport
	PF_Err
	EffectMain(
		PF_Cmd			cmd,
		PF_InData		*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output,
		void			*extra);

}

