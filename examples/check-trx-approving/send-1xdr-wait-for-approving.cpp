//
// Created by lotus mile on 17/11/2018.
//
// Example shows how to prepare MILE transaction, then send to node use MILE JSON-RPC API
// and waiting for transaction approving by blockchain

//
// Alternate C++ wrappper for json-rpc API: https://github.com/mile-core/mile-csa-jsonrpc-client
//

#include <string>
#include <iostream>
#include <milecsa_light_api.hpp>
#include <time.h>
#include <unistd.h>

#include "json.hpp"

#ifdef WITH_CURL
#include <curl/curl.h>
#else
#error "curl library has not been configured"
#endif

#define __DEBUG_RPC__ 0

using milecsa::light::Pair;
using result = milecsa::light::result;
using namespace milecsa::transaction;
using namespace milecsa::keys;
using namespace std;
using namespace nlohmann;

string url = "https://lotus000.testnet.mile.global/v1/api";

uint64_t       get_block_id();
nlohmann::json get_block(uint64_t block_id);
int            send_transaction(const string &body);

nlohmann::json request(string method, const json &params);

int main(int argc, char *argv[]) {

    if (argc != 3) {
        cerr << "Usage to send 1 XDR from -> to" << endl;
        cerr << argv[0] << " <from address as secret phrase> <to>" << endl;
        return -1;
    }

    string from = argv[1];
    string to = argv[2];
    string errorMessage;

    milecsa::light::Pair pair;

    ///
    /// Restore wallet private and public keys pair from secret phrase
    ///
    if (generate_with_secret(pair, from, errorMessage) != result::OK) {
        cerr << " from secret phrase error: " << errorMessage << endl;
        return  -1;
    }

    ///
    /// Validate destination address
    ///
    if (validate_public_key(to, errorMessage) != result::OK) {
        cerr << " destination public key error: " << errorMessage << endl;
        return  -1;
    }

    uint64_t block_id = (uint64_t)-1;

    ///
    /// Try getting block-id 3 times
    ///
    int repeats = 0;
    while (block_id == (uint64_t)-1) {
        block_id = get_block_id();
        if (repeats >= 3) {
            cerr << "Node: " << url << " is not available..."<< endl;
            exit(EXIT_FAILURE);
        }
        repeats ++;
    }

    ///
    /// trx id can be create for user reason or can be default
    ///
    uint64_t trx_id = default_transaction_id;

    ///
    /// Transaction container string
    ///
    std::string transaction;

    ///
    /// Transaction container uniq digest
    ///
    std::string digest;

    ///
    /// Asset code
    ///
    milecsa::token token = milecsa::assets::XDR;

    ///
    /// Prepare fixed point presentation asset amount
    ///
    float amount = 0.01f;

    ///
    /// Build signed transfer transaction
    ///
    if (milecsa::transaction::prepare_transfer(
            pair.private_key,          /// "from" private key
            to,                        /// "to" public key
            to_string(block_id),       /// block id
            trx_id,                    /// user defined transaction id or number
            token,                     /// asset
            amount,                    /// amount of transfer

            0.0,                       /// fee is always 0
            "0xtest:approving",        /// description

            transaction,               /// returned signed transaction as json string
            digest,                    /// uinq transaction digest string

            errorMessage               /// error message if something failed
    )){

        cerr << " prepare_transfer error: " << errorMessage << endl;
        return -1;
    }

    bool is_appoved = false;

    for (int repeats = 0; repeats < 10 && !is_appoved;) {

        cout << endl << " Sending transaction: " << digest << " repeat: "<< repeats << endl;

        ///
        /// Send transfer the first time or resend next 10 blocks
        ///

        send_transaction(transaction);

        ///
        /// Try the next 3 blocks
        ///

        for (uint64_t i = 0; i < 3; ++i, ++repeats) {

            ///
            /// Waiting the next block
            ///

            cout << " Waiting for transaction: " << digest << endl;

            sleep(20);

            uint64_t next_block_id = block_id + repeats;

            json transactions = get_block(next_block_id);

            if (transactions.is_array()) {

                cout << " Block: " << next_block_id << " transactions: " << endl;

                for (json::iterator it = transactions.begin(); it != transactions.end(); ++it) {

                    cout << *it << '\n';

                    if (digest == it->at("digest").get<string>()) {
                        //
                        // approved
                        //
                        cout << " Transaction: " << digest << " has been approved in :" << next_block_id << " block"<< endl;
                        is_appoved = true;
                    }
                }
            }

            if (is_appoved)
                break;
            else {
                cout << " Transaction: " << digest << " has not been found in :" << next_block_id << " block"<< endl;
            }
        }
    }

    exit(EXIT_SUCCESS);
}

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

json get_block(uint64_t block_id){

    string id = to_string(block_id);

    json response = request("get-block-by-id", {{"id",id}});

    if (!response.empty() && response.count("block-data")) {
        return response.at("block-data").at("transactions");
    }

    return {};
}

uint64_t get_block_id(){

    uint64_t block_id = (uint64_t)-1;

    json response = request("get-current-block-id", {});

    if (!response.empty()) {
        block_id = stoull(response["current-block-id"].get<string>());
    }

    return block_id;
}

int send_transaction(const string &body)
{
    int ret = -1;

    json params = json::parse(body);

    json response = request("send-transaction", params);

    if (!response.empty()) {
        ret = 0;
    }

    return ret;
}

json request(string method, const json &params) {
    int ret = 0;

    CURL *curl;
    CURLcode res;

    json request = {
            {"jsonrpc","2.0"},
            {"id","0"},
            {"method",method},
            {"params",params}
    };

    string request_string = request.dump();

#if __DEBUG_RPC__
    cout << "Send: " << endl;
    cout << "      " << request << endl;
#endif

    ///
    /// Create curl object
    ///
    curl = curl_easy_init();

    if(curl) {

#if __DEBUG_RPC__
        cout << endl << "Response:" << endl;
#endif

        ///
        /// Setup curl request
        ///
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_string.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)request_string.length());

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
            ret = -1;
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }
        else {
#if __DEBUG_RPC__
            cout << " : " << response_string << endl;
#endif
        }

        ///
        /// Always cleanup
        ///
        curl_easy_cleanup(curl);

        return json::parse(response_string).at("result");
    }

    return {};
}
