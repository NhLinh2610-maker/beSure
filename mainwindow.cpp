#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTabWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QGroupBox>
#include <QTabBar>
#include "resultdialog.h"
#include <QClipboard>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "RSA.h"
#include "playfair.h"
#include <QtConcurrent>
#include <QFutureWatcher>
#include "Checksum.h"
#include <QThread>
#include <QStyle>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTabBar *tabBar = ui->tabWidget->findChild<QTabBar *>();
    if (tabBar)
    {
        tabBar->setExpanding(true);
    }
    on_cbbAlgoE_currentIndexChanged(0);
    on_cbbAlgoD_currentIndexChanged(0);
    ui->pbC->setValue(0);
    ui->btnCpyMD5->setIcon(style()->standardIcon(QStyle::SP_DialogSaveAllButton));
    ui->btnCpySHA256->setIcon(style()->standardIcon(QStyle::SP_DialogSaveAllButton));
    ui->btnCpySHA512->setIcon(style()->standardIcon(QStyle::SP_DialogSaveAllButton));
    ui->btnCpySHA1->setIcon(style()->standardIcon(QStyle::SP_DialogSaveAllButton));
    ui->btnCpyPVKey->setIcon(style()->standardIcon(QStyle::SP_DialogSaveAllButton));
    ui->btnCpyPbKey->setIcon(style()->standardIcon(QStyle::SP_DialogSaveAllButton));
    ui->btnCpyHash->setIcon(style()->standardIcon(QStyle::SP_DialogSaveAllButton));

    ui->btnPstChecksum->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    ui->btnPstKeyD->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    ui->btnPstKeyRSAD->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    ui->btnKeyPstE->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    ui->btnPstSignature->setIcon(style()->standardIcon(QStyle::SP_FileIcon));


    ui->btnBrowse->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
    ui->btnOpenFileE->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
    ui->btnOpenFileD->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));

    ui->btnHashCal->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    this->setWindowIcon(QIcon(":/icons/logo.png"));
    this->setAcceptDrops(true);

    ui->btnHashCal->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_rbtnRSAEncryptE_clicked(bool checked)
{
    if (checked) {
        ui->lblHash->hide();
        ui->leHash->hide();
        ui->btnCpyHash->hide();
    }
}


void MainWindow::on_rbtnRSASignE_clicked(bool checked)
{
    if (checked) {
        ui->lblHash->show();
        ui->leHash->show();
        ui->btnCpyHash->show();
    }
}


void MainWindow::on_cbbAlgoD_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->gbRSAD->hide();
        ui->rbtnRSADecrypt->hide();
        ui->rbtnRSAVerify->hide();
        ui->lblKeyD->show();
        ui->leKeyD->show();
        ui->btnPstKeyD->show();
        ui->pteInputD->setPlaceholderText("Type in your ciphertext...");
    } else if (index == 1)
    {
        ui->gbRSAD->show();
        ui->rbtnRSADecrypt->show();
        ui->rbtnRSAVerify->show();
        ui->lblKeyD->hide();
        ui->leKeyD->hide();
        ui->btnPstKeyD->hide();
        ui->pteInputD->setPlaceholderText("Type in your ciphertext...");
        if (ui->rbtnRSADecrypt->isChecked())
        {
            ui->lblSignature->hide();
            ui->leSignature->hide();
            ui->btnPstSignature->hide();
            ui->leKeyRSAD->setPlaceholderText("Type in your Private key (<d>, <n>)...");
        }
        else
        {
            ui->lblSignature->show();
            ui->leSignature->show();
            ui->btnPstSignature->show();
            ui->leKeyRSAD->setPlaceholderText("Type in your Public key (<e>, <n>)...");
            ui->pteInputD->setPlaceholderText("Type in your original text...");
        }
    }
}


void MainWindow::on_cbbAlgoE_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->lblKeyE->show();
        ui->leKeyE->show();
        ui->btnKeyPstE->show();
        ui->rbtnRSAEncryptE->hide();
        ui->rbtnRSASignE->hide();
        ui->gbRSAE->hide();
    } else if (index == 1)
    {
        ui->rbtnRSAEncryptE->show();
        ui->rbtnRSASignE->show();
        ui->gbRSAE->show();
        ui->lblKeyE->hide();
        ui->leKeyE->hide();
        ui->btnKeyPstE->hide();
        if (ui->rbtnRSAEncryptE->isChecked())
        {
            ui->lblHash->hide();
            ui->leHash->hide();
            ui->btnCpyHash->hide();
        }
        else
        {
            ui->lblHash->show();
            ui->leHash->show();
            ui->btnCpyHash->show();
        }
    }
}


void MainWindow::on_rbtnRSADecrypt_clicked(bool checked)
{
    if (checked)
    {
        ui->lblSignature->hide();
        ui->leSignature->hide();
        ui->btnPstSignature->hide();
        ui->leKeyRSAD->setPlaceholderText("Type in your Private key (<d>, <n>)...");
        ui->pteInputD->setPlaceholderText("Type in your cipher text...");
    }
}


void MainWindow::on_rbtnRSAVerify_clicked(bool checked)
{
    if (checked)
    {
        ui->lblSignature->show();
        ui->leSignature->show();
        ui->btnPstSignature->show();
        ui->leKeyRSAD->setPlaceholderText("Type in your Public key (<e>, <n>)...");
        ui->pteInputD->setPlaceholderText("Type in your original text...");
    }
}




void MainWindow::on_btnStart_clicked()
{
    QString qMessage = ui->pteInputE->toPlainText();
    if (qMessage.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please type in something!");
        return;
    }
    std::string message = qMessage.toStdString();

    int algoIndex = ui->cbbAlgoE->currentIndex();
    QString qKeyPf = ui->leKeyE->text();
    bool isRsaEncrypt = ui->rbtnRSAEncryptE->isChecked();
    bool isRsaSign = ui->rbtnRSASignE->isChecked();
    QString pubKeyStr = ui->lePBKey->text();
    QString privKeyStr = ui->lePVKey->text();

    if (algoIndex == 1 && isRsaSign) {
        std::string hashedText = SHA256::hash(message);
        ui->leHash->setText(QString::fromStdString(hashedText));
    }

    ui->btnStart->setEnabled(false);
    ui->btnStart->setText("Processing...");

    QFutureWatcher<QString> *watcher = new QFutureWatcher<QString>(this);

    connect(watcher, &QFutureWatcher<QString>::finished, this, [this, watcher]() {

        ui->btnStart->setEnabled(true);
        ui->btnStart->setText("START");

        QString finalResult = watcher->result();

        if (finalResult.startsWith("ERROR:")) {
            QMessageBox::warning(this, "Error", finalResult.mid(6));
        } else {
            ResultDialog dialog(this);
            if (ui->rbtnRSASignE->isChecked()) dialog.setWindowTitle("Digital Signature");
            else dialog.setWindowTitle("Ciphertext");
            dialog.setOutputText(finalResult);
            dialog.exec();
        }

        watcher->deleteLater();
    });

    QFuture<QString> future = QtConcurrent::run([=]() -> QString {


        if (algoIndex == 0) {
            if (qKeyPf.isEmpty()) return "ERROR: Please type in a key!";
            PlayfairCipher pf;
            pf.setConfig(5, 'X', 'Y');
                return QString::fromStdString(pf.transform(message, qKeyPf.toStdString(), true));
        }
        else if (algoIndex == 1) {
            RSACipher rsa;
            if (isRsaEncrypt) {
                QStringList parts = pubKeyStr.split(",");
                if (parts.size() != 2) return "ERROR: Incorrect public key format. Correct format: '<e>, <n>'";
                bool ok1, ok2;
                long long e = parts[0].trimmed().toLongLong(&ok1);
                long long n = parts[1].trimmed().toLongLong(&ok2);

                if (!ok1 || !ok2 || n == 0) return "ERROR: Invalid arguments!";

                return QString::fromStdString(rsa.encrypt(message, e, n));
            }
            else if (isRsaSign) {
                QStringList parts = privKeyStr.split(",");
                if (parts.size() != 2) return "ERROR: Incorrect private key format. Correct format: '<d>, <n>'";
                bool ok1, ok2;
                long long d = parts[0].trimmed().toLongLong(&ok1);
                long long n = parts[1].trimmed().toLongLong(&ok2);

                if (!ok1 || !ok2 || n == 0) return "ERROR: Invalid arguments!";

                return QString::fromStdString(rsa.sign(message, d, n));
            }
        }
        return "ERROR: Unknown!";
    });

    watcher->setFuture(future);
}





void MainWindow::on_btnOpenFileE_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Choose text from a file",
                                                    "",
                                                    "Text Files (*.txt);;All Files (*.*)");
    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cant open this file. Please check the file permission!");
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();

    ui->pteInputE->setPlainText(content);

    file.close();
    ui->leKeyE->clear();
    ui->lePBKey->clear();
    ui->lePVKey->clear();
    ui->leHash->clear();
}


void MainWindow::on_btnKeyPstE_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString textToPaste = clipboard->text();
    ui->leKeyE->setText(textToPaste);
}


void MainWindow::on_btnPstKeyD_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString textToPaste = clipboard->text();
    ui->leKeyD->setText(textToPaste);
}


void MainWindow::on_btnPstKeyRSAD_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString textToPaste = clipboard->text();
    ui->leKeyRSAD->setText(textToPaste);
}


void MainWindow::on_btnPstSignature_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString textToPaste = clipboard->text();
    ui->leSignature->setText(textToPaste);
}


void MainWindow::on_btnPstChecksum_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString textToPaste = clipboard->text();
    ui->leChecksum->setText(textToPaste);
}


void MainWindow::on_btnCpyPbKey_clicked()
{
    QString result = ui->lePBKey->text();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(result);
    ui->statusbar->showMessage("Copied public key!", 1250);
}


void MainWindow::on_btnCpyPVKey_clicked()
{
    QString result = ui->lePVKey->text();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(result);
    ui->statusbar->showMessage("Copied private key!", 1250);
}


void MainWindow::on_btnCpyHash_clicked()
{
    QString result = ui->leHash->text();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(result);
    ui->statusbar->showMessage("Copied the hash SHA256 of this message!", 1250);
}


void MainWindow::on_btnCpyMD5_clicked()
{
    QString result = ui->leMD5->text();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(result);
    ui->statusbar->showMessage("Copied MD5!", 1250);
}


void MainWindow::on_btnCpySHA1_clicked()
{
    QString result = ui->leSHA1->text();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(result);
    ui->statusbar->showMessage("Copied SHA1!", 1250);
}


void MainWindow::on_btnCpySHA256_clicked()
{
    QString result = ui->leSHA256->text();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(result);
    ui->statusbar->showMessage("Copied SHA256!", 1250);
}


void MainWindow::on_btnCpySHA512_clicked()
{
    QString result = ui->leSHA512->text();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(result);
    ui->statusbar->showMessage("Copied SHA512!", 1250);
}


void MainWindow::on_btnOpenFileD_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Choose text from a file",
                                                    "",
                                                    "Text Files (*.txt);;All Files (*.*)");
    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cant open this file. Please check the file permission!");
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();

    ui->pteInputD->setPlainText(content);

    file.close();
    ui->leKeyD->clear();
    ui->leKeyRSAD->clear();
    ui->leSignature->clear();
}


void MainWindow::on_btnBrowse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Choose a file",
                                                    "",
                                                    "All Files (*.*)");
    if (fileName.isEmpty()) {
        return;
    }

    ui->leChooseFile->setText(fileName);
    ui->leMD5->clear();
    ui->leSHA1->clear();
    ui->leSHA256->clear();
    ui->leSHA512->clear();
    ui->pbC->setValue(0);
    ui->leChecksum->clear();
    ui->btnHashCal->setEnabled(true);
}


void MainWindow::on_btnGen_clicked()
{
    RSACipher rsa;
    RSAKeyPair keys = rsa.generateKeyPair();
    QString publicKeyStr = QString::number(keys.e) + ", " + QString::number(keys.n);
    QString privateKeyStr = QString::number(keys.d) + ", " + QString::number(keys.n);
    ui->lePBKey->setText(publicKeyStr);
    ui->lePVKey->setText(privateKeyStr);
    if (ui->leHash->isVisible())
    {
        QString qMessage = ui->pteInputE->toPlainText();
        std::string stdMessage = qMessage.toStdString();
        std::string hashedText = SHA256::hash(stdMessage);
        ui->leHash->setText(QString::fromStdString(hashedText));
    }
}


void MainWindow::on_btnStartD_clicked()
{
    QString qMessage = ui->pteInputD->toPlainText();
    if (qMessage.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please type in something!");
        return;
    }
    std::string message = qMessage.toStdString();

    int algoIndex = ui->cbbAlgoD->currentIndex();
    QString qKeyPf = ui->leKeyD->text();
    bool isRsaDecrypt = ui->rbtnRSADecrypt->isChecked();
    bool isRsaVerify = ui->rbtnRSAVerify->isChecked();
    QString rsaKeyStr = ui->leKeyRSAD->text();
    QString rsaSignature = ui->leSignature->text();
    ui->btnStartD->setEnabled(false);
    ui->btnStartD->setText("Processing...");

    QFutureWatcher<QString> *watcher = new QFutureWatcher<QString>(this);
    connect(watcher, &QFutureWatcher<QString>::finished, this, [this, watcher]() {

        ui->btnStartD->setEnabled(true);
        ui->btnStartD->setText("START");

        QString finalResult = watcher->result();

        if (finalResult.startsWith("ERROR:")) {
            QMessageBox::warning(this, "Error", finalResult.mid(6));
        } else {
            ResultDialog dialog(this);
            if (ui->rbtnRSAVerify->isChecked()) dialog.setWindowTitle("Signature checking");
            else dialog.setWindowTitle("Plaintext");
            dialog.setOutputText(finalResult);
            dialog.exec();
        }

        watcher->deleteLater();
    });
    QFuture<QString> future = QtConcurrent::run([=]() -> QString {
        if (algoIndex == 0) {
            if (qKeyPf.isEmpty()) return "ERROR: Please type in a key!";
            PlayfairCipher pf;
            pf.setConfig(5, 'X', 'Y');
            return QString::fromStdString(pf.transform(message, qKeyPf.toStdString(), false));
        }
        else if (algoIndex == 1) {
            RSACipher rsa;
            if (isRsaDecrypt) {
                QStringList parts = rsaKeyStr.split(",");
                if (parts.size() != 2) return "ERROR: Incorrect private key format. Correct format: '<d>, <n>'";

                bool ok1, ok2;
                long long d = parts[0].trimmed().toLongLong(&ok1);
                long long n = parts[1].trimmed().toLongLong(&ok2);

                if (!ok1 || !ok2 || n == 0) return "ERROR: Invalid arguments!";

                try {
                    return QString::fromStdString(rsa.decrypt(message, d, n));
                } catch (const std::exception& ex) {
                    return "ERROR: Malformed Ciphertext! Make sure it is a valid Hex string.";
                }
            }
            else if (isRsaVerify) {
                if (rsaSignature.trimmed().isEmpty()) return "ERROR: Please provide a Signature to verify!";

                QStringList parts = rsaKeyStr.split(",");
                if (parts.size() != 2) return "ERROR: Incorrect public key format. Correct format: '<e>, <n>'";

                bool ok1, ok2;
                long long e = parts[0].trimmed().toLongLong(&ok1);
                long long n = parts[1].trimmed().toLongLong(&ok2);

                if (!ok1 || !ok2 || n == 0) return "ERROR: Invalid arguments!";

                try {
                    bool matched = rsa.verify(message, rsaSignature.trimmed().toStdString(), e, n);
                    if (matched) return "The signature MATCHES!";
                    return "The signature does NOT MATCH!";
                } catch (const std::exception& ex) {
                    return "ERROR: Malformed Signature! Make sure it is a valid Hex string.";
                }
            }
        }
        return "ERROR: Unknown!";
    });

    watcher->setFuture(future);

}


void MainWindow::on_btnHashCal_clicked()
{
    processHashAndCheck(false);

}


void MainWindow::on_btnCheck_clicked()
{
    processHashAndCheck(true);
}

void MainWindow::processHashAndCheck(bool autoCheck)
{
    QString filePath = ui->leChooseFile->text();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please choose a file!");
        return;
    }

    QString expected = ui->leChecksum->text().trimmed().toLower();
    if (autoCheck && expected.isEmpty()) {
        QMessageBox::warning(this, "Checksum empty!", "Please type in a checksum!");
        return;
    }

    if (autoCheck && !ui->leMD5->text().isEmpty()) {
        QString matchAlgo = "";
        if (expected == ui->leMD5->text().toLower()) matchAlgo = "MD5";
        else if (expected == ui->leSHA1->text().toLower()) matchAlgo = "SHA1";
        else if (expected == ui->leSHA256->text().toLower()) matchAlgo = "SHA256";
        else if (expected == ui->leSHA512->text().toLower()) matchAlgo = "SHA512";

        if (!matchAlgo.isEmpty()) QMessageBox::information(this, "Result", "This checksum matchs : " + matchAlgo);
        else QMessageBox::critical(this, "Result", "This checksum does NOT MATCH with any of the calculated hashes!");
        return;
    }

    ui->btnHashCal->setEnabled(false);
    ui->btnCheck->setEnabled(false);
    ui->pbC->setValue(0);

    QThread *thread = new QThread;
    HashWorker *worker = new HashWorker(filePath);
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &HashWorker::process);
    connect(worker, &HashWorker::progress, ui->pbC, &QProgressBar::setValue);

    connect(worker, &HashWorker::resultReady, this, [=](QString md5, QString sha1, QString sha256, QString sha512){
        ui->leMD5->setText(md5);
        ui->leSHA1->setText(sha1);
        ui->leSHA256->setText(sha256);
        ui->leSHA512->setText(sha512);

        if (autoCheck) {
            QString matchAlgo = "";
            if (expected == md5.toLower()) matchAlgo = "MD5";
            else if (expected == sha1.toLower()) matchAlgo = "SHA1";
            else if (expected == sha256.toLower()) matchAlgo = "SHA256";
            else if (expected == sha512.toLower()) matchAlgo = "SHA512";

            if (!matchAlgo.isEmpty()) QMessageBox::information(this, "Result", "This checksum matchs : " + matchAlgo);
            else QMessageBox::critical(this, "Result", "This checksum does NOT MATCH with any of the calculated hashes!");
        }
    });

    connect(worker, &HashWorker::error, this, [=](QString msg){
        QMessageBox::critical(this, "An error has occured while trying to read this file!", msg);
    });

    connect(worker, &HashWorker::finished, thread, &QThread::quit);
    connect(worker, &HashWorker::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    connect(thread, &QThread::finished, this, [=](){
        ui->btnCheck->setEnabled(true);
    });

    thread->start();
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (ui->tabWidget->currentIndex() == 2 && event->mimeData()->hasUrls()) {
        event->acceptProposedAction(); // Mở cửa
    } else {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (ui->tabWidget->currentIndex() != 2) return;

    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        QString filePath = mimeData->urls().at(0).toLocalFile();

        ui->leChooseFile->setText(filePath);

        ui->leMD5->clear();
        ui->leSHA1->clear();
        ui->leSHA256->clear();
        ui->leSHA512->clear();
        ui->pbC->setValue(0);
        ui->leChecksum->clear();
        processHashAndCheck(false);
    }
}
