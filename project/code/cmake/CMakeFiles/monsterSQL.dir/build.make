# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.11.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.11.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yee/Desktop/monster-sql/MiniSQL/project/code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake

# Include any dependencies generated for this target.
include CMakeFiles/monsterSQL.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/monsterSQL.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/monsterSQL.dir/flags.make

CMakeFiles/monsterSQL.dir/BPTree.cpp.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/BPTree.cpp.o: ../BPTree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/monsterSQL.dir/BPTree.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/BPTree.cpp.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/BPTree.cpp

CMakeFiles/monsterSQL.dir/BPTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/BPTree.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/BPTree.cpp > CMakeFiles/monsterSQL.dir/BPTree.cpp.i

CMakeFiles/monsterSQL.dir/BPTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/BPTree.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/BPTree.cpp -o CMakeFiles/monsterSQL.dir/BPTree.cpp.s

CMakeFiles/monsterSQL.dir/BPTreeKey.cpp.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/BPTreeKey.cpp.o: ../BPTreeKey.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/monsterSQL.dir/BPTreeKey.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/BPTreeKey.cpp.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/BPTreeKey.cpp

CMakeFiles/monsterSQL.dir/BPTreeKey.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/BPTreeKey.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/BPTreeKey.cpp > CMakeFiles/monsterSQL.dir/BPTreeKey.cpp.i

CMakeFiles/monsterSQL.dir/BPTreeKey.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/BPTreeKey.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/BPTreeKey.cpp -o CMakeFiles/monsterSQL.dir/BPTreeKey.cpp.s

CMakeFiles/monsterSQL.dir/BPTreeNode.cpp.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/BPTreeNode.cpp.o: ../BPTreeNode.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/monsterSQL.dir/BPTreeNode.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/BPTreeNode.cpp.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/BPTreeNode.cpp

CMakeFiles/monsterSQL.dir/BPTreeNode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/BPTreeNode.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/BPTreeNode.cpp > CMakeFiles/monsterSQL.dir/BPTreeNode.cpp.i

CMakeFiles/monsterSQL.dir/BPTreeNode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/BPTreeNode.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/BPTreeNode.cpp -o CMakeFiles/monsterSQL.dir/BPTreeNode.cpp.s

CMakeFiles/monsterSQL.dir/api.cc.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/api.cc.o: ../api.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/monsterSQL.dir/api.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/api.cc.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/api.cc

CMakeFiles/monsterSQL.dir/api.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/api.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/api.cc > CMakeFiles/monsterSQL.dir/api.cc.i

CMakeFiles/monsterSQL.dir/api.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/api.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/api.cc -o CMakeFiles/monsterSQL.dir/api.cc.s

CMakeFiles/monsterSQL.dir/buffer.cc.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/buffer.cc.o: ../buffer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/monsterSQL.dir/buffer.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/buffer.cc.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/buffer.cc

CMakeFiles/monsterSQL.dir/buffer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/buffer.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/buffer.cc > CMakeFiles/monsterSQL.dir/buffer.cc.i

CMakeFiles/monsterSQL.dir/buffer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/buffer.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/buffer.cc -o CMakeFiles/monsterSQL.dir/buffer.cc.s

CMakeFiles/monsterSQL.dir/catalog.cc.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/catalog.cc.o: ../catalog.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/monsterSQL.dir/catalog.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/catalog.cc.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/catalog.cc

CMakeFiles/monsterSQL.dir/catalog.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/catalog.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/catalog.cc > CMakeFiles/monsterSQL.dir/catalog.cc.i

CMakeFiles/monsterSQL.dir/catalog.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/catalog.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/catalog.cc -o CMakeFiles/monsterSQL.dir/catalog.cc.s

CMakeFiles/monsterSQL.dir/config.cc.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/config.cc.o: ../config.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/monsterSQL.dir/config.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/config.cc.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/config.cc

CMakeFiles/monsterSQL.dir/config.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/config.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/config.cc > CMakeFiles/monsterSQL.dir/config.cc.i

CMakeFiles/monsterSQL.dir/config.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/config.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/config.cc -o CMakeFiles/monsterSQL.dir/config.cc.s

CMakeFiles/monsterSQL.dir/file.cc.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/file.cc.o: ../file.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/monsterSQL.dir/file.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/file.cc.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/file.cc

CMakeFiles/monsterSQL.dir/file.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/file.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/file.cc > CMakeFiles/monsterSQL.dir/file.cc.i

CMakeFiles/monsterSQL.dir/file.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/file.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/file.cc -o CMakeFiles/monsterSQL.dir/file.cc.s

CMakeFiles/monsterSQL.dir/index.cc.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/index.cc.o: ../index.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/monsterSQL.dir/index.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/index.cc.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/index.cc

CMakeFiles/monsterSQL.dir/index.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/index.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/index.cc > CMakeFiles/monsterSQL.dir/index.cc.i

CMakeFiles/monsterSQL.dir/index.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/index.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/index.cc -o CMakeFiles/monsterSQL.dir/index.cc.s

CMakeFiles/monsterSQL.dir/interpreter.cc.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/interpreter.cc.o: ../interpreter.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/monsterSQL.dir/interpreter.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/interpreter.cc.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/interpreter.cc

CMakeFiles/monsterSQL.dir/interpreter.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/interpreter.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/interpreter.cc > CMakeFiles/monsterSQL.dir/interpreter.cc.i

CMakeFiles/monsterSQL.dir/interpreter.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/interpreter.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/interpreter.cc -o CMakeFiles/monsterSQL.dir/interpreter.cc.s

CMakeFiles/monsterSQL.dir/main.cc.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/main.cc.o: ../main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/monsterSQL.dir/main.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/main.cc.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/main.cc

CMakeFiles/monsterSQL.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/main.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/main.cc > CMakeFiles/monsterSQL.dir/main.cc.i

CMakeFiles/monsterSQL.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/main.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/main.cc -o CMakeFiles/monsterSQL.dir/main.cc.s

CMakeFiles/monsterSQL.dir/minisql.cc.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/minisql.cc.o: ../minisql.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/monsterSQL.dir/minisql.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/minisql.cc.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/minisql.cc

CMakeFiles/monsterSQL.dir/minisql.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/minisql.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/minisql.cc > CMakeFiles/monsterSQL.dir/minisql.cc.i

CMakeFiles/monsterSQL.dir/minisql.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/minisql.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/minisql.cc -o CMakeFiles/monsterSQL.dir/minisql.cc.s

CMakeFiles/monsterSQL.dir/record.cc.o: CMakeFiles/monsterSQL.dir/flags.make
CMakeFiles/monsterSQL.dir/record.cc.o: ../record.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/monsterSQL.dir/record.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/monsterSQL.dir/record.cc.o -c /Users/yee/Desktop/monster-sql/MiniSQL/project/code/record.cc

CMakeFiles/monsterSQL.dir/record.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/monsterSQL.dir/record.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yee/Desktop/monster-sql/MiniSQL/project/code/record.cc > CMakeFiles/monsterSQL.dir/record.cc.i

CMakeFiles/monsterSQL.dir/record.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/monsterSQL.dir/record.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yee/Desktop/monster-sql/MiniSQL/project/code/record.cc -o CMakeFiles/monsterSQL.dir/record.cc.s

# Object files for target monsterSQL
monsterSQL_OBJECTS = \
"CMakeFiles/monsterSQL.dir/BPTree.cpp.o" \
"CMakeFiles/monsterSQL.dir/BPTreeKey.cpp.o" \
"CMakeFiles/monsterSQL.dir/BPTreeNode.cpp.o" \
"CMakeFiles/monsterSQL.dir/api.cc.o" \
"CMakeFiles/monsterSQL.dir/buffer.cc.o" \
"CMakeFiles/monsterSQL.dir/catalog.cc.o" \
"CMakeFiles/monsterSQL.dir/config.cc.o" \
"CMakeFiles/monsterSQL.dir/file.cc.o" \
"CMakeFiles/monsterSQL.dir/index.cc.o" \
"CMakeFiles/monsterSQL.dir/interpreter.cc.o" \
"CMakeFiles/monsterSQL.dir/main.cc.o" \
"CMakeFiles/monsterSQL.dir/minisql.cc.o" \
"CMakeFiles/monsterSQL.dir/record.cc.o"

# External object files for target monsterSQL
monsterSQL_EXTERNAL_OBJECTS =

monsterSQL: CMakeFiles/monsterSQL.dir/BPTree.cpp.o
monsterSQL: CMakeFiles/monsterSQL.dir/BPTreeKey.cpp.o
monsterSQL: CMakeFiles/monsterSQL.dir/BPTreeNode.cpp.o
monsterSQL: CMakeFiles/monsterSQL.dir/api.cc.o
monsterSQL: CMakeFiles/monsterSQL.dir/buffer.cc.o
monsterSQL: CMakeFiles/monsterSQL.dir/catalog.cc.o
monsterSQL: CMakeFiles/monsterSQL.dir/config.cc.o
monsterSQL: CMakeFiles/monsterSQL.dir/file.cc.o
monsterSQL: CMakeFiles/monsterSQL.dir/index.cc.o
monsterSQL: CMakeFiles/monsterSQL.dir/interpreter.cc.o
monsterSQL: CMakeFiles/monsterSQL.dir/main.cc.o
monsterSQL: CMakeFiles/monsterSQL.dir/minisql.cc.o
monsterSQL: CMakeFiles/monsterSQL.dir/record.cc.o
monsterSQL: CMakeFiles/monsterSQL.dir/build.make
monsterSQL: CMakeFiles/monsterSQL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable monsterSQL"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/monsterSQL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/monsterSQL.dir/build: monsterSQL

.PHONY : CMakeFiles/monsterSQL.dir/build

CMakeFiles/monsterSQL.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/monsterSQL.dir/cmake_clean.cmake
.PHONY : CMakeFiles/monsterSQL.dir/clean

CMakeFiles/monsterSQL.dir/depend:
	cd /Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yee/Desktop/monster-sql/MiniSQL/project/code /Users/yee/Desktop/monster-sql/MiniSQL/project/code /Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake /Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake /Users/yee/Desktop/monster-sql/MiniSQL/project/code/cmake/CMakeFiles/monsterSQL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/monsterSQL.dir/depend

