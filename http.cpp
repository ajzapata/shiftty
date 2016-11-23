#include "http.h"

/* C++ Standard Library */

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cassert>

/* OS-specific Libraries */

#ifndef WIN32
#include <unistd.h>
#endif

/* CURL Libraries (from package "libcurl-devel") */

#include <curl/curl.h>

using namespace std;

std::string http_get(std::string url)
{
	/// CURL objects
	CURL* http_req;
	CURLcode curl_retcode;
	char* url_cstr = (char*)malloc(sizeof(char) * url.length());

	/// Need to convert (string)url to (char*)
	for (size_t i = 0; i < url.length(); i++) {
		url_cstr[i] = url[i];
	}

	#ifdef WIN32
	curl_global_init(CURL_GLOBAL_ALL); // Initializes winsock stuff
	#endif

	/// Initialize CURL object
	http_req = curl_easy_init();

	/// Check for errors (http_req null)
	if (http_req == nullptr) {
		cerr << "http.cpp::http_get error: CURL object returned NULL."
			<< endl;
		return "";
	}

	/// Allocate memory for http response
	/// Data will *not* terminate with NULL byte
	FILE* http_response = fopen(".http_response", "w");

	/// Load URL to CURL object
	curl_easy_setopt(http_req, CURLOPT_URL, url_cstr);
	curl_easy_setopt(http_req, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_setopt(http_req, CURLOPT_WRITEDATA, http_response);

	/// Snippet from libcurl example "https_get"
	#ifdef SKIP_PEER_VERIFICATION
	/*
	 * If you want to connect to a site who isn't using a certificate that is
	 * signed by one of the certs in the CA bundle you have, you can skip the
	 * verification of the server's certificate. This makes the connection
	 * A LOT LESS SECURE.
	 *
	 * If you have a CA cert for the server stored someplace else than in the
	 * default bundle, then the CURLOPT_CAPATH option might come handy for
	 * you.
	 */
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	#endif

	#ifdef SKIP_HOSTNAME_VERIFICATION
	/*
	 * If the site you're connecting to uses a different host name that what
	 * they have mentioned in their server certificate's commonName (or
	 * subjectAltName) fields, libcurl will refuse to connect. You can skip
	 * this check, but this will make the connection less secure.
	 */
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	#endif

	/// Execute request
	curl_retcode = curl_easy_perform(http_req);

	/// Check for errors with the request
	if (curl_retcode != CURLE_OK) {
		cerr << "http.cpp::http_get HTTP Request failed." << endl
			<< "> " << curl_easy_strerror(curl_retcode) << endl;
		return "";
	}

	fclose(http_response);

	/// Copy data to string
	http_response = fopen(".http_response", "r");
	string http_response_str = "";
	char c = ' ';

	while (c != EOF) {
		c = fgetc(http_response);
		http_response_str += c;
	}

	/* Cleanup */
	fclose(http_response);
	free(url_cstr);
	curl_easy_cleanup(http_req);
	curl_global_cleanup();

	return http_response_str;
}


std::string http_post(std::string url, std::string post_data)
{
	/// CURL objects
	CURL* http_req;
	CURLcode curl_retcode;
	char* url_cstr = (char*)malloc(sizeof(char) * url.length());
	char* post_data_cstr = (char*)malloc(sizeof(char) * post_data.length());

	/// Need to convert (string)url to (char*)
	for (size_t i = 0; i < url.length(); i++) {
		url_cstr[i] = url[i];
	}

	/// Need to convert (string)post_data to (char*)
	for (size_t i = 0; i < post_data.length(); i++) {
		post_data_cstr[i] = post_data[i];
	}

	#ifdef WIN32
	curl_global_init(CURL_GLOBAL_ALL); // Initializes winsock stuff
	#endif

	/// Initialize CURL object
	http_req = curl_easy_init();

	/// Check for errors (http_req null)
	if (http_req == nullptr) {
		cerr << "http.cpp::http_get error: CURL object returned NULL."
			<< endl;
		return "";
	}

	/// Allocate memory for http response
	/// Data will *not* terminate with NULL byte
	FILE* http_response = fopen(".http_response", "w");

	/// Load URL to CURL object
	curl_easy_setopt(http_req, CURLOPT_URL, url_cstr);
	curl_easy_setopt(http_req, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_setopt(http_req, CURLOPT_WRITEDATA, http_response);
	curl_easy_setopt(http_req, CURLOPT_POSTFIELDS, post_data_cstr);

	/// Snippets from libcurl example "https_get"
	#ifdef SKIP_PEER_VERIFICATION
	/*
	 * If you want to connect to a site who isn't using a certificate that is
	 * signed by one of the certs in the CA bundle you have, you can skip the
	 * verification of the server's certificate. This makes the connection
	 * A LOT LESS SECURE.
	 *
	 * If you have a CA cert for the server stored someplace else than in the
	 * default bundle, then the CURLOPT_CAPATH option might come handy for
	 * you.
	 */
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	#endif

	#ifdef SKIP_HOSTNAME_VERIFICATION
	/*
	 * If the site you're connecting to uses a different host name that what
	 * they have mentioned in their server certificate's commonName (or
	 * subjectAltName) fields, libcurl will refuse to connect. You can skip
	 * this check, but this will make the connection less secure.
	 */
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	#endif

	/// Execute request
	curl_retcode = curl_easy_perform(http_req);

	/// Check for errors with the request
	if (curl_retcode != CURLE_OK) {
		cerr << "http.cpp::http_get HTTP Request failed." << endl
			<< "> " << curl_easy_strerror(curl_retcode) << endl;
		return "";
	}

	fclose(http_response);

	/// Copy data to string
	http_response = fopen(".http_response", "r");
	string http_response_str = "";
	char c = ' ';

	while ((c = fgetc(http_response)) != EOF)
		http_response_str += c;

	/* Cleanup */
	fclose(http_response);
	free(url_cstr);
	curl_easy_cleanup(http_req);
	curl_global_cleanup();

	return http_response_str;
}
