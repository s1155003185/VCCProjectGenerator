# command:
# make (same as make debug)
# make debug (build debug version and gtest)
# make release (build release version)
# make clean (remove all .o and executable files)

# Note: make command should start with tab instead of space, else throw exception
# Need to ensure these 2 in settings.json = false
# "editor.insertSpaces": true,
# "editor.detectIndentation": true,
# Note: cannot seperate object files as some sources only have hpp or h files. Object files will be reused by unittest
# Note: main.cpp need to be in root directory to avoid duplicate main function by unit test
# Note: files in .vscode is used for F5 only. if need to build Release vesrion, please use "make release"

# Field to change:
# PROJECT: project name
# DEBUG_FLAGS, RELEASE_FLAGS, CXXFLAGS_DEBUG, CXXFLAGS_RELEASE: flag for debug and release
# LFLAGS: library that not in the project, need to be handled in if case, as different platform has different paths 
# EXCLUDE_FOLDER: igore that folder when compile

# Gtest
# to add new gtest, follow unittest

# Project Info
PROJECT := libVCCCore

# Compiler Info
# must have CXXFLAGS for default compile flags, .cpp.o only use CXXFLAGS
CXX := g++
CXXVERSION := c++20
DEBUG_FLAGS := -D__DEBUG__ -D__DEBUGLOG__
RELEASE_FLAGS :=
#PRECOMPILE_FLAGS := -H
PRECOMPILE_FLAGS :=
CXXFLAGS_DEBUG := -fdiagnostics-color=always -std=$(CXXVERSION) -Wall -Wextra -pthread -fPIC -shared $(PRECOMPILE_FLAGS) $(DEBUG_FLAGS) -g
CXXFLAGS_RELEASE := -fdiagnostics-color=always -std=$(CXXVERSION) -pthread -O3 -fPIC -shared $(RELEASE_FLAGS)
CXXFLAGS_GTEST := -fdiagnostics-color=always -std=$(CXXVERSION) -Wall -Wextra -pthread $(PRECOMPILE_FLAGS) $(DEBUG_FLAGS) -g
CXXFLAGS := $(CXXFLAGS_DEBUG)
GTESTFLAGS := -lgtest -lpthread

# Base Directory
# LFLAGS - keep empty and handle in if case
BASE := .
INC := include
SRC := src
LIB := lib
LFLAGS :=
DEBUG_FOLDER := bin/Debug
RELEASE_FOLDER := bin/Release
# gtest
GTEST_PROJECT := unittest
GTEST_FOLDER := unittest

# exclude folder
ifeq ($(OS),Windows_NT)
EXCLUDE_FOLDER := .git% .svn% .vscode%
else
EXCLUDE_FOLDER := ./.git% ./.svn% ./.vscode%
endif
GTEST_EXCLUDE_FOLDER := $(EXCLUDE_FOLDER)
PROJECT_EXCLUDE_FOLDER := $(EXCLUDE_FOLDER) $(GTEST_FOLDER)%

ifeq ($(OS),Windows_NT)
PROJECT := $(PROJECT).dll
MAIN := $(PROJECT)
GTEST := $(GTEST_PROJECT).exe
GTESTMAIN := $(GTEST)

# All Sub Directory - Source need *.cpp instead of directory
# g++ param
INCDIRS_SUB := $(filter-out $(EXCLUDE_FOLDER), $(patsubst $(shell CHDIR )\\%,-I%,$(shell DIR /A:D /B /S $(INC))))
LIBDIRS_SUB := $(filter-out $(EXCLUDE_FOLDER), $(patsubst $(shell CHDIR )\\%,-L%,$(shell DIR /A:D /B /S $(LIB))))

# All directory to remove *.o
ALL_DIRECTORY := $(filter-out $(EXCLUDE_FOLDER), $(patsubst $(shell CHDIR )\\%,%,$(shell DIR /A:D /B /S $(BASE))))

# cpp
ALL_FILES := $(filter-out $(EXCLUDE_FOLDER), $(patsubst $(shell CHDIR )\\%,%,$(shell DIR /B /S *.h *.c *.hpp *.cpp)))

ALL_PROJECT_FILES :=  $(filter-out $(PROJECT_EXCLUDE_FOLDER), $(ALL_FILES))
ALL_PROJECT_CPP_FILES := $(filter-out %.h %.c %.hpp, $(ALL_PROJECT_FILES))
ALL_PROJECT_CPP_FOLDERS := *.cpp $(patsubst %, %$\\*.cpp, $(filter-out .%, $(sort $(dir $(ALL_PROJECT_CPP_FILES)))))

GTEST_FILES := $(filter-out $(GTEST_EXCLUDE_FOLDER), $(ALL_FILES))
GTEST_CPP_FILES := $(filter-out %.h %.c %.hpp, $(GTEST_FILES))
GTEST_CPP_FOLDERS := $(patsubst %, %\\*.cpp, $(filter-out ./%, $(sort $(dir $(GTEST_CPP_FILES)))))

ALL_O_FILES := *.o $(patsubst %, %\\*.o, $(ALL_DIRECTORY))

# adjust to include base folder
INCDIRS = -I$(INC) $(INCDIRS_SUB)
LIBDIRS = -L$(LIB) $(LIBDIRS_SUB)

# System Lib with -Lpath
#LFLAGS :=

# Command
RM			:= del /q /f
RMDIR		:= rmdir /s /q
MKDIR		:= mkdir
else
PROJECT := $(PROJECT).so
MAIN := $(PROJECT)
GTEST := $(GTEST_PROJECT)
GTESTMAIN := $(GTEST)

# All Sub Directory - Source need *.cpp instead of directory
# g++ param
INCDIRS_SUB := $(filter-out $(EXCLUDE_FOLDER), $(patsubst %,-I%, $(shell find $(INC) -type d)))
LIBDIRS_SUB := $(filter-out $(EXCLUDE_FOLDER), $(patsubst %,-L%, $(shell find $(LIB) -type d)))

# All directory to remove *.o
ALL_DIRECTORY := $(filter-out $(EXCLUDE_FOLDER),$(patsubst %, %, $(shell find $(BASE) -type d)))

# cpp
ALL_PROJECT_CPP_FILES := $(filter-out $(PROJECT_EXCLUDE_FOLDER), $(patsubst %,%, $(shell find $(SRC) -type f -name "*.cpp")))
ALL_PROJECT_FILES :=  $(filter-out $(PROJECT_EXCLUDE_FOLDER), $(patsubst %,%, $(shell find $(INC) -type f -name "*.h")))
ALL_PROJECT_FILES +=  $(filter-out $(PROJECT_EXCLUDE_FOLDER), $(patsubst %,%, $(shell find $(INC) -type f -name "*.hpp")))
ALL_PROJECT_FILES +=  $(filter-out $(PROJECT_EXCLUDE_FOLDER), $(patsubst %,%, $(shell find $(SRC) -type f -name "*.c")))
ALL_PROJECT_FILES +=  $(ALL_PROJECT_CPP_FILES)

ALL_PROJECT_CPP_FOLDERS := *.cpp $(patsubst %, %/*.cpp, $(sort $(dir $(ALL_PROJECT_CPP_FILES))))

GTEST_FILES := $(filter-out $(GTEST_EXCLUDE_FOLDER), $(ALL_PROJECT_FILES)) 
GTEST_CPP_FILES := $(filter-out %.h %.c %.hpp, $(GTEST_FILES)) $(filter-out $(GTEST_EXCLUDE_FOLDER), $(patsubst %,%, $(shell find $(GTEST_FOLDER) -type f -name "*.cpp")))
#GTEST_CPP_FOLDERS := $(patsubst %, %$(CPPEXT), $(filter-out ./%, $(sort $(dir $(GTEST_CPP_FILES)))))
GTEST_CPP_FOLDERS := $(patsubst %, %/*.cpp, $(sort $(dir $(GTEST_CPP_FILES))))

ALL_O_FILES := *.o $(patsubst %, %/*.o, $(ALL_DIRECTORY))

# adjust to include base folder
INCDIRS = $(INCDIRS_SUB)
LIBDIRS = $(LIBDIRS_SUB)

# System Lib with -Lpath
#LFLAGS :=

# Command
RM 			:= rm -f
RMDIR		:= rm -rf
MKDIR			:= mkdir -p
endif

ALL_PROJECT_O_FILES := $(ALL_PROJECT_CPP_FILES:.cpp=.o)
GTEST_O_FILES := $(GTEST_CPP_FILES:.cpp=.o)

all: debug

# need to seperate gtest process to get o file again
debug: $(ALL_PROJECT_FILES) $(MAIN)
	$(MAKE) gtest
	@echo Building complete!

$(MAIN): $(ALL_PROJECT_CPP_FILES) $(ALL_PROJECT_O_FILES)
	@echo Build Debug Start
	$(CXX) $(CXXFLAGS_DEBUG) \
	$(ALL_PROJECT_CPP_FOLDERS) \
	$(INCDIRS) \
	$(LIBDIRS) \
	$(LFLAGS) \
	-o $(DEBUG_FOLDER)/$(PROJECT)
	@echo Build Debug Complete

.cpp.o:
	$(CXX) $(CXXFLAGS_DEBUG) $(INCDIRS) -c $< -o $@

release: clean_release release_no_object

debug_no_object:
	$(CXX) $(CXXFLAGS_DEBUG) \
	$(ALL_PROJECT_CPP_FOLDERS) \
	$(INCDIRS) \
	$(LIBDIRS) \
	$(LFLAGS) \
	-o $(DEBUG_FOLDER)/$(PROJECT)
	@echo Build Debug No Object Complete

release_no_object:
	$(CXX) $(CXXFLAGS_RELEASE) \
	$(ALL_PROJECT_CPP_FOLDERS) \
	$(INCDIRS) \
	$(LIBDIRS) \
	$(LFLAGS) \
	-o $(RELEASE_FOLDER)/$(PROJECT)
	@echo Build Release No Object Complete

gtest: $(GTEST_FILES) $(GTEST)

$(GTEST): $(GTEST_CPP_FILES) $(GTEST_O_FILES)
	@echo Build gtest Start
	$(CXX) $(CXXFLAGS_GTEST) \
	$(GTEST_CPP_FOLDERS) \
	$(INCDIRS) \
	$(LIBDIRS) \
	$(LFLAGS) \
	-o ${DEBUG_FOLDER}/${GTESTMAIN} $(GTESTFLAGS)
	@echo Build gtest Complete
	${DEBUG_FOLDER}/${GTESTMAIN}

#clean
.PHONY: clean clean_object clean_debug clean_release

clean: clean_object clean_debug clean_release
	@echo Clean All Complete

clean_object:
	$(RM) $(ALL_O_FILES)
	@echo Clean Object Complete

clean_debug:
	$(RMDIR) "$(DEBUG_FOLDER)"
	$(MKDIR) "$(DEBUG_FOLDER)"
	@echo Clean Debug Complete

clean_release:
	$(RMDIR) "$(RELEASE_FOLDER)"
	$(MKDIR) "$(RELEASE_FOLDER)"
	@echo Clean Release Complete
