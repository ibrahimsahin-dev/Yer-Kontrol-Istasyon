#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mavlink/common/mavlink.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // UDP Soketini Başlat
    udpSocket = new QUdpSocket(this);

    // 14550 Portunu dinlemeye başla (SITL genelde buraya gönderir)
    udpSocket->bind(QHostAddress::AnyIPv4, 14550);

    // Veri geldiğinde "processPendingDatagrams" fonksiyonunu çalıştır
    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::processPendingDatagrams);

    ui->lblMode->setText("SITL Bağlantısı Bekleniyor...");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBaglan_clicked()
{
    ui->lblDurum->setText("Durum: Drone'a Bağlandı! (Simüle)");

    ui->lblDurum->setStyleSheet("color: green; font-weight: bold; font-size: 12px;");
}

void MainWindow::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());

        // Gelen her byte'ı MAVLink parsar'a sok
        mavlink_message_t msg;
        mavlink_status_t status;

        for (int i = 0; i < datagram.size(); ++i) {
            uint8_t c = static_cast<uint8_t>(datagram[i]);

            // MAVLink mesajı başarıyla çözüldü mü?
            if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {

                // Mesaj Tipi HEARTBEAT ise
                if (msg.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
                    mavlink_heartbeat_t heartbeat;
                    mavlink_msg_heartbeat_decode(&msg, &heartbeat);

                    // Mod bilgisini al ve ekrana yaz
                    // ArduPilot'ta asıl mod "custom_mode" içindedir
                    QString modeName = getModeName(heartbeat.custom_mode);
                    ui->lblMode->setText("Mod: " + modeName);

                    // İsteğe bağlı: Base mode'dan "Armed" durumunu da kontrol edebilirsin
                    if (heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED) {
                        ui->lblMode->setStyleSheet("color: red; font-weight: bold;"); // Armed ise Kırmızı
                    } else {
                        ui->lblMode->setStyleSheet("color: green; font-weight: bold;"); // Disarmed ise Yeşil
                    }
                }
            }
        }
    }
}

// ArduCopter Mod Numaraları (Referans: ArduPilot Docs)
QString MainWindow::getModeName(uint32_t custom_mode) {
    switch (custom_mode) {
    case 0: return "STABILIZE";
    case 1: return "ACRO";
    case 2: return "ALT_HOLD";
    case 3: return "AUTO";
    case 4: return "GUIDED";
    case 5: return "LOITER";
    case 6: return "RTL";
    case 7: return "CIRCLE";
    case 9: return "LAND";
    case 11: return "DRIFT";
    case 13: return "SPORT";
    case 14: return "FLIP";
    case 15: return "AUTOTUNE";
    case 16: return "POSHOLD";
    case 17: return "BRAKE";
    default: return "BILINMIYOR (" + QString::number(custom_mode) + ")";
    }
}
