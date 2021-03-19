CC=gcc
CFLAGS=-std=c11 -D_GNU_SOURCE

EXEC=diceware

SDIR=src
ODIR=obj
INS_DIR=/usr/local/bin

WLIST_PATH=lists/diceware_wordlist.txt
WLIST_OBJ=binary.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))
_OBJS=main.o diceware.o $(WLIST_OBJ)

all: init $(EXEC)

init:
	@mkdir -p $(ODIR)/
	ld -r -b binary -o $(ODIR)/$(WLIST_OBJ) $(WLIST_PATH)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

install:
	cp $(EXEC) $(INS_DIR)/

uninstall:
	rm $(INS_DIR)/$(EXEC)

clean:
	rm -rf $(ODIR)/
	rm $(EXEC)
