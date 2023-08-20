all: donut.c Constant.h
	emcc donut.c -o output.js -s WASM=1 -s EXPORTED_FUNCTIONS="['_main','_get_screen_buffer']"

c: donut.c Constant.h
	gcc donut.c -o output

debug: donut.c Constant.h
	gcc donut.c -o output -g

clean:
	rm -f output output.js output.wasm