# ===== Compiler settings =====
CXX      := g++
STD      := -std=c++11
OPT      := -O3
OMP      := -fopenmp

CXXFLAGS := $(OPT) $(OMP) $(STD)

# ===== Build target =====
APP := correlate

# ===== Source files =====
SRC := app.cpp signal_ops.cpp
OBJ := $(SRC:.cpp=.o)

# ===== Default rule =====
all: $(APP)

# ===== Link =====
$(APP): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# ===== Compile =====
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# ===== Utilities =====
.PHONY: clean run

clean:
	rm -f $(OBJ) $(APP)

run:
	./$(APP) 1000 1000
