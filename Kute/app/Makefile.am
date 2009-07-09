
include ../flagsandlibs
include ../sources

kute_SOURCES = $(COMMONSRC)

kute_CFLAGS = -O3

bin_PROGRAMS = kute

man1_MANS = ../resources/man/kute.1

BUILT_SOURCES = kute
CLEANFILES = ../resources/man/kute.1

$(shell mkdir -p ../resources/man)
$(shell help2man -N ./kute > ../resources/man/kute.1)
