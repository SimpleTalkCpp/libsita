.PHONY: all build run clean

all: build


build:
	sh gen_makefile.sh
	make -C _build/libsita-makefile

run: build
	_build/libsita-makefile/net/examples/sita_net_example_001/sita_net_example_001

clean:
	rm -rf _build