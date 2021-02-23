LIBRARY_DIR = /home/sray/code/lib/WMTUtil
BIN = daemonservice
SRC = DaemonService.cpp
CXX = g++
INCLUDES = -I$(LIBRARY_DIR)/inc
OBJ = $(SRC:.cpp=.o)

$(BIN):$(OBJ)
	$(CXX) $(OBJ) -o $(BIN) -L$(LIBRARY_DIR) -lwmtutil
	# ./$(BIN)
%.o:%.cpp
	$(CXX) -c $^ -o $@ $(INCLUDES)

.PHONY:clean
clean:
	rm -rf $(BIN) $(OBJ)