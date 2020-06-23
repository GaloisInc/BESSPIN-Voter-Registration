#include <stdlib.h>
#include <unistd.h>
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
  CURL *curl;
  long http;

  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_COOKIE, "electionofficialsession-id=1;electionofficialsession-token=ABCDEFGH12345678ABCDEFGH12345678;");
  curl_easy_setopt(curl, CURLOPT_URL, req);
  if (CURLE_OK != curl_easy_perform(curl))
    return(0);
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http);
  printf("return code: %ld\n", code);
  curl_easy_cleanup(curl);
  curl_global_cleanup();
  assert(code == http);

  return 1;
}

static int
parent(void *fp)
{
  do_test("http://localhost:17123/official_query_voters.json?"
          "field-name=lastname&field-contains=lastname",
          200);

  printf("\n[cgi9] Test Passed. - Valid Official Session\n");
  return 1;
}

int
main(int argc, char **argv)
{
  return(kcgi_regress_fcgi(parent, NULL, child, argv[1]) ?
    EXIT_SUCCESS : EXIT_FAILURE);
}
