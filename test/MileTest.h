#pragma once

#include "milecsa_light_api.hpp"
#include <boost/test/included/unit_test.hpp>

#ifdef WITH_CURL
#include <curl/curl.h>
#else
#warning "curl library has not been configured"
#endif

using namespace std;

struct MileTest {

    MileTest(const string &name): name(name) {
        BOOST_TEST_MESSAGE("Setup "+name);
    }

    void print_key_pair(milecsa::light::Pair *keyPair) {
        string mess = "Public key: ";
        mess += keyPair->public_key;
        BOOST_TEST_MESSAGE(mess);
        mess = "Private key: ";
        mess += keyPair->private_key;
        BOOST_TEST_MESSAGE(mess);
    }

    ~MileTest(){
            if (!errorDescription.empty()) {
                    BOOST_TEST_MESSAGE(errorDescription);
            }
        BOOST_TEST_MESSAGE("Teardown "+name);
    }

    std::string errorDescription;
    std::string name;
};
