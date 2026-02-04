.PHONY: clean all

CC = gcc
SRCDIR=src
OBJDIR=obj
EXECDIR=bin
LIB_PATH=lib_word
SRCS = $(wildcard $(SRCDIR)/*.c) 	#récupère tous les fichiers .c
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)	#permet de faire traitement de texte il change pour tous element de SRCS qui possèdent un .c par un .o
EXEC = main#			les objectif ALL
CFLAGS = -Wall -Werror
CPPFLAGS = -I$(LIB_PATH)/include -Iinclude
LDFLAGS = -lm -L$(LIB_PATH)/lib -lword

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




all: $(EXECDIR)/$(EXEC) doxy

$(EXECDIR)/$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	-rm $(OBJDIR)/*.o $(EXECDIR)/* 
	@echo "clean effectué"
#le - permet d'ignorer les erreurs
doxy: $(wildcard include/*.h) $(wildcard $(LIB_PATH)/include/*.h)
	doxygen Doxyfile