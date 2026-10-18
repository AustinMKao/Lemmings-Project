# SFML-based Lemmings game
# To install SFML on macOS: brew install sfml
# To install SFML on Ubuntu: sudo apt-get install libsfml-dev

ifeq ($(shell arch),arm64)
    HOMEBREW_PREFIX = /opt/homebrew
else
    HOMEBREW_PREFIX = /usr/local
endif

CC = g++
INCLUDES = -I$(HOMEBREW_PREFIX)/include
LIBS = -L$(HOMEBREW_PREFIX)/lib -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
STD = -std=c++20
CCFLAGS = -Wall -Wextra

DEBUG_DIR = .
SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, $(DEBUG_DIR)/%.o, $(SOURCES))
HEADERS = $(wildcard *.h)

.PHONY: default all clean

PRODUCT = lemmings

all: $(DEBUG_DIR) $(DEBUG_DIR)/$(PRODUCT)

$(DEBUG_DIR):
	mkdir -p $(DEBUG_DIR)

$(DEBUG_DIR)/%.o: %.cpp $(HEADERS)
	$(CC) -c $(STD) $(CCFLAGS) $(INCLUDES) $< -o $@

$(DEBUG_DIR)/$(PRODUCT): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LIBS) -o $@
	@echo "Build complete: $(DEBUG_DIR)/$(PRODUCT)"

clean:
	rm -rf $(DEBUG_DIR)/*.o
	rm -f $(DEBUG_DIR)/$(PRODUCT)
