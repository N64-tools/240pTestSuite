/* 
 * 240p Test Suite
 * Copyright (C)2011 Artemio Urbina
 *
 * This file is part of the 240p Test Suite
 *
 * The 240p Test Suite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The 240p Test Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include <kos.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"
#include "vmufs.h"

#include "controller.h"
#include "tests.h"
#include "patterns.h"

#include "help.h"
#include "menu.h"

/* romdisk */
extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);
KOS_INIT_FLAGS(INIT_DEFAULT);

int	region = FLASHROM_REGION_UNKNOWN;

void TestPatternsMenu(ImagePtr title, ImagePtr sd);
void DrawCredits(ImagePtr back);
void DrawIntro();

int main(void)
{
	int 		done = 0, sel = 1, joycnt = 0;
	uint16		pressed;
	ImagePtr	title, sd;
	controller	*st;
	char		error[256];

	if(cdrom_init() != 0)
		dbglog(DBG_ERROR, "Could not initialize GD-ROM\n");
	if(cdrom_spin_down() != ERR_OK)
		dbglog(DBG_ERROR,"Could not stop GD-ROM from spinning\n");

	vcable = vid_check_cable();
	region = flashrom_get_region();

	InitImages();

	if(!LoadVMUSave(error))
	{
		if(region == FLASHROM_REGION_EUROPE)
		{
			settings.EnablePAL = 1;
			IsPALDC = 1;
		}
	}
	else
	{
		if(region == FLASHROM_REGION_EUROPE)
			IsPALDC = 1;
		else
			settings.EnablePAL = 0;
	}
	
	if(vcable != CT_VGA)
		ChangeResolution(VIDEO_240P);
	else
		ChangeResolution(VIDEO_480P_SL);

	LoadFont();
	LoadScanlines();
	
	title = LoadKMG("/rd/back.kmg.gz", 0);
	sd = LoadKMG("/rd/SD.kmg.gz", 0);
	if(sd)
	{
		sd->x = 221;
		sd->y = 94;
	}
	
	DrawIntro();
	refreshVMU = 1;
	while(!done && !EndProgram) 
	{
		char	res[40];
		float 	r = 1.0f;
		float 	g = 1.0f;
		float 	b = 1.0f;
		int   	c = 1;
		float 	x = 40.0f;
		float 	y = 55.0f;
#ifndef NO_FFTW
		maple_device_t *sip = NULL;
#endif

		// Check cable again in case it was changed on the fly
		vcable = vid_check_cable();

		StartScene();
		DrawImage(title);
		DrawImage(sd);
		
#ifndef NO_FFTW
		sip = maple_enum_type(0, MAPLE_FUNC_MICROPHONE);
#endif
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Test Patterns"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Drop Shadow Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Striped Sprite Test"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Lag Test"); y += fh; c++;
#ifndef NO_FFTW
		if(sip)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Microphone Lag Test"); y += fh; c++;
		}
		else
		{
#endif
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Timing & Reflex Test"); y += fh; c++;
#ifndef NO_FFTW
		}
#endif
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid Scroll Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Horizontal Stripes"); y += fh; c++;    
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Checkerboard"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Backlit Zone Test"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Diagonal Test"); y += fh; c++;
		if(vcable == CT_VGA)
		{
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				"Alternating 240p/480i Test"); y += fh; c++;
		}
		else
		{
			if(!IsPAL)
			{
				DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
					"Alternating 240p/480i Test"); y += fh; c++;
			}
			else
			{
				DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
					"Alternating 288p/576i Test"); y += fh; c++;
			}
		}
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sound Test"); y += fh; c++;

		res[0] = '\0';
		GetVideoModeStr(res, 0);
		DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, res); y += fh; c++;
		DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Configuration"); y += fh; c++;
		DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Help"); 

#ifdef DCLOAD
		//if((vmode == VIDEO_480P || vmode == VIDEO_480P_SL) && vcable == CT_VGA)
		{
			c++;
			DrawStringS(x, y +fh, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Video Settings"); 
		}
#endif

		r = 0.8f;
		g = 0.8f;
		b = 0.8f;
		switch(vcable)
		{
			case CT_RGB:
				DrawStringS(225.0f, 225.0f, r, g, b, "RGB");
				break;
			case CT_VGA:
				DrawStringS(225.0f, 225.0f, r, g, b, "VGA");
				break;
			case CT_COMPOSITE:
				DrawStringS(225.0f, 225.0f, r, g, b, "Composite");
				break;
		}
		
		switch(region)
		{
			case FLASHROM_REGION_UNKNOWN:
				DrawStringS(225.0f, 215.0f, r, g, b, "??????");
				break;
			case FLASHROM_REGION_JAPAN:
				DrawStringS(225.0f, 215.0f, r, g, b, "Japan");
				break;
			case FLASHROM_REGION_US:
				DrawStringS(225.0f, 215.0f, r, g, b, "USA");
				break;
			case FLASHROM_REGION_EUROPE:
				DrawStringS(225.0f, 215.0f, r, g, b, "Europe");
				break;
		}
		
		EndScene();
		VMURefresh("240p Test", "");
		
		st = ReadController(0, &pressed);
	
		if (pressed & CONT_START)
			ShowMenu(GENERALHELP);

		if (pressed & CONT_X)
			ToggleScanlineEvenOdd();

		if (pressed & CONT_DPAD_RIGHT && st->buttons & CONT_Y)
			RaiseScanlineIntensity();

		if (pressed & CONT_DPAD_LEFT && st->buttons & CONT_Y)
			LowerScanlineIntensity();

		if (pressed & CONT_DPAD_UP)
		{
			sel --;
			if(sel < 1)
				sel = c;
		}
		
		if (pressed & CONT_DPAD_DOWN)
		{
			sel ++;
			if(sel > c)
				sel = 1;
		}
		
		if( st && st->joyy != 0)
		{
			if(++joycnt > 5)
			{
				if(st && st->joyy > 0)
					sel ++;
				if(st && st->joyy < 0)
					sel --;
	
				if(sel < 1)
					sel = c;
				if(sel > c)
					sel = 1;
				joycnt = 0;
			}
		}
		else
			joycnt = 0;
		
		if (pressed & CONT_A)
		{
			refreshVMU = 1;
			switch(sel)
			{
				case 1:
					TestPatternsMenu(title, sd);
					break;
				case 2:	
					DropShadowTest();
					break;
				case 3:
					StripedSpriteTest();
					break;
				case 4:
					PassiveLagTest();
					break;
				case 5:
#ifndef NO_FFTW
					if(sip)
						SIPLagTest();
					else
#endif
						ReflexNTimming();
					break;
				case 6:
					ScrollTest();
					break;
				case 7:
					GridScrollTest();
					break;
				case 8:
					DrawStripes();
					break;
				case 9:
					DrawCheckBoard();
					break;
				case 10:
					LEDZoneTest();
					break;
				case 11:
					DiagonalPatternTest();
					break;
				case 12:
					if(vcable != CT_VGA)
						Alternate240p480i();
					break;
				case 13:
					SoundTest();
					break;
				case 14:
					SelectVideoMode(title);
					break;
				case 15:
					ShowMenu(GENERALHELP);
					break;
				case 16:
					HelpWindow(GENERALHELP, title);
					break;
#ifdef DCLOAD
				case 17:
					TestVideoMode(vmode);
					break;
#endif
			}
			refreshVMU = 1;
		}
	}

	updateVMU(" Goodbye ", " m(_ _)m ", 1);

	FreeImage(&title);		
	FreeImage(&sd);		
	ReleaseScanlines();
	ReleaseFont();
	CleanImages();
	cdrom_shutdown();
#ifndef DCLOAD
	arch_menu();
#endif
	return 0;
}

void TestPatternsMenu(ImagePtr title, ImagePtr sd)
{
	int 			done = 0, sel = 1, joycnt = 0;
	uint16			pressed;		
	controller		*st;

	while(!done && !EndProgram) 
	{		
		float 	r = 1.0f;
		float 	g = 1.0f;
		float 	b = 1.0f;
		int			c = 1;
		float 	x = 40.0f;
		float 	y = 55.0f;

		StartScene();
		DrawImage(title);
		DrawImage(sd);

		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Pluge"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars"); y += fh; c++;
		if(IsPAL)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "EBU Color Bars"); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "SMPTE Color Bars"); y += fh; c++;
		}
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bars with Gray Scale"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Color Bleed Check"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Grid 224p"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Linearity"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Monoscope"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Gray Ramp"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "White & RGB screens"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "100 IRE"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Sharpness"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Overscan"); y += fh; c++;
		DrawStringS(x, y + fh, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); y += fh; 

		r = 0.8f;
		g = 0.8f;
		b = 0.8f;
		switch(vcable)
		{
			case CT_RGB:
				DrawStringS(265.0f, 225.0f, r, g, b, "RGB");
				break;
			case CT_VGA:
				DrawStringS(265.0f, 225.0f, r, g, b, "VGA");
				break;
			case CT_COMPOSITE:
				DrawStringS(265.0f, 225.0f, r, g, b, "Composite");
				break;
		}
		
		switch(region)
		{
			case FLASHROM_REGION_UNKNOWN:
				DrawStringS(265.0f, 215.0f, r, g, b, "??????");
				break;
			case FLASHROM_REGION_JAPAN:
				DrawStringS(265.0f, 215.0f, r, g, b, "Japan");
				break;
			case FLASHROM_REGION_US:
				DrawStringS(265.0f, 215.0f, r, g, b, "USA");
				break;
			case FLASHROM_REGION_EUROPE:
				DrawStringS(265.0f, 215.0f, r, g, b, "Europe");
				break;
		}
		
		EndScene();
		VMURefresh("Patterns", "");
		st = ReadController(0, &pressed);
		if (pressed & CONT_START)
			ShowMenu(GENERALHELP);

		if (pressed & CONT_DPAD_RIGHT && st->buttons & CONT_Y)
			RaiseScanlineIntensity();

		if (pressed & CONT_DPAD_LEFT && st->buttons & CONT_Y)
			LowerScanlineIntensity();

		if (pressed & CONT_DPAD_UP)
		{
			sel --;
			if(sel < 1)
				sel = c;
		}
	
		if (pressed & CONT_DPAD_DOWN)
		{
			sel ++;
			if(sel > c)
				sel = 1;
		}
	
		if(st && st->joyy != 0)
		{
			if(++joycnt > 5)
			{
				if(st && st->joyy > 0)
					sel ++;
				if(st && st->joyy < 0)
					sel --;
	
				if(sel < 1)
					sel = c;
				if(sel > c)
					sel = 1;
				joycnt = 0;
			}
		}
		else
			joycnt = 0;
	
		if (pressed & CONT_B)
			done = 1;
	
		if (pressed & CONT_A)
		{
			refreshVMU = 1;
			switch(sel)
			{
				case 1:
					DrawPluge();
					break;
				case 2:
					DrawColorBars();
					break;
				case 3:
					DrawSMPTEColorBars();
					break;
				case 4:
					Draw601ColorBars();
					break;
				case 5:
					DrawColorBleed();
					break;
				case 6:
					DrawGrid(1);
					break;
				case 7:
					DrawGrid(0);
					break;
				case 8:
					DrawLinearity(1);
					break;					
				case 9:
					//DrawLinearity(0);
					DrawMonoscope();
					break;					
				case 10:
					DrawGrayRamp();
					break;
				case 11:
					DrawWhiteScreen();
					break;
				case 12:
					Draw100IRE();
					break;
				case 13:
					DrawSharpness();
					break;
				case 14:
					DrawOverscan();
					break;
				case 15:
					done = 1;
					break;
			} 			
			refreshVMU = 1;
		}			
	}

	return;
}
