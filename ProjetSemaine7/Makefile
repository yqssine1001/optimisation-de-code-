.PHONY: clean all mk_dir libword static shared

CC=gcc
ifdef FUZZ
  PROF=/home/m/mounlaur
  export AFL_PATH=$(PROF)/AFL/afl-2.52b
  CC=$(AFL_PATH)/afl-gcc
endif
SRCDIR=src
OBJDIR=obj
EXECDIR=bin
TEST_SRCDIR=test_src
TEST_OBJDIR=test_obj
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
CFLAGS = -Wall -Werror -g
ifdef FUZZ
CFLAGS+= -O0 
endif
CFLAGS = -Wall -Werror -g
CPPFLAGS = -I$(LIB_PATH)/include -Iinclude -Itest_include
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

ifdef COV
OPTGCOV= -fprofile-arcs -ftest-coverage
CFLAGS+=$(OPTGCOV)
LDFLAGS+=$(OPTGCOV)
$(shell touch $(SRCS))
endif 

ifdef FINAL # La variable FINAL doit être définie à 1 pour compiler le code final
CPPFLAGS+= -DFINAL=$(FINAL)
$(shell touch $(SRCS))
else
TEST_SRCS = $(wildcard $(TEST_SRCDIR)/*.c)
TEST_OBJS = $(TEST_SRCS:$(TEST_SRCDIR)/%.c=$(TEST_OBJDIR)/%.o)
endif

ifdef ASAN
CFLAGS += -fsanitize=address
LDFLAGS+=-fsanitize=address
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
static: mk_dir $(LIB_LIBDIR)/libword.a $(OBJS) $(TEST_OBJS)
	$(CC) $(OBJS) $(TEST_OBJS) $(LIB_LIBDIR)/libword.a $(LDFLAGS) -o $(EXECDIR)/$(EXEC)
	@echo "=== Application compilée avec library statique ==="

# app avec library partagée
shared: mk_dir $(LIB_LIBDIR)/libword.so $(OBJS) $(TEST_OBJS)
	$(CC) $(OBJS) $(TEST_OBJS) $(LDFLAGS_SHARED) $(LDFLAGS) -o $(EXECDIR)/$(EXEC)
	@echo "=== Application compilée avec library partagée ==="
	@echo -e "\033[1;33mIMPORTANT : exécuter 'export LD_LIBRARY_PATH=lib_word/lib:$\{LD_LIBRARY_PATH}' avant d'exécuter l'application\033[0m"

$(EXECDIR)/$(EXEC): $(OBJS) $(TEST_OBJS) $(LIB_LIBDIR)/libword.a
	$(CC) $(OBJS) $(TEST_OBJS) $(LIB_LIBDIR)/libword.a $(LDFLAGS) -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(TEST_OBJDIR)/%.o: $(TEST_SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@


clean:
	 rm -rf $(OBJDIR) $(TEST_OBJDIR) $(LIB_OBJDIR) $(EXECDIR) afl_out

mk_dir: 
	-mkdir -p obj
	-mkdir -p bin
	-mkdir -p documentation
	-mkdir -p $(LIB_OBJDIR)
	-mkdir -p $(LIB_LIBDIR)
	-mkdir -p $(TEST_OBJDIR)
	-mkdir -p afl_out
	-mkdir -p afl_in
#le - permet d'ignorer les erreurs
doxy: $(wildcard include/*.h) $(wildcard $(LIB_PATH)/include/*.h)
	doxygen Doxyfile
