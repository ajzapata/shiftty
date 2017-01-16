#include "http.h"

#include <cstdlib>
#include <cinttypes>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cassert>
#include <curl/curl.h> /// pkg "libcurl-devel" (Fedora Linux 24)

#if !defined _WIN32 && !defined _WIN64
#include <unistd.h>
#else
/// Microsoft Windows (x64)
/// Note that "#pragma comment" will only work in Visual Studio; otherwise,
/// the library will have to be manually linked in the IDE/compiler options.
/// Must download and compile curl and list its include and lib directories
/// in the project/compiler settings and copy the dll to the project
/// directory. See http://stackoverflow.com/q/30288415
#ifdef _DEBUG
#pragma comment(lib, "libcurl_debug.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif // _DEBUG
#endif // !defined _WIN32 && !defined _WIN64

using namespace std;

/// This function is called by libcurl to save downloaded data as a string
/// rather than the default (to a file using FILE*)
size_t write_callback_str(char* ptr, size_t size, size_t nmemb,
	std::string* userdata)
{
	/// Downloaded data is accessed as a character array of size
	/// (size * nmemb), having a maximum size of CURL_MAX_WRITE_SIZE; the
	/// data is NOT null-terminated. The characters are saved to the
	/// std::string object pointed-to by userdata and the function returns the
	/// new size of the string.

	/// Check if userdata already points to a string
	if (userdata == nullptr)
		userdata = new string;

	/// Copy data to string
	for (size_t i = 0; i < (size * nmemb); i++)
		*userdata += ptr[i];

	/// If end of string isn't null-terminated, append it
	//if (userdata->at(userdata->length() - 1) != '\0') *userdata += '\0';

	/// Cheating, but otherwise we get CURLE_WRITE_ERROR too frequently...
	return (size * nmemb);
}

std::string http_get(std::string url)
{
	/// CURL objects
	CURL* http_request;
	CURLcode curl_retcode;
	string http_response;
	//double http_code;

	/// Windows: Initializes winsock stuff
	curl_global_init(CURL_GLOBAL_ALL);

	/// Initialize CURL object
	http_request = curl_easy_init();

	/// Check for errors (http_request null)
	if (http_request == nullptr) {
		cerr << "http.cpp::http_get error: CURL object returned NULL."
			<< endl;
		return "";
	}

	/// Load URL to CURL object
	curl_easy_setopt(http_request, CURLOPT_URL, url.c_str());
	curl_easy_setopt(http_request, CURLOPT_WRITEFUNCTION, write_callback_str);
	curl_easy_setopt(http_request, CURLOPT_WRITEDATA, &http_response);

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
	curl_retcode = curl_easy_perform(http_request);

	/// Get HTTP response code
	//curl_easy_getinfo(http_request, CURLINFO_RESPONSE_CODE, &http_code);

	/// Only log HTTP code if not zero
	//if (http_code != 0)
	//	cerr << "HTTP GET " << url << "; RETURN CODE " << http_code << endl;
	/// (^ non-functional; http_code never becomes defined)

	/// Check for errors with the request
	if (curl_retcode != CURLE_OK) {
		cerr << "http.cpp::http_get HTTP Request failed. " <<
			curl_easy_strerror(curl_retcode) << endl;
		return "";
	}

	/* Cleanup */
	curl_easy_cleanup(http_request);
	curl_global_cleanup();

	return http_response;
}

std::string http_post(std::string url, std::string post_data)
{
	/// CURL objects
	CURL* http_request;
	CURLcode curl_retcode;
	string http_response;
	//double http_code;

	/// Windows: Initializes winsock stuff
	curl_global_init(CURL_GLOBAL_ALL);

	/// Initialize CURL object
	http_request = curl_easy_init();

	/// Check for errors (http_request null)
	if (http_request == nullptr) {
		cerr << "http.cpp::http_get error: CURL object returned NULL."
			<< endl;
		return "";
	}

	/// URL-encode HTTP POST data
	//char* post_data_urlenc =
	//	curl_easy_escape(http_request, post_data.c_str(),
	//	strlen(post_data.c_str()));

	/// If post_data is enclosed with brackets, assume that it is JSON data.
	/// This is required since the ShapeShift API won't correctly parse the
	/// post data if the data isn't declared as type "application/json"
	bool isJSON = !post_data.empty() &&
		post_data.at(0) == '{' && post_data.at(post_data.length() - 1) == '}';
	if (isJSON)
	{
		/// Make a custom HTTP header with the appropriate information
		curl_slist*header = NULL;
		header = curl_slist_append(header, "Accept: application/json");
		header = curl_slist_append(header, "Content-Type: application/json");
		curl_easy_setopt(http_request, CURLOPT_HTTPHEADER, header);
	}

	/// Load URL to CURL object
	curl_easy_setopt(http_request, CURLOPT_URL, url.c_str());
	curl_easy_setopt(http_request, CURLOPT_WRITEFUNCTION, write_callback_str);
	curl_easy_setopt(http_request, CURLOPT_WRITEDATA, &http_response);
	curl_easy_setopt(http_request, CURLOPT_POSTFIELDS, post_data.c_str());

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
	curl_retcode = curl_easy_perform(http_request);

	/// Get HTTP response code
	//curl_easy_getinfo(http_request, CURLINFO_RESPONSE_CODE, &http_code);

	/// Only log HTTP code if not zero
	//if (http_code != 0)
	//	cerr << "HTTP POST " << url << "; RETURN CODE " << http_code << endl;

	/// Check for errors with the request
	if (curl_retcode != CURLE_OK) {
		cerr << "http.cpp::http_post HTTP Request failed. " <<
			curl_easy_strerror(curl_retcode) << endl;
		return "";
	}

	/* Cleanup */
	curl_easy_cleanup(http_request);
	curl_global_cleanup();

	return http_response;
}
