/* 
 * r
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

#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "font.h"
#include "video.h"
#include "options.h"
#include "controller.h"

#include "patterns.h"

#include "help.h"
#include "menu.h"

void DrawPluge()
{
	int 		done = 0, text = 0, oldvmode = vmode, ShowHelp = 0;
	u32			pressed;		
	ImagePtr	back = NULL, backPAL, backNTSC, black, highlight;	
	char		msg[50];
	
	backNTSC = LoadImage(PLUGEIMG, 0);
	if(!backNTSC)
		return;
	backPAL = LoadImage(PLUGEPALIMG, 0);
	if(!backPAL)
	{
		FreeImage(&backNTSC);	
		return;
	}	
	black = LoadImage(WHITEIMG, 1);
	if(!black)
	{
		FreeImage(&backPAL);
		FreeImage(&backNTSC);
		return;
	}	
	highlight = LoadImage(PLUGEBORDERIMG, 0);
	if(!highlight)
	{
		FreeImage(&backPAL);
		FreeImage(&black);
		FreeImage(&backNTSC);
		return;
	}	
	
	SetTextureColor(black, 0, 0, 0);	
	
	back = backPAL;
	if(!IsPAL)
		black->h = 240;
	else
		black->h = 264;	
			
	while(!done && !EndProgram) 
	{
		if(oldvmode != vmode)
		{
			back = backNTSC;
			if(!IsPAL)
				black->h = 240;
			else
				black->h = 264;
			oldvmode = vmode;
		}
				
		StartScene();
		
		DrawImage(black);  
		DrawImage(back);
		
		if(ShowHelp)
		{
			SetTextureColor(highlight, 0xff, 0x00, 0x00);
			
			highlight->y = 39;

			DrawStringB(14, 205, 0xff, 0x00, 0, back == backNTSC ? "11.5" : "3.5");
			highlight->x = 14;			
			DrawImage(highlight);
			
			DrawStringB(44, 205, 0xff, 0x00, 0, back == backNTSC ? "7.5" : "2");
			highlight->x = 44;			
			DrawImage(highlight);
						
			highlight->r = 0x0;
			highlight->g = 0xff;							
			DrawStringB(74, 205, 0xff, 0x00, 0, back == backNTSC ? "3.5" : "1");
			highlight->x = 74;			
			DrawImage(highlight);
			
			
			DrawStringB(228, 205, 0xff, 0x00, 0, back == backNTSC ? "3.5" : "1");
			highlight->x = 228;			
			DrawImage(highlight);
						
			highlight->r = 0xff;
			highlight->g = 0x0;
			DrawStringB(259, 205, 0xff, 0x00, 0, back == backNTSC ? "7.5" : "2");
			highlight->x = 259;			
			DrawImage(highlight);
			
			DrawStringB(289, 205, 0xff, 0x00, 0, back == backNTSC ? "11.5" : "3.5");
			highlight->x = 289;			
			DrawImage(highlight);

			ShowHelp--;
		}
		
		
		if(text)
		{			
			DrawStringB(228, 20, 0, 0xff, 0, msg);
			text --;
		}		
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;			
			HelpData = PLUGEHELP;			
		}
			
		if (pressed & PAD_BUTTON_B)
			done =	1;										
			
		if (pressed & PAD_BUTTON_A)
		{
			if(!IsPAL)
			{
				if(back == backNTSC)
				{
					sprintf(msg, "RGB Full Range");
					back = backPAL;
				}
				else
				{
					sprintf(msg, "NTSC 7.5 IRE");
					back = backNTSC;
				}
					
				text = 60;				
			}
		}
		
		if (pressed & PAD_BUTTON_X)
			ShowHelp = 100;

	}
	FreeImage(&highlight);
	FreeImage(&backNTSC);
	FreeImage(&backPAL);
	FreeImage(&black);
	return;
}

void DrawGrayRamp()
{
	int 		done = 0;
	u32			pressed;		
	ImagePtr	back;	
	
	back = LoadImage(GRAYIMG, 0);
	if(!back)
		return;
			
	while(!done && !EndProgram) 
	{				
		StartScene();
		        
		DrawImage(back);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if (pressed & PAD_BUTTON_B)
			done =	1;								
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = GRAYHELP;
		}		
	}
	FreeImage(&back);
	return;
}

void DrawWhiteScreen()
{
	int 		done = 0, color = 0, BlackLevel = 0x00, text = 0;
	int			cr, cb, cg, sel = 1, editmode = 0, oldvmode = 0;
	u32			pressed;		
	ImagePtr	back;	
	char		msg[100], *mode[5] = { "White", "Black", "Red", "Green", "Blue" };
	
	back = LoadImage(WHITEIMG, 1);
	if(!back)
		return;
			
	back->w = dW;
	back->h = dH;
	
	if(!IsPAL)
		BlackLevel = 0x13; // 7.5 IRE
	else
		BlackLevel = 0x00; // 0 IRE
		
	cr = cb = cg = 0xff; // white
	while(!done && !EndProgram) 
	{		
		if(oldvmode != vmode)
		{
			back->w = dW;
			back->h = dH;
			oldvmode = vmode;
		}
		
		if(IsPAL)
			BlackLevel = 0x00;
			
		StartScene();
		        
		DrawImage(back);
		
		if(text)
		{						
			DrawStringB(200, 20, 0xff, 0xff, 0xff, msg);			
			text --;
		}			
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if (pressed & PAD_BUTTON_B)
			done =	1;						
			
		if (pressed & PAD_BUTTON_A && color == 1 && !IsPAL)
		{
			if(!BlackLevel)
			{
				BlackLevel = 0x13;
				sprintf(msg, "#GBlack Level: 7.5 IRE#G");
			}
			else
			{
				BlackLevel = 0x0;
				sprintf(msg, "#GBlack Level: 0 IRE#G");
			}
			text = 140;
		}				
		
		if (pressed & PAD_BUTTON_A && color == 0)
			editmode = !editmode;
		
		if(editmode)
		{
			int *current = NULL;
							
			sprintf(msg, "#%cR:0x%x#W #%cG:0x%x#W #%cB:0x%x#W", 
					sel == 1 ? 'G' : 'W', cr,
					sel == 2 ? 'G' : 'W', cg, 
					sel == 3 ? 'G' : 'W', cb);
			text = 1;

			if ( pressed & PAD_BUTTON_LEFT )
			{
				sel --;
				if(sel < 1)
					sel = 3;
			}
			
			if ( pressed & PAD_BUTTON_RIGHT )
			{
				sel ++;
				if(sel > 3)
					sel = 1;
			}
			
			switch(sel)
			{
				case 1:
					current = &cr;
					break;
				case 2:
					current = &cg;
					break;
				case 3:
					current = &cb;
					break;
			}
			
			if ( pressed & PAD_BUTTON_UP )
			{				
				if(current)
				{
					(*current) ++;
					if(*current > 0xff)
						*current = 0xff;
				}
			}
			
			if ( pressed & PAD_BUTTON_DOWN )
			{			
				if(current)
				{
					(*current) --;
					if(*current < 0)
						*current = 0;
				}
			}	

			if ( pressed & PAD_BUTTON_Y )
			{				
				if(current)			
					*current  = 0;					
			}
			
			if ( pressed & PAD_BUTTON_X )
			{			
				if(current)				
					*current = 0xff;
			}	
		}
		
		if (pressed & PAD_TRIGGER_R)
		{
			color ++;
			if(color > 4)
				color = 0;		
			
			editmode = 0;
			if(color == 0 && cr + cb + cg != 3*0xff)
				sprintf(msg, "%s [EDITED]", mode[color]);
			else
				sprintf(msg, "%s", mode[color]);
			text = 30;
		}

		if (pressed & PAD_TRIGGER_L)
		{			
			color --;
			if(color < 0)
				color = 4;			
				
			editmode = 0;
			if(color == 0 && cr + cb + cg != 3*0xff)
				sprintf(msg, "%s [edited]", mode[color]);
			else
				sprintf(msg, "%s", mode[color]);
			text = 30;
		}
			
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = WHITEHELP;
		}			
		
		switch(color)
		{
				case 0:
					SetTextureColor(back, cr, cg, cb);
					break;
				case 1:
					SetTextureColor(back, BlackLevel, BlackLevel, BlackLevel);
					break;
				case 2:
					SetTextureColor(back, 0xff, 0x00, 0x00);
					break;
				case 3:
					SetTextureColor(back, 0x00, 0xff, 0x00);
					break;
				case 4:
					SetTextureColor(back, 0x00, 0x00, 0xff);
					break;
		}
	}
	FreeImage(&back);
	return;
}

void DrawColorBars()
{
	int 		done = 0, type = 2;
	u32         pressed;		
	ImagePtr	back, backhigh, backlow, backgrid;
	
	back = LoadImage(COLORIMG, 0);
	if(!back)
		return;
	backlow = LoadImage(COLORLOWIMG, 0);
	if(!backlow)
		return;
	backhigh = LoadImage(COLORHIGHIMG, 0);
	if(!backhigh)
		return;
	backgrid = LoadImage(COLORGRIDIMG, 0);
	if(!backgrid)
	{
		FreeImage(&back);
		return;
	}
			
	while(!done && !EndProgram) 
	{				
		StartScene();
		        
		switch(type)
		{
			case 1:
				DrawImage(backlow);
				break;
			case 2:
				DrawImage(back);
				break;
			case 3:
				DrawImage(backhigh);
				break;
			case 4:
				DrawImage(backgrid);
				break;
		}
		
        EndScene();	

		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_B)
			done =	1;								
		
		if (pressed & PAD_BUTTON_LEFT)
			type--;
			
		if (pressed & PAD_BUTTON_RIGHT)
			type++;
			
		if(type < 1)
			type = 1;
		if(type > 4)
			type = 4;

		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = COLORBARSHELP;
		}			
	}
	FreeImage(&backgrid);
	FreeImage(&backlow);
	FreeImage(&backhigh);
	FreeImage(&back);
	return;
}

void Draw601ColorBars()
{
	int 		done = 0;
	u32			pressed;		
	ImagePtr	back;
	
	back = LoadImage(CB601701IMG, 0);
	if(!back)
		return;
			
	while(!done && !EndProgram) 
	{			
		StartScene();
		        
		DrawImage(back);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_B)
			done =	1;								
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = COLOR601HELP;
		}	
	}
	FreeImage(&back);
	return;
}

void DrawEBUColorBars()
{
	int 		done = 0, is75 = 0, text = 0;
	u32			pressed;		
	ImagePtr	backEBU75, backEBU100;
	char		msg[40];
		
	backEBU75 = LoadImage(EBUCB75IMG, 0);
	if(!backEBU75)
	{
		return;
	}
		
	backEBU100 = LoadImage(EBUCB100IMG, 0);
	if(!backEBU100)
	{
		FreeImage(&backEBU75);
		return;
	}
	
	IgnoreOffset(backEBU75);
	IgnoreOffset(backEBU100);
			
	while(!done && !EndProgram) 
	{		
		StartScene();
		
		if(is75)
			DrawImage(backEBU75);
		else
			DrawImage(backEBU100);
		
		if(text)
		{			
			DrawStringB(260, 20, 0, 0xff, 0, msg);
			text --;
		}		
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_B)
			done =	1;								
			
		if (pressed & PAD_BUTTON_A)
		{
			is75 = !is75;
			text = 30;
			sprintf(msg, "%s%%", is75 ? "75" : "100");
		}
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = SMPTECOLOR;
		}		

	}
	FreeImage(&backEBU75);	
	FreeImage(&backEBU100);	
	return;
}

void DrawSMPTEColorBars()
{
	int 		done = 0, is75 = 0, text = 0;
	u32			pressed;		
	ImagePtr	backNTSC75, backNTSC100;
	char		msg[40];
	
	backNTSC75 = LoadImage(SMPTECB75IMG, 0);
	if(!backNTSC75)
		return;
		
	backNTSC100 = LoadImage(SMPTECB100IMG, 0);
	if(!backNTSC75)
	{
		FreeImage(&backNTSC75);
		return;
	}
				
	while(!done && !EndProgram) 
	{		
		StartScene();
		
		if(is75)
			DrawImage(backNTSC75);
		else
			DrawImage(backNTSC100);

		if(text)
		{			
			DrawStringB(260, 20, 0, 0xff, 0, msg);
			text --;
		}		
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_B)
			done =	1;								
			
		if (pressed & PAD_BUTTON_A)
		{
			is75 = !is75;
			text = 30;
			sprintf(msg, "%s%%", is75 ? "75" : "100");
		}
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = SMPTECOLOR;
		}		

	}
	FreeImage(&backNTSC75);
	FreeImage(&backNTSC100);
	return;
}

void DrawColorBleed()
{
	int 		done = 0, type = 0;
	u32			pressed;		
	ImagePtr	back, backchk;
	
	back = LoadImage(COLORBLEEDIMG, 0);
	if(!back)
		return;
	backchk = LoadImage(COLORBLEEDCHKIMG, 0);
	if(!backchk)
		return;
		
	while(!done && !EndProgram) 
	{		
		StartScene();
		        
		if(!type)
			DrawImage(back);
		else
			DrawImage(backchk);
		
        EndScene();		
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_BUTTON_B)
			done =	1;								

		if (pressed & PAD_BUTTON_A)
			type = !type;

		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = COLORBLEEDHELP;
		}				
	}
	FreeImage(&back);
	FreeImage(&backchk);
	return;
}

void DrawGrid()
{
	int 		done = 0, oldvmode = vmode, text = 0, bggreen = 0;
	u32			pressed;		
	ImagePtr	back = NULL, blckbg = NULL;
	char		msg[60];
	
	
	blckbg = LoadImage(WHITEIMG, 1);
	if(!blckbg)
		return;
	
	SetTextureColor(blckbg, 0, 0, 0);
	
	blckbg->w = dW;
	blckbg->h = dH;
		
	while(!done && !EndProgram) 
	{		
		if(oldvmode != vmode)
		{
			if(back)
				FreeImage(&back);	
			oldvmode = vmode;
			
			blckbg->w = dW;
			blckbg->h = dH;
		}
		
		if(!back)
		{
			if(vmode == VIDEO_480I || vmode == VIDEO_480P)
			{
				back = LoadImage(GRID480IMG, 0);
				if(!back)
					return;
				back->scale = 0;		
			}
			
			if(vmode == VIDEO_288P || vmode == VIDEO_576I_A264)
			{
				back = LoadImage(GRIDPALIMG, 0);
				if(!back)
					return;    				
			}
			
			if(vmode == VIDEO_576I)
			{
				back = LoadImage(GRIDPAL480IMG, 0);
				if(!back)
					return;
			}
			
			if(!back)
			{
				back = LoadImage(GRIDIMG, 0);
				if(!back)
					return;		
			}
			
			IgnoreOffset(back);
		}
		
		StartScene();
		
		DrawImage(blckbg);        
		DrawImage(back);
		
		if(text)
		{			
			DrawStringB(60, 40, 0, 0xff, 0, msg);
			text --;
		}
		
        EndScene();		
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_B)
			done =	1;								
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = GRIDHELP;
			oldvmode = vmode;
		}				
		
		if ( pressed & PAD_BUTTON_UP ) 
		{
			text = 30;
			back->y --;
		}
		if ( pressed & PAD_BUTTON_DOWN ) 		
		{
			text = 30;
			back->y ++;
		}	
		if ( pressed & PAD_BUTTON_LEFT ) 		
		{
			text = 30;
			back->x --;
		}
		if ( pressed & PAD_BUTTON_RIGHT ) 		
		{
			text = 30;
			back->x ++;
		}
		
		if ( pressed & PAD_BUTTON_A ) 		
		{
			text = 30;
			back->x = 0;
			back->y = 0;
		}
		
		if (pressed & PAD_BUTTON_X)
		{
			bggreen = !bggreen;
			if(bggreen == 1)
				blckbg->g = 0xff;
			else
				blckbg->g = 0x00;
		}
		
		if(text)
			sprintf(msg, "Grid origin in video signal: [%d,%d]", (int)back->x, (int)back->y);
	}

	if(back)
		FreeImage(&back);
	if(blckbg)
		FreeImage(&blckbg);
	return;
}

void DrawGrid224()
{
	int 		done = 0, genny = 0, text = 0;
	u32			pressed;		
	ImagePtr	back = NULL;
	char		msg[50];
	
	if(vmode != VIDEO_240P) 
		return;
		
	ChangeVideoEnabled = 0;
	
	while(!done && !EndProgram) 
	{		
		if(!back)
		{
			/*
			if(vmode == VIDEO_288P || vmode == VIDEO_576I_A264)
			{
				back = LoadImage(GRIDPALIMG, 0);
				if(!back)
					return;    				
			}
			*/
			
			if(!back)
			{
				back = LoadImage(GRID224IMG, 0);
				if(!back)
					return;	
				back->y = (240-224)/2 + 1; //SNES
			}
			
			IgnoreOffset(back);
		}
		
		StartScene();
		        
		DrawImage(back);
		
		if(text)
		{			
			DrawStringB(260, 20, 0, 0xff, 0, msg);
			text --;
		}	
		
        EndScene();		
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_B)
			done =	1;								
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;		
			HelpData = GRID224HELP;
		}				
		
		if ( pressed & PAD_BUTTON_A ) 		
		{
			genny = !genny;
			
			if(genny)
			{
				back->y = (240-224)/2 + 2;
				sprintf(msg, "Genesis");
			}
			else
			{
				back->y = (240-224)/2 + 1; // SNES
				sprintf(msg, "SNES");
			}
			
			text = 30;
		}
	}

	ChangeVideoEnabled = 1;
	FreeImage(&back);
	
	return;
}

void DrawMonoscope()
{
	int 		done = 0, irecount = 10, oldvmode = vmode, disabledPG = 0;
	u32			pressed;
	ImagePtr	back = NULL, overlay = NULL;
	int			irevalues[11] = { 0, 26, 51, 77, 102, 128, 153, 179, 204, 230, 255};

	if(Options.Enable720Stretch)
		ShowStretchWarning();

	while(!done && !EndProgram) 
	{   
		if(!back || oldvmode != vmode)
		{
			if(back)
				FreeImage(&back);	
			if(overlay)
				FreeImage(&overlay);

			oldvmode = vmode;
				
			if(IsPAL)
			{
				back = LoadImage(MONOSCOPEPALIMG, 1);
				if(!back)
					return;
				
				overlay = LoadImage(MONOSCOPEPALLINIMG, 1);
				if(!overlay)
				{
					FreeImage(&back);
					return;
				}
			}
			else
			{
				back = LoadImage(MONOSCOPEIMG, 1);
				if(!back)
					return;
					
				overlay = LoadImage(MONOSCOPELINIMG, 1);
				if(!overlay)
				{
					FreeImage(&back);
					return;
				}
			}
			IgnoreOffset(back);
		}
		
		if(Options.EnablePALBG && IsPAL)
		{
			Options.EnablePALBG = 0;
			ShowPALBGWarning();
			disabledPG = 1;
		}

		StartScene();		
		
		DrawImage(back);
		if(overlay)
			DrawImage(overlay);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if (pressed & PAD_BUTTON_B)
			done =	1;	

		if (pressed & PAD_TRIGGER_L)
		{
      		irecount --;
				
			if(irecount < 0)
				irecount = 10;	    						
        	
			back->alpha = irevalues[irecount];
		}
		
		if (pressed & PAD_TRIGGER_R)
		{    
			irecount ++;
			
			if(irecount > 10)
				irecount = 0;	
				
			back->alpha = irevalues[irecount];				
		}
			
		if (pressed & PAD_BUTTON_START) 		
		{
			DrawMenu = 1;					
			HelpData = MONOSCOPEHELP;
			oldvmode = vmode;		
		}	
	}

	FreeImage(&back);
	FreeImage(&overlay);
	
	if(disabledPG)
		Options.EnablePALBG = 1;
	
	return;
}

void Draw100IRE()
{
	int 			done = 0;
	int				irecount = 10, iremax = 10;  
	u32				pressed, text = 0, invert = 0;	
	ImagePtr		back, white;	
	char			msg[50];
	int				*irevalues = NULL;
	int				irevalues100[11] = { 0, 26, 51, 77, 102, 128, 153, 179, 204, 230, 255};
	int				irevalues140[5] = { 0, 63, 127, 190, 255 };	
	
	irevalues = irevalues100;
	back = LoadImage(IRE100IMG, 0);
	if(!back)
		return;
  	white = LoadImage(WHITEIMG, 1);
	if(!white)
  	{
  		FreeImage(&back);
		return;
  	}	
	
	while(!done && !EndProgram) 
	{		
		StartScene();
		if(invert)
			DrawImage(white);	
		DrawImage(back);		

		if(text)
		{
    		if(!invert)
      		{
				if(text > 30)
					sprintf(msg, "RANGE 0-100 IRE");
				else
					sprintf(msg, "%0.1f IRE", ((back->alpha*100.0)/255.0));
			  	DrawStringS(225, 225, 0xff, 0xff, 0xff, msg);
			  	text --;
      		}
      		else
      		{
				if(text > 30)
					sprintf(msg, "RANGE 100-140 IRE");
				else
					sprintf(msg, "%0.0f IRE", 100.0f + abs(40.0 - ((back->alpha * 40.0)/255.0)));
			  	DrawStringS(225, 225, 0xff, 0xff, 0xff, msg);
			  	text --;
      		}
		}
		EndScene();
		
		ControllerScan();
				
		pressed = Controller_ButtonsDown(0);
		if (pressed & PAD_TRIGGER_L)
		{
      		if(!invert)
        	{    		
				irecount --;
				
				if(irecount < 0)
					irecount = 0;	    						
        	}
        	else
        	{    
				irecount ++;
				
				if(irecount > iremax)
					irecount = iremax;					
        	}

			text = 30;
			back->alpha = irevalues[irecount];
		}
	
		if (pressed & PAD_TRIGGER_R) 
		{
      		if(invert)
        	{    		
				irecount --;
				
				if(irecount < 0)
					irecount = 0;	    						
        	}
        	else
        	{    
				irecount ++;
				
				if(irecount > iremax)
					irecount = iremax;					
        	}

			text = 30;
			back->alpha = irevalues[irecount];
		}
		
     	if (pressed & PAD_BUTTON_A)
      	{
			invert = !invert;
        	back->alpha = 0xff;
			text = 60;

			if(invert)
			{
				irevalues = irevalues140;
				iremax = 4;
				irecount = 4;
			}
			else
			{
				irevalues = irevalues100;
				iremax = 10;
				irecount = 10;
			}
      	}
	
		if (pressed & PAD_BUTTON_B)
			done =	1;				
							
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = IREHELP;
		}						
	}

	FreeImage(&back);
	FreeImage(&white);
	return;
}

void DrawSharpness()
{
	int 		done = 0;
	u32			pressed;		
	ImagePtr	back;	
	
	back = LoadImage(SHARPNESSIMG, 0);
	if(!back)
		return;
			
	while(!done && !EndProgram) 
	{		
		StartScene();
		        
		DrawImage(back);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = SHARPNESSHELP;
		}
			
		if (pressed & PAD_BUTTON_B)
			done =	1;										

	}
	FreeImage(&back);
	return;
}


void DrawOverscan()
{
	int 		done = 0, oLeft = 0, oTop = 0, 
				oRight = 0, oBottom = 0, 
				sel = 0, oldvmode = vmode, reset = 0;
	u32			pressed, held;		
	ImagePtr	square, border;	
	char		msg[50];
	
	square = LoadImage(WHITEIMG, 1);
	if(!square)
		return;
		
	border = LoadImage(WHITEIMG, 1);
	if(!border)
		return;		
	
	SetTextureColor(border, 0xff, 0xff, 0xff);
	SetTextureColor(square, 0x60, 0x60, 0x60);
			
	while(!done && !EndProgram) 
	{			
		int x = 0, y = 0;
		
		if(oldvmode != vmode || reset)
		{
			oTop = oLeft = oBottom = oRight = 0;
			CalculateUV(0, 0, dW, dH, square);
			CalculateUV(0, 0, dW, dH, border);
			square->x = square->y = 0;			
			oldvmode = vmode;
			reset = 0;
		}		
		
		StartScene();
		        
		DrawImage(border);
		DrawImage(square);	

		x = dW/2;
		y = dH/2-2*fh;
		
		DrawStringS(x-110, y+(fh*sel), 0xff, 0xff, 0xff, ">");
				
		DrawStringS(x-100, y, 0xff, 0xff, 0xff, "Top Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oTop, (oTop*100.0f)/(dH/2));
		DrawStringS(x+20, y, 0xff, 0xff, 0xff, msg);
		
		y+= fh;
		
		DrawStringS(x-100, y, 0xff, 0xff, 0xff, "Bottom Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oBottom, (oBottom*100.0f)/(dH/2));
		DrawStringS(x+20, y, 0xff, 0xff, 0xff, msg);
		
		y+= fh;		
		
		DrawStringS(x-100, y, 0xff, 0xff, 0xff, "Left Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oLeft, (oLeft*100.0f)/(dW/2));
		DrawStringS(x+20, y, 0xff, 0xff, 0xff, msg);
		
		y+= fh;
		
		DrawStringS(x-100, y, 0xff, 0xff, 0xff, "Right Overscan:");
		sprintf(msg, "%d pixels (%g%%)", oRight, (oRight*100.0f)/(dW/2));
		DrawStringS(x+20, y, 0xff, 0xff, 0xff, msg);			
				
		if(IsPAL && Options.PALScale576)
			DrawStringS(50, 50, 0xff, 0xff, 0x00, "When using PAL stretched modes, the pixel\ncount is inaccurate due to vertical scaling.\nRead the #MHELP#M for details");
			
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		held = Controller_ButtonsHeld(0);
				
		if(pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = OVERSCANHELP;
		}
		
		if ( pressed & PAD_BUTTON_UP ) 
			sel--;
		
		if ( pressed & PAD_BUTTON_DOWN ) 		
			sel++;
			
		if(sel < 0)
			sel = 3;
		if(sel > 3)
			sel = 0;
			
		// Top
		if((pressed & PAD_TRIGGER_R && sel == 0) ||
			(held & PAD_BUTTON_X && sel == 0))
		{
			if(square->y + 1 <= dH/2 && oTop + 1 <= dH/2)
			{				
				square->y++;
				square->h--;
				oTop++;
			}
		}
		
		if((pressed & PAD_TRIGGER_L && sel == 0) ||
			(held & PAD_BUTTON_Y && sel == 0))
		{
			if(square->y - 1 >= 0 && oTop - 1 >= 0)
			{				
				square->y--;
				square->h++;	
				oTop--;
			}
		}
		
		// Bottom
		if((pressed & PAD_TRIGGER_R && sel == 1) ||
			(held & PAD_BUTTON_X && sel == 1))
		{
			if(square->h - 1 >= 0 && oBottom + 1 <= dH/2)
			{								
				square->h--;
				oBottom++;
			}
		}
		
		if((pressed & PAD_TRIGGER_L && sel == 1) ||
			(held & PAD_BUTTON_Y && sel == 1))
		{
			if(square->h + 1 <= dW && oBottom - 1 >=0 )
			{								
				square->h++;	
				oBottom--;
			}
		}
		
		// Left
		if((pressed & PAD_TRIGGER_R && sel == 2) ||
			(held & PAD_BUTTON_X && sel == 2))
		{
			if(square->x + 1 <= dW/2 && oLeft + 1 <= dW/2)
			{				
				square->x++;
				square->w--;
				oLeft++;
			}
		}
		
		if((pressed & PAD_TRIGGER_L && sel == 2) ||
			(held & PAD_BUTTON_Y && sel == 2))
		{
			if(square->x - 1 >= 0 && oLeft - 1 >= 0)
			{				
				square->x--;
				square->w++;
				oLeft--;
			}
		}
		
		// Right
		if((pressed & PAD_TRIGGER_R && sel == 3) ||
			(held & PAD_BUTTON_X && sel == 3))
		{
			if(square->w - 1 >= 0 && oRight + 1 <= dW/2)
			{								
				square->w--;
				oRight++;
			}
		}
		
		if((pressed & PAD_TRIGGER_L && sel == 3) ||
			(held & PAD_BUTTON_Y && sel == 3))
		{
			if(square->w + 1 <= dW && oRight - 1 >= 0)
			{								
				square->w++;	
				oRight--;
			}
		}
				
		if (pressed & PAD_BUTTON_A)	
			reset = 1;
		
		if (pressed & PAD_BUTTON_B)
			done =	1;										

	}
	FreeImage(&border);
	FreeImage(&square);
	return;
}

#define	NUM_CONV	5
void DrawConvergence()
{
    int 		done = 0, current = 0;
	int			patterns[NUM_CONV] = {CONVERGE01, CONVERGE02, CONVERGE03, CONVERGE04, CONVERGE05};
	u32			pressed;		
	ImagePtr	back[NUM_CONV];	
	
	for(current = 0; current < NUM_CONV; current++)
	{
		back[current] = LoadImage(patterns[current], 0);
		if(!back[current])
			return;
	}
	
	current = 0;
	while(!done && !EndProgram) 
	{				
		StartScene();
		        
		DrawImage(back[current]);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if (pressed & PAD_BUTTON_B)
			done =	1;								
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = CONVHELP;
		}
		
		if (pressed & PAD_BUTTON_A || pressed & PAD_TRIGGER_R)
		{
			current ++;
			if(current >= NUM_CONV)
				current = 0;
		}
		
		if (pressed & PAD_TRIGGER_L)
		{
			current --;
			if(current < 0)
				current = NUM_CONV - 1;
		}
	}
	for(current = 0; current < NUM_CONV; current++)
		FreeImage(&back[current]);
	return;
}
