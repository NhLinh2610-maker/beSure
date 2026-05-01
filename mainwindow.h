#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_rbtnRSAEncryptE_clicked(bool checked);

    void on_rbtnRSASignE_clicked(bool checked);

    void on_cbbAlgoD_currentIndexChanged(int index);

    void on_cbbAlgoE_currentIndexChanged(int index);

    void on_rbtnRSADecrypt_clicked(bool checked);

    void on_rbtnRSAVerify_clicked(bool checked);


    void on_btnStart_clicked();


    void on_btnOpenFileE_clicked();

    void on_btnKeyPstE_clicked();

    void on_btnPstKeyD_clicked();

    void on_btnPstKeyRSAD_clicked();

    void on_btnPstSignature_clicked();

    void on_btnPstChecksum_clicked();

    void on_btnCpyPbKey_clicked();

    void on_btnCpyPVKey_clicked();

    void on_btnCpyHash_clicked();

    void on_btnCpyMD5_clicked();

    void on_btnCpySHA1_clicked();

    void on_btnCpySHA256_clicked();

    void on_btnCpySHA512_clicked();

    void on_btnOpenFileD_clicked();

    void on_btnBrowse_clicked();

    void on_btnGen_clicked();

    void on_btnStartD_clicked();

    void on_btnHashCal_clicked();

    void on_btnCheck_clicked();

    void processHashAndCheck(bool autoCheck);
private:
    Ui::MainWindow *ui;
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};
#endif // MAINWINDOW_H
