# Makefile for KevEdit program

AUTOMAKE_OPTIONS = 1.6.3

AM_CPPFLAGS = -DDATAPATH=\"$(pkgdatadir)\"

bin_PROGRAMS = kevedit

if HAVE_WINDRES
windres_SOURCES = $(top_srcdir)/inst/platform/windows/kevedit.rc
endif

kevedit_SOURCES = \
	kevedit.c main.c menu.c misc.c patbuffer.c screen.c \
	kevedit.h menu.h misc.h patbuffer.h screen.h $(windres_SOURCES) \
	../mkdtemp/mkdtemp.c

kevedit_LDADD = ../zlaunch/libzlaunch.a ../texteditor/libtexteditor.a ../dialogs/libdialogs.a ../help/libhelp.a ../themes/rusty/librusty.a ../synth/libsynth.a ../display/libdisplay.a ../structures/libstructures.a ../libzzt2/libzzt2.a $(GLOBLIB)

.rc.o:
	$(WINDRES) -o $@ $<

resources.o: $(windres_SOURCES)
