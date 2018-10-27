#pragma once
#include "../MileTest.h"
#include "json.hpp"


size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}


struct MIleTestTransaction: public MileTest  {

    std::string url =  "http://167.99.186.253:8080/v1/api";

    MIleTestTransaction(const std::string &name) : MileTest(name) {
        if (milecsa::keys::generate_with_secret(keyPair, "secret-phrase", errorDescription)) {
            BOOST_TEST_MESSAGE("Error happened in Pair");
        }
        else {
            print_key_pair(&keyPair);
        }

        if (milecsa::keys::generate_with_secret(destinationPair, "destination-secret-phrase", errorDescription)) {
            BOOST_TEST_MESSAGE("Error happened in destinationPair");
        }
        else {
            print_key_pair(&destinationPair);
        }
    }

    std::string get_block_id(){

        string block_id = "-1";

        string request = "{\"id\":0,\"jsonrpc\":\"2.0\",\"method\":\"get-current-block-id\",\"params\":null,\"version\":0.0}";

        CURL *curl;
        CURLcode res;
        ///
        /// Create curl object
        ///
        curl = curl_easy_init();


        if(curl) {

            cout << endl << "Response:" << endl;

            ///
            /// Setup curl request
            ///
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)request.length());

            std::string response_string;
            std::string header_string;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

            ///
            /// Perform the request, res will get the return code
            ///

            res = curl_easy_perform(curl);

            ///
            /// Check for errors
            ///
            if(res != CURLE_OK){
                cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;

                cerr << " ------ " << endl;
                cerr << " : " << header_string << endl;
                cerr << " : " << response_string << endl;
                cerr << " -------" << endl;

            }
            else {
                cerr << " : " << response_string << endl;

                nlohmann::json json = nlohmann::json::parse(response_string);

                nlohmann::json result = json.at("result");

                if (!result.empty()) {
                    block_id = result["current-block-id"];
                }
            }

            ///
            /// Always cleanup
            ///
            curl_easy_cleanup(curl);
        }

        return block_id;

    }

    int send(const string &body)
    {
        int ret = 0;
#ifdef WITH_CURL
        CURL *curl;
        CURLcode res;

        string request = "{\"id\":0,\"jsonrpc\":\"2.0\",\"method\":\"send-transaction\",\"version\":0.0, \"params\": ";
        request += body + "}";

        cout << "Send: " << endl;
        cout << "      " << request << endl;

        ///
        /// Create curl object
        ///
        curl = curl_easy_init();

        if(curl) {

            cout << endl << "Response:" << endl;

            ///
            /// Setup curl request
            ///
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)request.length());

            ///
            /// Perform the request, res will get the return code
            ///

            res = curl_easy_perform(curl);

            ///
            /// Check for errors
            ///
            if(res != CURLE_OK){
                ret = -1;
                cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            }

            ///
            /// Always cleanup
            ///
            curl_easy_cleanup(curl);
        }
#else
#warning "curl library has not been configured"
#endif
        return ret;
    }

    milecsa::light::Pair keyPair;
    milecsa::light::Pair destinationPair;
    const std::string nodeAddress = "2n9z7C3f9SdCrLuCkekxFRgaFq8eoJMizRzbJDpxGoXnYgTaoz";

};

