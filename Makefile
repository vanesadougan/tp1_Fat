.PHONY: all clean

SRC = read_boot.c read_mbr.c read_root.c recovery.c print_file.c
BIN = $(SRC:.c=)

clean:
	rm -f $(BIN) $(OBJ)

all: 
	gcc read_boot.c -o read_boot
	gcc read_mbr.c -o  read_mbr
	gcc read_root.c -o read_root
	gcc recovery.c -o recovery
	gcc print_file.c -o print_file
