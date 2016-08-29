UNAME := $(shell uname)
DEBUG_FLAGS := -DFOURS_DEBUG -g -O0

ifeq ($(UNAME),Darwin)
	CLANG = ccache clang-3.7
	CLANG_CPP = ccache clang++-3.7

	CPP = $(CLANG_CPP) -g -O0 -std=c++11 -I /usr/include/c++/v1
	CC = $(CLANG)
	LINKER = $(CLANG)
	LINKER_OPTS := \
		$(DEBUG_FLAGS) \
		-stdlib=libc++ \
		-lstdc++

	LINKER_DEBUG_OPTS := $(DEBUG_FLAGS)
else

ifeq ($(UNAME),Linux)
	CLANG := ccache clang-3.6
	CLANG_CPP := ccache clang++-3.6

	CPP = $(CLANG_CPP) -g -O0 -std=c++11 -I/usr/include/x86_64-linux-gnu/c++/4.9 -I/usr/include/c++/4.9
	CC = $(CLANG)
	LINKER = $(CLANG)
	LINKER_OPTS := \
		$(DEBUG_FLAGS) \
		-stdlib=libc++ \
		-lstdc++ \

	LINKER_DEBUG_OPTS := $(DEBUG_FLAGS)
endif

endif

VPATH = .:$(BUILD_DIR)
BUILD_DIR = build

CFLAGS := \
	-c \
	-Wall \
	-Werror \
	-pthread \
	-DFOURS_DEBUG \
	-g \
	-O0 \
	-fms-extensions \

FOURS_SOURCES = fours.cpp

FOURS_OBJECTS = $(addprefix $(BUILD_DIR)/,$(FOURS_SOURCES:.cpp=.o))
FOURS_TARGET = fours

TARGETS = $(FOURS_TARGET)

all: $(TARGETS)

-include $(FOURS_OBJECTS:.o=.d)

$(BUILD_DIR)/.gitignore:
	mkdir -p $(BUILD_DIR)
	echo "*" > $(BUILD_DIR)/.gitignore

$(FOURS_TARGET): $(BUILD_DIR)/.gitignore $(FOURS_OBJECTS)
	@echo $(FOURS_OBJECTS)
	@echo Linking $@
	$(LINKER) $(LINKER_OPTS) $(FOURS_OBJECTS) -o $@
	@echo $@ successfully built
	@ccache -s
	@du -hs $@ | cut -f1 | xargs echo Target \`$@\` is

$(BUILD_DIR)/%.e: %.cpp
	@echo Precompiling $<
	@$(CPP) $(CFLAGS) -E $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@echo Compiling $<
	@$(CPP) $(CFLAGS) $< -E -MMD -MP -MF $(patsubst %.o, %.d, $@) -MT $@ > /dev/null
	@$(CPP) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)/* $(TARGETS)
