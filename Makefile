CC := gcc
CFLAGS := -Wall -Wextra -std=c11
LDFLAGS := -lm

SRCS := main.c generator.c
ANALYZE_SRCS := analyze_wav.c
OBJS := $(SRCS:.c=.o)
ANALYZE_OBJS := $(ANALYZE_SRCS:.c=.o)
TARGET := main
ANALYZE_TARGET := analyze_wav

.PHONY: all clean

all: $(TARGET) $(ANALYZE_TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(ANALYZE_TARGET): $(ANALYZE_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(ANALYZE_OBJS) $(TARGET) $(ANALYZE_TARGET)
