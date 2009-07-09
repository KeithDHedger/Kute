

include ../flagsandlibs
include ../sources

kutedebug_SOURCES = $(COMMONSRC)

kutedebug_CFLAGS = -ggdb -O0

bin_PROGRAMS = kutedebug

install-exec-local:
	cd $(DESTDIR)$(bindir) && \
	rm kutedebug

