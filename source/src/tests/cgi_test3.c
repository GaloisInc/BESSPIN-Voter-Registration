#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <curl/curl.h>
#include <kcgiregress.h>

static int
child(void *arg)
{
  execl("bvrs", "bvrs", arg, NULL);
  _exit(EXIT_FAILURE);
}

static int
do_test(const char *req, long code)
{
}

static int
parent(void *fp)
{
  CURL *curl;
  long http;
  char *postdata =
                   "voter-lastname=lastname&"
                   "voter-givennames=firstname&"
                   "voter-birthdate=1986-02-04&"
                   "voter-resaddress=1234 address&"
                   "voter-mailaddress=1234 address&"
                   "voter-idinfo=blob1";

  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL,"http://localhost:17123/voter_update_login.json");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(postdata));
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
  curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");

  if (CURLE_OK != curl_easy_perform(curl))
    return(0);
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http);
  struct curl_slist *cookies;
  curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
  struct curl_slist *each = cookies;
  int ok = 0;
  while(each) {
    each = each->next;
    ok++;
  }
  curl_easy_cleanup(curl);
  curl_global_cleanup();
  // We should have gotten a session-id and a session-token
  assert(ok == 2);

  printf("Test Passed.\n");
  return 1;
}

int
main(int argc, char **argv)
{
  return(kcgi_regress_fcgi(parent, NULL, child, argv[1]) ?
    EXIT_SUCCESS : EXIT_FAILURE);
}
