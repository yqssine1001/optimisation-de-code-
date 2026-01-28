.PHONY: clean

CC = gcc    			#metode de compilation
CFLAGS = -Wall 		#on compile comment
SRCS = $(wildcard *.c) 	#recupére tous les FIle de .c
OBJS = $(SRCS:.c=.o)	#perme de faire traitment de text il change pour tous element de SRCS qui posser un .c par un .o
EXEC = main#			les objectif ALL


#CFLAGS+= -D varableC=$(varableMAKE)
ifdef DEBUG
CFLAGS+= -DDEBUG=$(DEBUG)
$(shell touch $(SRCS))
endif

ifdef FICJSON
CFLAGS+= -DFICJSON=$(FICJSON)
$(shell touch $(SRCS))
endif




ALL: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) -lm
	@echo "$(EXEC) are redy"

clean:
	-rm *.o $(EXEC)
	@echo "clean effectué"
#le - perme de inioré les erreur