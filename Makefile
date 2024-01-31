RELEASE = FALSE

ifeq ($(OS),Windows_NT)
	EXEC = exe
	LFLAGS = -lglfw3 -lopengl32 -lgdi32 -lm
else
	EXEC = bin
	LFLAGS = -lglfw3 -lGL -lm
endif

ifeq ($(RELEASE), TRUE)
	CFLAGS = -O2 -DNDEBUG
else
	CFLAGS = -g -Wall
endif

ifndef GL_VER
	GL_VER = 4.5
endif

ifndef CC
	CC = gcc
endif

EXTRACTOR = tools/exlit.$(EXEC)
EXT = c
DEFINES = -DOPENGL_VER=$(GL_VER)
INCLUDE = include
IFLAGS = -I$(INCLUDE)
SRC = src
OBJ = obj
SHADERS = $(SRC)/shaders
LIB = pkgs
BINDIR = bin
PROJNAME = GLUI
BINNAME = $(PROJNAME).$(EXEC)
LIBS = $(LIB)/glad_gl=$(GL_VER).tar
LIBSRCS = $(INCLUDE)/glad $(INCLUDE)/KHR $(SRC)/glad.c
SRCS = $(wildcard $(SRC)/*.$(EXT))
OBJS = $(patsubst $(SRC)/%.$(EXT), $(OBJ)/%.o, $(SRCS))
SHADER_FILES = $(wildcard $(SHADERS)/*.glsl)
SHADER_RAWS = $(patsubst $(SHADERS)/%.glsl, $(INCLUDE)/%_shader.h, $(SHADER_FILES))
BIN = $(BINDIR)/$(BINNAME)

SUBMITNAME = $(PROJNAME).tar

all: $(BINDIR)

release: 
	make RELEASE=TRUE

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJS) -o $@ $(LFLAGS)

$(OBJ)/%.o: $(SRC)/%.$(EXT)
	$(CC) $(CFLAGS) $(IFLAGS) $(DEFINES) -c $< -o $@

$(INCLUDE)/%_shader.h: $(SHADERS)/%.glsl
	$(EXTRACTOR) $< $@

%.o: $(SRC)/%.$(EXT)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $(OBJ)/$@	

link: $(OBJS)
	$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS) $(OBJS) -o $(BIN)

clean:
	rm -r $(BINDIR) $(OBJ) $(LIBSRCS) $(EXTRACTOR) $(SHADER_RAWS)

$(BINDIR):
	$(CC) $(CFLAGS) tools/extract_literals.c -o $(EXTRACTOR)
	@mkdir -p $(BINDIR) $(OBJ)
	@tar -xf $(LIBS)
	@make $(SHADER_RAWS) RELEASE=$(RELEASE) $(BIN)

new: clean
new: all

submit:
	rm -f $(SUBMITNAME)
	tar -cf $(SUBMITNAME) $(BINDIR)