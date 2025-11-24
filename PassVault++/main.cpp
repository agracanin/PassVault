#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <sodium.h>

#include "core/Vault.h"
#include "core/PasswordEntry.h"
#include "core/Crypto.h"
#include "core/VaultStorage.h"

int main() {
    if (sodium_init() < 0) {
        std::cerr << "Failed to initialize libsodium\n";
        return 1;
    }

    const std::string filePath = "vault.enc";

    std::string masterPassword;
    std::cout << "Enter master password: ";
    std::getline(std::cin, masterPassword);

    // Check if vault file already exists
    if (std::filesystem::exists(filePath)) {
        std::cout << "Encrypted vault file found. Attempting to open...\n";

        Vault loaded;
        if (!VaultStorage::loadEncrypted(loaded, filePath, masterPassword)) {
            std::cerr << "Failed to open vault. Wrong password or file is corrupted.\n";
            return 1;
        }

        std::cout << "Vault opened successfully. Entries:\n";
        for (const auto& entry : loaded.getEntries()) {
            std::cout << "- " << entry.title << " (" << entry.username << ")\n";
        }

    } else {
        std::cout << "No existing vault found. Creating a new one...\n";

        Vault vault;

        // Samples for now
        PasswordEntry e1;
        e1.id = "1";
        e1.title = "GitHub";
        e1.username = "test";
        e1.password = "supersecret123";
        e1.url = "https://github.com";
        e1.notes = "Personal account";
        e1.lastModified = "2025-11-24T00:00:00Z";
        vault.addEntry(e1);

        PasswordEntry e2;
        e2.id = "2";
        e2.title = "Gmail";
        e2.username = "test@example.com";
        e2.password = "anotherSecret!";
        e2.url = "https://mail.google.com";
        e2.notes = "Main email";
        e2.lastModified = "2025-11-24T00:05:00Z";
        vault.addEntry(e2);

        if (!VaultStorage::saveEncrypted(vault, filePath, masterPassword)) {
            std::cerr << "Failed to save new encrypted vault.\n";
            return 1;
        }

        std::cout << "New encrygpted vault created at " << filePath << "\n";
    }

    return 0;
}
