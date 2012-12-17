#ifndef COSMCOMMUNICATOR_H_
#define COSMCOMMUNICATOR_H_

#include <iostream>
#include <curl/curl.h>

using namespace std;

static int SendCosmNotification(string apikey, string feedId, string datastreamId, string value)
{
	  CURL *curl;
	  CURLcode res;

	    curl = curl_easy_init();
	    if(curl) {

		 struct curl_slist *headers=NULL;

		 headers = curl_slist_append(headers, ("X-ApiKey: " + apikey).c_str());

		 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ("{ \"current_value\":\"" + value + "\"}").c_str());
		 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		 curl_easy_setopt(curl, CURLOPT_URL, ("api.cosm.com/v2/feeds/" + feedId + "/datastreams/" + datastreamId + "?_method=put").c_str());

	      /* Perform the request, res will get the return code */
	      res = curl_easy_perform(curl);
	      /* Check for errors */
	      if(res != CURLE_OK)
	        fprintf(stderr, "curl_easy_perform() failed: %s\n",
	                curl_easy_strerror(res));

	      /* always cleanup */
	      curl_easy_cleanup(curl);

	      /* free slist */
	      curl_slist_free_all (headers);
	    }
	    return 0;
}


#endif /* COSMCOMMUNICATOR_H_ */
