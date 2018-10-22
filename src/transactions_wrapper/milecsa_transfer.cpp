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

extern milecsa::light::result prepare_transaction(const std::string &name,
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
                                                  std::string &errorMessage);

milecsa::light::result milecsa::transaction::prepare_transfer(const std::string &privateKey,
                                                              const std::string &dstWalletPublicKey,

                                                              const std::string &blockId,
                                                              const uint64_t  transactionId,

                                                              unsigned short assetCode,
                                                              const std::string &amount,
                                                              const std::string &description,
                                                              const std::string &fee,

                                                              std::string &transaction,
                                                              std::string &digest,
                                                              std::string &errorMessage) {


    return prepare_transaction("TransferAssetsTransaction",
                               privateKey,
                               dstWalletPublicKey,

                               blockId,
                               transactionId,

                               assetCode,
                               amount,
                               description,
                               fee,
                               transaction,
                               digest,
                               errorMessage

    );
}