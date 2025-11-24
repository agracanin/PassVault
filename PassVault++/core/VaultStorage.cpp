#include "VaultStorage.h"
#include "Crypto.h"
#include <fstream>
#include <vector>

// Simple file format:
// "PWVT" (4 bytes magic)
// uint8_t version
// uint32_t saltLen
// uint32_t nonceLen
// uint32_t cipherLen
// [salt][nonce][ciphertext]

namespace {
    const char MAGIC[4] = { 'P', 'W', 'V', 'T' };
    const uint8_t VERSION = 1;

    void writeUint32(std::ofstream& out, uint32_t value) {
        // Write as little-endian
        unsigned char buf[4];
        buf[0] = static_cast<unsigned char>(value & 0xFF);
        buf[1] = static_cast<unsigned char>((value >> 8) & 0xFF);
        buf[2] = static_cast<unsigned char>((value >> 16) & 0xFF);
        buf[3] = static_cast<unsigned char>((value >> 24) & 0xFF);
        out.write(reinterpret_cast<char*>(buf), 4);
    }

    bool readUint32(std::ifstream& in, uint32_t& value) {
        unsigned char buf[4];
        if (!in.read(reinterpret_cast<char*>(buf), 4)) {
            return false;
        }
        value = static_cast<uint32_t>(buf[0])
              | (static_cast<uint32_t>(buf[1]) << 8)
              | (static_cast<uint32_t>(buf[2]) << 16)
              | (static_cast<uint32_t>(buf[3]) << 24);
        return true;
    }
}

bool VaultStorage::saveEncrypted(const Vault& vault,
                                 const std::string& filePath,
                                 const std::string& masterPassword)
{
    std::string jsonStr = vault.toJsonString();
    std::vector<unsigned char> plaintext(jsonStr.begin(), jsonStr.end());

    EncryptedData enc;
    if (!Crypto::encrypt(masterPassword, plaintext, enc)) {
        return false;
    }

    std::ofstream out(filePath, std::ios::binary);
    if (!out.is_open()) {
        return false;
    }

    // Write header
    out.write(MAGIC, 4);
    out.put(static_cast<char>(VERSION));

    writeUint32(out, static_cast<uint32_t>(enc.salt.size()));
    writeUint32(out, static_cast<uint32_t>(enc.nonce.size()));
    writeUint32(out, static_cast<uint32_t>(enc.ciphertext.size()));

    // Write data blobs
    out.write(reinterpret_cast<const char*>(enc.salt.data()), enc.salt.size());
    out.write(reinterpret_cast<const char*>(enc.nonce.data()), enc.nonce.size());
    out.write(reinterpret_cast<const char*>(enc.ciphertext.data()), enc.ciphertext.size());

    return true;
}

bool VaultStorage::loadEncrypted(Vault& vault,
                                 const std::string& filePath,
                                 const std::string& masterPassword)
{
    std::ifstream in(filePath,  std::ios::binary);
    if (!in.is_open()) {
        return false;
    }

    // Read and verify header
    char magic[4];
    in.read(magic, 4);
    if(magic[0] != MAGIC[0] || magic[1] != MAGIC[1] ||
       magic[2] != MAGIC[2] || magic[3] != MAGIC[3]) {
        return false;
    }

    char versionChar;
    if(!in.get(versionChar)) {
        return false;
    }
    uint8_t version = static_cast<uint8_t>(versionChar);
    if (version != VERSION) {
        return false;
    }

    // Read lengths
    uint32_t saltLen = 0, nonceLen = 0, cipherLen = 0;
    if (!readUint32(in, saltLen) ||
        !readUint32(in, nonceLen) ||
        !readUint32(in, cipherLen)) {
        return false;
    }

    // Read data blobs
    EncryptedData enc;
    enc.salt.resize(saltLen);
    enc.nonce.resize(nonceLen);
    enc.ciphertext.resize(cipherLen);

    if (!in.read(reinterpret_cast<char*>(enc.salt.data()), saltLen)) {
        return false;
    }
    if (!in.read(reinterpret_cast<char*>(enc.nonce.data()), nonceLen)) {
        return false;
    }
    if (!in.read(reinterpret_cast<char*>(enc.ciphertext.data()), cipherLen)) {
        return false;
    }

    // Decrypt
    std::vector<unsigned char> plaintext;
    if (!Crypto::decrypt(masterPassword, enc, plaintext)) {
        return false;
    }

    std::string jsonStr(plaintext.begin(), plaintext.end());

    // Rebuilt vault from JSON
    return vault.fromJsonString(jsonStr);
}