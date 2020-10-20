CC=gcc
WACC=emcc
WACFLAGS=-O2 -s EXPORTED_FUNCTIONS='["_leven"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'

all: leven_test leven.js leven.c.wat

leven_test: src/leven.c
	$(CC) src/leven.c -o $@

leven.js: src/leven.c
	$(WACC) src/leven.c -o $@ $(WACFLAGS)

leven.c.wat: leven.wasm
	wasm2wat leven.wasm -o leven.c.wat

clean:
	rm leven.js
	rm leven.c.wat
	rm leven.wasm
	rm leven_test