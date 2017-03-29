CC      = gcc
CFLAGS  = -Wall -O2
GSL     = `gsl-config --libs`
LCRYPTO = -lcrypto
LM      = -lm

KEYGPROG = keygen_IP2S
KEYGSRCS = keygen.c
SIGNPROG = sign_IP2S
SIGNSRCS = sign.c
VERFPROG = verify_IP2S
VERFSRCS = verify.c
MSGPROG  = mkmsg_IP2S
MSGSRCS  = mkmsg.c
MKALPROG = mkalg_IP2S
MKALSRCS = mkalg.c

ifeq ($(OS),Windows_NT)
	KEYGPROG := $(addsuffix _win.exe,$(KEYGPROG))
	SIGNPROG := $(addsuffix _win.exe,$(SIGNPROG))
	VERFPROG := $(addsuffix _win.exe,$(VERFPROG))
	MSGPROG := $(addsuffix _win.exe,$(MSGPROG))
	MKALPROG := $(addsuffix _win.exe,$(MKALPROG))
	EXTS := _win
	KEYGOBJS := $(KEYGSRCS:.c=$(EXTS).o)
	SIGNOBJS := $(SIGNSRCS:.c=$(EXTS).o)
	VERFOBJS := $(VERFSRCS:.c=$(EXTS).o)
	MSGOBJS := $(MSGSRCS:.c=$(EXTS).o)
	MKALOBJS := $(MKALSRCS:.c=$(EXTS).o)
else
	ifeq ($(shell uname -s),Linux)
		KEYGPROG := $(addsuffix _linux,$(KEYGPROG))
		SIGNPROG := $(addsuffix _linux,$(SIGNPROG))
		VERFPROG := $(addsuffix _linux,$(VERFPROG))
		MSGPROG := $(addsuffix _linux,$(MSGPROG))
		MKALPROG := $(addsuffix _linux,$(MKALPROG))
		EXTS := _linux
		KEYGOBJS := $(KEYGSRCS:.c=$(EXTS).o)
		SIGNOBJS := $(SIGNSRCS:.c=$(EXTS).o)
		VERFOBJS := $(VERFSRCS:.c=$(EXTS).o)
		MSGOBJS := $(MSGSRCS:.c=$(EXTS).o)
		MKALOBJS := $(MKALSRCS:.c=$(EXTS).o)
	endif
	ifeq ($(shell uname -s),Darwin)
		KEYGPROG := $(addsuffix _osx,$(KEYGPROG))
		SIGNPROG := $(addsuffix _osx,$(SIGNPROG))
		VERFPROG := $(addsuffix _osx,$(VERFPROG))
		MSGPROG := $(addsuffix _osx,$(MSGPROG))
		MKALPROG := $(addsuffix _osx,$(MKALPROG))
		EXTS := _osx
		KEYGOBJS := $(KEYGSRCS:.c=$(EXTS).o)
		SIGNOBJS := $(SIGNSRCS:.c=$(EXTS).o)
		VERFOBJS := $(VERFSRCS:.c=$(EXTS).o)
		MSGOBJS := $(MSGSRCS:.c=$(EXTS).o)
		MKALOBJS := $(MKALSRCS:.c=$(EXTS).o)
	endif
endif

.SUFFIXES: .c $(EXTS).o

.PHONY: all
all: depend $(KEYGPROG) $(KEYGOBJS) $(SIGNPROG) $(SIGNOBJS) $(VERFPROG) $(VERFOBJS) $(MSGPROG) $(MSGOBJS) $(MKALPROG) $(MKALOBJS)

$(KEYGPROG): $(KEYGOBJS)
	$(CC) $(KEYGOBJS) $(GSL) -o $@ $(LM) $(LCRYPTO)
$(SIGNPROG): $(SIGNOBJS)
	$(CC) $(SIGNOBJS) $(GSL) -o $@ $(LM) $(LCRYPTO)
$(VERFPROG): $(VERFOBJS)
	$(CC) $(VERFOBJS) $(GSL) -o $@ $(LM) $(LCRYPTO)
$(MSGPROG): $(MSGOBJS)
	$(CC) $(MSGOBJS) $(GSL) -o $@ $(LM) $(LCRYPTO)
$(MKALPROG): $(MKALOBJS)
	$(CC) $(MKALOBJS) $(GSL) -o $@ $(LM) $(LCRYPTO)

.c$(EXTS).o:
	$(CC) $(CFLAGS) -c $< $(GSL) -o $*$(EXTS).o $(LM) $(LCRYPTO)

.PHONY: clean
clean:
	$(RM) $(KEYGPROG) $(KEYGOBJS) $(SIGNPROG) $(SIGNOBJS) $(VERFPROG) $(VERFOBJS) $(MSGPROG) $(MSGOBJS) $(MKALPROG) $(MKALOBJS) depend$(EXTS).inc

.PHONY: depend
#$(OBJS:.o=.c) 
#	ex.) OBJS = main.oのとき、$(OBJS:.o=.c) = main.c
depend: $(KEYGSRCS) $(SIGNSRCS) $(VERFSRCS) $(MSGSRCS) $(MKALSRCS)#$(OBJS:.o=.c)
	-@ $(RM) depend$(EXTS).inc
#.hファイルだけ抜き出し
	-@ for i in $^; do cpp -MM $$i|sed "s/\.o/$(EXTS).o/g"|sed "s/\ [_a-zA-Z0-9][_a-zA-Z0-9]*\.c//g" >> depend$(EXTS).inc; done

-include depend$(EXTS).inc
