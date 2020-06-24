# simple handwritten makefile for practice

# vpath %.h ./include ./include/textbookHeader
# vpath %.c ./src ./src/textbookSrc # ./testBin

CC       = gcc -Wall -O2 # -Werror
SRC      = list.c test1.c
OBJ      = $(patsubst %.c, %.o, $(SRC))
INCS     = -I"./include"
BIN      = test.exe
LDFLAGS  = -static-libgcc -g3
CFLAGS   = $(INCS) -g3

.PHONY: all clean
all: $(BIN) 

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

# (.o: .c)

clean:
	${RM} $(OBJ) $(BIN)