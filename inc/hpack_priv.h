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

#define HPT_STATIC_MAX 61

/**********************************************************************
 * Data Structures
 */

enum hpack_encoding_e { /* Section 5.2 */
	HPACK_HUFFMAN	= 0x80,
	HPACK_RAW	= 0x00,
};

struct hpt_field {
	char		*nam;
	char		*val;
	uint16_t	nam_sz;
	uint16_t	val_sz;
};

struct hpt_entry {
	uint32_t	magic;
#define HPT_ENTRY_MAGIC	0xe4582b39
	uint32_t	align;
	int64_t		pre_sz;
	uint16_t	nam_sz;
	uint16_t	val_sz;
	uint16_t	pad[6];
};

struct hpt_priv {
	struct hpack_ctx	*ctx;
	struct hpt_entry	*he;
	void			*wrt;
	size_t			len;
	unsigned		nam;
};

struct hpack {
	uint32_t			magic;
#define ENCODER_MAGIC			0x8ab1fb4c
#define DECODER_MAGIC			0xab0e3218
#define DEFUNCT_MAGIC			0xdffadae9
	const struct hpack_alloc	*alloc;
	size_t				max;
	size_t				lim;
	size_t				len;
	size_t				cnt;
	ptrdiff_t			off;
	struct hpt_entry		tbl[0];
};

struct hpack_ctx {
	enum hpack_res_e	res;
	struct hpack		*hp;
	const uint8_t		*buf;
	uint8_t			*cur;
	size_t			len;
	size_t			max;
	union {
		hpack_decoded_f	*dec;
		hpack_encoded_f	*enc;
		hpack_encoded_f	*cb; /* dirty covariance hack */
	};
	void			*priv;
};

typedef int hpack_validate_f(struct hpack_ctx*, const char *, size_t, unsigned);

/**********************************************************************
 * Utility Macros
 */

#define TRUST_ME(ptr)	((void *)(uintptr_t)(ptr))

#define HPACK_CTX	struct hpack_ctx *ctx
#define HPACK_ITM	const struct hpack_item *itm

#define CALL(func, args...)				\
	do {						\
		if ((func)(args) != 0)			\
			return (-1);			\
	} while (0)

#define CALLBACK(ctx, args...)				\
	do {						\
		(ctx)->cb((ctx)->priv, args);		\
	} while (0)

#define EXPECT(ctx, err, cond)				\
	do {						\
		if (!(cond)) {				\
			(ctx)->res = HPACK_RES_##err;	\
			return (-1);			\
		}					\
	} while (0)

/**********************************************************************
 * Function Signatures
 */

void HPE_push(HPACK_CTX, const void *, size_t);
void HPE_send(HPACK_CTX);

int HPI_decode(HPACK_CTX, size_t, uint16_t *);
int HPI_encode(HPACK_CTX, size_t, uint8_t, uint16_t);

int HPH_decode(HPACK_CTX, hpack_validate_f, size_t);

hpack_validate_f HPV_token;
hpack_validate_f HPV_value;

hpack_decoded_f HPT_insert;
void HPT_adjust(struct hpack_ctx *, size_t);
int  HPT_search(HPACK_CTX, size_t, struct hpt_field *);
void HPT_foreach(HPACK_CTX);
int  HPT_decode(HPACK_CTX, size_t);
int  HPT_decode_name(HPACK_CTX, size_t);
