#ifndef HTTP_INCLUDED
#define HTTP_INCLUDED

#include <string>

size_t write_callback_str(char* ptr, size_t size, size_t nmemb,
	std::string* userdata);
std::string http_get(std::string url);
std::string http_post(std::string url, std::string post_data);

#endif // HTTP_INCLUDED
