CC = gcc
CFLAGS = -Wall -Wextra -I./include -g
SRCDIR = src
INCDIR = include
OBJDIR = obj
TARGET = timeshare

# Source files
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/scheduler.c $(SRCDIR)/process.c $(SRCDIR)/timer.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Default target
all: $(TARGET)

# Create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Link object files to create executable
$(TARGET): $(OBJDIR) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "Clean complete"

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
