#pragma once

#include <QtWidgets/QMainWindow>
#include <string>
#include "../core/Vault.h"

class QTableView;
class QStandardItemModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(const std::string& masterPassword,
        QWidget* parent = nullptr);

private:
    void setupUi();
    void loadVault();
    void addEntry();

    std::string m_masterPassword;
    Vault m_vault;
    QTableView* m_tableView;
    QStandardItemModel* m_model;
};
