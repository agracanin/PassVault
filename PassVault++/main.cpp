#include <iostream>
#include <string>
#include <vector>
#include <sodium.h>

#include "core/Vault.h"
#include "core/PasswordEntry.h"
#include "core/Crypto.h"


int main() {
    if (sodium_init() < 0) {
        std::cerr << "Failed to initialize libsodium\n";
        return 1;
    }

    std::cout << "libsodium initialized successfully.\n";

    // --- Test Crypto on a simple message ---
    std::string masterPassword = "My$uperSecureMasterPassword!";
    std::string message = "Hello, this is secret JSON or something.";

    std::vector<unsigned char> plaintext(message.begin(), message.end());
    EncryptedData encrypted;

    if (!Crypto::encrypt(masterPassword, plaintext, encrypted)) {
        std::cerr << "Encryption failed.\n";
        return 1;
    }

    std::cout << "Encryption succeeded. Ciphertext size: "
              << encrypted.ciphertext.size() << "\n";

    std::vector<unsigned char> decrypted;
    if (!Crypto::decrypt(masterPassword, encrypted, decrypted)) {
        std::cerr << "Decryption failed.\n";
        return 1;
    }

    std::string decryptedStr(decrypted.begin(), decrypted.end());
    std::cout << "Decrypted text: " << decryptedStr << "\n";

    return 0;
}