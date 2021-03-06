/*-
 * Copyright (c) 2016 Dridi Boukelmoune
 * All rights reserved.
 *
 * Author: Dridi Boukelmoune <dridi.boukelmoune@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hpack.h"
#include "hpack_assert.h"

#include "tst.h"

struct dyn_ctx {
	size_t	cnt;
	size_t	len;
	size_t	sz;
};

static void
tst_print_cb(void *priv, enum hpack_evt_e evt, const char *buf, size_t len)
{
	struct dyn_ctx *ctx;
	char str[sizeof "\n[IDX] (s = LEN) "];
	int l;

	assert(priv != NULL);
	ctx = priv;
	if (ctx->cnt == 0)
		OUT("\n");

	switch (evt) {
	case HPACK_EVT_FIELD:
		assert(buf == NULL);
		assert(len > 0);
		ctx->cnt++;
		ctx->len += len;
		l = snprintf(str, sizeof str, "\n[%3zu] (s = %3zu) ",
		    ctx->cnt, len);
		assert(l + 1 == sizeof  str);
		WRT(str, l);
		break;
	case HPACK_EVT_VALUE:
		OUT(": ");
		/* fall through */
	case HPACK_EVT_NAME:
		assert(buf != NULL);
		WRT(buf, len);
		break;
	default:
		WRONG("Unexpected event");
	}
}

void
TST_print_table(struct hpack *hp)
{
	struct dyn_ctx ctx;
	char buf[8];

	ctx.cnt = 0;
	ctx.len = 0;

	OUT("Dynamic Table (after decoding):");

	(void)hpack_foreach(hp, tst_print_cb, &ctx);

	if (ctx.cnt == 0) {
		assert(ctx.len == 0);
		OUT(" empty.\n");
	}
	else {
		assert(ctx.len > 0);
		ctx.sz = snprintf(buf, sizeof buf, "%3zu\n", ctx.len);
		OUT("\n      Table size: ");
		WRT(buf, ctx.sz);
	}
}

int
TST_decode(struct dec_ctx *ctx)
{
	tst_decode_f *cb;
	size_t len;
	int res;

	assert(ctx->len > 0);

	OUT("Decoded header list:\n");
	res = 0;

	do {
		assert(res == 0);
		switch (*ctx->spec) {
		case '\0':
			cb = ctx->dec;
			len = ctx->len;
			break;
		case 'd':
			ctx->spec++;

			cb = ctx->dec;
			len = atoi(ctx->spec);
			assert(len <= ctx->len);
			break;
		case 'r':
			ctx->spec++;

			cb = ctx->rsz;
			len = atoi(ctx->spec);
			break;
		default:
			WRONG("Invalid spec");
		}

		if (*ctx->spec != '\0') {
			ctx->spec = strchr(ctx->spec, ',');
			assert(ctx->spec != NULL);
			ctx->spec++;
		}

		res = cb(ctx->priv, ctx->buf, len);

		if (cb == ctx->dec) {
			ctx->buf += len;
			ctx->len -= len;
		}
	} while (ctx->len > 0);

	return (res);
}

enum hpack_res_e
TST_translate_error(const char *str)
{
#define HPR_ERRORS_ONLY
#define HPR(val, cod, txt)			\
		if (!strcmp(str, #val))	\
			return (HPACK_RES_##val);
#include "tbl/hpack_tbl.h"
#undef HPR
#undef HPR_ERRORS_ONLY
	WRONG("Unknown error");
	return (-1);
}
