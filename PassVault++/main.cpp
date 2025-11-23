#include <iostream>
#include "core/Vault.h"
#include "core/PasswordEntry.h"

int main() {
    Vault vault;

    PasswordEntry e1;
    e1.id = "1";
    e1.title = "GitHub";
    e1.username = "test";
    e1.password = "supersecret123";
    e1.url = "https://github.com";
    e1.notes = "Personal account";
    e1.lastModified = "2025-11-22T19:00:00Z";

    vault.addEntry(e1);

    // Save
    vault.saveToFile("vault.json");
    std::cout << "Saved vault.json\n";

    // Load into a new vault
    Vault loaded;
    if (loaded.loadFromFile("vault.json")) {
        std::cout << "Loaded entries:\n";
        for (const auto& entry : loaded.getEntries()) {
            std::cout << "- " << entry.title << " (" << entry.username << ")\n";
        }
    }
    else {
        std::cout << "Failed to load vault.json\n";
    }

    return 0;
}