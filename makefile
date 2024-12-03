CXX = g++
CXXFLAGS = -std=c++17 -Wall

# List of source files
FILE = day_1
ifdef NAME
    FILE = $(NAME)
endif

# Generate corresponding object file names
OBJECTS = utils.o


# Makefile rules
all: execute clean

execute : $(FILE)
	./$(FILE)

$(FILE): $(OBJECTS) $(FILE).cc
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(FILE).cc -o $(FILE)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(FILE)