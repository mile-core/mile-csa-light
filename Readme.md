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

### Tokens

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
   
   std::string transaction_body;
   std::string fee;
   std::string digest;

   if (milecsa::transaction::prepare_transfer(
           privateKey,
           destination,
           "0",
           0,
           1,
           "1000",
           "memo",
           fee,

           transaction_body,
           digest,

           errorDescription)){
           //
           // handle error
           //
       return false;
   }
   
  
   // transaction is a json string -> ...
   
    std::string request = "{\"id\":0,\"jsonrpc\":\"2.0\",\"method\":\"send-transaction\",\"version\":0.0, \"params\": ";
    
    request += transaction_body + "}";
    
     //
     // Send transaction
     //
     
     ...  
   
```
