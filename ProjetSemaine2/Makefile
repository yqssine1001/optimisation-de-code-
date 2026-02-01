.PHONY: clean

CC = gcc    			#méthode de compilation
CFLAGS = -Wall 		#on compile comment
SRCS = $(wildcard *.c) 	#récupère tous les fichiers .c
OBJS = $(SRCS:.c=.o)	#permet de faire traitement de texte il change pour tous element de SRCS qui possèdent un .c par un .o
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
	@echo "$(EXEC) are ready"

clean:
	-rm *.o $(EXEC)
	@echo "clean effectué"
#le - perme de inioré les erreur