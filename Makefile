CXX := -clang++
CXX_FLAGS := -std=c++17 -Werror -Wall -Wextra -Wconversion -gdwarf-4 -g
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

.PHONY: all
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

.PHONY: build
build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

.PHONY: run
run: all
	./$(APP_DIR)/$(TARGET)

.PHONY: check-leak-run
check-leak-run: all
	valgrind --leak-check=full -v ./$(APP_DIR)/$(TARGET)

.PHONY: debug
debug: all
	lldb ./$(APP_DIR)/$(TARGET)

.PHONY: test
test: build $(APP_TEST_DIR)/$(TARGET)

.PHONY: run-tests
run-tests: test
	./$(APP_TEST_DIR)/$(TARGET)

.PHONY: check-leak-test
check-leak-test: test
	valgrind --leak-check=full -v ./$(APP_TEST_DIR)/$(TARGET)

.PHONY: clean
clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
	-@rm -rvf $(TEST_OBJ_DIR)/*
	-@rm -rvf $(APP_TEST_DIR)/*

.PHONY: info
info:
	@echo "- Application dir: ${APP_DIR}     "
	@echo "- Object dir:      ${OBJ_DIR}     "
	@echo "- Sources:         ${SRC}         "
	@echo "- Objects:         ${OBJECTS}     "
	@echo "- Dependencies:    ${DEPENDENCIES}"
	@echo "- Test dir:    	  ${APP_TEST_DIR}"
	@echo "- Test object dir: ${TEST_OBJ_DIR}"
	@echo "- Test sources:    ${TEST_SRC}    "
	@echo "- Test objects:    ${TEST_OBJECTS}"
