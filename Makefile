CC=g++
CFLAGS=-Wall -g -std=c++11
LDFLAGS=
EXTENSION= cc
SOURCES=main.cc inputbuf.cc lexer.cc parser.cc preprocessor.cc
SOURCE_FOLDER= src
OBJECTS = $(patsubst %.$(EXTENSION),$(OBJECT_FOLDER)/%.o,$(SOURCES))
OBJECT_FOLDER=bin
TARGET=a.out

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

$(OBJECT_FOLDER)/main.o: $(SOURCE_FOLDER)/main.cc $(OBJECT_FOLDER)/parser.o
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_FOLDER)/inputbuf.o: $(SOURCE_FOLDER)/inputbuf.cc $(SOURCE_FOLDER)/inputbuf.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_FOLDER)/lexer.o: $(SOURCE_FOLDER)/lexer.cc $(SOURCE_FOLDER)/lexer.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_FOLDER)/parser.o: $(SOURCE_FOLDER)/parser.cc $(SOURCE_FOLDER)/parser.h $(SOURCE_FOLDER)/data/expression.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_FOLDER)/preprocessor.o: $(SOURCE_FOLDER)/preprocessor.cc $(SOURCE_FOLDER)/preprocessor.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: mkdirs
mkdirs:
	@if [ ! -d src ] ; then \
		mkdir src; \
	fi
	@if [ ! -d bin ] ; then \
		mkdir bin; \
	fi

.PHONY: clean
clean:
	rm -rf $(OBJECT_FOLDER)/*.o
	rm -f $(TARGET)
