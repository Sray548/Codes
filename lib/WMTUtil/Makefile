LIBRARY_DIR = ./src
INCLUDES = -I./inc
LIB_NAME = libwmtutil
SRC = ${LIBRARY_DIR}/DynArray.cpp \
	  ${LIBRARY_DIR}/debug_new.cpp
CXX = g++
LD_FLAGS	=  -fPIC -shared 

HEADER = $(shell find . -name "*.h")
# HEADER = ${LIBRARY_DIR}/hello.h
OBJ = $(SRC:.cpp=.o)

.PHONY: all clean
all : $(OBJ)
	$(CXX)  $(OBJ) -o $(LIB_NAME).so  $(LD_FLAGS)
	# ar rcs $(LIB_NAME).a $(OBJ)

%.o:%.cpp
	$(CXX) -c $^ $(INCLUDES) -o $@    

clean:
	rm -rf $(OBJ) $(LIB_NAME).so