#include "AddEntryDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QMessageBox>

#include <chrono>
#include <iomanip>
#include <sstream>

AddEntryDialog::AddEntryDialog(QWidget* parent)
    : QDialog(parent),
    m_titleEdit(nullptr),
    m_usernameEdit(nullptr),
    m_passwordEdit(nullptr),
    m_urlEdit(nullptr),
    m_notesEdit(nullptr),
    m_buttonBox(nullptr)
{
    setWindowTitle("Add Entry");

    auto* mainLayout = new QVBoxLayout(this);
    auto* formLayout = new QFormLayout();

    m_titleEdit = new QLineEdit(this);
    m_usernameEdit = new QLineEdit(this);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_urlEdit = new QLineEdit(this);
    m_notesEdit = new QTextEdit(this);

    formLayout->addRow("Title:", m_titleEdit);
    formLayout->addRow("Username:", m_usernameEdit);
    formLayout->addRow("Password:", m_passwordEdit);
    formLayout->addRow("URL:", m_urlEdit);
    formLayout->addRow("Notes:", m_notesEdit);

    mainLayout->addLayout(formLayout);

    m_buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        this
    );
    mainLayout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted,
        this, &AddEntryDialog::onAccept);
    connect(m_buttonBox, &QDialogButtonBox::rejected,
        this, &AddEntryDialog::reject);
}

static std::string makeIsoTimestamp() {
    using clock = std::chrono::system_clock;
    auto now = clock::now();
    std::time_t tt = clock::to_time_t(now);
    std::tm tm{};
    localtime_s(&tm, &tt);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return oss.str();
}

PasswordEntry AddEntryDialog::toEntry() const {
    PasswordEntry e;
    e.id = ""; // will be filled by caller
    e.title = m_titleEdit->text().toStdString();
    e.username = m_usernameEdit->text().toStdString();
    e.password = m_passwordEdit->text().toStdString();
    e.url = m_urlEdit->text().toStdString();
    e.notes = m_notesEdit->toPlainText().toStdString();
    e.lastModified = makeIsoTimestamp();
    return e;
}

void AddEntryDialog::onAccept() {
    if (m_titleEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Add Entry",
            "Title is required.");
        return;
    }
    accept();
}
