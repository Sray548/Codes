BIN = daemonservice
SRC = DaemonService.cpp
CXX = g++
	
OBJ = $(SRC:.cpp=.o)
$(warning  $(OBJ))

.PHONY: all clean install run

all : $(BIN)
	./$(BIN)

$(BIN): $(OBJ)
	$(CXX) $^ -o $@

%.o:%.cpp
	$(CXX) -c $^ -o $@

clean:
	rm -vf $(BIN) $(OBJ)