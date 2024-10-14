#ifndef CURLSTUFF_H
#define CURLSTUFF_H

#include <string>
#include <future>

std::future<std::string> GetHtmlContent(const std::string& url);


#endif 