CC = g++
INCLUDE = $(shell pwd)/src
CFLAGS = -I $(INCLUDE) -ggdb -Wall -Wextra -Werror --std=c++11
LIBS = -lpthread -lgtest
export

subdirs := $(shell find * -maxdepth 0 -type d) 
objects := $(wildcard *.o)

subdirs:
	for dir in $(subdirs); do \
	$(MAKE) -C $$dir;	\
	done

all: main

main:$(objects)
	@echo ${objects}
	$(CC) $(CFLAGS) ${objects} $(LIBS) -o $@ 

.PHONY: clean

clean:
	rm  *.o
	rm main