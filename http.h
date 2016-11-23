#ifndef HTTP_INCLUDED
#define HTTP_INCLUDED

#include <string>

std::string http_get(std::string url);
std::string http_post(std::string url, std::string post_data);

#endif // HTTP_INCLUDED
