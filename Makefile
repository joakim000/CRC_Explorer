$(info $(SHELL))
# $(info $(PATH))

.DELETE_ON_ERROR :

SRC = src/
BUILD = build/
LIB = lib/
JLIBC = $(LIB)jlibc/


LFLAGS = 
ifdef PROF
CFLAGS = -std=c11 -O3 -pg -g
LFLAGS = -pg
else
ifdef DEBUG
CFLAGS = -std=c11 -O0 -g
else
CFLAGS = -std=c11 -O3 
endif
endif

objects = $(BUILD)main.o $(BUILD)crc.o $(BUILD)engine.o $(BUILD)binutils.o $(BUILD)cmdargs.o $(BUILD)common.o $(BUILD)datagenerator.o $(BUILD)files.o 


extra : $(objects)
	cc $(LFLAGS) -o crc $(objects)

# Program
$(BUILD)main.o : $(SRC)main.c assets/zoo.h 
	cc -c $(CFLAGS) $(SRC)main.c -o $(@D)/main.o -I$(JLIBC) -I$(SRC)	
$(BUILD)crc.o : $(SRC)crc.c
	cc -c $(CFLAGS) $(SRC)crc.c -o $(@D)/crc.o -I$(JLIBC) -I$(SRC)
$(BUILD)engine.o : $(SRC)engine.c   
	cc -c $(CFLAGS) $(SRC)engine.c -o $(@D)/engine.o -I$(JLIBC) -I$(SRC)

# jlibc
$(BUILD)binutils.o : $(JLIBC)/binutils.c 
	cc -c $(CFLAGS) $(JLIBC)/binutils.c -o $(@D)/binutils.o -I$(JLIBC)
$(BUILD)cmdargs.o : $(JLIBC)/cmdargs.c 
	cc -c $(CFLAGS) $(JLIBC)/cmdargs.c -o $(@D)/cmdargs.o -I$(JLIBC)
$(BUILD)common.o : $(JLIBC)/common.c 
	cc -c $(CFLAGS) $(JLIBC)/common.c -o $(@D)/common.o -I$(JLIBC)
#$(BUILD)da.o :  $(JLIBC)/da.c 
#	cc -c $(CFLAGS) $(JLIBC)/da.c -o $(@D)/.o -I$(JLIBC)
$(BUILD)files.o : $(JLIBC)/files.c 
	cc -c $(CFLAGS) $(JLIBC)/files.c -o $(@D)/files.o -I$(JLIBC)
$(BUILD)datagenerator.o : $(JLIBC)/datagenerator.c 
	cc -c $(CFLAGS) $(JLIBC)/datagenerator.c -o $(@D)/datagenerator.o -I$(JLIBC)


.PHONY : debug
debug :
	$(MAKE) DEBUG=1

.PHONY : prof
prof :
	$(MAKE) PROF=1

.PHONY : clean
clean : 
	rm -f $(BUILD)*.o
	
.PHONY : winclean
winclean :
	powershell rm $(BUILD)*.o

.PHONY : superclean
superclean:
	rm -f $(BUILD)*.o
	rm -f crc
	rm -f crc.exe

#.PHONY : cleanportable
#cleanportable : 
#	$(info $(OS))
#ifeq ("$(OS)","Windows_NT")
#cleanportable ::
#	powershell rm $(BUILD)*.o
#else
#cleanportable ::
#	rm -f $(BUILD)*.o
#endif


# Notes
