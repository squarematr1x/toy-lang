CXX := -clang++
CXX_FLAGS := -std=c++17 -Wall -Wextra -g
LD_FLAGS := -L/usr/lib -lstdc++ -lm
LD_TEST_FLAGS := -L /usr/local/lib -lpthread
BUILD := build
OBJ_DIR := $(BUILD)/obj
TEST_OBJ_DIR := $(BUILD)/test_obj
APP_DIR := $(BUILD)/app
APP_TEST_DIR := $(BUILD)/test
TARGET := toylang
INCLUDE := -Iinclude/
GTEST_INCLUDE := -I /usr/include -I /usr/src/gtest
SRC := $(wildcard src/*.cpp)
TEST_SRC := $(wildcard test/*.cpp)
GTEST_SRC := /usr/src/gtest/src/gtest_main.cc /usr/src/gtest/src/gtest-all.cc

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJECTS := $(filter-out $(OBJ_DIR)/src/main.o, $(OBJECTS)) $(TEST_SRC:%.cpp=$(TEST_OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -c $< -MMD -o $@

$(TEST_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LD_FLAGS)

$(APP_TEST_DIR)/$(TARGET): $(TEST_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -o $(APP_TEST_DIR)/$(TARGET) $^ $(GTEST_SRC) $(GTEST_INCLUDE) $(LD_TEST_FLAGS)

-include $(DEPENDENCIES)

.PHONY: all build run clean test run-tests debug release info

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

run: all
	./$(APP_DIR)/$(TARGET)

test: build $(APP_TEST_DIR)/$(TARGET)

run-tests: test
	./$(APP_TEST_DIR)/$(TARGET)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
	-@rm -rvf $(TEST_OBJ_DIR)/*
	-@rm -rvf $(APP_TEST_DIR)/*

info:
	@echo "- Application dir: ${APP_DIR}     "
	@echo "- Object dir:      ${OBJ_DIR}     "
	@echo "- Sources:         ${SRC}         "
	@echo "- Objects:         ${OBJECTS}     "
	@echo "- Dependencies:    ${DEPENDENCIES}"
