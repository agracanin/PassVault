#include "Crypto.h"
#include <sodium.h>

bool Crypto::deriveKey(const std::string& password,
                        const std::vector<unsigned char>& salt,
                        std::vector<unsigned char>& keyOut)
{
    keyOut.resize(crypto_aead_xchacha20poly1305_ietf_KEYBYTES);

    if(salt.size() != crypto_pwhash_SALTBYTES) {
        return false;
    }

    int result = crypto_pwhash(
        keyOut.data(),
        keyOut.size(),
        password.c_str(),
        password.size(),
        salt.data(),
        crypto_pwhash_OPSLIMIT_INTERACTIVE,
        crypto_pwhash_MEMLIMIT_INTERACTIVE,
        crypto_pwhash_ALG_DEFAULT
    );

    return result == 0;
}

bool Crypto::encrypt(const std::string& password,
                     const std::vector<unsigned char>& plaintext,
                     EncryptedData& outData)
{
    // Generate random salt and nonce
    outData.salt.resize(crypto_pwhash_SALTBYTES);
    outData.nonce.resize(crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);

    randombytes_buf(outData.salt.data(), outData.salt.size());
    randombytes_buf(outData.nonce.data(), outData.nonce.size());

    // Derive key
    std::vector<unsigned char> key;
    if(!deriveKey(password, outData.salt, key)) {
        return false;
    }

    // Allocate ciphertext plantext + MAC
    outData.ciphertext.resize(plaintext.size() + crypto_aead_xchacha20poly1305_ietf_ABYTES);

    unsigned long long ciphertext_len;
    int rc = crypto_aead_xchacha20poly1305_ietf_encrypt(
        outData.ciphertext.data(),
        &ciphertext_len,
        plaintext.data(),
        plaintext.size(),
        nullptr, 0,
        nullptr,
        outData.nonce.data(),
        key.data()        
    );

    if (rc != 0) {
        return false;
    }

    outData.ciphertext.resize(ciphertext_len);
    return true;
}


bool Crypto::decrypt(const std::string& password,
                     const EncryptedData& data,
                     std::vector<unsigned char>& plaintextOut)
{
    if (data.salt.size() != crypto_pwhash_SALTBYTES ||
        data.nonce.size() != crypto_aead_xchacha20poly1305_ietf_NPUBBYTES) {
        return false;
    }

    std::vector<unsigned char> key;
    if (!deriveKey(password, data.salt, key)) {
        return false;
    }

    // Allocate space for plaintext
    plaintextOut.resize(
        data.ciphertext.size() - crypto_aead_xchacha20poly1305_ietf_ABYTES
    );

    unsigned long long plaintextLen = 0;

    int rc = crypto_aead_xchacha20poly1305_ietf_decrypt(
        plaintextOut.data(), &plaintextLen,
        nullptr,
        data.ciphertext.data(), data.ciphertext.size(),
        nullptr, 0,                 // no additional data
        data.nonce.data(),
        key.data()
    );

    if (rc != 0) {
        // wrong password or corrupted data
        return false;
    }

    plaintextOut.resize(plaintextLen);
    return true;
}

