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

        std::string transaction;
        std::string fee;
        std::string digest;

        if (milecsa::transaction::prepare_transfer(
                keyPair.private_key,
                destination,
                "0",
                milecsa::transaction::default_transaction_id,
                1,
                "1000",
                "memo",
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
