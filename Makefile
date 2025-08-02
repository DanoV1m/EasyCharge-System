
CC = gcc

CFLAGS = -Wall -g -Iinclude

SRCDIR = src
INCDIR = include
BUILDDIR = build
BINDIR = bin

TARGET = $(BINDIR)/easycharge

SOURCES = $(wildcard $(SRCDIR)/*.c)

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@echo "Linking..."
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) -lm
	@echo "Build finished! Run with: ./$(TARGET)"

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning project..."
	rm -rf $(BUILDDIR) $(BINDIR)
	@echo "Clean complete."

.PHONY: all clean