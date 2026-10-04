# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -Iinclude
LDFLAGS = 

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = .

# Target executable
TARGET = ls

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/ls.c \
       $(SRC_DIR)/format.c \
       $(SRC_DIR)/utils.c

# Object files
OBJS = main.o ls.o format.o utils.o

# Header files
HEADERS = $(INC_DIR)/ls.h \
          $(INC_DIR)/format.h \
          $(INC_DIR)/utils.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo "Build successful! Executable: $(TARGET)"

# Compile source files to object files
main.o: $(SRC_DIR)/main.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

ls.o: $(SRC_DIR)/ls.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

format.o: $(SRC_DIR)/format.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

utils.o: $(SRC_DIR)/utils.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Cleaned build artifacts"

# Remove all generated files
distclean: clean
	rm -f *~ $(SRC_DIR)/*~ $(INC_DIR)/*~
	@echo "Deep clean completed"

# Rebuild from scratch
rebuild: clean all

# Run the program with default arguments (for testing)
test: $(TARGET)
	@echo "Testing basic ls:"
	./$(TARGET)
	@echo "\nTesting ls -l:"
	./$(TARGET) -l
	@echo "\nTesting ls -la:"
	./$(TARGET) -la

# Install (optional - copy to /usr/local/bin or similar)
install: $(TARGET)
	@echo "To install, run: sudo cp $(TARGET) /usr/local/bin/myls"

# Phony targets (not actual files)
.PHONY: all clean distclean rebuild test install

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the project (default)"
	@echo "  clean     - Remove object files and executable"
	@echo "  distclean - Remove all generated files including backups"
	@echo "  rebuild   - Clean and build from scratch"
	@echo "  test      - Build and run basic tests"
	@echo "  install   - Show installation instructions"
	@echo "  help      - Show this help message"
