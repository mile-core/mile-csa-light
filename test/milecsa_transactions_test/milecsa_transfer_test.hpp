//
// Created by lotus mile on 02/10/2018.
//

#ifndef MILECSA_MILECSA_TRANSFER_TEST_HPP
#define MILECSA_MILECSA_TRANSFER_TEST_HPP

#include "milecsa_light_api.hpp"
#include "MIleTestTransaction.hpp"
#include "json.hpp"

using json = nlohmann::json ;

struct Wallet: public MIleTestTransaction {

    Wallet():MIleTestTransaction("Wallet"){}


    const std::string destination = "2n9z7C3f9SdCrLuCkekxFRgaFq8eoJMizRzbJDpxGoXnYgTaoz";

    bool test(const milecsa::token &token) {

        std::string blockId = "0";

        std::string transaction;
        std::string digest;

        std::string memo = "memo";
        float fee = 0;

        if (milecsa::transaction::prepare_transfer(
                keyPair.private_key,
                destination,
                blockId,
                milecsa::transaction::default_transaction_id,
                token,
                1.0,
                fee,
                memo,

                transaction,
                digest,

                errorDescription)){
            BOOST_TEST_MESSAGE("Error happened in Pair");
            return false;
        }

        BOOST_TEST_MESSAGE("Wallet    trx: " + transaction);
        BOOST_TEST_MESSAGE("Wallet    fee: " + token.value_to_string(fee));
        BOOST_TEST_MESSAGE("Wallet digest: " + digest);

        return  true;

    }
};

#endif //MILECSA_MILECSA_TRANSFER_TEST_HPP
