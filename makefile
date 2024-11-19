# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Source files
SRCS = main.c hop.c reveal.c seek.c iman2.c alias.c activities.c display.c fgbg.c log.c neonate.c proclore.c signal.c spec101112.c testmain.c trimspaces.c

# Output executable
TARGET = my_shell

# Default target
all: $(TARGET)

# Linking the object files to create the executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Clean up the object files and executable
clean:
	rm -f $(TARGET)

# Phony targets to prevent conflict with files of the same name
.PHONY: all clean
