ifeq ($(OS),Windows_NT)
	EXEC = exe
	LFLAGS = -lglfw3 -lopengl32 -lgdi32 -lm
else
	EXEC = bin
	LFLAGS = -lglfw3 -lGL -lm
endif

ifndef GL_VER
	GL_VER = 4.5
endif

CC = gcc
EXT = c
CFLAGS = -g -Wall -DOPENGL_VER=$(GL_VER)
INCLUDE = include
IFLAGS = -I$(INCLUDE)
SRC = src
OBJ = obj
LIB = pkgs
BINDIR = bin
PROJNAME = GLUI
BINNAME = $(PROJNAME).$(EXEC)
LIBS = $(LIB)/glad_gl=$(GL_VER).tar
LIBSRCS = $(INCLUDE)/glad $(INCLUDE)/KHR $(SRC)/glad.c
SRCS = $(wildcard $(SRC)/*.$(EXT))
OBJS = $(patsubst $(SRC)/%.$(EXT), $(OBJ)/%.o, $(SRCS))
BIN = $(BINDIR)/$(BINNAME)

SUBMITNAME = $(PROJECT_NAME).tar

all: $(BINDIR)
all: $(BIN)

release: CFLAGS = -O2 -DNDEBUG
release: all

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJS) $(DEFINES) -o $@ $(LFLAGS)

$(OBJ)/%.o: $(SRC)/%.$(EXT)
	$(CC) $(CFLAGS) $(IFLAGS) $(DEFINES) -c $< -o $@

%.o: $(SRC)/%.$(EXT)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $(OBJ)/$@	

link: $(OBJS)
	$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS) $(OBJS) -o $(BIN)

clean:
	rm -r $(BINDIR) $(OBJ) $(LIBSRCS)

$(BINDIR):
	@mkdir -p $(BINDIR) $(OBJ)
	@tar -xf $(LIBS)
	@make $(BIN)

new: clean
new: all

submit:
	rm -f $(SUBMITNAME)
	tar -cf $(SUBMITNAME) $(BINDIR)