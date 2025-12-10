#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>

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
    // Sinyal ile tetiklenen fonksiyonlar buraya
    void on_btnBaglan_clicked();
    void processPendingDatagrams();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;

    // Yardımcı fonksiyonlar buraya (Çift tanımlama silindi)
    QString getModeName(uint32_t custom_mode);
};
#endif // MAINWINDOW_H
