CC      ?= cc
AR      ?= ar
ARFLAGS ?= rcs

CFLAGS  ?= -Wall -Wextra -std=gnu17 -g
INCLUDES = -Iinclude -Ideps -Isrc

SRC_DIR  = src
TEST_DIR = test

LIB_NAME = libclig.a
TEST_BIN = clig_tests

SRCS  = $(SRC_DIR)/clig.c \
		$(SRC_DIR)/clig_xform.c \
		$(SRC_DIR)/clig_repl.c \
		$(SRC_DIR)/sc_map.c
OBJS  = $(SRCS:.c=.o)

TEST_SRCS = $(TEST_DIR)/test_main.c \
			$(TEST_DIR)/xform.c \
			$(TEST_DIR)/clig.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

CUNIT_CFLAGS ?= $(shell pkg-config --cflags cunit)
CUNIT_LIBS   ?= $(shell pkg-config --libs cunit)

demo/demo: demo/demo.c libclig.a
	gcc $^ -o demo/demo $(INCLUDES) 

.PHONY: all clean test

all: $(LIB_NAME) $(TEST_BIN)

$(LIB_NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(CUNIT_CFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_OBJS) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) $(LIB_NAME) $(CUNIT_LIBS)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(LIB_NAME) $(TEST_BIN)
