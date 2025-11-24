#pragma once

#include <string>
#include "Vault.h"


class VaultStorage {
public:
    // Save vault encrypted with given master password
    static bool saveEncrypted(const Vault& vault,
                            const std::string& filePath,
                            const std::string& masterPassword);
                            
    // Load vault from encrypted file
    static bool loadEncrypted(Vault& vault,
                            const std::string& filePath,
                            const std::string& masterPassword);
};