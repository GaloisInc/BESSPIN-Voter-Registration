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
#include <stdbool.h>

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
  /*  run an update on the entire voter db */
  PAGE_OFFICIAL_UPDATE_VOTERS,
  PAGE__MAX
};

struct field_error {
  char field[32];
  char desc[128];
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
    DBG("%s\n",r->fieldmap[key]->parsed.s);
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
get_blob_param(struct kreq *r, enum valid_keys key, const char **buf, size_t *buf_sz)
{
  if (NULL != r->fieldmap[key] && r->fieldmap[key]->valsz > 0) {
    *buf    = r->fieldmap[key]->val;
    *buf_sz = r->fieldmap[key]->valsz;
    return OK;
  }

  DBG("get_blob_param: %s (%d) --> NOT_FOUND\n", valid_keys[key].name, key);
  return NOT_FOUND;
}

static void
official_query_voters(struct kreq *r)
{
  const char *field_name, *field_contains, *date_field;
  time_t date_from, date_thru;

  get_str_param('field-name', , &field_name);
  get_str_param(r, ,&field_contains);
  get_int_param(r, , &date_from);
  get_int_param(r, ,date_thru);

}

static void
voter_register_page(struct kreq *r)
{
  const char *lastname, *givennames,
             *resaddress, *resaddress2, *reszip, *resstate,
             *mailaddress, *mailaddress2, *mailzip, *mailstate,
             *party, *idinfo;
  time_t birthdate;
  size_t idinfo_sz;
  struct kjsonreq req;
  size_t num_fields = 11;
  struct field_error errors[num_fields];

  memset(errors, 0, sizeof(errors));
  DBG("\nWTH\n");
  int error_count = 0;
  if (OK != get_str_param(r, VALID_VOTER_LASTNAME,   &lastname)) {
    errors[error_count] = (struct field_error) { "voter-lastname", "Last name field required."};
    error_count++;
  }
  if(OK != get_str_param(r, VALID_VOTER_GIVENNAMES, &givennames)) {
    errors[error_count] = (struct field_error) { "voter-givennames", "Given name field required."};
    error_count++;
  }
  if(OK != get_str_param(r, VALID_VOTER_RESADDRESS, &resaddress)) {
    errors[error_count] = (struct field_error) { "voter-resaddress", "Residential address required."};
    error_count++;
  }
  if(OK != get_str_param(r, VALID_VOTER_RESZIP, &reszip)) {
    errors[error_count] = (struct field_error) { "voter-reszip", "Residential zipcode required."};
    error_count++;
  }
  if(OK != get_str_param(r, VALID_VOTER_RESSTATE, &resstate)) {
    errors[error_count] = (struct field_error) { "voter-resstate", "Residential state required."};
    error_count++;
  }
  if (OK != get_str_param(r, VALID_VOTER_MAILADDRESS, &mailaddress)) {
    errors[error_count] = (struct field_error) { "voter-mailaddress", "Mailing address required."};
    error_count++;
  }
  if (OK != get_str_param(r, VALID_VOTER_MAILZIP, &mailzip)) {
    errors[error_count] = (struct field_error) { "voter-mailzip", "Mailing zipcode required."};
    error_count++;
  }
  if (OK != get_str_param(r, VALID_VOTER_MAILSTATE, &mailstate)) {
    errors[error_count] = (struct field_error) { "voter-mailstate", "Mailing state required."};
    error_count++;
  }
  DBG("mailstate: %s\n", mailstate);
  if (OK != get_int_param(r, VALID_VOTER_BIRTHDATE, &birthdate)) {
    errors[error_count] = (struct field_error) { "voter-birthdate", "Date of birth field required."};
    error_count++;
  }
  if (OK != get_str_param(r, VALID_VOTER_REGISTEREDPARTY, &party)) {
    errors[error_count] = (struct field_error) { "voter-registeredparty", "Registered party field required."};
    error_count++;
  } else if(strcmp(party, "---") == 0) {
    DBG("Registered party not valid\n");
    errors[error_count] = (struct field_error) { "voter-registeredparty", "Registered party field required(2)."};
    error_count++;
  }
  if (OK != get_blob_param(r, VALID_VOTER_IDINFO, &idinfo, &idinfo_sz)) {
    errors[error_count] = (struct field_error) { "voter-idinfo", "ID field required."};
    error_count++;
  }

  if(0 == error_count) {
    int64_t voter_id;
    status_t reg_status = register_voter(r->arg,
                                         lastname,
                                         givennames,
                                         resaddress,
                                         resaddress2,
                                         reszip,
                                         resstate,
                                         mailaddress,
                                         mailaddress2,
                                         mailzip,
                                         mailstate,
                                         party,
                                         birthdate,
                                         idinfo,
                                         idinfo_sz,
                                         0,
                                         &voter_id);
    if (OK == reg_status) {
      http_open(r, KHTTP_200);
      empty_json(r);
    } else {
      // encountered a db error.  Could be unique key constraint or
      // something similar
      http_open(r, KHTTP_400);
    }
  } else {

    // Form generated errors
    http_open(r, KHTTP_400);
    kjson_open(&req, r);
    kjson_obj_open(&req); // {
    kjson_objp_open(&req, "errors"); // "errors:" {
    kjson_putintp(&req, "count", error_count);
    for(int i=0; i<error_count; i++) {
      kjson_putstringp(&req, errors[i].field, errors[i].desc);
    }
    kjson_obj_close(&req); //    }
    kjson_obj_close(&req); // }
    kjson_close(&req);
  }

}

status_t
do_voter_updateinfo(struct kreq *r, int64_t voter_id)
{
  const char *lastname, *givennames,
             *resaddress, *resaddress2, *reszip, *resstate,
             *mailaddress, *mailaddress2, *mailzip, *mailstate,
             *party, *idinfo;
  time_t birthdate;
  size_t idinfo_sz;
  enum regstatus status;
  int64_t status_int;
  status_t ret = ERROR;

  if ( (OK == get_str_param(r, VALID_VOTER_LASTNAME,   &lastname)) &&
       (OK == get_str_param(r, VALID_VOTER_GIVENNAMES, &givennames)) &&
       (OK == get_str_param(r, VALID_VOTER_RESADDRESS, &resaddress)) &&
       (OK == get_str_param(r, VALID_VOTER_MAILADDRESS, &mailaddress)) &&
       (OK == get_int_param(r, VALID_VOTER_BIRTHDATE, &birthdate))  &&
       (OK == get_int_param(r, VALID_VOTER_STATUS, &status_int))  &&
       (OK == get_str_param(r, VALID_VOTER_REGISTEREDPARTY, &party))   &&
       (OK == get_blob_param(r, VALID_VOTER_IDINFO, &idinfo, &idinfo_sz))  ) {
    status = (enum regstatus)status_int; // This is safe as kcgi has validated it.
    ret = update_voter_information(r->arg,
                                   voter_id,
                                   lastname,
                                   givennames,
                                   resaddress,
                                   resaddress2,
                                   reszip,
                                   resstate,
                                   mailaddress,
                                   mailaddress2,
                                   mailzip,
                                   mailstate,
                                   party,
                                   birthdate,
                                   idinfo,
                                   idinfo_sz,
                                   status,
                                   0 /*  not confidential */);
  }

  return ret;
}

static void
voter_update_info_page(struct kreq *r)
{
  int64_t voterid;
  status_t lookup;
  int64_t sid;
  int64_t token;

  // 1. Check cookie
  if ( (r->cookiemap[VALID_VOTERUPDATESESSION_ID] != NULL) &&
       (r->cookiemap[VALID_VOTERUPDATESESSION_TOKEN] != NULL) ) {
    sid   = r->cookiemap[VALID_VOTERUPDATESESSION_ID]->parsed.i;
    token = r->cookiemap[VALID_VOTERUPDATESESSION_TOKEN]->parsed.i;
    // 2. Get session
    lookup = lookup_voter_session(r->arg, sid, token, &voterid);
    if ( (OK == lookup) &&
         // 3. Do the update
         do_voter_updateinfo(r, voterid) ) {
      http_open(r, KHTTP_200);
      empty_json(r);
      return;
    }
  }

  http_open(r, KHTTP_403);
  empty_json(r);
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
  const char *lastname, *givennames,
    *resaddress, *resaddress2, *reszip, *resstate,
    *mailaddress, *mailaddress2, *mailzip, *mailstate,
    *idinfo;

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
                                                resaddress2,
                                                reszip,
                                                resstate,
                                                mailaddress,
                                                mailaddress2,
                                                mailzip,
                                                mailstate,
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
      // Unknown DB Error
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
    if (openok != OK) {
      return EXIT_FAILURE;
    }

    flush_old_sessions(ctxt, 5*60, 5*60);

    r.arg = ctxt;

    //Get session? Bail Otherwise?
    switch (r.page) {
      case PAGE_VOTER_CHECK_STATUS:
        voter_check_status_page(&r);
        break;
      case PAGE_VOTER_REGISTER:
        voter_register_page(&r);
        break;
      case PAGE_VOTER_UPDATE_LOGIN:
        voter_login_page(&r);
        break;
      case PAGE_VOTER_UPDATE_INFO:
        voter_update_info_page(&r);
        break;
      case PAGE_OFFICIAL_QUERY_VOTERS:
        official_query_voters(&r);
        break;
      default:
        http_open(&r, KHTTP_404);
        break;
    }

    close_db(ctxt);
    khttp_free(&r);
  }
}
