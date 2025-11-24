#include "Vault.h"
#include "../external/json/json.hpp"
#include <fstream>

using json = nlohmann::json;

void Vault::addEntry(const PasswordEntry& entry) {
	m_entries.push_back(entry);
}

bool Vault::removeEntry(const std::string& id) {
	for (auto it = m_entries.begin(); it != m_entries.end(); ++it) {
		if (it->id == id) {
			m_entries.erase(it);
			return true;
		}
	}
	return false;
}

const std::vector<PasswordEntry>& Vault::getEntries() const {
	return m_entries;
}

const PasswordEntry* Vault::findEntryById(const std::string& id) const {
	for (const auto& entry : m_entries) {
		if (entry.id == id) {
			return &entry;
		}
	}
	return nullptr;
}

void Vault::saveToFile(const std::string& filePath) const {
	json j;
	j["entries"] = m_entries;

	std::ofstream out(filePath);
	out << j.dump(4);
}

bool Vault::loadFromFile(const std::string& filePath) {
	std::ifstream in(filePath);
	if (!in.is_open()) {
		return false;
	}

	json j;
	in >> j;
	if (!j.contains("entries")) {
		return false;
	}

	m_entries = j["entries"].get<std::vector<PasswordEntry>>();
	return true;
}

std::string Vault::toJsonString() const {
	json j;
	j["entries"] = m_entries;
	return j.dump(4);
}

bool Vault::fromJsonString(const std::string& jsonStr) {
	json j = json::parse(jsonStr, nullptr, false);
	if (j.is_discarded() || !j.contains("entries")) {
		return false;
	}

	m_entries = j["entries"].get<std::vector<PasswordEntry>>();
	return true;
}