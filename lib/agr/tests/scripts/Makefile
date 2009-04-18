

ROOT    = /usr/local
SRC_DIR = ../src
INC_DIR = ../include
BIN_DIR = ../bin
OBJ_DIR = ../obj
OBJ_DIR_D = $(OBJ_DIR)/debug
OBJ_DIR_R = $(OBJ_DIR)/release

AGR_DIR = ../..
AGR_INC_DIR = $(AGR_DIR)/include
AGR_LIB_DIR = $(AGR_DIR)/lib

CC = g++
CC_FLAGS_D = -Wall -g -D_DEBUG -ffriend-injection
CC_FLAGS_R = -Wall -O3 -DRELEASE -ffriend-injection
CC_LD_FLAGS_D = -g
CC_LD_FLAGS_R = 
CC_INC = -I$(INC_DIR) -I$(AGR_INC_DIR) -I$(ROOT)/include
CC_LIB = -L$(AGR_LIB_DIR) -L$(ROOT)/lib



#***************************
# Build 

all: debug release

debug: $(OBJ_DIR_D) $(BIN_DIR) $(BIN_DIR)/test_d

release: $(OBJ_DIR_R) $(BIN_DIR) $(BIN_DIR)/test
	


#***************************
# Build rules
LIBS_FILES := -ldl -lcppunit
LIBS_FILES_D := $(LIBS_FILES) -lAGR_d
LIBS_FILES_R := $(LIBS_FILES) -lAGR

OBJS_FILES = main.o SampleTest.o TestVector3int.o TestMovement.o TestClustering.o TestVocabulary.o TestHMM.o TestMovementRecogniser.o TestGestureRecogniser.o


OBJS_D := $(foreach OBJLIB,$(OBJS_FILES),$(addprefix $(OBJ_DIR_D)/,$(OBJLIB)))
OBJS_R := $(foreach OBJLIB,$(OBJS_FILES),$(addprefix $(OBJ_DIR_R)/,$(OBJLIB)))


$(BIN_DIR)/test_d: $(OBJS_D)
	$(CC) $(CC_LIB) $(OBJS_D) -o "$@" $(LIBS_FILES_D) 

$(BIN_DIR)/test: $(OBJS_R)
	$(CC) $(CC_LIB) $(OBJS_R) -o "$@" $(LIBS_FILES_R) 


$(OBJ_DIR_D)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CC_FLAGS_D) $(CC_INC) -o "$@" "$<"

$(OBJ_DIR_R)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CC_FLAGS_R) $(CC_INC) -o "$@" "$<"


$(OBJ_DIR_D):
	mkdir -p "$@"

$(OBJ_DIR_R):
	mkdir -p "$@"

$(BIN_DIR):
	mkdir -p "$@"


#****************************
# Cleaning

clean: 
	rm -R -f $(OBJ_DIR)
	rm -R -f $(BIN_DIR)



