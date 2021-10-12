LIBRARIES=cairo pango pangocairo freetype2 libinput libudev
INCLUDE=-I./microui/src/ -I./src/include $(shell pkg-config --cflags --libs $(LIBRARIES))
LIBS=-L./build

ARGS=$(INCLUDE) $(LIBS) -lmicroui

FILES=$(wildcard ./src/*.cpp)

.PHONY: build

libmicroui.so:
	mkdir -p ./build
	gcc -fPIC -c ./microui/src/microui.c -o ./build/libmicroui.so

app:
	g++ $(FILES) $(ARGS) --std=gnu++20 -o ./build/app

install: libmicroui.so app
	scp -P 2222 ./build/app jcake@localhost:/home/jcake/microui
	scp -P 2222 ./build/libmicroui.so jcake@localhost:/usr/lib
