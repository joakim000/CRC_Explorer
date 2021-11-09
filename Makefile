$(info $(SHELL))
# $(info $(PATH))

.DELETE_ON_ERROR :

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

objects = main.o crc.o engine.o jlibc/binutils.o jlibc/cmdargs.o jlibc/datagenerator.o jlibc/files.o 


extra : $(objects)
	cc $(LFLAGS) -o crc $(objects)

# Program
main.o : main.c crc.h assets/zoo.h jlibc/common.h jlibc/binutils.h jlibc/cmdargs.h jlibc/datagenerator.h jlibc/files.h
	cc -c $(CFLAGS) main.c
crc.o : crc.c crc.h engine.h jlibc/common.h jlibc/binutils.h 
	cc -c $(CFLAGS) crc.c
engine.o : engine.c engine.h  
	cc -c $(CFLAGS) engine.c

# jlibc
binutils.o : jlibc/binutils.c jlibc/binutils.h jlibc/common.h  
	cc -c $(CFLAGS) jlibc/binutils.c
cmdargs.o : jlibc/cmdargs.c jlibc/cmdargs.h jlibc/common.h
	cc -c $(CFLAGS) jlibc/cmdargs.c
#da.o :  jlibc/da.c jlibc/da.h jlibc/common.h
#	cc -c $(CFLAGS) jlibc/da.c
files.o : jlibc/files.c jlibc/files.h  jlibc/common.h 
	cc -c $(CFLAGS) jlibc/files.c
datagenerator.o : jlibc/datagenerator.c jlibc/datagenerator.h  jlibc/common.h 
	cc -c $(CFLAGS) jlibc/datagenerator.c


.PHONY : debug
debug :
	$(MAKE) DEBUG=1

.PHONY : prof
prof :
	$(MAKE) PROF=1

.PHONY : clean
clean : 
	$(info $(OS))
#	ifeq ($(OS),Windows_NT)
#	clean ::
#		powershell rm *.exe 
#		powershell rm *.o
#		powershell rm jlibc/*.o
#	else
#	clean ::
#		rm -rf crc
		rm -f *.o jlibc/*.o
#	endif
	
.PHONY : winclean
winclean : 
#	powershell rm *.exe 
	powershell rm *.o
	powershell rm jlibc/*.o
