//
// Created by lotus mile on 22/10/2018.
//

#include "milecsa_light_api.hpp"

#include <cstring>
#include <string>
#include <string.h>
#include <boost/multiprecision/cpp_int.hpp>
#include "json.hpp"

extern milecsa::light::result prepare_transaction(const std::string &name, const std::string &privateKey,
                                                  const std::string &dstWalletPublicKey,

                                                  const std::string &blockId,
                                                  const uint64_t  transactionId,

                                                  const milecsa::token  &asset,
                                                  float                 amount,
                                                  float                 fee,
                                                  const std::string     &description,
//
// Signed json
//
                                                  std::string &transaction,
                                                  std::string &digest,
                                                  std::string &errorMessage);

milecsa::light::result milecsa::transaction::prepare_emission(const std::string &privateKey,
                                                              const std::string &dstWalletPublicKey,

                                                              const std::string &blockId,
                                                              const uint64_t  transactionId,

                                                              const milecsa::token  &asset,
                                                              float                 amount,
                                                              float                 fee,
                                                              const std::string     &description,
//
// Signed json
//
                                                              std::string &transaction,
                                                              std::string &digest,
                                                              std::string &errorMessage){

    return prepare_transaction("EmissionTransaction",
                               privateKey,
                               dstWalletPublicKey,

                               blockId,
                               transactionId,

                               asset,
                               amount,
                               fee,
                               description,
                               transaction,
                               digest,
                               errorMessage
    );
}