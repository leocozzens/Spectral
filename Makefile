CC = gcc
EXT = c
CFLAGS = -g -Wall
INCLUDE = include
IFLAGS = -I$(INCLUDE)
LFLAGS = -lglfw3 -lGL -lm
SRC = src
OBJ = obj
BINDIR = bin
PROJNAME = GLUI
BINNAME = $(PROJNAME).bin
SRCS = $(wildcard $(SRC)/*.$(EXT))
OBJS = $(patsubst $(SRC)/%.$(EXT), $(OBJ)/%.o, $(SRCS))
BIN = $(BINDIR)/$(BINNAME)

SUBMITNAME = $(PROJECT_NAME).zip
ZIP = zip

all: create_dirs
all: $(BIN)

release: CFLAGS = -O2 -DNDEBUG
release: all

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJS) -o $@ $(LFLAGS)

$(OBJ)/%.o: $(SRC)/%.$(EXT)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

%.o: $(SRC)/%.$(EXT)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $(OBJ)/$@	

link: $(OBJS)
	$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS) $(OBJS) -o $(BIN)

clean:
	rm -r $(BINDIR) $(OBJ)

create_dirs:
	@mkdir -p $(BINDIR) $(OBJ)

new: clean
new: all

submit:
	rm -f $(SUBMITNAME)
	$(ZIP) $(SUBMITNAME) $(BIN)