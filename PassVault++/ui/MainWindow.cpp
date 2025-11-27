#include "MainWindow.h"

#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QAbstractItemView>
#include <QtGui/QStandardItemModel>
#include <filesystem>

#include "../core/VaultStorage.h"

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
