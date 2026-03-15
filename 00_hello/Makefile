# Define the target system for cc65
TARGET = atari

# Source files
SRC_NAME = main.c

# Define the name of the output executable
EXE_NAME = 01_hello.xex

# Define the cc65 tools (assuming they are in your PATH or you specify CC65_HOME)
CC = cl65

# Default rule to build the executable
all: $(EXE_NAME)

$(EXE_NAME): $(SRC_NAME)
	$(CC) -t $(TARGET) -o $@ $<

# Clean up rule
clean:
	$(RM) $(EXE_NAME) $(SRC_NAME).o
