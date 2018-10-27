//
// Created by lotus mile on 22/10/2018.
//

#include <cstring>
#include <string>
#include <string.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <milecsa_light_api.hpp>

#include "mile_crypto.h"
#include "json.hpp"

milecsa::light::result prepare_parameters(
        const std::string &privateKey,
        const std::string &dstWalletPublicKey,

        const std::string &blockId,
        uint64_t  &transactionId,

        const milecsa::token  &asset,

        milecsa::light::Pair &keyPair,
        uint256_t &bid,
        PublicKey &source,
        PrivateKey &sourcePrivate,
        PublicKey &destination,
        std::string &errorMessage){

    static  time_t _trx_counter = 0;

    milecsa::light::result result;

    if ( (result = milecsa::keys::generate_from_private_key(keyPair, privateKey, errorMessage)) != milecsa::light::result::OK) {
        return result;
    }

    if (keyPair.public_key == dstWalletPublicKey) {
        errorMessage = "destination address is equal the source";
        return milecsa::light::result::ALREADY_EXIST;
    }

    if (!StringToUInt256(blockId, bid, false)) {
        errorMessage = "block could not be converted to uint256_t";
        return milecsa::light::result::FAIL;
    }

    uint64_t trx_id = transactionId;

    if (transactionId == milecsa::transaction::default_transaction_id) {
        /* initialize random seed: */
        srand (time(NULL) + _trx_counter++ );
        trx_id = (uint64_t) rand();
    }
    else {
        trx_id = transactionId;
    }

    if(!source.SetBase58CheckString(keyPair.public_key, errorMessage)) {
        return milecsa::light::result::FAIL;
    }

    if(!sourcePrivate.SetBase58CheckString(keyPair.private_key, errorMessage)) {
        return milecsa::light::result::FAIL;
    }

    if(!destination.SetBase58CheckString(dstWalletPublicKey, errorMessage)) {
        return milecsa::light::result::FAIL;
    }

    transactionId = trx_id;

    return  milecsa::light::result::OK;

}
