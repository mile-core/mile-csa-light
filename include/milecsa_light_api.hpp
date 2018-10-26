#ifndef MILECSA_MILECSA_LIGHT_API_HPP
#define MILECSA_MILECSA_LIGHT_API_HPP

#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <iomanip>
#include <sstream>

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)

#if !(GCC_VERSION > 70200 || __clang__)
#define __USE_MILECSA_FIXED_POINT_IMP__ 1
#endif

extern void float2FixedPoint(float n, std::string &output, int afterpoint);

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

    /**
     * Token description
     */
    struct token {
        unsigned short code;
        unsigned short precision;
        std::string name;

        template <typename T>
        /**
         * Prepare asset amount from asset code
         *
         * @tparam T
         * @param amount - amount
         * @param assetCode - asset code
         * @return prepared amount fixed point number presented as string
         */
        std::string value_to_string(T amount) const {
            //
            // truncate rest of the number
            //
            std::string value;
            float2FixedPoint((float)amount, value, precision);
            return value;
        }

        /**
         * Build tocken description
         *
         * @param code
         * @param precision
         * @param name
         */
        token(unsigned short code, unsigned short precision, const std::string &name);
        token(const token &);

    private:
        token();
    };

    /**
     * Available blockchain tokens
     */
    struct assets {
        /**
         * Stable coin
         */
        static token XDR;

        /**
         * Index coin
         */
        static token MILE;
    };

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
         * @param fee - always 0
         * @param description - transfer description
         * @param transaction - returned SIGNED transaction json body, can send as "params" keyword for json-rpc method "send-transaction"
         * @param digest - uniq transaction digest string
         * @param errorMessage - error message if something wrong happened
         * @return ok or reason
         */
        light::result prepare_transfer(const std::string &privateKey,
                                       const std::string &destinationPublicKey,

                                       const std::string &blockId,
                                       const uint64_t transactionId,

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

        /**
         * Prepare signed transaction JSON packet for making emission.
         * If asset == MILE then XDR will be emitted,
         * in other hand if asset == XDR then MILE reverse emissions will be handled
         *
         * @param privateKey
         * @param dstWalletPublicKey
         * @param blockId
         * @param transactionId
         * @param asset
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

                                       const milecsa::token  &asset,
                                       float                 fee,
                //
                // Signed json
                //
                                       std::string &transaction,
                                       std::string &digest,
                                       std::string &errorMessage);
    }
}


#endif //MILECSA_MILECSA_LIGHT_API_HPP