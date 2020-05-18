/*  $Id$ */
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
#include <kcgijson.h>

#include "backend.h"

/*
 * Start with five pages.
 * As you add more pages, you'll start by giving them an identifier key
 * in this enum.
 */
enum page {
  /*  unauthenticated status checking */
  PAGE_VOTER_CHECK_STATUS,
  /*  register a new voter */
  PAGE_VOTER_REGISTER,
  /*  get a new voter update session */
  PAGE_VOTER_UPDATE_LOGIN,
  /*  given a voter update session,
   *  update the corresponding voter's info */
  PAGE_VOTER_UPDATE_INFO,
  /*  get a new official session */
  PAGE_OFFICIAL_LOGIN,
  /*  run a query on the entire voter db */
  PAGE_OFFICIAL_QUERY_VOTERS,
  /*  run an on the entire voter db */
  PAGE_OFFICIAL_UPDATE_VOTERS,
  PAGE__MAX
};

static const char *const pages[PAGE__MAX] = {
  "voter_check_status",
  "voter_register",
  "voter_update_login",
  "voter_update_info",
  "official_login",
  "official_query_voters",
  "official_update_voters"
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

static void
empty_json(struct kreq *r)
{
  struct kjsonreq req;

  kjson_open(&req, r);
  kjson_obj_open(&req);
  kjson_obj_close(&req);
  kjson_close(&req);
}

status_t
get_int_param(struct kreq *r, enum valid_keys key, int64_t *int_val)
{
  if (NULL != r->fieldmap[key]) {
   *int_val = r->fieldmap[key]->parsed.i;
   return OK;
  }

  DBG("get_int_param: %s (%d) --> NOT_FOUND\n", valid_keys[key].name, key);
  return NOT_FOUND;
}

status_t
get_str_param(struct kreq *r, enum valid_keys key, const char **str_val)
{
  if (NULL != r->fieldmap[key]) {
    if (r->fieldmap[key]->parsed.s) {
     *str_val = r->fieldmap[key]->parsed.s;
     return OK;
    } else {
      DBG("get_str_param: %s (%d) --> NULL (NOT_FOUND)\n", valid_keys[key].name, key);
    }
  }

  DBG("get_str_param: %s (%d) --> NOT_FOUND\n", valid_keys[key].name, key);
  return NOT_FOUND;
}

status_t
get_blob_param(struct kreq *r, enum valid_keys key, char **buf, size_t *buf_sz)
{
  if (NULL != r->fieldmap[key] && r->fieldmap[key]->valsz > 0) {
    *buf    = r->fieldmap[key]->val;
    *buf_sz = r->fieldmap[key]->valsz;
    return OK;
  }

  DBG("get_blob_param: %s (%d) --> NOT_FOUND\n", valid_keys[key].name, key);
  return NOT_FOUND;
}

/*
 * Should this just re-use an existing session if one is found?
 * Or always create a new one?
 * */
static void
voter_login_page(struct kreq *r)
{
  char buf[64];
  time_t birthdate;
  size_t idinfo_sz;
  const char *lastname, *givennames, *resaddress, *mailaddress, *idinfo;

  if ( (OK == get_str_param(r, VALID_VOTER_LASTNAME,   &lastname)) &&
       (OK == get_str_param(r, VALID_VOTER_GIVENNAMES, &givennames)) &&
       (OK == get_str_param(r, VALID_VOTER_RESADDRESS, &resaddress)) &&
       (OK == get_str_param(r, VALID_VOTER_MAILADDRESS, &mailaddress)) &&
       (OK == get_int_param(r, VALID_VOTER_BIRTHDATE, &birthdate)) &&
       (OK == get_blob_param(r, VALID_VOTER_IDINFO, &idinfo, &idinfo_sz)) ) {
    int64_t sid, token;
    struct voter *voter;
    status_t session_create = new_voter_session(r->arg,
                                                lastname,
                                                givennames,
                                                resaddress,
                                                mailaddress,
                                                birthdate,
                                                idinfo,
                                                idinfo_sz,
                                                0 /*  not confidential */,
                                                &voter,
                                                &sid,
                                                &token);
    if (OK == session_create) {
      /*  TODO: correct time */
      kutil_epoch2str(time(NULL) + 60*5, buf, sizeof(buf));
      khttp_head(r, kresps[KRESP_SET_COOKIE],
                 "%s=%" PRId64 "; %s HttpOnly; path=/; expires=%s",
                 valid_keys[VALID_VOTERUPDATESESSION_TOKEN].name, token, "", buf);
      khttp_head(r, kresps[KRESP_SET_COOKIE],
                 "%s=%" PRId64 "; %s HttpOnly; path=/; expires=%s",
                 valid_keys[VALID_VOTERUPDATESESSION_ID].name, sid, "", buf);
      http_open(r, KHTTP_200);
      empty_json(r);
      db_voter_free(voter);
    } else {
      http_open(r, KHTTP_401);
    }
  } else {
    http_open(r, KHTTP_400);
  }
}

static void
voter_check_status_page(struct kreq *r)
{
  time_t birthddate;
  const char *lastname, *givennames;
  struct voter_q *voters;

  if ( (OK == get_str_param(r, VALID_VOTER_LASTNAME, &lastname)) &&
       (OK == get_str_param(r, VALID_VOTER_GIVENNAMES, &givennames)) &&
       (OK == get_int_param(r, VALID_VOTER_BIRTHDATE, &birthddate)) ) {
    status_t lookup = lookup_voter_information(r->arg, lastname, givennames, birthddate,
                                               0, /*  IMPORTANT: this block confidential voters! */
                                               &voters);
    if (OK == lookup) {
      struct kjsonreq req;

      http_open(r, KHTTP_200);
      kjson_open(&req, r);
      kjson_obj_open(&req);
      json_voter_array(&req, voters);
      kjson_obj_close(&req);
      kjson_close(&req);
    } else {
      http_open(r, KHTTP_500);
    }
  } else {
    http_open(r, KHTTP_400);
  }
}

int
main(int argc, char **argv)
{
  struct kreq  r;
  struct kfcgi *fcgi;
  enum kcgi_err err;

  bvrs_ctxt_t *ctxt;

  /* If this applies to your OS, you've made bad decisions. */
#if !HAVE_ARC4RANDOM
  srandom(time(NULL) + getpid());
#endif

  if ( KCGI_OK !=
       (err = khttp_fcgi_initx(&fcgi, kmimetypes, KMIME__MAX,
                               valid_keys, VALID__MAX,
                               ksuffixmap, KMIME_TEXT_HTML,
                               pages, PAGE__MAX,
                               PAGE_VOTER_CHECK_STATUS,
                               NULL,
                               NULL,
#ifdef DEBUG
                               1,
#else
                               0,
#endif
                               NULL)) ) {
     return EXIT_FAILURE;
  }

  while ( (err = khttp_fcgi_parse(fcgi, &r)) == KCGI_OK ) {
    if (r.method != KMETHOD_GET && r.method != KMETHOD_POST) {
      http_open(&r, KHTTP_405);
      khttp_free(&r);
      continue;
    } else if (r.page == PAGE__MAX /* @todo Add a MIME check? */) {
      http_open(&r, KHTTP_404);
      khttp_puts(&r, "Page not found.");
      khttp_free(&r);
      continue;
    }

    status_t openok = open_db(argv[1], &ctxt);

    flush_old_sessions(ctxt, 5*60, 5*60);

    r.arg = ctxt;

    //Get session? Bail Otherwise?
    switch (r.page) {
      case PAGE_VOTER_CHECK_STATUS:
        voter_check_status_page(&r);
        break;
        break;
      case PAGE_VOTER_UPDATE_LOGIN:
        voterlogin(&r);
        break;
      default:
        http_open(&r, KHTTP_404);
        break;
    }

    close_db(ctxt);
    khttp_free(&r);
  }
}
