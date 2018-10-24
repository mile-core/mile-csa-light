#ifndef MILECSA_MILECSA_LIGHT_API_HPP
#define MILECSA_MILECSA_LIGHT_API_HPP

#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <iomanip>
#include <sstream>

namespace milecsa {

    namespace light {
        struct Pair {
            std::string public_key;
            std::string private_key;
        };

        /**
          * Common mile csa handling codes
          *
          * */
        typedef enum: int {
            UNKNOWN = -1,
            OK = 0,
            FAIL = 1,
            EMPTY = 2,
            NOT_SUPPORTED = 3,
            NOT_FOUND = 4,
            ALREADY_EXIST = 5,
            EXCEPTION = 6,
            LAST = EXCEPTION
        } result;
    }

    namespace asset {

        typedef enum: unsigned short {
            XDR = 0,
            MILE = 1
        } code ;

        /**
       *
       * Prepare asset amount from asset with user defined precision
       *
       * @tparam T
       * @param amount - amount
       * @param precision - user defined precision
       * @return prepared amount fixed point number presented as string
       */
        template <typename T>
        std::string amount_to_string_with_precision(T amount, int precision){
            std::stringstream stream;
            stream << std::fixed << std::setprecision(precision) << amount;
            return stream.str();
        }

        template <typename T>
        /**
         * Prepare asset amount from asset code
         *
         * @tparam T
         * @param amount - amount
         * @param assetCode - asset code
         * @return prepared amount fixed point number presented as string
         */
        std::string amount_to_string(T amount, milecsa::asset::code assetCode){
            switch (assetCode){
                case XDR:
                    return amount_to_string_with_precision(amount, 5);
                default:
                    return amount_to_string_with_precision(amount, 2);
            }
        }

    }

    namespace keys {

        /**
         * Generate random Wallet Keys Pair
         *
         * @param pair - generated pair
         * @param errorMessage - error message if something wrong happened
         * @return ok or reason
         */
        light::result generate(
                light::Pair &pair,
                std::string &errorMessage);


        /**
         * Generate Wallet pair from private key
         *
         * @param pair - generated pair
         * @param privateKey - private key
         * @param errorMessage - error message if something wrong happened
         * @return ok or reason
         * */
        light::result generate_from_private_key(
                light::Pair &pair,
                const std::string &privateKey,
                std::string &errorMessage);

        /**
         * Generate Wallet pair from private key
         *
         * @param pair - generated pair
         * @param phrase - secret phrase
         * @param errorMessage - error message if something wrong happened
         * @return ok or reasone
         * */
        light::result generate_with_secret(
                light::Pair &pair,
                const std::string &phrase,
                std::string &errorMessage);


        /**
         * Validate key pair, (i.e. exist the pairs on curve ed25519 or not)
         *
         * @param pair wallet keys
         * @param errorMessage - error message if something wrong happened
         * @return ok or reason
         * */
        light::result validate(
                const light::Pair &pair,
                std::string &errorMessage);


        /**
         * Validate public key, (i.e. exist the pairs on curve ed25519 or not)
         *
         * @param publicKey
         * @param errorMessage - error message if something wrong happened
         * @return ok or reason
         * */
        light::result validate_public_key(
                const std::string &publicKey,
                std::string &errorMessage);


        /**  Validate private key, (i.e. exist the pairs on curve ed25519 or not)
         *
         * @param privateKey
         * @param errorMessage - error message if something wrong happened
         * @return ok or reason
         * */
        light::result validate_private_key(
                const std::string &privateKey,
                std::string &errorMessage);


    }

    namespace transaction {

        static const uint64_t default_transaction_id = (uint64_t)-1;
        static const unsigned short empty_asset_code = (unsigned short)-1;

        /**
         *
         * Prepare signed transaction JSON packet for transfer assets to send to the MILE blockchain.
         *
         * @param privateKey - owner private key
         * @param destinationPublicKey - recipient address, public key
         * @param blockId - current blockchain network block id
         * @param transactionId - user defined id or milecsa::transaction::default_transaction_id
         * @param assetCode - asset code
         * @param amount - transfer amount
         * @param description - transfer description
         * @param fee - always empty
         * @param transaction - returned SIGNED transaction json body, can send as "params" keyword for json-rpc method "send-transaction"
         * @param digest - uniq transaction digest string
         * @param errorMessage - error message if something wrong happened
         * @return ok or reason
         */
        light::result prepare_transfer(const std::string &privateKey,
                                       const std::string &destinationPublicKey,

                                       const std::string &blockId,
                                       const uint64_t transactionId,

                                       asset::code assetCode,
                                       const std::string &amount,
                                       const std::string &description,
                                       const std::string &fee,

                //
                // Signed json
                //
                                       std::string &transaction,
                                       std::string &digest,
                                       std::string &errorMessage);

        /**
         * Prepare signed transaction JSON packet for making emission of XDR to send to the MILE blockchain.
         *
         * @param privateKey
         * @param dstWalletPublicKey
         * @param blockId
         * @param transactionId
         * @param asset
         * @param amount
         * @param description
         * @param fee
         * @param transaction
         * @param digest
         * @param errorMessage
         * @return
         */
        light::result prepare_emission(const std::string &privateKey,
                                       const std::string &dstWalletPublicKey,

                                       const std::string &blockId,
                                       const uint64_t transactionId,

                                       unsigned short asset,
                                       const std::string &amount,
                                       const std::string &description,
                                       const std::string &fee,

                //
                // Signed json
                //
                                       std::string &transaction,
                                       std::string &digest,
                                       std::string &errorMessage);
    }
}


#endif //MILECSA_MILECSA_LIGHT_API_HPP