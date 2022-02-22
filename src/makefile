# Build project
# TODO: check if this works as expected
CC :=gcc
SOURCES :=$(wildcard *.c)
EXECUTABLE :=$(SOURCES:.c=)
 
all:$(EXECUTABLE)

$(EXECUTABLE):$(SOURCES)
		$(CC) $< -o $@

clean:
		rm $(EXECUTABLE)