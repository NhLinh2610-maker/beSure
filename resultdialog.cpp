#include "resultdialog.h"
#include "ui_resultdialog.h"
#include <QClipboard>
#include <QStyle>
ResultDialog::ResultDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResultDialog)
{
    ui->setupUi(this);
    ui->btnCpyResult->setIcon(style()->standardIcon(QStyle::SP_DialogSaveAllButton));
}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::setOutputText(const QString &text)
{
    ui->pteOutput->setPlainText(text);
}

void ResultDialog::on_btnCpyResult_clicked()
{
    QString result = ui->pteOutput->toPlainText();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(result);
    ui->btnCpyResult->setText("Copied!");
}

