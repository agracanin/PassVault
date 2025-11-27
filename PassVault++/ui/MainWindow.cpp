#include "MainWindow.h"

#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QAbstractItemView>
#include <QtGui/QStandardItemModel>

#include "../core/VaultStorage.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    m_tableView(nullptr),
    m_model(nullptr)
{
    setupUi();
    loadVault();
}

void MainWindow::setupUi() {
    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);

    m_tableView = new QTableView(central);
    m_model = new QStandardItemModel(this);

    // Define columns
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
    // For now, just load with a hardcoded password and path
    // Later we'll askuser
    const std::string filePath = "vault.enc";
    const std::string masterPassword = "test"; // TEMP

    Vault vault;
    if (!VaultStorage::loadEncrypted(vault, filePath, masterPassword)) {
        // For now, show a message if we can't load
        QMessageBox::warning(this, "Vault",
            "Failed to load encrypted vault.\n"
            "Later this will prompt for a password.");
        return;
    }

    m_vault = vault;

    // Populate table
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
