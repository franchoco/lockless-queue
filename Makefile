CC ?= clang
CFLAGS += -std=c11 


ifdef PROFILE
    CFLAGS += -pg
    LDFLAGS += -pg
endif
ifdef DEBUG
    CFLAGS += -Wall -Werror -g -O0
else
    CFLAGS += -O3
endif

OBJ=locklessqueue.o
DEPS=locklessqueue.h

all: $(OBJ) 
 
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 
	
clean:
	rm *.o
