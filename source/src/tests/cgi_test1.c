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
  curl_easy_setopt(curl, CURLOPT_URL, req);
  if (CURLE_OK != curl_easy_perform(curl))
    return(0);
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http);
  curl_easy_cleanup(curl);
  curl_global_cleanup();
  assert(code == http);

  return 1;
}

static int
parent(void *fp)
{

  do_test("http://localhost:17123/voter_check_status.json?"
                 "voter-givennames=%25&"
                 "voter-birthdate=1986-02-04",
                 400);

  return 1;
}

int
main(int argc, char **argv)
{
  return(kcgi_regress_fcgi(parent, NULL, child, argv[1]) ?
    EXIT_SUCCESS : EXIT_FAILURE);
}
