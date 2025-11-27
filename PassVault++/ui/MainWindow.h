#pragma once

#include <QtWidgets/QMainWindow>
#include "../core/Vault.h"

class QTableView;
class QStandardItemModel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void setupUi();
    void loadVault();

    Vault m_vault;
    QTableView* m_tableView;
    QStandardItemModel* m_model;
};