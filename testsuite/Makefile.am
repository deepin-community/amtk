include $(top_srcdir)/aminclude_static.am
clean-local: code-coverage-clean
distclean-local: code-coverage-dist-clean

@VALGRIND_CHECK_RULES@

# https://github.com/dtrebbien/GNOME.supp
VALGRIND_SUPPRESSIONS_FILES =					\
	$(GLIB_PREFIX)/share/glib-2.0/valgrind/glib.supp	\
	$(top_srcdir)/../GNOME.supp/build/base.supp		\
	$(top_srcdir)/../GNOME.supp/build/pango.supp		\
	$(top_srcdir)/../GNOME.supp/build/gdk.supp		\
	$(top_srcdir)/../GNOME.supp/build/gtk3.supp

AM_CPPFLAGS = 			\
	-I$(top_srcdir) 	\
	$(WARN_CFLAGS) 		\
	$(AMTK_DEP_CFLAGS)

AM_LDFLAGS = $(WARN_LDFLAGS)

LDADD = $(top_builddir)/amtk/libamtk-core.la	\
	$(AMTK_DEP_LIBS)

noinst_PROGRAMS = $(UNIT_TEST_PROGS)
TESTS = $(UNIT_TEST_PROGS)
UNIT_TEST_PROGS =

UNIT_TEST_PROGS += test-action-info-store
test_action_info_store_SOURCES = test-action-info-store.c

UNIT_TEST_PROGS += test-action-map
test_action_map_SOURCES = test-action-map.c

UNIT_TEST_PROGS += test-utils
test_utils_SOURCES = test-utils.c

if INSTALLED_TESTS
insttestdir = $(libexecdir)/installed-tests/$(PACKAGE)-@AMTK_API_VERSION@
insttest_PROGRAMS = $(UNIT_TEST_PROGS)

%.test: %$(EXEEXT) Makefile
	$(AM_V_GEN) (echo '[Test]' > $@.tmp; \
	echo 'Description=Amtk test' >> $@.tmp; \
	echo 'Exec=$(insttestdir)/$<' >> $@.tmp; \
	echo 'Type=session' >> $@.tmp; \
	echo 'Output=TAP' >> $@.tmp; \
	mv $@.tmp $@)

test_files = $(UNIT_TEST_PROGS:=.test)

CLEANFILES = $(test_files)

testmetadir = $(datadir)/installed-tests/$(PACKAGE)-@AMTK_API_VERSION@
testmeta_DATA = $(test_files)

endif # INSTALLED_TESTS

-include $(top_srcdir)/git.mk
