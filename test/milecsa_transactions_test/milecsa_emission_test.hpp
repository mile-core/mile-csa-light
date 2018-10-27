//
// Created by lotus mile on 02/10/2018.
//

#ifndef MILECSA_MILECSA_EMISSION_TEST_HPP
#define MILECSA_MILECSA_EMISSION_TEST_HPP

#include "milecsa_light_api.hpp"
#include "MIleTestTransaction.hpp"

struct Emission: public MIleTestTransaction {

    Emission():MIleTestTransaction("Emission"){}

    bool test(const milecsa::token &token) {

        float fee = 0.0f;

        std::string transaction;
        std::string digest;

        if (milecsa::transaction::prepare_emission(
                keyPair.private_key,
                get_block_id(),
                milecsa::transaction::default_transaction_id,
                token,
                fee,
                transaction,
                digest,
                errorDescription)){
            BOOST_TEST_MESSAGE("Error happened in Pair");
            return false;
        }

        BOOST_TEST_MESSAGE("Wallet    trx: " + transaction);
        BOOST_TEST_MESSAGE("Wallet    fee: " + token.value_to_string(fee));
        BOOST_TEST_MESSAGE("Wallet digest: " + digest);

        return  send(transaction) == 0;

    }
};

#endif //MILECSA_MILECSA_EMISSION_TEST_HPP
