#pragma once

#include <string>
#include "../external/json/json.hpp"

using json = nlohmann::json;

struct PasswordEntry {
    std::string id;
    std::string title;
    std::string username;
    std::string password;
    std::string url;
    std::string notes;
    std::string lastModified;
};

// Serialize PasswordEntry -> json
inline void to_json(json& j, const PasswordEntry& p) {
    j = json{
        {"id", p.id},
        {"title", p.title},
        {"username", p.username},
        {"password", p.password},
        {"url", p.url},
        {"notes", p.notes},
        {"lastModified", p.lastModified}
    };
}

// Deserialize json -> PasswordEntry
inline void from_json(const json& j, PasswordEntry& p) {
    j.at("id").get_to(p.id);
    j.at("title").get_to(p.title);
    j.at("username").get_to(p.username);
    j.at("password").get_to(p.password);
    j.at("url").get_to(p.url);
    j.at("notes").get_to(p.notes);
    j.at("lastModified").get_to(p.lastModified);
}