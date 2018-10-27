//
// Created by lotus mile on 22/10/2018.
//


#include "milecsa_light_api.hpp"

#include <cstring>
#include <string>
#include <string.h>

#include "mile_crypto.h"
//#include "milecsa_utils.hpp"

milecsa::light::result milecsa::keys::generate(
        milecsa::light::Pair &pair,
        std::string &errorMessage) {

    pair.public_key.clear();
    pair.private_key.clear();

    PrivateKey privateKey;
    PublicKey publicKey;

    CreateKeyPair(privateKey, publicKey);

    if ( publicKey.IsZero()){
        errorMessage = ErrorFormat("public key couldn't be empty");
        return milecsa::light::result::EMPTY;
    }

    if (privateKey.IsZero()){
        errorMessage = ErrorFormat("private key couldn't be empty");
        return milecsa::light::result::EMPTY;
    }

    pair.public_key = publicKey.ToBase58CheckString();
    pair.private_key = privateKey.ToBase58CheckString();

    return milecsa::light::result::OK;

}

milecsa::light::result milecsa::keys::generate_with_secret(
        milecsa::light::Pair &pair,
        const std::string &phrase,
        std::string &errorMessage)
{
    pair.public_key.clear();
    pair.private_key.clear();

    PrivateKey privateKey;
    PublicKey publicKey;

    Seed seed;
    seed.Clear();
    sha3_256((const unsigned char*) phrase.c_str(), phrase.length(), seed.Data.data());

    CreateKeyPair(privateKey, publicKey, seed);

    if ( publicKey.IsZero()){
        errorMessage = ErrorFormat("public key couldn't be empty");
        return milecsa::light::result::EMPTY;
    }

    if (privateKey.IsZero()){
        errorMessage = ErrorFormat("private key couldn't be empty");
        return milecsa::light::result::EMPTY;
    }

    pair.public_key = publicKey.ToBase58CheckString();
    pair.private_key = privateKey.ToBase58CheckString();

    return milecsa::light::result::OK;
}

milecsa::light::result milecsa::keys::generate_from_private_key(
        milecsa::light::Pair &pair,
        const std::string &privateKey,
        std::string &errorMessage)
{

    pair.public_key.clear();
    pair.private_key.clear();

    PrivateKey _privateKey;
    PublicKey publicKey;

    if ( !_privateKey.SetBase58CheckString(privateKey, errorMessage) ) {
        return milecsa::light::result::FAIL;
    }

    RestoreKeyPairFromPrivate(_privateKey, publicKey);

    pair.public_key = publicKey.ToBase58CheckString();
    pair.private_key = _privateKey.ToBase58CheckString();

    return milecsa::light::result::OK;

}

milecsa::light::result milecsa::keys::validate_public_key(
        const std::string &publicKey,
        std::string &errorMessage) {
    PublicKey _publicKey;
    if (!_publicKey.SetBase58CheckString(publicKey, errorMessage)){
        return milecsa::light::result::FAIL;
    }
    return milecsa::light::result::OK;
}

milecsa::light::result milecsa::keys::validate_private_key(
        const std::string &privateKey,
        std::string &errorMessage) {
    PrivateKey _privateKey;
    if (!_privateKey.SetBase58CheckString(privateKey, errorMessage)){
        return milecsa::light::result::FAIL;
    }
    return milecsa::light::result::OK;
}

milecsa::light::result milecsa::keys::validate(const milecsa::light::Pair &keysPair, std::string &errorMessage){

    milecsa::light::result result = milecsa::keys::validate_public_key(keysPair.public_key, errorMessage);
    if (result != milecsa::light::result::OK)
        return result;
    return milecsa::keys::validate_private_key(keysPair.private_key, errorMessage);
}