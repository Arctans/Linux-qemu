CC=arm-linux-gnueabihf-gcc
# Compiler and flags
CFLAGS = -Wall -g
DRM_PATH=../libdrm/install_arm32
LDFLAGS = -L$(DRM_PATH)/lib -ldrm
# Source and object files
SRC=$(wildcard *.c)
INC=-I$(DRM_PATH)/include/ -I$(DRM_PATH)/include/libdrm/ 
OBJ = $(SRC:.c=.o)
# Executable name
EXEC =  $(SRC:.c=)

# Default target
all: $(EXEC)
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Rule to make the object file
#
%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

install:
	cp $(EXEC) ../../../../kmodules_arm32/
# Clean up
clean:
	rm -f $(OBJ) $(EXEC)


