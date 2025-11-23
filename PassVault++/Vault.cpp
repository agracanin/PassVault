#include "Vault.h"

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