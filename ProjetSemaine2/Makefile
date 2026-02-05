.PHONY: clean all mk_dir libword static shared

CC = gcc
SRCDIR=src
OBJDIR=obj
EXECDIR=bin
LIB_PATH=lib_word
LIB_SRCDIR=$(LIB_PATH)/src
LIB_OBJDIR=$(LIB_PATH)/obj
LIB_LIBDIR=$(LIB_PATH)/lib
SRCS = $(wildcard $(SRCDIR)/*.c) 	#récupère tous les fichiers .c
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)	#permet de faire traitement de texte il change pour tous element de SRCS qui possèdent un .c par un .o
LIB_SRCS = $(wildcard $(LIB_SRCDIR)/*.c)
LIB_OBJS = $(LIB_SRCS:$(LIB_SRCDIR)/%.c=$(LIB_OBJDIR)/%.o)
LIB_OBJS_PIC = $(LIB_SRCS:$(LIB_SRCDIR)/%.c=$(LIB_OBJDIR)/%.pic.o) # objet pic pour library partagée
EXEC = main#			les objectif ALL
CFLAGS = -Wall -Werror
CPPFLAGS = -I$(LIB_PATH)/include -Iinclude
LDFLAGS = -lm # pour lier la math library
LDFLAGS_SHARED = -L$(LIB_LIBDIR) -lword # pour lier la library partagée

ifdef DEBUG
CPPFLAGS+= -DDEBUG=$(DEBUG)
$(shell touch $(SRCS))
endif

ifdef FICJSON
CPPFLAGS+= -DFICJSON=$(FICJSON) -I/opt/json-c-master/include/json-c
LDFLAGS+= -L/opt/json-c-master/lib -ljson-c
$(shell touch $(SRCS))
endif



all: mk_dir libword $(EXECDIR)/$(EXEC) doxy

#library statique libword.a
$(LIB_LIBDIR)/libword.a: $(LIB_OBJS)
	ar rcs $@ $^
#objet pour library statique
$(LIB_OBJDIR)/%.o: $(LIB_SRCDIR)/%.c
	$(CC) -c $(CFLAGS) -I$(LIB_PATH)/include $< -o $@

#library partagée libword.so
$(LIB_LIBDIR)/libword.so: $(LIB_OBJS_PIC)
	$(CC) -shared -o $@ $^

#objet pic pour library partagée
$(LIB_OBJDIR)/%.pic.o: $(LIB_SRCDIR)/%.c
	$(CC) -c $(CFLAGS) -fPIC -I$(LIB_PATH)/include $< -o $@




libword: $(LIB_LIBDIR)/libword.a $(LIB_LIBDIR)/libword.so

# app avec library statique
static: mk_dir $(LIB_LIBDIR)/libword.a $(OBJS)
	$(CC) $(OBJS) $(LIB_LIBDIR)/libword.a $(LDFLAGS) -o $(EXECDIR)/$(EXEC)
	@echo "=== Application compilée avec library statique ==="

# app avec library partagée
shared: mk_dir $(LIB_LIBDIR)/libword.so $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS_SHARED) $(LDFLAGS) -o $(EXECDIR)/$(EXEC)
	@echo "=== Application compilée avec library partagée ==="
	@echo -e "\033[1;33mIMPORTANT : exécuter 'export LD_LIBRARY_PATH=lib_word/lib:$\{LD_LIBRARY_PATH}' avant d'exécuter l'application\033[0m"

$(EXECDIR)/$(EXEC): $(OBJS) $(LIB_LIBDIR)/libword.a
	$(CC) $(OBJS) $(LIB_LIBDIR)/libword.a $(LDFLAGS) -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	-rm $(OBJDIR)/*.o $(EXECDIR)/* $(LIB_OBJDIR)/*.o $(LIB_LIBDIR)/*.a $(LIB_LIBDIR)/*.so
	@echo "clean effectué"

mk_dir: 
	-mkdir -p obj
	-mkdir -p bin
	-mkdir -p documentation
	-mkdir -p $(LIB_OBJDIR)
	-mkdir -p $(LIB_LIBDIR)
#le - permet d'ignorer les erreurs
doxy: $(wildcard include/*.h) $(wildcard $(LIB_PATH)/include/*.h)
	doxygen Doxyfile
