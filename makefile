CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Set file name
DAY ?= 1
FILE = $(DAY)_day

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