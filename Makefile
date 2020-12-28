CXX := g++
CXXFLAGS := -c
LD := g++
LDFLAGS := -lncurses

SRCDIR := src
OBJDIR := obj

SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
EXE := eddytion

install: all
	cp $(EXE) /usr/local/bin/

all: $(EXE)

clean:
	rm -f $(OBJ)
	rm -f $(EXE)

$(EXE): $(OBJDIR) $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -std=c++17 -o $@

$(OBJDIR):
	mkdir $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -std=c++17 $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.h
	$(CXX) $(CXXFLAGS) -std=c++17 $< -o $@
