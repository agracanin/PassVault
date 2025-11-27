#pragma once

#include <QtWidgets/QDialog>

class QLineEdit;
class QCheckBox;
class QDialogButtonBox;

class UnlockDialog : public QDialog {
    Q_OBJECT

public:
    explicit UnlockDialog(QWidget* parent = nullptr);

    QString password() const;

private:
    QLineEdit* m_passwordEdit;
    QCheckBox* m_showPasswordCheck;
    QDialogButtonBox* m_buttonBox;

private slots:
    void onShowPasswordToggled(bool checked);
    void onAccept();
};
