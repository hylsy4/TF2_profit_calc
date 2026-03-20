#include <iostream>
#include <curl/curl.h>
#include <string>
#include <fstream>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {

    std::fstream key_file("key");
    std::string api_key;
    getline(key_file, api_key);
    api_key.erase(api_key.find_first_not_of("\n") + 1);
    std::string request_url = "https://backpack.tf/api/IGetPrices/v4?key=" + api_key;
    std::cout << request_url << std::endl;
    std::string price_data_buffer;
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, request_url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &price_data_buffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);

        std::cout << price_data_buffer << std::endl;
    }



    return 0;
}