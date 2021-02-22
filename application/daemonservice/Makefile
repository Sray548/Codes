BIN = daemonservice
SRC = DaemonService.cpp
CXX = g++
LIBRARY_DIR = /home/sray/Documents/Codes/lib/WMTUtil
INCLUDES = -I$(LIBRARY_DIR)/inc
WARNING = -Wall
LD_FLAGS = -L$(LIBRARY_DIR) -lwmtutil
C_FLAGS = $(INCLUDES) $(WARNING)
	
OBJ = $(SRC:.cpp=.o)
$(warning  $(OBJ))

.PHONY: all clean install run

all : $(BIN)
	./$(BIN)

$(BIN): $(OBJ)
	$(CXX) $^ -o $@ $(LD_FLAGS)

%.o:%.cpp
	$(CXX) -c $^ $(C_FLAGS) -o $@

clean:
	rm -vf $(BIN) $(OBJ)