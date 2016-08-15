# Mostly written by Jonathan Larmour, Red Hat, Inc.
# Reference to ecos.mak added by John Dallaway, eCosCentric Limited, 2003-01-20
# This file is in the public domain and may be used for any purpose

# Usage:   make INSTALL_DIR=/path/to/ecos/install
###############################################################################
#	                                                                          #
#	                          Target Setting                                  #
#                                                                             #
###############################################################################

# Please select the servo mudule

# =========================== #
#           CT909R                              #
# =========================== #
# VESTEL_DV36_SYO_HD65    1  (for customer)
# HYS_ARIMA_651PH         5     
# DM520_X_HTC_1400        6
# FUS_DV34_FUS_FOP5000    7
# QSI_086_TOPRAY_1100S    8
# SAM_S75_SAM_DL3C        9 
# SONY_313_TONIC          10
# AAI_DV34_SAM_DL6        11 (Autotuning support system)
# skywood_DV34_MIM_502W   12
# WORLDTECH_DV34_SYO_HD65 13
# CMS_S76C_SAM_DL6FS      14

# =========================== #
#           CT909P            #
# =========================== #
# CT909P_DV34_SYO_HD65           21
# CT909P_DM520_HTC_1200W         22   
# CT909P_CMS_S76C_SAM_DL6FS      23   

# =========================== #
#           CT909G            #
# =========================== #
# CT909G_DV34_SYO_HD65           71 
# CT909G_SONY_313_SONY_313       72
# CT909G_AAI_DV34_SAM_DL6        74

SERVO_MODULE = 21

###############################################################################
#	                                                                          #
#	                          Build Setting                                   #
#                                                                             #
###############################################################################

# INSTALL_DIR=$$(INSTALL_DIR) # override on make command line



ifeq ($(INSTALL_DIR), )
INSTALL_DIR=/ecos-c/ct909/ecos_build/ct909_release_install_2.0.1
endif
# INSTALL_DIR=/ecos-c/ct909/ecos_build/ct909_debug_install
ECOS_GLOBAL_CFLAGS_DBG = -msoft-float -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef -Woverloaded-virtual -g -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fvtable-gc -finit-priority
include $(INSTALL_DIR)/include/pkgconf/ecos.mak

XCC             = $(ECOS_COMMAND_PREFIX)gcc
XCXX            = $(XCC)
XLD             = $(XCC)

TARGET_PATH     = ../
OBJ_PATH        = ./
INC_PATH        = ../
MPEG_HEADER_PATH = ../

CFLAGS          = -I$(INSTALL_DIR)/include -g -I$(INC_PATH)
CFLAGS_PROC2    = 
DIVXCFLAGS      = -DROMCODE 
SERVOCFLAGS     = -DPCB_SYSTEM=$(SERVO_MODULE) 
CXXFLAGS        = $(CFLAGS)
LDFLAGS         = -nostartfiles -L$(INSTALL_DIR)/lib -Ttarget.ld
ARFLAGS         = -r

OBJ             =	$(OBJ_PATH)srvude.o \
			$(OBJ_PATH)srvtimer.o \
			$(OBJ_PATH)srvucode.o \
			$(OBJ_PATH)srvservo.o \
			$(OBJ_PATH)srvcal.o \
			$(OBJ_PATH)srvtable.o \
			$(OBJ_PATH)srvstate.o \
			$(OBJ_PATH)sv_unit.o \
			$(OBJ_PATH)srvmain.o \
			$(OBJ_PATH)srvbase.o \
			$(OBJ_PATH)srvbm.o \
			$(OBJ_PATH)srvqainfo.o

OBJ_EXT = $(OBJ_PATH)srvbmknl.o \
			$(OBJ_PATH)wuagc.o \
			$(OBJ_PATH)srvtrack.o \
			$(OBJ_PATH)bx_fun.o \
            $(OBJ_PATH)autotune.o\
			$(OBJ_PATH)qobserver.o
#DBGOBJ          =	$(OBJ_PATH)srvmain.o \
#			$(OBJ_PATH)srvbase.o \
#			$(OBJ_PATH)srvbm.o
#
#DBGFILES        =	$(addsuffix .d,$(DBGOBJ))

OBJ2             =$(OBJ_PATH)srv_dram.o


# =========================== #
#           CT909R                              #
# =========================== #
ifeq ($(SERVO_MODULE), 1)
CHIP = CT909R
SRC_SERVO =  $(OBJ_PATH)sv_hd65.c
MO_XXX = $(OBJ_PATH)Mo_hd65.H
PM_XXX = $(OBJ_PATH)Pm_hd65.h
SV_FILE =  ./$(CHIP)/sv_hd65.c
MO_FILE = ./$(CHIP)/mo_hd65.h
PM_FILE = ./$(CHIP)/pm_hd65.h
endif

ifeq ($(SERVO_MODULE), 5)
CHIP = CT909R
SRC_SERVO =  $(OBJ_PATH)sv_651ph.c
MO_XXX = $(OBJ_PATH)Mo_651ph.H
PM_XXX = $(OBJ_PATH)Pm_651ph.h
SV_FILE =  ./$(CHIP)/sv_651ph.c
MO_FILE = ./$(CHIP)/Mo_651ph.h
PM_FILE = ./$(CHIP)/Pm_651ph.h
endif

ifeq ($(SERVO_MODULE), 6)
CHIP = CT909R
SRC_SERVO =  $(OBJ_PATH)sv_shw14.c
MO_XXX = $(OBJ_PATH)Mo_shw14.H
PM_XXX = $(OBJ_PATH)Pm_shw14.h
SV_FILE =  ./$(CHIP)/sv_shw14.c
MO_FILE = ./$(CHIP)/Mo_shw14.h
PM_FILE = ./$(CHIP)/Pm_shw14.h
endif

ifeq ($(SERVO_MODULE), 7)
CHIP = CT909R
SRC_SERVO =  $(OBJ_PATH)sv_fop5000.c
MO_XXX = $(OBJ_PATH)Mo_fop5000.H
PM_XXX = $(OBJ_PATH)Pm_fop5000.h
SV_FILE =  ./$(CHIP)/sv_fop5000.c
MO_FILE = ./$(CHIP)/Mo_fop5000.h
PM_FILE = ./$(CHIP)/Pm_fop5000.h
endif

ifeq ($(SERVO_MODULE), 8)
CHIP = CT909R
SRC_SERVO =  $(OBJ_PATH)sv_qsitr.c
MO_XXX = $(OBJ_PATH)Mo_qsitr.H
PM_XXX = $(OBJ_PATH)Pm_qsitr.h
SV_FILE =  ./$(CHIP)/sv_qsitr.c
MO_FILE = ./$(CHIP)/Mo_qsitr.h
PM_FILE = ./$(CHIP)/Pm_qsitr.h
endif

ifeq ($(SERVO_MODULE), 9)
CHIP = CT909R
SRC_SERVO =  $(OBJ_PATH)sv_s75.c
MO_XXX = $(OBJ_PATH)Mo_s75.h
PM_XXX = $(OBJ_PATH)Pm_s75.h
SV_FILE =  ./$(CHIP)/sv_s75.c
MO_FILE = ./$(CHIP)/Mo_s75.h
PM_FILE = ./$(CHIP)/Pm_s75.h
endif

ifeq ($(SERVO_MODULE), 10)
CHIP = CT909R
SRC_SERVO =  $(OBJ_PATH)sv_sony313.c
MO_XXX = $(OBJ_PATH)Mo_sony313.H
PM_XXX = $(OBJ_PATH)Pm_sony313.h
SV_FILE =  ./$(CHIP)/sv_sony313.c
MO_FILE = ./$(CHIP)/Mo_sony313.h
PM_FILE = ./$(CHIP)/Pm_sony313.h
endif

ifeq ($(SERVO_MODULE), 11)
CHIP = CT909R
SRC_SERVO =  $(OBJ_PATH)sv_dl634.c
MO_XXX = $(OBJ_PATH)Mo_dl634.H
PM_XXX = $(OBJ_PATH)Pm_dl634.h
SV_FILE =  ./$(CHIP)/sv_dl634.c
MO_FILE = ./$(CHIP)/Mo_dl634.h
PM_FILE = ./$(CHIP)/Pm_dl634.h
endif

ifeq ($(SERVO_MODULE), 12)
CHIP = CT909R
SRC_SERVO =  $(OBJ_PATH)sv_s502w.c
MO_XXX = $(OBJ_PATH)Mo_s502w.H
PM_XXX = $(OBJ_PATH)Pm_s502w.h
SV_FILE =  ./$(CHIP)/sv_s502w.c
MO_FILE = ./$(CHIP)/Mo_s502w.h
PM_FILE = ./$(CHIP)/Pm_s502w.h
endif

ifeq ($(SERVO_MODULE), 13)
CHIP = CT909R
SRC_SERVO =  $(OBJ_PATH)sv_world.c
MO_XXX = $(OBJ_PATH)Mo_world.H
PM_XXX = $(OBJ_PATH)Pm_world.h
SV_FILE =  ./$(CHIP)/sv_world.c
MO_FILE = ./$(CHIP)/Mo_world.h
PM_FILE = ./$(CHIP)/Pm_world.h
endif

ifeq ($(SERVO_MODULE), 14)
CHIP = CT909R
SRC_SERVO =  $(OBJ_PATH)sv_dl6s76c.c
MO_XXX = $(OBJ_PATH)Mo_dl6s76c.H
PM_XXX = $(OBJ_PATH)Pm_dl6s76c.h
SV_FILE =  ./$(CHIP)/sv_dl6s76c.c
MO_FILE = ./$(CHIP)/Mo_dl6s76c.h
PM_FILE = ./$(CHIP)/Pm_dl6s76c.h
endif

# =========================== #
#           CT909P                              #
# =========================== #
ifeq ($(SERVO_MODULE), 21)
CHIP = CT909P
SRC_SERVO =  $(OBJ_PATH)sv_hd65p.c
MO_XXX = $(OBJ_PATH)Mo_hd65p.H
PM_XXX = $(OBJ_PATH)Pm_hd65p.h
SV_FILE =  ./$(CHIP)/sv_hd65p.c
MO_FILE = ./$(CHIP)/mo_hd65p.h
PM_FILE = ./$(CHIP)/pm_hd65p.h
endif

ifeq ($(SERVO_MODULE), 22)
CHIP = CT909P
SRC_SERVO =  $(OBJ_PATH)sv_HTC12Wp.c
MO_XXX = $(OBJ_PATH)Mo_HTC12Wp.H
PM_XXX = $(OBJ_PATH)Pm_HTC12Wp.h
SV_FILE =  ./$(CHIP)/sv_HTC12Wp.c
MO_FILE = ./$(CHIP)/mo_HTC12Wp.h
PM_FILE = ./$(CHIP)/pm_HTC12Wp.h
endif

ifeq ($(SERVO_MODULE), 23)
CHIP = CT909P
SRC_SERVO =  $(OBJ_PATH)sv_dl6s76cp.c
MO_XXX = $(OBJ_PATH)Mo_dl6s76cp.H
PM_XXX = $(OBJ_PATH)Pm_dl6s76cp.h
SV_FILE =  ./$(CHIP)/sv_dl6s76cp.c
MO_FILE = ./$(CHIP)/mo_dl6s76cp.h
PM_FILE = ./$(CHIP)/pm_dl6s76cp.h
endif
# =========================== #
#           CT909G                             #
# =========================== #
ifeq ($(SERVO_MODULE), 74)
CHIP = CT909G
SRC_SERVO =  $(OBJ_PATH)sv_dl6dv34G.c
MO_XXX = $(OBJ_PATH)Mo_dl6dv34G.H
PM_XXX = $(OBJ_PATH)Pm_dl6dv34G.h
SV_FILE =  ./$(CHIP)/sv_dl6dv34G.c
MO_FILE = ./$(CHIP)/Mo_dl6dv34G.h
PM_FILE = ./$(CHIP)/Pm_dl6dv34G.h
endif

ifeq ($(SERVO_MODULE), 71)
CHIP = CT909G
SRC_SERVO =  $(OBJ_PATH)sv_hd65G.c
MO_XXX = $(OBJ_PATH)Mo_hd65G.H
PM_XXX = $(OBJ_PATH)Pm_hd65G.h
SV_FILE =  ./$(CHIP)/sv_hd65G.c
MO_FILE = ./$(CHIP)/Mo_hd65G.h
PM_FILE = ./$(CHIP)/Pm_hd65G.h
endif

ifeq ($(SERVO_MODULE), 72)
CHIP = CT909G
SRC_SERVO =  $(OBJ_PATH)sv_sony313G.c
MO_XXX = $(OBJ_PATH)Mo_sony313G.H
PM_XXX = $(OBJ_PATH)Pm_sony313G.h
SV_FILE =  ./$(CHIP)/sv_sony313G.c
MO_FILE = ./$(CHIP)/Mo_sony313G.h
PM_FILE = ./$(CHIP)/Pm_sony313G.h
endif


# ========================================= #
#  set variable for the files that depend on different chip  #
# ========================================= #
# set BX variable
BX_C = bx_fun.c
BX_H = bx_fun.h

# set ucode variable
UCODE = srvucod.h
UCODE_OLD = srvucod_old.h
UCODE_OS = srvucod_os.H

CHIP_PATH = ./$(CHIP)/

# ========================================= #


ARCHIVE_FILE     = servo.a
ARCHIVE_FILE2	= srv_dram.a

# RULES

.PHONY: all clean ARCHIVE ARCHIVE2

all: $(TARGET_PATH)$(ARCHIVE_FILE) $(TARGET_PATH)$(ARCHIVE_FILE2)

#	@echo $(CFLAGS) $(DIVXCFLAGS) $(CFLAGS_PROC2) $(ECOS_GLOBAL_CFLAGS)
#	-rm -f $(OBJ_PATH)sv_unit.c
#	cp $(SRC_SERVO) $(OBJ_PATH)sv_unit.c
#	cp $(SRC_SERVO) $(OBJ_PATH)sv_unit.c
#	@make ARCHIVE  

COPY:
	@echo "================================"
	@echo "=    SERVO SYSTEM IS $(CHIP)    = "
	@echo "================================"

	cp $(CHIP_PATH)$(BX_C) .
	cp $(CHIP_PATH)$(BX_H) .
ifeq ($(CHIP), CT909G)	
	cp $(CHIP_PATH)$(UCODE) .
	cp $(CHIP_PATH)$(UCODE_OS) .
else
	cp $(CHIP_PATH)$(UCODE) .
	cp $(CHIP_PATH)$(UCODE_OLD) .
endif
	cp $(SV_FILE) . -i
	cp $(MO_FILE) . -i
	cp $(PM_FILE) . -i


clean:
	-rm -f $(OBJ) $(OBJ_EXT) $(OBJ2)
#	-rm -f $(OBJ_PATH)sv_unit.c

autotune.o:autotune.c autotune.h srvgene.h
srvbase.o: srvbase.c srvgene.h qobserver.h
srvmain.o: srvmain.c srvgene.h srvmain.h bx_fun.h
wuagc.o: wuagc.c wuagc.h
wuagc.h: typedef.h srvreg.h srvvar.h srvudeif.h srvmacro.h srvcom.h srvgene.h srvfunc.h
srvbmknl.o: srvbmknl.c srvgene.h 
srvude.o: srvude.c srvgene.h $(MPEG_HEADER_PATH)srcfilter.h
srvtimer.o: srvtimer.c srvgene.h
srvucode.o: srvucode.c srvgene.h srvucod.h srvucod_old.h srvucod_os.h
srvservo.o: srvservo.c srvgene.h srvparam.h bx_fun.h wuagc.h 
srvcal.o: srvcal.c srvgene.h bx_fun.h
srvtable.o: srvtable.c srvgene.h qobserver.h 
srvstate.o: srvstate.c srvgene.h bx_fun.h wuagc.h qobserver.h 
srvqainfo.o: srvqainfo.c srvqainfo.h srvgene.h
sv_unit.o: $(SRC_SERVO) srvgene.h bx_fun.h

	@echo "     COMPILING    :  $(SRC_SERVO) to sv_unit.o"
	@$(XCC) -c -o  $(OBJ_PATH)sv_unit.o $(CFLAGS) $(DIVXCFLAGS) $(SERVOCFLAGS) $(CFLAGS_PROC2) $(ECOS_GLOBAL_CFLAGS) $(CFLAGS_4M) $(SRC_SERVO)
srvgene.h: $(MPEG_HEADER_PATH)ctkav.h\
			$(MPEG_HEADER_PATH)int_gbl.h\
			$(MPEG_HEADER_PATH)customer.h\
			$(MPEG_HEADER_PATH)srcfilter.h\
			srvcom.h\
			srvudeif.h\
			srvfunc.h\
			srvmacro.h\
			srvreg.h\
			srvvar.h\
			srvucode.h\
			srvtest.h\
			srvdebug.h
srvcom.h: $(MO_XXX)
srvudeif.h:
srvfunc.h:
srvmacro.h: $(MPEG_HEADER_PATH)debug.h
srvreg.h:
srvvar.h: 	$(MPEG_HEADER_PATH)colordef.h
srvucode.h:
srvtest.h:
srvdebug.h:
srvparam.h: $(PM_XXX)
bx_fun.o: bx_fun.c bx_fun.h $(MPEG_HEADER_PATH)comdef.h srvgene.h
srv_dram.o: srv_dram.c srvgene.h 
qobserver.o: qobserver.c qobserver.h
qobserver.h: srvgene.h
%.o: %.c
	@echo "     COMPILING    :  $<"
	@$(XCC) -c -o  $(OBJ_PATH)$*.o $(CFLAGS) $(DIVXCFLAGS) $(SERVOCFLAGS) $(CFLAGS_PROC2) $(ECOS_GLOBAL_CFLAGS) $(CFLAGS_4M) $<

%.o: %.cxx
	@echo "     COMPILING    :  $<"
	@$(XCXX) -c -o  $(OBJ_PATH)$*.o $(CXXFLAGS) $(DIVXCFLAGS) $(SERVOCFLAGS) $(CFLAGS_PROC2) $(ECOS_GLOBAL_CFLAGS) $(CFLAGS_4M) $<

%.o: %.C
	@echo "     COMPILING    :  $<"
	@$(XCXX) -c -o $(OBJ_PATH)$*.o $(CXXFLAGS) $(DIVXCFLAGS) $(SERVOCFLAGS) $(CFLAGS_PROC2) $(ECOS_GLOBAL_CFLAGS) $(CFLAGS_4M) $<

%.o: %.cc
	@echo "     COMPILING    :  $<"
	@$(XCXX) -c -o  $(OBJ_PATH)$*.o $(CXXFLAGS) $(DIVXCFLAGS) $(SERVOCFLAGS) $(CFLAGS_PROC2) $(ECOS_GLOBAL_CFLAGS) $(CFLAGS_4M) $<

%.o.d: %.c
	@echo "     COMPILING    :  $<"
	@$(XCC) -c -o  $(OBJ_PATH)$*.o $(CFLAGS) $(DIVXCFLAGS) $(SERVOCFLAGS) $(CFLAGS_PROC2) $(ECOS_GLOBAL_CFLAGS_DBG) $(CFLAGS_4M) $<

%.h:
	@echo "		TOUCHING	:	$@"
	@touch $@
	
ARCHIVE:    $(TARGET_PATH)$(ARCHIVE_FILE)
ARCHIVE2:   $(TARGET_PATH)$(ARCHIVE_FILE2)
	
$(TARGET_PATH)$(ARCHIVE_FILE): $(OBJ) $(OBJ_EXT) $(DBGOBJ)
	@echo "     Archive :  $(TARGET_PATH)$(ARCHIVE_FILE)  by $(OBJ) $(OBJ_EXT) $(DBGOBJ) "
	@sparc-rtems-ar $(ARFLAGS)  $(TARGET_PATH)$(ARCHIVE_FILE) $(OBJ) $(OBJ_EXT) $(DBGOBJ)

$(TARGET_PATH)$(ARCHIVE_FILE2): $(OBJ2)
	@echo "     Archive :  $(TARGET_PATH)$(ARCHIVE_FILE2)  by $(OBJ2)  "
	@sparc-rtems-ar $(ARFLAGS)  $(TARGET_PATH)$(ARCHIVE_FILE2) $(OBJ2) 
