.PHONY: all build run clean

all: build


build:
	sh gen_makefile.sh
	make -C build/libsita-makefile

run: build
	build/libsita-makefile/src/net/examples/sita_net_example_001/sita_net_example_001

clean:
	rm -rf build