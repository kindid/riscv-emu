# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.23.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.23.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kindid/github/riscv-emu/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kindid/github/riscv-emu/src/build

# Include any dependencies generated for this target.
include CMakeFiles/rv32tst.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/rv32tst.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/rv32tst.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rv32tst.dir/flags.make

CMakeFiles/rv32tst.dir/bitops.cpp.o: CMakeFiles/rv32tst.dir/flags.make
CMakeFiles/rv32tst.dir/bitops.cpp.o: ../bitops.cpp
CMakeFiles/rv32tst.dir/bitops.cpp.o: CMakeFiles/rv32tst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kindid/github/riscv-emu/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rv32tst.dir/bitops.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rv32tst.dir/bitops.cpp.o -MF CMakeFiles/rv32tst.dir/bitops.cpp.o.d -o CMakeFiles/rv32tst.dir/bitops.cpp.o -c /Users/kindid/github/riscv-emu/src/bitops.cpp

CMakeFiles/rv32tst.dir/bitops.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rv32tst.dir/bitops.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kindid/github/riscv-emu/src/bitops.cpp > CMakeFiles/rv32tst.dir/bitops.cpp.i

CMakeFiles/rv32tst.dir/bitops.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rv32tst.dir/bitops.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kindid/github/riscv-emu/src/bitops.cpp -o CMakeFiles/rv32tst.dir/bitops.cpp.s

CMakeFiles/rv32tst.dir/here.cpp.o: CMakeFiles/rv32tst.dir/flags.make
CMakeFiles/rv32tst.dir/here.cpp.o: ../here.cpp
CMakeFiles/rv32tst.dir/here.cpp.o: CMakeFiles/rv32tst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kindid/github/riscv-emu/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/rv32tst.dir/here.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rv32tst.dir/here.cpp.o -MF CMakeFiles/rv32tst.dir/here.cpp.o.d -o CMakeFiles/rv32tst.dir/here.cpp.o -c /Users/kindid/github/riscv-emu/src/here.cpp

CMakeFiles/rv32tst.dir/here.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rv32tst.dir/here.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kindid/github/riscv-emu/src/here.cpp > CMakeFiles/rv32tst.dir/here.cpp.i

CMakeFiles/rv32tst.dir/here.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rv32tst.dir/here.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kindid/github/riscv-emu/src/here.cpp -o CMakeFiles/rv32tst.dir/here.cpp.s

CMakeFiles/rv32tst.dir/main.cpp.o: CMakeFiles/rv32tst.dir/flags.make
CMakeFiles/rv32tst.dir/main.cpp.o: ../main.cpp
CMakeFiles/rv32tst.dir/main.cpp.o: CMakeFiles/rv32tst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kindid/github/riscv-emu/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/rv32tst.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rv32tst.dir/main.cpp.o -MF CMakeFiles/rv32tst.dir/main.cpp.o.d -o CMakeFiles/rv32tst.dir/main.cpp.o -c /Users/kindid/github/riscv-emu/src/main.cpp

CMakeFiles/rv32tst.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rv32tst.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kindid/github/riscv-emu/src/main.cpp > CMakeFiles/rv32tst.dir/main.cpp.i

CMakeFiles/rv32tst.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rv32tst.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kindid/github/riscv-emu/src/main.cpp -o CMakeFiles/rv32tst.dir/main.cpp.s

CMakeFiles/rv32tst.dir/memory_device.cpp.o: CMakeFiles/rv32tst.dir/flags.make
CMakeFiles/rv32tst.dir/memory_device.cpp.o: ../memory_device.cpp
CMakeFiles/rv32tst.dir/memory_device.cpp.o: CMakeFiles/rv32tst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kindid/github/riscv-emu/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/rv32tst.dir/memory_device.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rv32tst.dir/memory_device.cpp.o -MF CMakeFiles/rv32tst.dir/memory_device.cpp.o.d -o CMakeFiles/rv32tst.dir/memory_device.cpp.o -c /Users/kindid/github/riscv-emu/src/memory_device.cpp

CMakeFiles/rv32tst.dir/memory_device.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rv32tst.dir/memory_device.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kindid/github/riscv-emu/src/memory_device.cpp > CMakeFiles/rv32tst.dir/memory_device.cpp.i

CMakeFiles/rv32tst.dir/memory_device.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rv32tst.dir/memory_device.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kindid/github/riscv-emu/src/memory_device.cpp -o CMakeFiles/rv32tst.dir/memory_device.cpp.s

CMakeFiles/rv32tst.dir/riscv32_reg.cpp.o: CMakeFiles/rv32tst.dir/flags.make
CMakeFiles/rv32tst.dir/riscv32_reg.cpp.o: ../riscv32_reg.cpp
CMakeFiles/rv32tst.dir/riscv32_reg.cpp.o: CMakeFiles/rv32tst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kindid/github/riscv-emu/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/rv32tst.dir/riscv32_reg.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rv32tst.dir/riscv32_reg.cpp.o -MF CMakeFiles/rv32tst.dir/riscv32_reg.cpp.o.d -o CMakeFiles/rv32tst.dir/riscv32_reg.cpp.o -c /Users/kindid/github/riscv-emu/src/riscv32_reg.cpp

CMakeFiles/rv32tst.dir/riscv32_reg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rv32tst.dir/riscv32_reg.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kindid/github/riscv-emu/src/riscv32_reg.cpp > CMakeFiles/rv32tst.dir/riscv32_reg.cpp.i

CMakeFiles/rv32tst.dir/riscv32_reg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rv32tst.dir/riscv32_reg.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kindid/github/riscv-emu/src/riscv32_reg.cpp -o CMakeFiles/rv32tst.dir/riscv32_reg.cpp.s

CMakeFiles/rv32tst.dir/riscv_exec.cpp.o: CMakeFiles/rv32tst.dir/flags.make
CMakeFiles/rv32tst.dir/riscv_exec.cpp.o: ../riscv_exec.cpp
CMakeFiles/rv32tst.dir/riscv_exec.cpp.o: CMakeFiles/rv32tst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kindid/github/riscv-emu/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/rv32tst.dir/riscv_exec.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rv32tst.dir/riscv_exec.cpp.o -MF CMakeFiles/rv32tst.dir/riscv_exec.cpp.o.d -o CMakeFiles/rv32tst.dir/riscv_exec.cpp.o -c /Users/kindid/github/riscv-emu/src/riscv_exec.cpp

CMakeFiles/rv32tst.dir/riscv_exec.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rv32tst.dir/riscv_exec.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kindid/github/riscv-emu/src/riscv_exec.cpp > CMakeFiles/rv32tst.dir/riscv_exec.cpp.i

CMakeFiles/rv32tst.dir/riscv_exec.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rv32tst.dir/riscv_exec.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kindid/github/riscv-emu/src/riscv_exec.cpp -o CMakeFiles/rv32tst.dir/riscv_exec.cpp.s

# Object files for target rv32tst
rv32tst_OBJECTS = \
"CMakeFiles/rv32tst.dir/bitops.cpp.o" \
"CMakeFiles/rv32tst.dir/here.cpp.o" \
"CMakeFiles/rv32tst.dir/main.cpp.o" \
"CMakeFiles/rv32tst.dir/memory_device.cpp.o" \
"CMakeFiles/rv32tst.dir/riscv32_reg.cpp.o" \
"CMakeFiles/rv32tst.dir/riscv_exec.cpp.o"

# External object files for target rv32tst
rv32tst_EXTERNAL_OBJECTS =

rv32tst: CMakeFiles/rv32tst.dir/bitops.cpp.o
rv32tst: CMakeFiles/rv32tst.dir/here.cpp.o
rv32tst: CMakeFiles/rv32tst.dir/main.cpp.o
rv32tst: CMakeFiles/rv32tst.dir/memory_device.cpp.o
rv32tst: CMakeFiles/rv32tst.dir/riscv32_reg.cpp.o
rv32tst: CMakeFiles/rv32tst.dir/riscv_exec.cpp.o
rv32tst: CMakeFiles/rv32tst.dir/build.make
rv32tst: CMakeFiles/rv32tst.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kindid/github/riscv-emu/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable rv32tst"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rv32tst.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rv32tst.dir/build: rv32tst
.PHONY : CMakeFiles/rv32tst.dir/build

CMakeFiles/rv32tst.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rv32tst.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rv32tst.dir/clean

CMakeFiles/rv32tst.dir/depend:
	cd /Users/kindid/github/riscv-emu/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kindid/github/riscv-emu/src /Users/kindid/github/riscv-emu/src /Users/kindid/github/riscv-emu/src/build /Users/kindid/github/riscv-emu/src/build /Users/kindid/github/riscv-emu/src/build/CMakeFiles/rv32tst.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rv32tst.dir/depend

