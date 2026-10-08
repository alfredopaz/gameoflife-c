CC = gcc
CFLAGS = -std=c2x -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -I.
LDLIBS = -lX11

TARGET = life
OBJECTS = main.o model/board.o model/model.o controller/controller.o view/view.o
TEST_TARGET = test_model
TEST_OBJECTS = tests/test_model.o model/board.o model/model.o

.PHONY: all clean test
all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDLIBS)
$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(CFLAGS) $(TEST_OBJECTS) -o $@
test: $(TEST_TARGET)
	./$(TEST_TARGET)
clean:
	rm -f $(TARGET) $(TEST_TARGET) $(OBJECTS) $(TEST_OBJECTS)
