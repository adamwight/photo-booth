.PHONY_TARGETS: \
	all \
	clean \
	cmake \
	compile \
	fetch_sounds \
	gettext \
	gettext_compile \
	gettext_extract \
	install

all: cmake compile gettext_compile

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

# FIXME: Sources should come from cmake.
SOURCES := $(wildcard *.cc) $(wildcard *.hpp)
gettext_extract: $(SOURCES)
	xgettext -k_ \
		--copyright-holder="Adam Roses Wight <adamw@ludd.net>" \
		--package-name=booths \
		--package-version=1.0.0-rc3 \
		--msgid-bugs-address=adamw@ludd.net \
		-o booths.po \
		$(SOURCES)
	mkdir -p locale
	mv booths.po locale/booths.pot

OBJDIR=build
$(OBJDIR)/%.mo: %.po
	mkdir -p `dirname $@`
	msgfmt $< -o $@

PO_FILES := $(wildcard locale/*/LC_MESSAGES/booths.po)
MO_FILES := $(addprefix $(OBJDIR)/, $(PO_FILES:.po=.mo))

gettext_compile: $(MO_FILES)

# TODO: msgmerge to push new strings into translation files
gettext: gettext_extract gettext_compile

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
