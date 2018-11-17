# Mile CSA Light C++ API

## Requirements
1. c++11
1. cmake
1. boost multiprecision installed includes (>=1.66, exclude 1.68!)

## Build
    $ git clone https://github.com/mile-core/mile-csa-light
    $ cd ./mile-csa-light; mkdir build; cd ./build
    $ cmake ..; make -j4
    $ make test

## Boost updates (if it needs)
    $ wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz
    $ tar -xzf boost_1_*
    $ cd boost_1_*
    $ ./bootstrap.sh --prefix=/usr
    $ ./b2 install --prefix=/usr --with=all -j4


## Tested
1. Centos7 (gcc v4.8.5)
1. OSX 10.13, XCode10

### Token properties

```cpp
    #include "milecsa_light_api.hpp"
    
    // available tokens: 
    milecsa::token stable = milecsa::assets::XDR;
    milecsa::token index  = milecsa::assets::MILE;

    // Properties
    std::cout << "Name:      " << stable.name;
    std::cout << "Code:      " << stable.code;
    std::cout << "Precision: " << stable.precision;

    // Fixed point conversion to string presentation    
    std::string amount = asset.value_to_string(100.1f);
     

```

### Wallet public/private keys Pair structure

```cpp
   #include "milecsa_light_api.hpp"

   milecsa::light::Pair keyPair;
   std::string errorDescription;
   
   if (milecsa::keys::generate(keyPair, errorDescription)) {
        //
        // Handle error
        //
       return false;
   } else {
       return true;
   }

```

### Make new wallet from secrete phrase

```cpp
    #include "milecsa_light_api.hpp"
   
   milecsa::light::Pair keyPair;
   std::string errorDescription;
   
   if (milecsa::keys::generate_with_secret(keyPair, "test string", errorDescription))
   {
       return false;
   }
   else
   {
       return true;
   }
```

### Restore wallet pair from private key encoded as base58 string

```cpp
    #include "milecsa_light_api.hpp"
    
    milecsa::light::Pair keyPair;
    std::string errorDescription;
    
    if(milecsa::keys::generate_from_private_key(
           keyPair,
           initialKeyPair.private_key,
           errorDescription)) {
       return false;
    }
    else
    {  
       return true;
    }
```


### Preparing wallet transactions request

#### Asset transfer request. Simple getting transaction body
```cpp

   #include "milecsa_light_api.hpp"
   
    ///
   /// trx id can be create for user reason or can be default
   ///
   uint64_t trx_id = default_transaction_id;

   ///
   /// Transaction container string
   ///
   std::string transaction;

   ///
   /// Transaction container uniq digest
   ///
   std::string digest;

   ///
   /// Asset code
   ///
   milecsa::token token = milecsa::assets::XDR;

   ///
   /// Prepare fixed point presentation asset amount
   ///
   float amount = 1.0f;

   ///
   /// Build signed transfer transaction
   ///
   if (milecsa::transaction::prepare_transfer(
           pair.private_key,          /// "from" private key
           to,                        /// "to" public key
           block_id,                  /// block id
           trx_id,                    /// user defined transaction id or number
           token,                     /// asset
           amount,                    /// amount of transfer

           0.0,                       /// fee is always 0
           "memo",                    /// description

           transaction,               /// returned signed transaction as json string
           digest,                    /// uinq transaction digest string

           errorMessage               /// error message if something failed
   )){

       cerr << " prepare_transfer error: " << errorMessage << endl;
       return -1;
   }

   ///
   /// Prepare transfer json-rpc buffer
   ///

   // transaction is a json string -> ...
   
    std::string request = "{\"id\":0,\"jsonrpc\":\"2.0\",\"method\":\"send-transaction\",\"version\":0.0, \"params\": ";
    
    request += transaction_body + "}";
    
     //
     // Send transaction
     //
     
     ...  
   
```

###  Common recommendations how to check transactions is approved by blockchain or not:
    
    . Send transaction 
    . Getting next 4 blocks including current block (till block contains this transaction)
    . Check transaction in section "transactions" in block 
    . If transactions is found with the same digest or public-key and transaction-id that have ben sent - blockchain is approved the operation
    . If within next 4 blocks (including current) the sent transaction did not appear in the blockchain, then we can assume that the transaction was not accepted and should be resend with new block-id and new transaction-id untill this one will be apporved 

#### [Check transaction approving Example](https://github.com/mile-core/mile-csa-light/blob/master/examples/check-trx-approving/send-1xdr-wait-for-approving.cpp)