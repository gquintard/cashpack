# CASHPACK_SANITY_CHECK
# ---------------------
AC_DEFUN([CASHPACK_SANITY_CHECK], [

	dnl hexcheck
	AC_MSG_CHECKING([for RFC 7541-compatible hexdumps])

	if ! "$srcdir"/tst/hexcheck 2>/dev/null
	then
		AC_MSG_RESULT([no])
		"$srcdir"/tst/hexcheck
		AC_MSG_FAILURE([hexdumps fail sanity check])
	fi

	AC_MSG_RESULT([yes])

	dnl bincheck
	AC_MSG_CHECKING([for working bindumps to hexdumps conversions])

	if ! "$srcdir"/tst/bincheck 2>/dev/null
	then
		AC_MSG_RESULT([no])
		"$srcdir"/tst/bincheck
		AC_MSG_FAILURE([bindumps fail sanity check])
	fi

	AC_MSG_RESULT([yes])

])

# CASHPACK_PROG_RST2MAN
-----------------------
AC_DEFUN([CASHPACK_PROG_RST2MAN], [

	AC_CHECK_PROGS(RST2MAN, [rst2man.py rst2man], [true])
	AC_SUBST([RST2MAN])

])

# CASHPACK_PROG_UNCRUSTIFY
# ------------------------
AC_DEFUN([CASHPACK_PROG_UNCRUSTIFY], [

	UNCRUSTIFY_OPTS="-c '\$(srcdir)/uncrustify.cfg' -q -l C --no-backup"

	AC_CHECK_PROGS(UNCRUSTIFY, [uncrustify], [true])
	test "$UNCRUSTIFY" = true && UNCRUSTIFY_OPTS=

	AC_SUBST([UNCRUSTIFY])
	AC_SUBST([UNCRUSTIFY_OPTS])

])

# CASHPACK_WITH_MEMCHECK
# ----------------------
AC_DEFUN([CASHPACK_WITH_MEMCHECK], [

	AC_CHECK_PROGS(VALGRIND, [valgrind], [no])

	AC_ARG_WITH([memcheck],
		AS_HELP_STRING(
			[--with-memcheck],
			[Run the test suite with Valgrind]),
		[MEMCHECK="$withval"],
		[MEMCHECK=OFF])

	test "$MEMCHECK" = yes && MEMCHECK=ON

	test "$MEMCHECK" = ON -a "$VALGRIND" = no &&
	AC_MSG_FAILURE([Valgrind is required with memcheck])

	AC_SUBST([MEMCHECK])

])

# _CASHPACK_ASAN
# --------------
AC_DEFUN([_CASHPACK_ASAN], [

	AC_CHECK_LIB(
		[asan],
		[__asan_address_is_poisoned], [
			LIBS="$ac_check_lib_save_LIBS"
			CFLAGS="$CFLAGS -fsanitize=address"
		],
		[AC_MSG_FAILURE([Missing libasan for address sanitizer])])

])

# CASHPACK_WITH_ASAN
# ------------------
AC_DEFUN([CASHPACK_WITH_ASAN], [

	AC_ARG_WITH([asan],
		AS_HELP_STRING(
			[--with-asan],
			[Build binaries with address sanitizer]),
		[_CASHPACK_ASAN],
		[])

])

# _CASHPACK_UBSAN
# ---------------
AC_DEFUN([_CASHPACK_UBSAN], [

	AC_CHECK_LIB(
		[ubsan],
		[__ubsan_handle_add_overflow], [
			LIBS="$ac_check_lib_save_LIBS"
			CFLAGS="$CFLAGS -fsanitize=undefined"
		],
		[AC_MSG_FAILURE([Missing libubsan for undefined sanitizer])])

])

# CASHPACK_WITH_UBSAN
# -------------------
AC_DEFUN([CASHPACK_WITH_UBSAN], [

	AC_ARG_WITH([ubsan],
		AS_HELP_STRING(
			[--with-ubsan],
			[Build binaries with undefined sanitizer]),
		[_CASHPACK_UBSAN],
		[])

])

# _CASHPACK_LCOV
# --------------
AC_DEFUN([_CASHPACK_LCOV], [

	AC_CHECK_PROGS(LCOV, [lcov], [no])
	test "$LCOV" = no &&
	AC_MSG_FAILURE([Lcov is required for code coverage])

	AC_CHECK_PROGS(GENHTML, [genhtml], [no])
	test "$GENHTML" = no &&
	AC_MSG_FAILURE([Lcov is missing genhtml for reports generation])

	LCOV_RULES="

lcov: all
	@\$(LCOV) -z -d .
	@\$(MAKE) \$(AM_MAKEFLAGS) -k check
	@\$(LCOV) -c -o tst.info -d tst
	@\$(LCOV) -c -o lib.info -d lib
	@\$(LCOV) -a tst.info -a lib.info -o raw.info
	@\$(LCOV) -r raw.info '/usr/*' -o cashpack.info
	@\$(GENHTML) -o lcov cashpack.info
	@echo file://\$(abs_builddir)/lcov/index.html

lcov-clean:

clean: lcov-clean
	@find \$(abs_builddir) -depth '(' \
		-name '*.gcda' -o \
		-name '*.gcov' -o \
		-name '*.gcno' -o \
		-name '*.info' \
		')' -delete
	@rm -rf \$(abs_builddir)/lcov/

.PHONY: lcov lcov-clean

"

	CPPFLAGS="$CPPFLAGS -DNDEBUG"
	CFLAGS="$CFLAGS -O0 -g -fprofile-arcs -ftest-coverage"
	LDFLAGS="$LDFLAGS -lgcov"

	AC_SUBST([LCOV])
	AC_SUBST([GENHTML])
	AC_SUBST([LCOV_RULES])
	m4_ifdef([_AM_SUBST_NOTMAKE], [_AM_SUBST_NOTMAKE([LCOV_RULES])])

])

# CASHPACK_WITH_LCOV
# ------------------
AC_DEFUN([CASHPACK_WITH_LCOV], [

	AC_ARG_WITH([lcov],
		AS_HELP_STRING(
			[--with-lcov],
			[Measure test suite code coverage with lcov]),
		[_CASHPACK_LCOV],
		[])

])