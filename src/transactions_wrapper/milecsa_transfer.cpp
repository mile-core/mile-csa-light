//
// Created by lotus mile on 22/10/2018.
//

#include <cstring>
#include <string>
#include <string.h>
#include <boost/multiprecision/cpp_int.hpp>

#include "milecsa_light_api.hpp"
#include "mile_crypto.h"
#include "json.hpp"

extern milecsa::light::result prepare_parameters(
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
        std::string &errorMessage);

milecsa::light::result milecsa::transaction::prepare_transfer(const std::string &privateKey,
                                                              const std::string &dstWalletPublicKey,

                                                              const std::string &blockId,
                                                              const uint64_t transactionId,

                                                              const milecsa::token  &asset,
                                                              float                 amount,
                                                              float                 fee,
                                                              const std::string     &description,

                                                              std::string &transaction,
                                                              std::string &digest,
                                                              std::string &errorMessage) {

    milecsa::light::result result;
    milecsa::light::Pair keyPair;

    uint256_t bid;
    uint64_t trx_id = transactionId;
    PublicKey source;
    PrivateKey sourcePrivate;
    PublicKey destination;


    result = prepare_parameters(
            privateKey,
            dstWalletPublicKey,
            blockId,
            trx_id,
            asset,
            keyPair,
            bid,
            source,
            sourcePrivate,
            destination,
            errorMessage);

    if (result != milecsa::light::result::OK){
        return result;
    }

    Signer    signer(sourcePrivate, source);
    Signature signature;
    Digest    _digest;

    std::string amount_string = asset.value_to_string(amount);

    DigestCalculator calculator;

    calculator.Initialize();

    calculator.Update(trx_id);
    calculator.Update(bid);

    calculator.Update(asset.code);

    calculator.Update(source);
    calculator.Update(destination);
    calculator.Update(amount_string, amount_string.size());
    calculator.Update(description, description.size());

    calculator.Finalize(_digest);

    signer.SignDigest(_digest, signature);
    digest = _digest.ToBase58CheckString();

    nlohmann::json parameters;

    std::string fee_string = asset.value_to_string(fee);

    parameters = {
            {"transaction-name", "TransferAssetsTransaction"},
            {"block-id",        blockId},
            {"transaction-id",  trx_id},
            {"digest",          digest},
            {"signature",       signature.ToBase58CheckString()},
            {"from",            keyPair.public_key},
            {"to",              dstWalletPublicKey},
            {"asset",           {{"amount", amount_string}, {"code", asset.code}}},
            {"fee",             fee_string},
            {"description",     description}
    };

    transaction = parameters.dump();

    return  milecsa::light::result::OK;
}
