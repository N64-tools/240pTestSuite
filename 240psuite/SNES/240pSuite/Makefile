# path to snesdev root directory (for emulators, devkitsnes, libsnes)
export DEVKITSNES := /c/snesdev/

# path to devkitsnes root directory for compiler
export DEVKIT65XX := /c/snesdev/devkitsnes

export CHNGREG	:=	$(DEVKIT65XX)/tools/changeregion
export UCON64	:=	$(DEVKIT65XX)/tools/ucon64

#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKIT65XX)),)
$(error "Please set DEVKIT65XX in your environment. export DEVKIT65XX=<path to>devkit65XX")
endif

include $(DEVKIT65XX)/snes_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(shell basename $(CURDIR))
SOURCES		:=	.

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	+=	$(INCLUDE) 

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(PVSNESLIB)
#LIBOBJS +:=	
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.asm)))

export AUDIOFILES :=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.it)))

ifneq ($(AUDIOFILES),)
ifeq ($(wildcard soundbank.asm),)
	SFILES		:=	soundbank.asm $(SFILES)
endif
endif
 
#---------------------------------------------------------------------------------

export OFILES	:=	$(BINFILES:.bin=.obj) $(CFILES:.c=.obj) $(SFILES:.asm=.obj)
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

GTITLE 		:= -ht"240p Test Suite SNES "

SMCONVFLAGS	:= -l -s -o soundbank -v -b 5
 
.PHONY: bitmaps all

#---------------------------------------------------------------------------------
all	:	bitmaps $(OUTPUT).sfc
	$(SNTOOLS) -hi! $(GTITLE) $(TARGET).sfc
	$(CHNGREG) $(TARGET).sfc $(TARGET)-PAL.sfc
	$(UCON64) --chk $(TARGET)-PAL.sfc
	rm $(TARGET)-PAL.bak
	
clean:
	@echo clean ...
	@rm -f $(OFILES) $(TARGET).sfc $(TARGET)-PAL.sfc $(TARGET).sym *.pic *.map *.pal *.brr *.bnk soundbank.asm soundbank.h *pc7 *.mp7

#---------------------------------------------------------------------------------

soundbank.asm : $(AUDIOFILES)
	@echo Compiling Soundbank ... 
	@echo @$(SMCONV) $(SMCONVFLAGS) $^
	@$(SMCONV) $(SMCONVFLAGS) $^
	
#---------------------------------------------------------------------------------
font.pic: font.bmp
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -n -gs8 -pc16 -pe0 -mR! -m! -p! $<

fontback.pic: fontback.bmp
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -n -gs8 -pc16 -pe0 -mR! -m! -p! $<

#----------------------------------SPRITES-----------------------------------------

gillian.pic: gillian.bmp
	@echo convert bitmap to sprites... $(notdir $@)
	$(GFXCONV) -pR! -gs16 -pc16 -po16 -n $<		

sprites.pic: sprites.bmp
	@echo convert bitmap to sprites... $(notdir $@)
	$(GFXCONV) -pR! -gs32 -pc16 -po16 -n $<		

striped.pic: striped.bmp
	@echo convert bitmap to sprites... $(notdir $@)
	$(GFXCONV) -pR! -gs32 -pc16 -po16 -n $<		
	
lag-spr.pic: lag-spr.bmp
	@echo convert bitmap to sprites... $(notdir $@)
	$(GFXCONV) -pR! -gs32 -pc16 -po16 -n $<		
	
numbers.pic: numbers.bmp
	@echo convert bitmap to sprites... $(notdir $@)
	$(GFXCONV) -pR! -gs32 -pc16 -po16 -n $<		

LEDsprites.pic: LEDsprites.bmp
	@echo convert bitmap to sprites... $(notdir $@)
	$(GFXCONV) -pR! -gs8 -pc16 -po16 -n $<		
	
#----------------------------------END SPRITES-------------------------------------

back.pic: back.bmp
	@echo convert bitmap to palette 1... $(notdir $@)
	$(GFXCONV) -pR! -pr -pc16 -po16 -n -gs8 -pe1 -fbmp -m $<
	
donna.pic: donna.bmp
	@echo convert bitmap in 256 colors... $(notdir $@)
	$(GFXCONV) -pR! -pc256 -n -gs8 -pe0 -fbmp -m $<
	
color.pic: color.bmp
	@echo convert bitmap in 256 colors wihout palette rounding... $(notdir $@)
	$(GFXCONV) -pR! -pc128 -po128 -n -gs8 -pe0 -fbmp -m $<
	
color_grid.pic: color_grid.bmp
	@echo convert bitmap in 256 colors wihout palette rounding... $(notdir $@)
	$(GFXCONV) -pR! -pc128 -po128 -n -gs8 -pe0 -fbmp -m $<
	
grayramp.pic: grayramp.bmp
	@echo convert bitmap in 256 colors wihout palette rounding... $(notdir $@)
	$(GFXCONV) -pR! -pc128 -po128 -n -gs8 -pe0 -fbmp -m $<
	
fullscreen256.pic: fullscreen256.bmp
	@echo convert font 256 with no color reduction ... $(notdir $@)
	$(GFXCONV) -pc256 -po16 -n -gs8 -pe0 -fbmp -m $<	

100IRE.pic: 100IRE.bmp
	@echo convert font 256 with no color reduction ... $(notdir $@)
	$(GFXCONV) -pc256 -n -gs8 -pe0 -fbmp -m $<
	
sonicback.pic: sonicback.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc256 -n -gs8 -pe0 -fbmp -m $<

sonicfloor.pic: sonicfloor.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe2 -fbmp -mp -m $<

SMPTECB75.pic: SMPTECB75.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
SMPTECB100.pic: SMPTECB100.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
EBUCB75.pic: EBUCB75.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
EBUCB100.pic: EBUCB100.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<

pluge.pic: pluge.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<

plugePAL.pic: plugePAL.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
601701cb.pic: 601701cb.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
sharpness.pic: sharpness.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
gridm7.pc7	: gridm7.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -n -fbmp -m7 $<
	
kiki.pic: kiki.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<

fireshark.pic: fireshark.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<

bricks.pic: bricks.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<

monoscope.pic: monoscope.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
monoscope_PAL.pic: monoscope_PAL.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
convcolor.pic: convcolor.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	
convcolor_blk.pic: convcolor_blk.bmp
	@echo convert bitmap without rearranging palette... $(notdir $@)
	$(GFXCONV) -pR! -pc16 -po16 -n -gs8 -pe0 -fbmp -m $<
	

bitmaps : grid.pic font.pic back.pic gillian.pic grid240.pic donna.pic SMPTECB75.pic\
	SMPTECB100.pic monoscope.pic color.pic pluge.pic grayramp.pic colorbleed.pic\
	colorbleedchk.pic 601701cb.pic sharpness.pic fullscreen.pic fullscreen256.pic\
	hstripes.pic vstripes.pic fontback.pic check.pic sprites.pic 100IRE.pic\
	numbers.pic lagtest.pic sonicback.pic sonicfloor.pic small_grid.pic\
	LEDsprites.pic striped.pic color_grid.pic lag-spr.pic soundbank.asm gridm7.pc7\
	EBUCB100.pic EBUCB75.pic kiki.pic plugePAL.pic audiosync.pic fireshark.pic\
	bricks.pic convdot.pic convcross.pic convgrid.pic convcolor.pic convcolor_blk.pic\
	float.pic monoscope.pic monoscope_PAL.pic
	
#---------------------------------------------------------------------------------
$(OUTPUT).sfc	: $(OFILES)
