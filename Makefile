.PHONY_TARGETS: \
	all \
	clean \
    cmake \
    compile \
    fetch_sounds

all: cmake compile fetch_sounds

cmake:
	mkdir -p build
	cd build && cmake ..

compile:
	make -C build

clean:
	rm -rf build

fetch_sounds:
	[ -d sounds ] || ( \
		echo "Fetching some classy sound FX..."; \
		mkdir sounds; \
		cd sounds; \
		wget http://trekcore.com/audio/aliensounds/borg_tractor_beam.mp3; \
		wget http://trekcore.com/audio/aliensounds/romulan_computerbeep1.mp3; \
		wget http://trekcore.com/audio/toscomputer/tos_keypress3.mp3; \
	)
