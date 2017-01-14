#include "http.h"

/* C++ Standard Library */

#include <cstdlib>
#include <cinttypes>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cassert>

/* OS-specific Libraries */

#ifdef __linux__
#include <unistd.h>
#include <curl/curl.h> /// pkg "libcurl-devel" (Fedora Linux 24)
#else /// Microsoft Windows (x64)
#include <winsock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <sstream>
#pragma comment(lib, "ws2_32.lib")
#endif
/* Project Libraries */

#include "logger.h"

using namespace std;

#ifdef __linux__
std::string http_get(std::string url)
{
	/// CURL objects
	CURL* http_req;
	CURLcode curl_retcode;
	//double http_code;

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
	curl_easy_setopt(http_req, CURLOPT_URL, url.c_str());
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

	/// Get HTTP response code
	//curl_easy_getinfo(http_req, CURLINFO_RESPONSE_CODE, &http_code);

	/// Only log HTTP code if not zero
	//if (http_code != 0)
	//	slog("HTTP GET " + url + "; RETURN CODE " + to_string((int)http_code));
	/// (^ non-functional; http_code never becomes defined)

	/// Check for errors with the request
	if (curl_retcode != CURLE_OK) {
		slog("http.cpp::http_get HTTP Request failed. " +
			string(curl_easy_strerror(curl_retcode)), MessageType::error);
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
	curl_easy_cleanup(http_req);
	curl_global_cleanup();

	return http_response_str;
}
#else /// Windows
std::string http_get(std::string url)
{
	/// Snippet mostly from http://stackoverflow.com/q/1011339
	WSADATA wsa_data;
	SOCKET sock;
	SOCKADDR_IN sai;
	string http_request;
	string http_response;

	http_request =
		"GET /index.html HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";

	/// WSA Failure
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		cerr << "WSAStartup failed" << endl;
		exit(1); // fatal
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// tmp
	ADDRINFO* ll_addrinfo;
	int retval_ai = getaddrinfo(url.c_str(), "HTTP", NULL, &ll_addrinfo);
	if (retval_ai) /// Error
	{
		cerr << "ERR: getaddrinfo returned " << retval_ai << endl;
		exit(4);
	}

	/// Socket stuff (IPv4/IPv6)
	while (ll_addrinfo != NULL)
	{
		if (ll_addrinfo->ai_addr->sa_family == AF_INET) {
			//sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			sai = *(SOCKADDR_IN*)ll_addrinfo->ai_addr;
			break;
		}
		else if (ll_addrinfo->ai_addr->sa_family == AF_INET6) {
			//sai = *(SOCKADDR_IN*)ll_addrinfo->ai_addr;
			//break;
		}
		ll_addrinfo = ll_addrinfo->ai_next;
	}

	/// Establish connection
	if (connect(sock, (SOCKADDR*)(&sai), sizeof(sai)) != 0)
	{
		cerr << "Could not connect" << endl;
		exit(2); // fatal
	}

	/// Execute HTTP GET
	send(sock, http_request.c_str(), (int)strlen(http_request.c_str()), 0);

	/// Read HTML data received
	int data_length;
	char buffer[1024];
	while ((data_length = recv(sock, buffer, 1024, 0)) > 0)
	{
		int i = 0;
		while ((buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') && i < data_length)
			http_response += buffer[i++];
	}

	/// Clean-up
	closesocket(sock);
	WSACleanup();

	cerr << "##### HTTP GET REQUEST OUT #####" << endl;
	cerr << http_request << endl;
	cerr << "##### HTTP GET RESPONSE IN #####" << endl;
	cerr << http_response << endl;
	return http_response;
}
#endif

std::string http_post(std::string url, std::string post_data)
{
#ifdef __linux__
	/// CURL objects
	CURL* http_req;
	CURLcode curl_retcode;
	//double http_code;

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
	curl_easy_setopt(http_req, CURLOPT_URL, url.c_str());
	curl_easy_setopt(http_req, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_setopt(http_req, CURLOPT_WRITEDATA, http_response);
	curl_easy_setopt(http_req, CURLOPT_POSTFIELDS, post_data.c_str());

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

	/// Get HTTP response code
	//curl_easy_getinfo(http_req, CURLINFO_RESPONSE_CODE, &http_code);

	/// Only log HTTP code if not zero
	//if (http_code != 0)
	//	slog("HTTP GET " + url + "; RETURN CODE " + to_string((int)http_code));

	/// Check for errors with the request
	if (curl_retcode != CURLE_OK) {
		slog("http.cpp::http_get HTTP Request failed. " +
			string(curl_easy_strerror(curl_retcode)), MessageType::error);
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
	curl_easy_cleanup(http_req);
	curl_global_cleanup();

	return http_response_str;
#else // Windows
	return "";
#endif
}
