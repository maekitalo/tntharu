ACLOCAL_AMFLAGS = -I m4 --install
AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS = -I$(top_srcdir)/include

bin_PROGRAMS = tntharu

noinst_HEADERS = \
	configuration.h \
	include/hpdf++.h \
	model/index.ecpp

ecppSources = \
	controller/index.ecpp \
	pdf/example.ecpp \
	pdf/fontpage.ecpp \
	pdf/ttfontpage.ecpp \
	pdf/outline.ecpp \
	view/index.ecpp \
	view/fonts.ecpp \
	view/ttfonts.ecpp \
	webmain.ecpp

staticSources = \
	resources/tntharu.css

tntharu_SOURCES = \
	main.cpp \
	configuration.cpp \
	src/hpdf++.cpp \
	$(ecppSources)

nodist_tntharu_SOURCES = \
	resources.cpp

tntharu_LDADD = -lcxxtools -ltntnet -lhpdf

nobase_dist_noinst_DATA = $(staticSources)

CLEANFILES = $(ecppSources:.ecpp=.cpp) $(ecppSources:.ecpp=.deps) resources.cpp

EXTRA_DIST = $(staticSources)

#
# Rules for tntnet applications
#

ECPPFLAGS=-I$(srcdir)

# resources
resources.cpp: $(staticSources) Makefile.am
	$(AM_V_GEN)$(ECPPC) -bb -z -n resources -p -o resources.cpp $(ECPPFLAGS) $(staticSources)

SUFFIXES=.ecpp .cpp

.ecpp.cpp:
	$(AM_V_GEN)$(ECPPC) -n `echo $<|$(SED) 's|^$(srcdir)/||; s|\.ecpp$$||'` $(ECPPFLAGS) -o $@ $<
.ecpp.deps:
	$(AM_V_GEN)$(ECPPC) -M $(ECPPFLAGS) $< | $(SED) '1s/\(.*\).cpp:/\1.cpp \1.deps:/' > $@

-include $(ecppSources:.ecpp=.deps)
