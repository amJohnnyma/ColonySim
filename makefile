# Compiler and flags
CXX = g++ 
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 

# Automatically gather all header paths under src/private/**/(headers|include)
INCLUDE_DIRS = $(shell find src/private -type d \( -name headers -o -name include \))
INCLUDES = $(addprefix -I, $(INCLUDE_DIRS))
CXXFLAGS += $(INCLUDES)

# Source and object files
SRCS = $(shell find src/private -name '*.cpp')
OBJS = $(patsubst src/private/%.cpp, src/private/%.o, $(SRCS))

# Output binary
TARGET = main

# Linker flags
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Default rule
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile each .cpp to .o in corresponding directory
src/private/%.o: src/private/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	find src/private -name '*.o' -delete
	rm -f $(TARGET)



run: $(TARGET)
	./$(TARGET)
