//
// Created by lotus mile on 22/10/2018.
//

#include <cstring>
#include <string>
#include <string.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <milecsa_light_api.hpp>

#include "milecsa_light_api.hpp"
#include "crypto.h"
#include "crypto_types.h"
#include "json.hpp"

milecsa::light::result prepare_transaction(const std::string &name,
                                           const std::string &privateKey,
                                           const std::string &dstWalletPublicKey,

                                           const std::string &blockId,
                                           const uint64_t  transactionId,

                                           unsigned short assetCode,
                                           const std::string &amount,
                                           const std::string &description,
                                           const std::string &fee,
//
// Signed json
//
                                           std::string &transaction,
                                           std::string &digest,
                                           std::string &errorMessage) {

    static  time_t _trx_counter = 0;

    milecsa::light::result result;

    milecsa::light::Pair keyPair;

    if ( (result = milecsa::keys::generate_from_private_key(keyPair, privateKey, errorMessage)) != milecsa::light::result::OK) {
        return result;
    }

    if (keyPair.public_key == dstWalletPublicKey) {
        errorMessage = "destination address is equal the source";
        return milecsa::light::result::ALREADY_EXIST;
    }

    uint256_t bid;

    if (!StringToUInt256(blockId, bid, false)) {
        errorMessage = "block could not be converted to uint256_t";
        return milecsa::light::result::FAIL;
    }

    uint64_t trx_id;

    if (transactionId == milecsa::transaction::default_transaction_id) {
        /* initialize random seed: */
        srand (time(NULL) + _trx_counter++ );
        trx_id = (uint64_t) rand();
    }
    else {
        trx_id = transactionId;
    }

    PublicKey source;
    if(!source.SetBase58CheckString(keyPair.public_key, errorMessage)) {
        return milecsa::light::result::FAIL;
    }

    PrivateKey sourcePrivate;
    if(!sourcePrivate.SetBase58CheckString(keyPair.private_key, errorMessage)) {
        return milecsa::light::result::FAIL;
    }

    PublicKey destination;
    if(!destination.SetBase58CheckString(dstWalletPublicKey, errorMessage)) {
        return milecsa::light::result::FAIL;
    }

    Signer    signer(sourcePrivate, source);
    Signature signature;
    Digest    _digest;

    DigestCalculator calculator;

    calculator.Initialize();

    calculator.Update(trx_id);
    calculator.Update(bid);
    calculator.Update(assetCode);

    calculator.Update(source);
    calculator.Update(destination);
    calculator.Update(amount, amount.size());
    calculator.Update(description, description.size());

    calculator.Finalize(_digest);

    signer.SignDigest(_digest, signature);
    digest = _digest.ToBase58CheckString();

    nlohmann::json parameters;

    std::string _fee =  fee.empty() ? "0" : fee;

    parameters = {
            {"transaction-name", name},
            {"block-id",        blockId},
            {"transaction-id",  trx_id},
            {"digest",          digest},
            {"signature",       signature.ToBase58CheckString()},
            {"from",            keyPair.public_key},
            {"to",              dstWalletPublicKey},
            {"asset",           {{"amount", amount}, {"code", assetCode}}},
            {"fee",             _fee},
            {"description",     description}
    };

    transaction = parameters.dump();

    return  milecsa::light::result::OK;

}