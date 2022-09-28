AM_CPPFLAGS = 			\
	-I$(top_srcdir) 	\
	$(WARN_CFLAGS) 		\
	$(AMTK_DEP_CFLAGS)

AM_LDFLAGS = $(WARN_LDFLAGS)

LDADD = $(top_builddir)/amtk/libamtk-core.la	\
	$(AMTK_DEP_LIBS)

TEST_PROGS =

TEST_PROGS += test-headerbar
test_headerbar_SOURCES = test-headerbar.c

TEST_PROGS += test-traditional-ui
test_traditional_ui_SOURCES = test-traditional-ui.c

noinst_PROGRAMS = $(TEST_PROGS)

-include $(top_srcdir)/git.mk
