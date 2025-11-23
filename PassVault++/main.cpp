#include <iostream>
#include "Vault.h"
#include "PasswordEntry.h"

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

    std::cout << "Current entries in vault:\n";
    for (const auto& entry : vault.getEntries()) {
        std::cout << "- [" << entry.id << "] "
            << entry.title << " ("
            << entry.username << ")\n";
    }

    const PasswordEntry* found = vault.findEntryById("1");
    if (found) {
        std::cout << "\nFound entry 1: " << found->title
            << " / " << found->username << "\n";
    }

    return 0;
}
