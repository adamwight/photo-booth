.PHONY_TARGETS: \
	all
	clean

all:
	mkdir -p build
	cd build && cmake ..
	make -C build

clean:
	rm -rf build
