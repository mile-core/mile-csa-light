//
// Created by lotus mile on 19/10/2018.
//

#include <string>
#include <iostream>
#include <milecsa_light_api.hpp>

using milecsa::light::Pair;
using  result = milecsa::light::result;
using namespace milecsa::transaction;
using namespace milecsa::keys;
using namespace std;

int main(int argc, char *argv[]) {

    //
    // block id should be getting from blockchain node, for example as json-rpc request by curl
    // curl  -H "Content-Type: application/json"  -d '{"id":0,"jsonrpc":"2.0","method":"get-current-block-id","version":0.0}' http://node002.testnet.mile.global/v1/api
    //
    // or any another way
    //
    //
    string block_id = "34923";

    //
    // trx id can be create for user reason or can be default
    //
    uint64_t trx_id = default_transaction_id;

    Pair    pair;
    string  errorMessage;

    if ( generate(pair,errorMessage) != result::OK ) {
        cerr << " generate wallet keys error: " << errorMessage << endl;
        return -1;
    }

    cout << " Wallet: " << endl;
    cout << "         private key = " << pair.private_key << endl;
    cout << "          public key = " << pair.public_key << endl;

    Pair    destination;

    if ( generate_with_secret(destination, "some secret phase!", errorMessage) != result::OK ) {
        cerr << " generate wallet keys from private error: " << errorMessage << endl;
        return -1;
    }

    cout << " Wallet with secret: " << endl;
    cout << "         private key = " << destination.private_key << endl;
    cout << "          public key = " << destination.public_key << endl;

    std::string transaction;
    std::string fee;
    std::string digest;

    //
    // Build signed transfer transaction
    //
    if (milecsa::transaction::prepare_transfer(
            pair.private_key,          // "from" private key
            destination.public_key,    // "to" public key
            block_id,                  // block id
            trx_id,                    // user defined transaction id or number
            milecsa::assets::XDR,      // asset
            1.0f,                      // amount of transfer
            0,                         // empty fee
            "memo",                    // description

            transaction,               // returned signed transaction as json string
            digest,                    // uinq transaction digest string

            errorMessage               // error message if something failed
            )){

        cerr << " prepare_transfer error: " << errorMessage << endl;
        exit(-1);
    }

    cout << " Signed transactions json digest : " << digest << endl;
    cout << " Signed transactions json body   : " << endl;
    cout << transaction << endl;

    return  0;
}