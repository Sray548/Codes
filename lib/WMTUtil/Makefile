CXX = g++
LIB_NAME = libwmtutil
SRC = ./src/DynArray.cpp \
		./src/WLogger.cpp
OBJ = $(SRC:.cpp=.o)
LD_FLAGS = -fPIC -shared
INCLUDES = -I./inc

$(LIB_NAME):$(OBJ)
	$(CXX) $(OBJ) -o $(LIB_NAME).so $(LD_FLAGS)

%.o:%.cpp
	$(CXX) -c $^ -o $@ $(INCLUDES)


.PHONY: clean
clean:
	rm -rf $(OBJ) $(LIB_NAME).so