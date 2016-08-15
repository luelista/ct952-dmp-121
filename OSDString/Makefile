ECOS_GLOBAL_CFLAGS_DBG = -msoft-float -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef -Woverloaded-virtual -g -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fvtable-gc -finit-priority
include $(INSTALL_DIR)/include/pkgconf/ecos.mak

XCC             = $(ECOS_COMMAND_PREFIX)gcc
XCXX            = $(XCC)
XLD             = $(XCC)
OBJCOPY         = $(ECOS_COMMAND_PREFIX)objcopy

OBJ_PATH        = ../OBJS
INC_PATH        = ../

CFLAGS          = -I$(INSTALL_DIR)/include -g -I$(INC_PATH)
CFLAGS_PROC2    =
DIVXCFLAGS      = -DROMCODE
SERVOCFLAGS     =
CXXFLAGS        = $(CFLAGS)
LDFLAGS         = -nostartfiles -L$(INSTALL_DIR)/lib -Ttarget.ld
ARFLAGS         = -r

ALL_CFLAGS      = -c $(CFLAGS) $(DIVXCFLAGS) $(SERVOCFLAGS) $(CFLAGS_PROC2) $(ECOS_GLOBAL_CFLAGS)

###################################################################################################
DEF_LANGUAGE    = English

MY_PATH         = OSDString

SRCFILES        = $(sort $(wildcard Str_*.c))
OBJFILES        = $(addsuffix .o, $(basename $(SRCFILES)))

DEF_OBJFILE     = $(filter %$(DEF_LANGUAGE).o, $(OBJFILES))
NONDEF_OBJFILES = $(filter-out %$(DEF_LANGUAGE).o, $(OBJFILES))
NONDEF_BINFILES = $(addsuffix .bin, $(basename $(NONDEF_OBJFILES)))

ROMCFG_FILE     = ostr_romcfg.txt

###################################################################################################
# RULES

.PHONY: all clean

all: $(OBJ_PATH)/stringdec.o $(ROMCFG_FILE)

$(ROMCFG_FILE): $(NONDEF_BINFILES)
	@rm -f $(ROMCFG_FILE)
	@make $(addsuffix .bin2romcfg, $(NONDEF_BINFILES))

$(OBJ_PATH)/stringdec.o: $(DEF_OBJFILE)
	@cp $< $@

%.bin2romcfg:
	@echo \"$(MY_PATH)/$*\", \"$(shell expr substr $* 5 4)\", , , 1, 0, 0\; >> $(ROMCFG_FILE)

%.bin: %.o
	@$(OBJCOPY) -j.data $< -Obinary $@

%.o: %.c
	@echo "     COMPILING    :  $<"
	@$(XCC) $(patsubst -fdata-sections, , $(ALL_CFLAGS)) -o $@ $<

clean:
	@rm -f -v $(ROMCFG_FILE) Str_*.bin Str_*.o $(OBJ_PATH)/stringdec.o
