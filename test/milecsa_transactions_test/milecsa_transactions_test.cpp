#define BOOST_TEST_MODULE transactions

#include "milecsa_light_api.hpp"
#include "milecsa_emission_test.hpp"
#include "milecsa_transfer_test.hpp"
#include "../MileTest.h"

BOOST_FIXTURE_TEST_CASE( TransferXDR, Wallet )
{
    BOOST_CHECK(test(milecsa::assets::XDR));
}

BOOST_FIXTURE_TEST_CASE( TransferMILE, Wallet )
{
    BOOST_CHECK(test(milecsa::assets::MILE));
}


BOOST_FIXTURE_TEST_CASE( EmissionDirect, Emission )
{
    BOOST_CHECK(test(milecsa::assets::MILE)); // direct
}

BOOST_FIXTURE_TEST_CASE( EmissionReverse, Emission )
{
    BOOST_CHECK(test(milecsa::assets::XDR));  // reverse
}


