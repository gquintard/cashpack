# Copyright (c) 2016 Dridi Boukelmoune
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

BUILD_MAN = $(RST2MAN) --strict --strip-comments

dist_man_MANS = \
	cashpack.3 \
	hpack_alloc.3 \
	hpack_decode.3 \
	hpack_decoder.3 \
	hpack_encoder.3 \
	hpack_foreach.3 \
	hpack_free.3 \
	hpack_resize.3 \
	hpack_strerror.3 \
	hpack_trim.3

CLEANFILES = $(dist_man_MANS)
EXTRA_DIST = \
	$(dist_man_MANS) \
	cashpack.3.rst \
	hpack_alloc.3.rst \
	hpack_decode.3.rst \
	hpack_foreach.3.rst

cashpack.3: cashpack.3.rst
	$(AM_V_GEN) $(BUILD_MAN) $(srcdir)/cashpack.3.rst cashpack.3

hpack_strerror.3:
	$(AM_V_GEN) echo ".so man3/cashpack.3" >hpack_strerror.3

hpack_alloc.3: hpack_alloc.3.rst
	$(AM_V_GEN) $(BUILD_MAN) $(srcdir)/hpack_alloc.3.rst hpack_alloc.3

hpack_decoder.3:
	$(AM_V_GEN) echo ".so man3/hpack_alloc.3" >hpack_decoder.3

hpack_encoder.3:
	$(AM_V_GEN) echo ".so man3/hpack_alloc.3" >hpack_encoder.3

hpack_resize.3:
	$(AM_V_GEN) echo ".so man3/hpack_alloc.3" >hpack_resize.3

hpack_trim.3:
	$(AM_V_GEN) echo ".so man3/hpack_alloc.3" >hpack_trim.3

hpack_free.3:
	$(AM_V_GEN) echo ".so man3/hpack_alloc.3" >hpack_free.3

hpack_decode.3: hpack_decode.3.rst
	$(AM_V_GEN) $(BUILD_MAN) $(srcdir)/hpack_decode.3.rst hpack_decode.3

hpack_foreach.3: hpack_foreach.3.rst
	$(AM_V_GEN) $(BUILD_MAN) $(srcdir)/hpack_foreach.3.rst hpack_foreach.3
