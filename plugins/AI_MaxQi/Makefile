####################################################################
# The 'Makefile' of MaxQi AI Engine.
#
####################################################################

# The name of the App.
LIBRARY = AI_MaxQi

# Common flags
CXX         = g++

CXXFLAGS = -fPIC -Wall -I../common
#DEBUGFLAGS  = -g

# The main source
MAIN_SRC := \
	AI_MaxQi.cpp \
	MaxQi.cpp

# Define our sources and object files
SOURCES := \
	$(MAIN_SRC)

OBJECTS := $(SOURCES:.cpp=.o)

.cpp.o :
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -c -o $@ $<

all: $(LIBRARY)
	cp -v libAI_MaxQi.so.1.0 ../AI_MaxQi.so

$(LIBRARY): $(OBJECTS)
	$(CXX) -shared -Wl,-soname,lib$(LIBRARY).so.1 -o lib$(LIBRARY).so.1.0 $(OBJECTS)

clean:
	rm -vrf lib$(LIBRARY).* *.o

############## END OF FILE ###############################################

