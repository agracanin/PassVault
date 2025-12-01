#include "MainWindow.h"
#include "../core/VaultStorage.h"
#include "AddEntryDialog.h"

#include <sodium.h>

#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QAbstractItemView>
#include <QtGui/QStandardItemModel>
#include <QtWidgets/QToolBar>
#include <filesystem>



static std::string generateId() {
    unsigned char bytes[16];
    randombytes_buf(bytes, sizeof(bytes));

    static const char* hex = "0123456789abcdef";
    std::string out;
    out.reserve(32);
    for (unsigned char b : bytes) {
        out.push_back(hex[b >> 4]);
        out.push_back(hex[b & 0x0F]);
    }
    return out;
}

void MainWindow::addEntry() {
    AddEntryDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    PasswordEntry e = dlg.toEntry();
    e.id = generateId();

    // Update vault in memory
    m_vault.addEntry(e);

    // Update table model
    int row = m_model->rowCount();
    m_model->insertRow(row);
    m_model->setItem(row, 0, new QStandardItem(QString::fromStdString(e.title)));
    m_model->setItem(row, 1, new QStandardItem(QString::fromStdString(e.username)));
    m_model->setItem(row, 2, new QStandardItem(QString::fromStdString(e.url)));

    // Save encrypted vault
    const std::string filePath = "vault.enc";
    if (!VaultStorage::saveEncrypted(m_vault, filePath, m_masterPassword)) {
        QMessageBox::warning(this, "Vault",
            "Failed to save vault after adding entry.");
    }
}

MainWindow::MainWindow(const std::string& masterPassword, QWidget* parent)
    : QMainWindow(parent),
    m_masterPassword(masterPassword),
    m_tableView(nullptr),
    m_model(nullptr)
{
    setupUi();
    loadVault();
}

void MainWindow::setupUi() {
    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);

    // Toolbar
    auto* toolbar = addToolBar("Main");
    QAction* addAction = toolbar->addAction("Add");
    connect(addAction, &QAction::triggered, this, &MainWindow::addEntry);

    m_tableView = new QTableView(central);
    m_model = new QStandardItemModel(this);

    m_model->setColumnCount(3);
    m_model->setHeaderData(0, Qt::Horizontal, "Title");
    m_model->setHeaderData(1, Qt::Horizontal, "Username");
    m_model->setHeaderData(2, Qt::Horizontal, "URL");

    m_tableView->setModel(m_model);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    layout->addWidget(m_tableView);

    setCentralWidget(central);
    setWindowTitle("PassVault");
    resize(800, 600);
}

void MainWindow::loadVault() {
    const std::string filePath = "vault.enc";

    Vault vault;

    if (std::filesystem::exists(filePath)) {
        if (!VaultStorage::loadEncrypted(vault, filePath, m_masterPassword)) {
            QMessageBox::warning(this, "Vault",
                "Failed to open encrypted vault.\n"
                "Wrong password or file is corrupted.");
            return;
        }
    }

    m_vault = vault;

    const auto& entries = m_vault.getEntries();
    m_model->setRowCount(static_cast<int>(entries.size()));

    int row = 0;
    for (const auto& e : entries) {
        m_model->setItem(row, 0, new QStandardItem(QString::fromStdString(e.title)));
        m_model->setItem(row, 1, new QStandardItem(QString::fromStdString(e.username)));
        m_model->setItem(row, 2, new QStandardItem(QString::fromStdString(e.url)));
        ++row;
    }
}
