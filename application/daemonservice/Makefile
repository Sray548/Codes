BIN = daemonservice
SRC = DaemonService.cpp
CXX = g++
INCLUDES = -I/home/sray/Documents/Codes/lib/WMTUtil/inc
WARNING = -Wall
C_FLAGS = $(INCLUDES) $(WARNING)
	
OBJ = $(SRC:.cpp=.o)
$(warning  $(OBJ))

.PHONY: all clean install run

all : $(BIN)
	./$(BIN)

$(BIN): $(OBJ)
	$(CXX) $^ -o $@

%.o:%.cpp
	$(CXX) -c $^ $(C_FLAGS) -o $@

clean:
	rm -vf $(BIN) $(OBJ)