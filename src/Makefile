TARGET = s21_decimal.a
CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11 -pedantic
CFLAGS = -fprofile-arcs -ftest-coverage

OS = $(shell uname)

SRC_DIR = ./sources
HDR_DIR = ./headers
COM_DIR = ../common
TEST_DIR = ./test
BUILD_DIR = ../build

SRC = $(SRC_DIR)/*.c
OBJ = $(SRC: .c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	rm -f $(BUILD_DIR)/$(TARGET)
	ar -src $(TARGET) $(OBJ)
	ranlib $(TARGET)
	mv $(TARGET) $(BUILD_DIR)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: test
test:
#	@checkmk clean_mode=1 test.check > test.c
	@lcov --directory . --zerocounters
	@rm -f $(TEST_DIR)/test
	@gcc $(CFLAGS) -g $(SRC) $(TEST_DIR)/test.c -o $(TEST_DIR)/test `pkg-config --cflags --libs check`
	@$(TEST_DIR)/test

gcov_report: test
	@rm -rf coverage_results/
	@mkdir coverage_results
	@lcov --directory . --capture -o coverage_results/lcov.info
	@genhtml -o coverage_results/ coverage_results/lcov.info --highlight --legend --title "Decimal project coverage"
	@rm -f $(TEST_DIR)/*.gc*
ifeq ($(OS), Darwin)
	open coverage_results/index.html
else
	xdg-open coverage_results/index.html
endif

clean:
	rm -f $(BUILD_DIR)/$(TARGET)
	rm -f $(TEST_DIR)/test
	rm -f $(TEST_DIR)/test.log
	rm -rf coverage_results/
	rm -f $(TEST_DIR)/*.gc*
	lcov --directory . --zerocounters
	rm -f $(TEST_DIR)/leaks.txt

style:
	clang-format -n $(SRC_DIR)/*.c $(HDR_DIR)/*.h

leaks: test
	rm -f $(TEST_DIR)/leaks.txt
	valgrind --leak-check=full --leak-resolution=low --quiet --log-file=$(TEST_DIR)/leaks.txt $(TEST_DIR)/test
	rm -f $(TEST_DIR)/*.gc*

rebuild: clean all
