CC=g++
CFLAGS=-Wall -g -std=c++11 -Werror -Wextra
LDFLAGS=
EXTENSION= cc
SOURCES=main.cc lexer.cc parser.cc preprocessor.cc symboltable.cc Application.cc Abstraction.cc Term.cc Program.cc
SOURCE_FOLDER= src
OBJECTS = $(patsubst %.$(EXTENSION),$(OBJECT_FOLDER)/%.o,$(SOURCES))
OBJECT_FOLDER=bin
TARGET=a.out

all: mkdirs $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

$(OBJECT_FOLDER)/main.o: $(SOURCE_FOLDER)/main.cc $(SOURCE_FOLDER)/parser.h $(SOURCE_FOLDER)/data/Program.h $(SOURCE_FOLDER)/json_helper.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_FOLDER)/lexer.o: $(SOURCE_FOLDER)/lexer.cc $(SOURCE_FOLDER)/lexer.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_FOLDER)/parser.o: $(SOURCE_FOLDER)/parser.cc $(SOURCE_FOLDER)/parser.h $(SOURCE_FOLDER)/data/Program.h $(SOURCE_FOLDER)/data/symboltable.h $(DATA_HEADERS_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_FOLDER)/preprocessor.o: $(SOURCE_FOLDER)/preprocessor.cc $(SOURCE_FOLDER)/preprocessor.h
	$(CC) $(CFLAGS) -c $< -o $@


DATA_HEADERS=Abstraction.h Application.h Term.h Atom.h Expression.h Name.h Variable.h Define.h
DATA_HEADERS_PATH= $(patsubst %,$(SOURCE_FOLDER)/data/%,$(DATA_HEADERS))

$(OBJECT_FOLDER)/symboltable.o: $(SOURCE_FOLDER)/data/symboltable.cc $(SOURCE_FOLDER)/data/symboltable.h $(DATA_HEADERS_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_FOLDER)/Application.o: $(SOURCE_FOLDER)/data/Application.cc $(DATA_HEADERS_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_FOLDER)/Abstraction.o: $(SOURCE_FOLDER)/data/Abstraction.cc $(DATA_HEADERS_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_FOLDER)/Term.o: $(SOURCE_FOLDER)/data/Term.cc $(DATA_HEADERS_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_FOLDER)/Program.o: $(SOURCE_FOLDER)/data/Program.cc $(DATA_HEADERS_PATH) $(SOURCE_FOLDER)/parser.h
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
