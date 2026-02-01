.PHONY: clean

CC = gcc    			#méthode de compilation
CFLAGS = -Wall 		#on compile comment
SRCS = $(wildcard *.c) 	#récupère tous les fichiers .c
OBJS = $(SRCS:.c=.o)	#permet de faire traitement de texte il change pour tous element de SRCS qui possèdent un .c par un .o
EXEC = main#			les objectif ALL
LDFLAGS= -lm

#CFLAGS+= -D varableC=$(varableMAKE)
ifdef DEBUG
CPPFLAGS+= -DDEBUG=$(DEBUG)
$(shell touch $(SRCS))
endif

ifdef FICJSON
CPPFLAGS+= -DFICJSON=$(FICJSON) -I/opt/json-c-master/include/json-c
LDFLAGS+= -L/opt/json-c-master/lib -ljson-c
$(shell touch $(SRCS))
endif




ALL: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)
	@echo "$(EXEC) are ready"

clean:
	-rm *.o $(EXEC)
	@echo "clean effectué"
#le - perme de inioré les erreur