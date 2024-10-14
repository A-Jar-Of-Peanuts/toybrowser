#include <curl/curl.h>
#include <iostream>
#include <vector>
#include <string>
#include <future>

// Callback function for writing data received by libcurl
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to perform the GET request asynchronously and return HTML content as a future string
std::future<std::string> GetHtmlContent(const std::string& url) {
    return std::async(std::launch::async, [url]() {
        CURL *curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if(res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                return std::string("");  // Return an empty string on failure
            }
        }
        return readBuffer;  // Return the content on success
    });
}