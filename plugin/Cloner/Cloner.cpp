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

/*	Cloner.cpp	

	This is a compiling husk of a project. Fill it in with interesting
	pixel processing code.
	
	Revision History

	Version		Change													Engineer	Date
	=======		======													========	======
	1.0			(seemed like a good idea at the time)					bbb			6/1/2002

	1.0			Okay, I'm leaving the version at 1.0,					bbb			2/15/2006
				for obvious reasons; you're going to 
				copy these files directly! This is the
				first XCode version, though.

	1.0			Let's simplify this barebones sample					zal			11/11/2010

	1.0			Added new entry point									zal			9/18/2017

*/

#include "Cloner.h"

static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"%s v%d.%d\r%s",
											StrName, 
											MAJOR_VERSION, 
											MINOR_VERSION, 
											StrDescription);
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

	out_data->out_flags =  PF_OutFlag_DEEP_COLOR_AWARE;	// just 16bpc, not 32bpc
	
	return PF_Err_NONE;
}

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;	

	AEFX_CLR_STRUCT(def);

	PF_ADD_POINT(StrCenterPoint.c_str(), 
		50, 
		50, 
		false, 
		CENTER_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_SLIDER(StrNumX_Param_Name.c_str(),
		CLONER_NUMX_MIN, 
		CLONER_NUMX_MAX,
		CLONER_NUMX_MIN,
		CLONER_NUMX_MAX,
		CLONER_NUMX_DEF,
		NUMX_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_SLIDER(StrNumY_Param_Name.c_str(),
		CLONER_NUMY_MIN,
		CLONER_NUMY_MAX,
		CLONER_NUMY_MIN,
		CLONER_NUMY_MAX,
		CLONER_NUMY_DEF,
		NUMY_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_SLIDER(StrDistance_Param_Name.c_str(),
		CLONER_DISTANCE_MIN,
		CLONER_DISTANCE_MAX,
		CLONER_DISTANCE_MIN,
		CLONER_DISTANCE_MAX,
		CLONER_DISTANCE_DEF,
		DISTANCE_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_SLIDER(StrDistanceY_Param_Name.c_str(),
		CLONER_DISTANCEY_MIN,
		CLONER_DISTANCEY_MAX,
		CLONER_DISTANCEY_MIN,
		CLONER_DISTANCEY_MAX,
		CLONER_DISTANCEY_DEF,
		DISTANCEY_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_SLIDER(StrSize_Param_Name.c_str(),
		CLONER_SIZE_MIN,
		CLONER_SIZE_MAX,
		CLONER_SIZE_MIN,
		CLONER_SIZE_MAX,
		CLONER_SIZE_DEF,
		SIZE_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_SLIDER(StrSizeY_Param_Name.c_str(),
		CLONER_SIZEY_MIN,
		CLONER_SIZEY_MAX,
		CLONER_SIZEY_MIN,
		CLONER_SIZEY_MAX,
		CLONER_SIZEY_DEF,
		SIZEY_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_LAYER(StrLayer.c_str(),
		PF_LayerDefault_NONE,
		LAYER_ID);
	
	out_data->num_params = CLONER_NUM_PARAMS;

	return err;
}

void DrawSquare(int xmin, int xmax, int ymin, int ymax, PF_EffectWorld *output)
{
	for (int x = xmin; x < xmax; x++)
	{
		for (int y = ymin; y < ymax; y++)
		{
			if (isInside(x, y, output))
			{
				PF_Pixel *myPixel = sampleIntegral32(*output, x, y);
				myPixel->red = myPixel->green = myPixel->blue = myPixel->alpha = 255;
			}
		}
	}
}

PF_Pixel *sampleIntegral32(PF_EffectWorld &def, int x, int y) 
{ 
	return (PF_Pixel*)((char*)def.data + 
		(y * def.rowbytes) + 
		(x * sizeof(PF_Pixel)));
}

PF_Boolean isInside(int x, int y, PF_EffectWorld *output)
{
	PF_Boolean isInside = false;

	if (x > 0 && x < output->width && y > 0 && y < output->height)
		isInside = true;

	return isInside;
}

static PF_Err 
Render (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);

	//clean slate
	err = PF_FILL(NULL, NULL, output);

	A_long numxF = 0, numyF = 0, distX, sizeX, distY, sizeY;
	A_long xmin = 0, xmax = 0;
	A_long ymin = 0, ymax = 0;
	A_long centerX, centerY;

	A_long xStart, xEnd, yStart, yEnd;
	A_long totalW, totalH;

	PF_ParamDef lyr;

	AEFX_CLR_STRUCT(lyr);

	//Obtaining selected layer at a specified time.
	ERR(PF_CHECKOUT_PARAM(in_data, CLONER_LAYER, in_data->current_time, in_data->time_step, in_data->time_scale, &lyr));
	
	PF_EffectWorld *layer = &lyr.u.ld;

	centerX = FIX2INT(params[CLONER_CENTER]->u.td.x_value);
	centerY = FIX2INT(params[CLONER_CENTER]->u.td.y_value);
	numxF = params[CLONER_NUMX]->u.sd.value;
	numyF = params[CLONER_NUMY]->u.sd.value;
	distX = params[CLONER_DISTANCE]->u.sd.value;
	distY = params[CLONER_DISTANCEY]->u.sd.value;
	sizeX = params[CLONER_SIZE]->u.sd.value;
	sizeY = params[CLONER_SIZEY]->u.sd.value;

	//If a layer is selected, set the sizeX and Y to width/height of that layer
	if (layer->data)
	{
		sizeX = layer->extent_hint.right;
		sizeY = layer->extent_hint.bottom;
	}

	//Setting total width and height of the clone area
	totalW = distX + (numxF*sizeX) + ((numxF - 1)*distX) + distX;
	totalH = distY + (numyF*sizeY) + ((numyF - 1)*distY) + distY;
	
	//To keep things centered, recalculating starting point depending on the center position
	xStart = centerX - (totalW / 2);
	yStart = centerY - (totalH / 2);

	PF_Rect rect = { 0,0,0,0 };

	for (int i = 0; i < numxF; i++)
	{
		for (int j = 0; j < numyF; j++)
		{
			//Setting up starting and ending X and Y pixel positions
			xmin = xStart + (i * (distX + sizeX)) + distX;
			xmax = xmin + sizeX;
			ymin = yStart + (j * (distY + sizeY)) + distY;
			ymax = ymin + sizeY;
			rect = { xmin, ymin, xmax, ymax };
			if (layer->data)
			{
				ERR(PF_COPY(layer, output, NULL, &rect));
			}
			else
			{
				DrawSquare(xmin, xmax, ymin, ymax, output);
			}
		}
	}
	/*
	Balance every PF_CHECKOUT_PARAM, with a PF_CHECKIN_PARAM. 
	Not doing so causes dismal performance and leaks memory. 
	Once checked in, the fields in the PF_ParamDef will no longer be valid.
	*/
	ERR(PF_CHECKIN_PARAM(in_data, &lyr));

	return err;
}


extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		"Cloner", // Name
		"ADBE Cloner", // Match Name
		"Sample Plug-ins", // Category
		AE_RESERVED_INFO); // Reserved Info

	return result;
}


PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_PARAMS_SETUP:

				err = ParamsSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

