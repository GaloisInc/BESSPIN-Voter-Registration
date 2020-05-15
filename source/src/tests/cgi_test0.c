#include <stdlib.h>
#include <unistd.h>

#include <curl/curl.h>
#include <kcgiregress.h>

static int
child(void *arg)
{
  execl("bvrs", "bvrs", arg, NULL);
  _exit(EXIT_FAILURE);
}

static int
parent(void *fp)
{
  CURL *curl;
  long http;

  const char *req =
    "http://localhost:17123/voter_check_status.json?"
    "voter-lastname=%25&"
    "voter-givennames=%25&"
    "voter-birthdate=1986-02-04";

  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, req);
  if (CURLE_OK != curl_easy_perform(curl))
    return(0);
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http);
  curl_easy_cleanup(curl);
  curl_global_cleanup();
  return(200 == http);
}

int
main(int argc, char **argv)
{
  return(kcgi_regress_fcgi(parent, NULL, child, argv[1]) ?
    EXIT_SUCCESS : EXIT_FAILURE);
}
