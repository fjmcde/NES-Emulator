# Compiler setup
CC = g++
CFLAGS = -Wall -Wextra -O3 -g -std=c++17

# Optional DEBUG compiler flag for compiling with debugging code:
# $ make DEBUG=1
ifeq ($(DEBUG), 1)
    CFLAGS += -DDEBUG
endif

# Source files
SRC_DIR     = src
MAIN_FILE   = main.cpp
SRC_FILES   = $(wildcard $(SRC_DIR)/*.cpp)

# Header files
HEADER_DIR      = inc
HEADER_FILES    = $(wildcard $(HEADER_DIR)/*.h)

# Object files
OBJ_DIR     = obj
OBJ_FILES   = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES)) $(OBJ_DIR)/main.o

# Cross-platform settings
ifeq ($(OS), Windows_NT)
    # Windows configuration
    RM                          = del /Q /F
    RM_OBJ_FILES 				= $(foreach file,$(OBJ_FILES),$(subst /,\,$(file)))
    RMDIR                       = rmdir /S /Q
    MKDIR                       = mkdir
    EXECUTABLE 					= nesEmu.exe
else
    # Unix configuration
    RM                          = rm -f
    RM_OBJ_FILES                = $(OBJ_FILES)
    RMDIR                       = rm -rf
    MKDIR                       = mkdir
    EXECUTABLE 					= nesEmu
endif


#################
#    Targets    #
#################

# Executable target
# Generate .exe
$(EXECUTABLE): $(OBJ_FILES) $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^

# Object directory target
# Having object files everywhere makes me crazy; so
# put them all in the same place
$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)

# Object file(s) target
# Generate object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_FILES) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

# Cleanup object files/directory, and executable
clean:
	$(RM) $(RM_OBJ_FILES)
	$(RM) $(EXECUTABLE)
	$(RMDIR) $(OBJ_DIR)

# Not real build targets
.PHONY: clean