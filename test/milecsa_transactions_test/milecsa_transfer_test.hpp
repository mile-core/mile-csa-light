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

    bool test() {

        std::string blockId = "0";

        std::string transaction;
        std::string digest;

        std::string memo = "memo";
        std::string fee = "";

        if (milecsa::transaction::prepare_transfer(
                keyPair.private_key,
                destination,
                blockId,
                milecsa::transaction::default_transaction_id,
                milecsa::asset::XDR,
                milecsa::asset::amount_to_string(100.0f, milecsa::asset::code::XDR),
                memo,
                fee,

                transaction,
                digest,

                errorDescription)){
            BOOST_TEST_MESSAGE("Error happened in Pair");
            return false;
        }

        BOOST_TEST_MESSAGE("Wallet    trx: " + transaction);
        BOOST_TEST_MESSAGE("Wallet    fee: " + fee);
        BOOST_TEST_MESSAGE("Wallet digest: " + digest);

        return  true;

    }
};

#endif //MILECSA_MILECSA_TRANSFER_TEST_HPP
