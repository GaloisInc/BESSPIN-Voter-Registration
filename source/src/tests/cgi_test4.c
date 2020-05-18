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
parent(void *fp)
{
  CURL *curl;
  long http;
  char *postdata =
      "voter-birthdate=1986-02-04&"
      "voter-lastname=Schmidt&"
      "voter-givennames=Leslie&"
      "voter-resaddress=1234 Main St.&"
      "voter-mailaddress=1234 Secondary St.&"
      "voter-registeredparty=NPP&"
      "voter-idinfo=SECRETENCODING";

  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL,"http://localhost:17123/voter_register.json");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(postdata));
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);

  if (CURLE_OK != curl_easy_perform(curl))
    return(0);
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http);
  assert(http == 200);

  printf("Test Passed.\n");
  return 1;
}

int
main(int argc, char **argv)
{
  return(kcgi_regress_fcgi(parent, NULL, child, argv[1]) ?
    EXIT_SUCCESS : EXIT_FAILURE);
}
