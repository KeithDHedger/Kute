#
#
AC_INIT([Kute],[0.1],[XXX],[Kute])

AC_CONFIG_HEADER([config.h])
AC_PROG_CXX

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_HEADER_STDC

AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

CFLAGS=-Wall
AC_SUBST(CFLAGS)


ID3_LIBS="-lid3"
ID3_CFLAGS="-I/usr/include"
AC_SUBST(ID3_CFLAGS)
AC_SUBST(ID3_LIBS)

ID3TAG_MODULES="id3tag >= 0.15.0"
PKG_CHECK_MODULES(ID3TAG, $ID3TAG_MODULES)
AC_SUBST(ID3TAG_CFLAGS)
AC_SUBST(ID3TAG_LIBS)

MP4_LIBS="-lmp4v2"
MP4_CFLAGS="-I/usr/include"
AC_SUBST(MP4_CFLAGS)
AC_SUBST(MP4_LIBS)



#TESTVAR="XXXXXXXXXXX"
#AC_DEFINE(TESTVAR,zxzx)
#AC_ARG_VAR (TESTVAR, description)
#AC_SUBST(TESTVAR)
#AC_ARG_ENABLE([spell],
#	AS_HELP_STRING([--disable-spell],[Disable spell plugin (default: enabled)]),
#	[enable_enchant=$enableval],
#	[enable_enchant=yes])
#
AC_ARG_WITH(ffmpeg-path,
          AC_HELP_STRING(
	  	  [--with-ffmpeg-path],
		  [Full path to ffmpeg]),
	[ TESTVAR="$withval"],
	[ TESTVAR="/usr/bin/ffmpeg" ])
AC_SUBST(TESTVAR)

#fubar=42
#AC_CONFIG_COMMANDS([fubar],[echo this is extra $fubar, and so on.],[fubar=$fubar])

#TAG_MODULES="taglib >= 1.5.0"
#PKG_CHECK_MODULES(TAGLIB, $TAG_MODULES)
#TAG_CFLAGS=-I/usr/include/taglib
#TAG_LIBS="--ltag"
#AC_SUBST(TAGLIB_CFLAGS)
#AC_SUBST(TAGLIB_LIBS)



FLAC_MODULES="flac >= 0.0.0"
PKG_CHECK_MODULES(FLAC, $FLAC_MODULES)
AC_SUBST(FLAC_CFLAGS)
AC_SUBST(FLAC_LIBS)

AC_OUTPUT(Makefile Kute/app/Makefile Kute/debug/Makefile)#***MMM***

