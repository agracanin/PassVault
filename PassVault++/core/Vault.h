#pragma once

#include <vector>
#include <string>
#include "PasswordEntry.h"

class Vault {
public:
    void addEntry(const PasswordEntry& entry);

    bool removeEntry(const std::string& id);

    // Get read-only view of all entries
    const std::vector<PasswordEntry>& getEntries() const;

    // Find a single entry by id (nullptr if not found)
    const PasswordEntry* findEntryById(const std::string& id) const;

    void saveToFile(const std::string& filePath) const;
    bool loadFromFile(const std::string& filePath);

private:
    std::vector<PasswordEntry> m_entries;
};