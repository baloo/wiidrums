

VERSION = 0.9.1

ROOT    = /usr/local
SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
OBJ_DIR = obj
OBJ_DIR_D = $(OBJ_DIR)/debug
OBJ_DIR_R = $(OBJ_DIR)/release

CC = g++
CC_FLAGS_D = -Wall -g -D_DEBUG -ffriend-injection
CC_FLAGS_R = -Wall -O3 -DRELEASE -ffriend-injection
CC_INC = -I$(INC_DIR) -I$(ROOT)/include

AR = ar
RANLIB = ranlib
STRIP = strip


#***************************
# Build 

all: debug release

debug: $(OBJ_DIR_D) $(LIB_DIR) $(LIB_DIR)/libAGR_d.a

release: $(OBJ_DIR_R) $(LIB_DIR) $(LIB_DIR)/libAGR.a
	


#***************************
# Build rules


OBJS_FILES = Movement.o Clustering.o KmeansClustering.o Vocabulary.o HMM.o MovementRecogniser.o GestureRecogniser.o


OBJS_D := $(foreach OBJLIB,$(OBJS_FILES),$(addprefix $(OBJ_DIR_D)/,$(OBJLIB)))
OBJS_R := $(foreach OBJLIB,$(OBJS_FILES),$(addprefix $(OBJ_DIR_R)/,$(OBJLIB)))


$(LIB_DIR)/libAGR_d.a: $(OBJS_D)
	$(AR) crs "$@" $(OBJS_D)
	$(RANLIB) "$@"

$(LIB_DIR)/libAGR.a: $(OBJS_R)
	$(AR) crs "$@" $(OBJS_R)
	$(STRIP) --discard-all "$@"
	$(RANLIB) "$@"

$(OBJ_DIR_D)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CC_FLAGS_D) $(CC_INC) -o "$@" "$<"

$(OBJ_DIR_R)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CC_FLAGS_R) $(CC_INC) -o "$@" "$<"


$(OBJ_DIR_D):
	mkdir -p "$@"

$(OBJ_DIR_R):
	mkdir -p "$@"

$(LIB_DIR):
	mkdir -p "$@"


#****************************
# Cleaning

clean: 
	rm -R -f $(OBJ_DIR)
	rm -R -f $(LIB_DIR)


#****************************
# Packaging


package:
	rm -R -f AGR-$(VERSION)

	mkdir -p AGR-$(VERSION)/src
	cp -r src AGR-$(VERSION)/src

	mkdir -p AGR-$(VERSION)/include
	cp -r src AGR-$(VERSION)/include

	mkdir -p AGR-$(VERSION)/lib
	cp -r src AGR-$(VERSION)/lib

	mkdir -p AGR-$(VERSION)/lib
	cp -r src AGR-$(VERSION)/lib




