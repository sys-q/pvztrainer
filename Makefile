TARGET=pvz.dll
CFLAGS=-mwindows -static -static-libgcc -static-libstdc++
SRC=dllmain.cpp
CC=i686-w64-mingw32-g++

build:
	$(CC) -shared -o $(TARGET) $(SRC) $(CFLAGS)

clean:
	rm -rf pvz.dll 