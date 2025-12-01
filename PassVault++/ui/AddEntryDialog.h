#pragma once

#include <QtWidgets/QDialog>
#include <string>
#include "../core/PasswordEntry.h"

class QLineEdit;
class QTextEdit;
class QDialogButtonBox;

class AddEntryDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddEntryDialog(QWidget* parent = nullptr);

    PasswordEntry toEntry() const;

private:
    QLineEdit* m_titleEdit;
    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
    QLineEdit* m_urlEdit;
    QTextEdit* m_notesEdit;
    QDialogButtonBox* m_buttonBox;

private slots:
    void onAccept();
};
