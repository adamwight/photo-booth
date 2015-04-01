.PHONY_TARGETS: \
	all \
	clean \
	cmake \
	compile \
	fetch_sounds \
	install

all: cmake compile

# TODO: Move install prefix cruft to debian/rules.
cmake: fetch_sounds
	mkdir -p build
	cd build && cmake -D CMAKE_INSTALL_PREFIX=/usr ..

compile:
	make -C build

clean:
	rm -rf build

deb: clean fetch_sounds
	# TODO: Need to workaround this.
	# pushd ..; tar cjf booths_1.0.0.orig.tar.bz2 booths; popd
	debuild -us -uc

fetch_sounds:
	[ -d sounds ] || ( \
		echo "Fetching some classy sound FX..."; \
		mkdir sounds; \
		cd sounds; \
		wget http://trekcore.com/audio/aliensounds/borg_tractor_beam.mp3; \
		wget http://trekcore.com/audio/aliensounds/romulan_computerbeep1.mp3; \
		wget http://trekcore.com/audio/toscomputer/tos_keypress3.mp3; \
	)

install:
	make -C build install/strip
