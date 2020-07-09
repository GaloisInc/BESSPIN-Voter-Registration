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

#define _GNU_SOURCE 

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

#define DATE_MIN -3000000000

static char* database_name;

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
  /* delete official session */
  PAGE_OFFICIAL_LOGOUT,
  /*  run a query on the entire voter db */
  PAGE_OFFICIAL_QUERY_VOTERS,
  /*  run an update on the entire voter db */
  PAGE_OFFICIAL_UPDATE_VOTERS,
  /* register a voter as an official */
  PAGE_OFFICIAL_REGISTER_VOTER,
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
  "official_logout",
  "official_query_voters",
  "official_update_voters",
  "official_register_voter"
};


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
get_date_param2(struct kreq *r, char* key, time_t *time) {
  for(size_t i=0; i < r->fieldsz; i++) {
    if(strcmp(r->fields[i].key, key) == 0) {
      if(strlen(r->fields[i].val) < 1) {
        return NOT_FOUND;
      }
      if(!kvalid_date(&r->fields[i])) {
        return ERROR;
      } else {
        *time = r->fields[i].parsed.i;
        return OK;
      }
    }
  }
  return NOT_FOUND;
}


/**
 * Fetches "non-valid" integer parameter which are not part of 
 * openrad tool models.
 */
 status_t
 get_int_param2(struct kreq *r, char* key, int64_t *int_val) {
  for(size_t i=0; i < r->fieldsz; i++) {
    if(strcmp(r->fields[i].key, key) == 0) {
      *int_val = strtol(r->fields[i].val, NULL, 10);
      DBG("%s: %ld\n", key, *int_val);
      return OK;
    }
  }
  DBG("get_int_param2: %s --> NULL (NOT_FOUND)\n", key);
  return NOT_FOUND;  
 }

 /**
 * Fetches "non-valid" bool parameter which are not part of 
 * openrad tool models.
 */
 status_t
 get_bool_param2(struct kreq *r, char* key, bool *int_val) {
   int64_t val;
   if(OK == get_int_param2(r, key, &val)) {
    *int_val = (bool) val;
   }
   return NOT_FOUND;
}

/**
 * Fetches "non-valid" parameters which are not part of an
 * openradtool model. 
 */
status_t
get_str_param2(struct kreq *r, char* key, const char **str_val) {
  for(size_t i=0; i < r->fieldsz; i++) {
    if(strcmp(r->fields[i].key, key) == 0) {
      *str_val = r->fields[i].val;
      DBG("FOUND: %s: %s\n", key, *str_val);
      return OK;
    }
  }
  DBG("get_str_param2: %s --> NULL (NOT_FOUND)\n", key);
  return NOT_FOUND;
}

status_t
get_str_param(struct kreq *r, enum valid_keys key, const char **str_val)
{
  if (NULL != r->fieldmap[key]) {
    DBG("get_str_param: %s\n",r->fieldmap[key]->parsed.s);
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

static char *util_cat(char *dest, char *end, const char *str)
{
    while (dest < end && *str)
        *dest++ = *str++;
    return dest;
}

size_t join_str(char *out_string, size_t out_bufsz, const char *delim, char **chararr)
{
    char *ptr = out_string;
    char *strend = out_string + out_bufsz;
    while (ptr < strend && *chararr)
    {
        ptr = util_cat(ptr, strend, *chararr);
        chararr++;
        if (*chararr)
            ptr = util_cat(ptr, strend, delim);
    }
    return ptr - out_string;
}

static void
official_update_voters(struct kreq *r)
{
  struct kjsonreq req;
  const char *voter_idstr = "";
  const char *form_action = "";
  char *rest = NULL;
  char * clean_ids = NULL;
  char *token = NULL;
  int token_count = 0;
  int voter_ids[100]; // Maximum records after that we stop processing
  char *update_stmt = "";
  size_t cleaned_id_count = 0;
  char *error = "";
  
  get_str_param2(r, "voter-ids", &voter_idstr);
  if(strlen(voter_idstr) > 0) {
    char* tokstr = strdup(voter_idstr);
    for(token = strtok_r(tokstr, ",", &rest); token != NULL; token = strtok_r(NULL, ",", &rest)) {
      voter_ids[token_count] = atoi(token);
      token_count++;
    }
  }
  if(0 == token_count) {
    error = "No voter-ids provided to update";
  }
  DBG("token count: %d\n", token_count);
  for(int i=0; i<token_count; i++) {
    DBG("voter id %d\n", voter_ids[i]);
  }


  if(OK == get_str_param2(r, "form-action", &form_action)) {
    if(strcmp("mark-confidential", form_action) == 0 ) {
      for(int i=0; i<token_count; i++) {
        if(!db_voter_update_confidential(r->arg, 1, time(NULL), voter_ids[i])) {
          error = "Error updating confidentiality.";
        }
      }
    } else if(strcmp("mark-not-confidential", form_action) == 0) {
      for(int i=0; i<token_count; i++) {
        if(!db_voter_update_confidential(r->arg, 0, time(NULL), voter_ids[i])) {
          error = "Error updating confidentiality.";
        };
      }
    } else if(strcmp("mark-active", form_action) == 0) {
      for(int i=0; i<token_count; i++) {
        if(!db_voter_update_status(r->arg, REGSTATUS_ACTIVE, time(NULL), voter_ids[i])) {
          error = "Error updating status.";
        }
      }
    } else if(strcmp("mark-inactive", form_action) == 0) {
      for(int i=0; i<token_count; i++) {
        if(!db_voter_update_status(r->arg, REGSTATUS_INACTIVE, time(NULL), voter_ids[i])) {
          error = "Error updating status.";
        }
      }
    } else if(strcmp("delete", form_action) == 0) {
      for(int i=0; i<token_count; i++) {
        if(db_voter_delete_by_id_eq(r->arg, voter_ids[i])) {
          error = "Error updating status.";
        }
      }
    } else {
      error = "Invalid action given";
    }
  } else {
    error = "Form action is invalid or unspecified";
  }


  if(strlen(error) > 0) {
    http_open(r, KHTTP_400);
    kjson_open(&req, r);
    kjson_objp_open(&req, "errors"); // "errors:" {
    kjson_putstringp(&req, "all", error);
    kjson_obj_close(&req); // }
    kjson_close(&req);
  } else {
    http_open(r, KHTTP_200);
    empty_json(r);
  }

}

static void
official_query_voters(struct kreq *r)
{
  struct kjsonreq req;
  const char *field_name = "";
  const char *field_contains = "";
  const char *date_field = "";
  time_t date_from = DATE_MIN;
  time_t date_thru;
  time(&date_thru);
  bool invert_contains = false;
  bool select_active = false;
  bool select_updated = false;
  bool invert_date_selection = false;
  status_t lookup;
  int error_count = 0;
  int num_fields = 8;
  struct field_error errors[num_fields];

  memset(errors, 0, sizeof(errors));
  
  DBG("official_query_voters\n");

  // Fetch boolean modifiers. All default to false
  get_bool_param2(r, "select-active", &select_active);
  get_bool_param2(r, "select-updated", &select_updated);
  get_bool_param2(r, "invert-contains", &invert_contains);
  get_bool_param2(r, "date-invert", &invert_date_selection);

  get_str_param2(r, "field-contains", &field_contains);

  get_str_param2(r, "field-name", &field_name);
  if(field_name != NULL && strlen(field_name) > 0) {
    if(!(strcmp(field_name,"lastname") == 0 || 
         strcmp(field_name, "givennames") == 0 ||
         strcmp(field_name, "resaddress") == 0 ||
         strcmp(field_name, "resaddress2") == 0 ||
         strcmp(field_name, "reszip") == 0 ||
         strcmp(field_name, "resstate") == 0 ||
         strcmp(field_name, "mailaddress") == 0 ||
         strcmp(field_name, "mailaddress2") == 0 ||
         strcmp(field_name, "mailzip") == 0 ||
         strcmp(field_name, "mailstate") == 0)) {

         errors[error_count] = (struct field_error) { "field_name", "Invalid field-name"};
         error_count++;
    }
  }

  get_str_param2(r, "date-field", &date_field);
  if("date-field" != NULL && strlen(date_field) > 0) {
    if( !(strcmp(date_field,"birthdate") == 0 || 
          strcmp(date_field, "initialregtime") == 0 ||
          strcmp(date_field, "lastupdatetime") == 0) ) {
          errors[error_count] = (struct field_error) { "date-field", "Invalid date-field"};
          error_count++;
    }
  }

  if(ERROR == get_date_param2(r, "date-from", &date_from)) {
    errors[error_count] = (struct field_error) { "date-from", "Invalid Date."};
    error_count++;
  }
  if(ERROR == get_date_param2(r, "date-thru", &date_thru)) {
    errors[error_count] = (struct field_error) { "date-thru", "Invalid Date."};
    error_count++;
  }

  if(error_count) {
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
    return;
  }

  struct voter_q *q = malloc(sizeof(struct voter_q));
  if (q == NULL) {
    perror(NULL);
    exit(EXIT_FAILURE);
  }
  TAILQ_INIT(q);

  DBG(
    "field-name: %s\n"
    "field-contains: %s\n"
    "field-invert: %d\n"
    "date-field: %s\n"
    "date-from: %ld\n"
    "date-thru: %ld\n"
    "date-invert: %d\n"
    "select-updated: %d\n"
    "select-active: %d\n",
    field_name,
    field_contains,
    invert_contains,
    date_field,
    date_from,
    date_thru,
    invert_date_selection,
    select_updated,
    select_active
  );

  lookup = official_query(database_name, field_name,
  field_contains, invert_contains, date_field, date_from, date_thru,
  invert_date_selection, select_active, select_updated, &q);

  if (OK == lookup) {
      http_open(r, KHTTP_200);
      kjson_open(&req, r);
      kjson_obj_open(&req);
      json_voter_array(&req, q);
      kjson_obj_close(&req);
      kjson_close(&req);
  } else {
    // Unknown DB Error
    http_open(r, KHTTP_500);
  }
  db_voter_freeq(q);
}

static void
register_voter_common(struct kreq *r, enum regstatus status) {
  const char *lastname = "";
  const char *givennames = "";
  const char *resaddress = "";
  const char *resaddress2 = "";
  const char *reszip = "";
  const char *resstate = "";
  const char *mailaddress = "";
  const char *mailaddress2 = "";
  const char *mailzip = "";
  const char *mailstate = "";
  const char *party = "";
  const char *idinfo = "";
  time_t birthdate;
  size_t idinfo_sz;
  int64_t confidential = 0;
  size_t num_fields = 11;
  int error_count;
  struct field_error errors[num_fields];
  struct kjsonreq req;


  memset(errors, 0, sizeof(errors));
 
  error_count = 0;
  if (OK != get_str_param(r, VALID_VOTER_LASTNAME, &lastname)) {
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
  // Optional params
  get_int_param(r, VALID_VOTER_CONFIDENTIAL, &confidential);
  get_str_param(r, VALID_VOTER_RESADDRESS2, &resaddress2);
  get_str_param(r, VALID_VOTER_MAILADDRESS2, &mailaddress2);

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
                                         confidential,
                                         status,
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

static void
official_register_voter(struct kreq *r) {
  register_voter_common(r, REGSTATUS_ACTIVE);
}

static void
voter_register_page(struct kreq *r)
{
  register_voter_common(r, REGSTATUS_PENDINGREVIEW);
}

status_t
do_voter_updateinfo(struct kreq *r, int64_t voter_id)
{
  const char *lastname = "";
  const char *givennames = "";
  const char *resaddress = "";
  const char *resaddress2 = "";
  const char *reszip = "";
  const char *resstate = "";
  const char *mailaddress = "";
  const char *mailaddress2 = "";
  const char *mailzip = "";
  const char *mailstate = "";
  const char *party = "";
  const char *idinfo = "";
  time_t birthdate;
  size_t idinfo_sz;
  int64_t confidential = 0;
  status_t ret = ERROR;

  // optional params
  get_str_param(r, VALID_VOTER_RESADDRESS2, &resaddress2);
  get_str_param(r, VALID_VOTER_MAILADDRESS2, &mailaddress2);
  get_int_param(r, VALID_VOTER_CONFIDENTIAL, &confidential);

  if ( (OK == get_str_param(r, VALID_VOTER_LASTNAME,   &lastname)) &&
       (OK == get_str_param(r, VALID_VOTER_GIVENNAMES, &givennames)) &&
       (OK == get_str_param(r, VALID_VOTER_RESADDRESS, &resaddress)) &&
       (OK == get_str_param(r, VALID_VOTER_RESSTATE, &resstate)) &&
       (OK == get_str_param(r, VALID_VOTER_RESZIP, &reszip)) &&
       (OK == get_str_param(r, VALID_VOTER_MAILADDRESS, &mailaddress)) &&
       (OK == get_str_param(r, VALID_VOTER_MAILZIP, &mailzip)) &&
       (OK == get_str_param(r, VALID_VOTER_MAILSTATE, &mailstate)) &&
       (OK == get_int_param(r, VALID_VOTER_BIRTHDATE, &birthdate))  &&
       (OK == get_str_param(r, VALID_VOTER_REGISTEREDPARTY, &party))   &&
       (OK == get_blob_param(r, VALID_VOTER_IDINFO, &idinfo, &idinfo_sz))  ) {
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
                                   REGSTATUS_PENDINGREVIEW,
                                   confidential);
  }

  return ret;
}

static void
voter_update_info_page(struct kreq *r)
{
  int64_t voterid;
  status_t lookup;
  int64_t sid;
  char token[TOKEN_SIZE];

  // 1. Check cookie
  if ( (r->cookiemap[VALID_VOTERUPDATESESSION_ID] != NULL) &&
       (r->cookiemap[VALID_VOTERUPDATESESSION_TOKEN] != NULL) ) {
    sid   = r->cookiemap[VALID_VOTERUPDATESESSION_ID]->parsed.i;
    strcpy(token, r->cookiemap[VALID_VOTERUPDATESESSION_TOKEN]->parsed.s);
    DBG("Update Info SID: %ld, TOKEN: %s\n", sid, token);
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
 * Delete official session
 */
static void
official_logout_page(struct kreq *r)
{
  int64_t sid;
  char token[TOKEN_SIZE];
  if ( (r->cookiemap[VALID_ELECTIONOFFICIALSESSION_ID] == NULL) ||
       (r->cookiemap[VALID_ELECTIONOFFICIALSESSION_TOKEN] == NULL) ) {
    DBG("official_logout_page: Invalid Election Official Session.\n");
  } else {
    sid   = r->cookiemap[VALID_ELECTIONOFFICIALSESSION_ID]->parsed.i;
    strcpy(token, r->cookiemap[VALID_ELECTIONOFFICIALSESSION_TOKEN]->parsed.s);
    db_electionofficialsession_delete_officialsession(r->arg, sid, token);
  }
  khttp_head(r, kresps[KRESP_LOCATION], "%s", "/bvrs/index.html");
  http_open(r, KHTTP_302);
}

/*
 * Start a new session for election official.
 */
static void 
official_login_page(struct kreq *r)
{
  const char *username, *password;
  int64_t sid;
  char token[TOKEN_SIZE] = "";
  char buf[64];


  if( (OK == get_str_param(r, VALID_ELECTIONOFFICIAL_USERNAME, &username)) &&
      (OK == get_str_param(r, VALID_ELECTIONOFFICIAL_HASH, &password)) ) {

    status_t session_create = new_official_session(r->arg,
                                                 username,
                                                 password,
                                                 &sid,
                                                 token);

    if(OK == session_create) {
      /*  TODO: correct time */
      khttp_epoch2str(time(NULL) + 60*60, buf, sizeof(buf));
      khttp_head(r, kresps[KRESP_SET_COOKIE],
                "%s=%s; %s HttpOnly; path=/; expires=%s",
                valid_keys[VALID_ELECTIONOFFICIALSESSION_TOKEN].name, token, "", buf);
      khttp_head(r, kresps[KRESP_SET_COOKIE],
                "%s=%ld; %s HttpOnly; path=/; expires=%s",
                valid_keys[VALID_ELECTIONOFFICIALSESSION_ID].name, sid, "", buf);
      khttp_head(r, kresps[KRESP_LOCATION], "%s", "/bvrs/election_official_home.html");
      http_open(r, KHTTP_302);
    } else {
      khttp_head(r, kresps[KRESP_LOCATION], "%s", "/bvrs/election_official_login.html");
      http_open(r, KHTTP_302);
    }
  } else {
    http_open(r, KHTTP_401);
  }
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
  const char *lastname = "";
  const char *givennames = "";
  const char *resaddress = "";
  const char *resaddress2 = "";
  const char *reszip = "";
  const char *resstate = "";
  const char *mailaddress = "";
  const char *mailaddress2 = "";
  const char *mailzip = "";
  const char *mailstate = "";
  const char *idinfo = "";

  // optional params
  get_str_param(r, VALID_VOTER_RESADDRESS2, &resaddress2);
  get_str_param(r, VALID_VOTER_MAILADDRESS2, &mailaddress2);

  // required params
  if ( (OK == get_str_param(r, VALID_VOTER_LASTNAME,   &lastname)) &&
       (OK == get_str_param(r, VALID_VOTER_GIVENNAMES, &givennames)) &&
       (OK == get_str_param(r, VALID_VOTER_RESADDRESS, &resaddress)) &&
       (OK == get_str_param(r, VALID_VOTER_RESZIP, &reszip)) &&
       (OK == get_str_param(r, VALID_VOTER_RESSTATE, &resstate)) &&
       (OK == get_str_param(r, VALID_VOTER_MAILADDRESS, &mailaddress)) &&
       (OK == get_str_param(r, VALID_VOTER_MAILZIP, &mailzip)) &&
       (OK == get_str_param(r, VALID_VOTER_MAILSTATE, &mailstate)) &&
       (OK == get_int_param(r, VALID_VOTER_BIRTHDATE, &birthdate)) &&
       (OK == get_blob_param(r, VALID_VOTER_IDINFO, &idinfo, &idinfo_sz)) ) {
    int64_t sid;
    char token[TOKEN_SIZE];
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
                                                token);
    if (OK == session_create) {
      /*  TODO: correct time */
      khttp_epoch2str(time(NULL) + 60*5, buf, sizeof(buf));
      khttp_head(r, kresps[KRESP_SET_COOKIE],
                 "%s=%s; %s HttpOnly; path=/; expires=%s",
                 valid_keys[VALID_VOTERUPDATESESSION_TOKEN].name, token, "", buf);
      khttp_head(r, kresps[KRESP_SET_COOKIE],
                 "%s=%ld; %s HttpOnly; path=/; expires=%s",
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
    } else if (r.page >= PAGE__MAX /* @todo Add a MIME check? */) {
      http_open(&r, KHTTP_404);
      khttp_puts(&r, "Page not found.");
      khttp_free(&r);
      continue;
    }

    status_t openok = open_db(argv[1], &ctxt);
    if (openok != OK) {
      return EXIT_FAILURE;
    }
    database_name = argv[1];

    flush_old_sessions(ctxt, 5*60*60, 5*60*60); // 5 hour

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
      case PAGE_OFFICIAL_LOGIN:
        official_login_page(&r);
        break;
      case PAGE_OFFICIAL_LOGOUT:
        official_logout_page(&r);
        break;
      case PAGE_OFFICIAL_QUERY_VOTERS:
        require_official(official_query_voters, &r);
        break;
      case PAGE_OFFICIAL_UPDATE_VOTERS:
        require_official(official_update_voters, &r);
        break;
      case PAGE_OFFICIAL_REGISTER_VOTER:
        require_official(official_register_voter, &r);
        break;
      default:
        http_open(&r, KHTTP_404);
        break;
    }

    close_db(ctxt);
    khttp_free(&r);
  }
}
