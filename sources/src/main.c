/*	$Id$ */
/*
 * Copyright (c) 2016, 2018, 2020 Kristaps Dzonsons <kristaps@bsd.lv>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#if HAVE_ERR
# include <err.h>
#endif

#include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <kcgi.h>

/*
 * Start with five pages.
 * As you add more pages, you'll start by giving them an identifier key
 * in this enum.
 */
enum	page {
	PAGE_INDEX,
	PAGE_LOGIN,
	PAGE_LOGOUT,
	PAGE_USER_MOD_EMAIL,
	PAGE_USER_MOD_PASS,
	PAGE__MAX
};

static const char *const pages[PAGE__MAX] = {
	"index", /* PAGE_INDEX */
	"login", /* PAGE_LOGIN */
	"logout", /* PAGE_LOGOUT */
	"usermodemail", /* PAGE_USER_MOD_EMAIL */
	"usermodpass", /* PAGE_USER_MOD_PASS */
};

/*
 * Fill out all headers then start the HTTP document body.
 * No more headers after this point!
 */
static void
http_open(struct kreq *r, enum khttp code)
{

	khttp_head(r, kresps[KRESP_STATUS], 
		"%s", khttps[code]);
	khttp_head(r, kresps[KRESP_CONTENT_TYPE], 
		"%s", kmimetypes[r->mime]);
	khttp_head(r, "X-Content-Type-Options", "nosniff");
	khttp_head(r, "X-Frame-Options", "DENY");
	khttp_head(r, "X-XSS-Protection", "1; mode=block");
	khttp_body(r);
	/*  khttp_puts, khttp_free ... */
}

/*
 * Send basic index page...(starter code)
 */
static void
sendindex(struct kreq *r)
{
	http_open(r, KHTTP_200);
	khttp_puts(r, "Hello world!");
	khttp_free(r);
	/*   khttp_puts, khttp_free ... */
}


int
main(void)
{
	struct kreq	 r;
	struct kfcgi *fcgi;

	/* If this applies to your OS, you've made bad decisions. */
#if !HAVE_ARC4RANDOM
	srandom(time(NULL) + getpid());
#endif

	if ( KCGI_OK !=
		 khttp_fcgi_init(&fcgi, NULL, 0, pages, PAGE__MAX, PAGE_INDEX) ) {
		 return EXIT_FAILURE;
	}

	while ( khttp_fcgi_parse(fcgi, &r) == KCGI_OK ) {
		printf("Got one...\n");
		/*
		 * Front line of defence: make sure we're a proper method, make
		 * sure we're a page, make sure we're a JSON file.
		 */
		if (r.method != KMETHOD_GET && r.method != KMETHOD_POST) {
			http_open(&r, KHTTP_405);
			khttp_free(&r);
			return EXIT_SUCCESS;
		} else if (r.page == PAGE__MAX /* @todo Add a MIME check? */) {
			http_open(&r, KHTTP_404);
			khttp_puts(&r, "Page not found.");
			khttp_free(&r);
			return EXIT_SUCCESS;
		}

		//Get session? Bail Otherwise?
		switch (r.page) {
		case PAGE_INDEX:
			sendindex(&r);
			break;
		case PAGE_LOGIN:
			break;
		case PAGE_LOGOUT:
			break;
		case PAGE_USER_MOD_EMAIL:
			break;
		case PAGE_USER_MOD_PASS:
			break;
		default:
			abort();
	}

	khttp_free(&r);
	return EXIT_SUCCESS;

	}
}
