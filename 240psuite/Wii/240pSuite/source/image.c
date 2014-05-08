/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (Wii GX)
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
/*---------------------------------------------------------------------------------

	Image Functions

---------------------------------------------------------------------------------*/

#include <stdio.h>
#include "image.h"

static u8 gp_fifo[GX_FIFO_MINSIZE] ATTRIBUTE_ALIGN(32);

TPLFile backsTPL;	

void InitGX()
{	
    memset(&gp_fifo, 0, sizeof(gp_fifo));    
    GX_Init(&gp_fifo, sizeof(gp_fifo));    
	
    // Load Textures
	TPL_OpenTPLFromMemory(&backsTPL, (void *)textures_tpl,textures_tpl_size);			
}

void SetupGX()
{
	f32 yscale;
	u32 xfbHeight;	
    u16 xfbWidth;
	u32 width = 0;
	u32 height = 0;
	Mtx44 perspective;
	GXColor background = {0, 0, 0, 0xff};	
	
	GX_SetCopyClear(background, 0x00ffffff);
 
    GX_SetViewport(0.0F, 0.0F, rmode->fbWidth, rmode->efbHeight, 0.0F, 1.0F);	
	yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);
    xfbWidth = VIDEO_PadFramebufferWidth(rmode->fbWidth);  
	GX_SetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
		
	GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
		
	GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopyDst(xfbWidth, xfbHeight);
	GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_FALSE, rmode->vfilter);
	GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));	

	GX_SetCullMode(GX_CULL_NONE);
		
	GX_CopyDisp(frameBuffer[vmode],GX_TRUE);
	GX_SetDispCopyGamma(GX_GM_1_0);
		
	GX_SetNumChans(1);
	GX_SetNumTexGens(1);
		
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		
	GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);	
		
	height = rmode->efbHeight;
	width = height == 480 ? xfbWidth : 320;
	
	guOrtho(perspective,0,height,0,width,0,300);    
	GX_LoadProjectionMtx(perspective, GX_ORTHOGRAPHIC);
	GX_SetDither(GX_FALSE);
}

void StartScene()
{
	Mtx GXmodelView2D;		
	
	GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
	GX_InvVtxCache();

	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);	
	GX_SetVtxDesc (GX_VA_CLR0, GX_DIRECT);
	GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	guMtxIdentity(GXmodelView2D);
	guMtxTransApply (GXmodelView2D, GXmodelView2D, 0.0F, 0.0F, -5.0F);
	GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);
}

void EndScene()
{
	GX_DrawDone();
		
	GX_SetZMode(GX_DISABLE, GX_LEQUAL, GX_FALSE);
	GX_SetBlendMode(GX_BM_BLEND,GX_BL_SRCALPHA,GX_BL_INVSRCALPHA,GX_LO_CLEAR);
	GX_SetAlphaUpdate(GX_TRUE);
	GX_SetColorUpdate(GX_TRUE);
	GX_CopyDisp(frameBuffer[vmode],GX_TRUE);
}

ImagePtr LoadImage(int Texture, int maptoscreen)
{		
    u32 fmt = 0;
	u16 t_width = 0, t_height = 0;
	ImagePtr image;
	
	image = (ImagePtr)malloc(sizeof(struct image_st));
	if(!image)
	{
		fprintf(stderr, "Could not malloc image struct %d\n", Texture);
		return(NULL);
	}
		
	TPL_GetTexture(&backsTPL, Texture, &image->tex);	
	GX_InitTexObjLOD(&image->tex, GX_NEAR, GX_NEAR, 0.0, 10.0, 0.0, GX_FALSE, GX_FALSE, GX_ANISO_1);
	TPL_GetTextureInfo(&backsTPL, Texture, &fmt, &t_width, &t_height);	

	image->r = 0xff;
	image->g = 0xff;
	image->b = 0xff;

	image->tw = t_width;
	image->th = t_height;
	image->x = 0;
	image->y = 0;
	image->u1 = 0.0f;
	image->v1 = 0.0f;
	image->u2 = 1.0f;
	image->v2 = 1.0f;
	image->layer = 1.0f;
	image->scale = 1;
	image->alpha = 0xff;
	image->w = image->tw;
	image->h = image->th;
	image->RefCount = 1;
	image->FH = 0;
	image->FV = 0;
	image->copyOf = NULL;
	if(maptoscreen)
	{
		if(image->w < dW && image->w != 8)
			CalculateUV(0, 0, 320, 240, image);
		else
			CalculateUV(0, 0, dW, dH, image);
	}

	return image;
}

ImagePtr CloneImage(ImagePtr source, int maptoscreen)
{	
	ImagePtr image;
	
	if(!source)
		return NULL;
	image = (ImagePtr)malloc(sizeof(struct image_st));	
	if(!image)
	{
		fprintf(stderr, "Could not malloc image struct during copy\n");
		return(NULL);
	}
	image->tex = source->tex;
	image->r = source->r;
	image->g = source->g;
	image->b = source->b;

	image->tw = source->tw;
	image->th = source->th;
	image->x = source->x;
	image->y = source->y;
	image->u1 = source->u1;
	image->v1 = source->v1;
	image->u2 = source->u2;
	image->v2 = source->v2;
	image->layer = source->layer;
	image->scale = source->scale;
	image->alpha = source->alpha;
	image->w = image->tw;
	image->h = image->th;
	image->RefCount = 0;
	image->FH = image->FH;
	image->FV = image->FV;
	image->copyOf = source;
	source->RefCount ++;
	if(maptoscreen)
	{
		if(image->w < dW && image->w != 8)
			CalculateUV(0, 0, 320, 240, image);
		else
			CalculateUV(0, 0, dW, dH, image);
	}

	return image;
}

void FreeImage(ImagePtr *image)
{	
	if(*image)
	{
		u16 *ref;

		if((*image)->copyOf)		
			ref = &((*image)->copyOf->RefCount);
		else
			ref = &((*image)->RefCount);
			
		(*ref)--;
		if(!(*ref) && !(*image)->copyOf)
		{
			//pvr_mem_free((*image)->tex); We don't delete images here
			free(*image);
			*image = NULL;
		}
	}
}

void CalculateUV(float posx, float posy, float width, float height, ImagePtr image)
{
	if(!image)
		return;
	
	// These just keep the numbers clean
	if(posx > image->tw)
		posx = posx - image->tw;
	if(posx < -1*image->tw)
		posx = posx + image->tw;

	if(posy > image->th)
		posy = posy - image->th;
	if(posy < -1*image->th)
		posy = posy + image->th;

	image->w = width;
	image->h = height;
	image->u1 = posx/image->tw;
	image->v1 = posy/image->th;
	image->u2 = (posx + width)/image->tw;
	image->v2 = (posy + height)/image->th;
}

void FlipH(ImagePtr image, u16 flip)
{
	if(!image)
		return;

	if((!image->FH && flip) || (image->FH && !flip))
	{
		float u1;
	
		u1 = image->u1;
		image->u1 = image->u2;	
		image->u2 = u1;	

		image->FH = !image->FH;
	}
}

void FlipV(ImagePtr image, u16 flip)
{
	if(!image)
		return;

	if((!image->FV && flip) || (image->FV && !flip))
	{
		float v1;
	
		v1 = image->v1;
		image->v1 = image->v2;	
		image->v2 = v1;	
		
		image->FV = !image->FV;
	}
}

void FlipHV(ImagePtr image, u16 flip)
{
	FlipH(image, flip);
	FlipV(image, flip);
}

void DrawImage(ImagePtr image)
{ 	
	float x, y, w, h; 

	
	if(!image)
		return;

	x = image->x;
	y = image->y;
	w = image->w;
	h = image->h;
	
	if(image->scale && (vmode == FAKE_640 || vmode == FAKE_640_SL))
	{
		x *= 2;
		y *= 2;
		w *= 2;
		h *= 2;
	}
		
	GX_LoadTexObj(&image->tex, GX_TEXMAP0);	
	GX_InvalidateTexAll();			
		
	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);			// Draw A Quad
		GX_Position2f32(x, y);					// Top Left
		GX_Color4u8(image->r, image->g, image->b, image->alpha);
		GX_TexCoord2f32(image->u1, image->v1);				
		
		GX_Position2f32(x+w, y);			// Top Right
		GX_Color4u8(image->r, image->g, image->b, image->alpha);
		GX_TexCoord2f32(image->u2, image->v1);		
		
		GX_Position2f32(x+w,y+h);	// Bottom Right
		GX_Color4u8(image->r, image->g, image->b, image->alpha);
		GX_TexCoord2f32(image->u2, image->v2);
		
		GX_Position2f32(x,y+h);			// Bottom Left
		GX_Color4u8(image->r, image->g, image->b, image->alpha);
		GX_TexCoord2f32(image->u1, image->v2);		
	GX_End();		
	GX_DrawDone();							// Done Drawing The Quad 

}

