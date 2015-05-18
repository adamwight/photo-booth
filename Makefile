.PHONY: \
	all \
	clean \
	cmake \
	compile \
	deb \
	gettext \
	gettext_compile \
	gettext_extract \
	install

all: cmake compile gettext_compile

# TODO: Move install prefix cruft to debian/rules.
cmake:
	mkdir -p build
	cd build && cmake \
		-D CMAKE_INSTALL_PREFIX=/usr ..

compile:
	make -C build

clean:
	rm -rf build

deb: clean
	debuild -us -uc

# FIXME: Sources and version number should come from cmake.
SOURCES := $(wildcard *.cc) $(wildcard *.hpp)
gettext_extract: $(SOURCES)
	xgettext -k_ \
		--copyright-holder="Adam Roses Wight <adamw@ludd.net>" \
		--package-name="photo-booth" \
		--package-version="1.0.0-rc4" \
		--msgid-bugs-address=adamw@ludd.net \
		--language=C++ \
		-o photo-booth.po \
		$(SOURCES)
	mkdir -p locale
	mv photo-booth.po locale/photo-booth.pot

OBJDIR=build
$(OBJDIR)/%.mo: %.po
	mkdir -p `dirname $@`
	msgfmt --use-fuzzy $< -o $@

PO_FILES := $(wildcard locale/*/LC_MESSAGES/photo-booth.po)
MO_FILES := $(addprefix $(OBJDIR)/, $(PO_FILES:.po=.mo))

gettext_compile: $(MO_FILES)

# TODO: msgmerge to push new strings into translation files
gettext: gettext_extract gettext_compile

install:
	make -C build install/strip
