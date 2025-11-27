#include "UnlockDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QMessageBox>

UnlockDialog::UnlockDialog(QWidget* parent)
    : QDialog(parent),
    m_passwordEdit(nullptr),
    m_showPasswordCheck(nullptr),
    m_buttonBox(nullptr)
{
    setWindowTitle("Unlock Vault");

    auto* layout = new QVBoxLayout(this);

    auto* label = new QLabel("Enter master password:", this);
    layout->addWidget(label);

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(m_passwordEdit);

    m_showPasswordCheck = new QCheckBox("Show password", this);
    layout->addWidget(m_showPasswordCheck);

    m_buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        this
    );
    layout->addWidget(m_buttonBox);

    connect(m_showPasswordCheck, &QCheckBox::toggled,
        this, &UnlockDialog::onShowPasswordToggled);

    connect(m_buttonBox, &QDialogButtonBox::accepted,
        this, &UnlockDialog::onAccept);

    connect(m_buttonBox, &QDialogButtonBox::rejected,
        this, &UnlockDialog::reject);
}

QString UnlockDialog::password() const {
    return m_passwordEdit->text();
}

void UnlockDialog::onShowPasswordToggled(bool checked) {
    m_passwordEdit->setEchoMode(
        checked ? QLineEdit::Normal : QLineEdit::Password
    );
}

void UnlockDialog::onAccept() {
    if (m_passwordEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Unlock Vault",
            "Please enter a master password.");
        return;
    }
    accept();
}
