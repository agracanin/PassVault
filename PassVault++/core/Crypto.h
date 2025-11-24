#pragma once

#include <string>
#include <vector>

struct EncryptedData {
    std::vector<unsigned char> salt;
    std::vector<unsigned char> nonce;
    std::vector<unsigned char> ciphertext;
};

class Crypto {
public:
    // Derive a key from a master password and salt using Argon2id
    static bool deriveKey(const std::string& password,
                          const std::vector<unsigned char>& salt,
                          std::vector<unsigned char>& keyOut);
    
    static bool encrypt(const std::string& password,
                        const std::vector<unsigned char>& plaintext,
                        EncryptedData& outData);

    static bool decrypt(const std::string& password,
                        const EncryptedData& data,
                        std::vector<unsigned char>& plaintextOut);                    

};