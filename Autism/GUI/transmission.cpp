#include "transmission.h"
#include <string.h>
#include <QQmlProperty>

#define DEVICE1_IP  "192.168.1.1"
#define DEVICE2_IP  "192.168.1.2"
#define DEVICE3_IP  "192.168.1.3"
#define DEVICE4_IP  "192.168.1.4"

#define MUSIC_MAX 6

Transmission::Transmission(QObject *ui,QObject *parent) : QObject(parent)
{
    //init
    charBuffer = '0';
    isBufferEmpty = true;
    commandMode=false;

    connect(&tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(&tcpClient, SIGNAL(connected()), this, SLOT(connected()));

    root = ui;
    upDevices = 0;
    reqDeviceList();
    play_music_id=1;

    lampIsOn=0;
    musicIsOn=0;
    lightColor = QColor(100,100,100);
}

Transmission::~Transmission()
{
    tcpClient.close();
}

int Transmission::startTransfer(const char* command)
{
    if (tcpClient.isOpen())
    {
        //tof_on_screen( "\ntransfering command " );
        int bytesToWrite = tcpClient.write(command);
        //tof_on_screen( QString::number(bytesToWrite) );
        //tof_on_screen( " byte written " );
        return 0;
    }
    else
    {
        tof_on_screen( "\nData cannot transfer, no connection established" );
    }
}

void Transmission::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

    QString error =  QString("\nNetwork error\nThe following error occurred: %1.").arg(tcpClient.errorString());
    tof_on_screen( error );
    tcpClient.close();
    emit errorConnection();

}

void Transmission::connected()
{
    qDebug() << "connected";
    connect(&tcpClient, SIGNAL(readyRead()), this, SLOT(dataReady()));
    tof_on_screen( "\nconnected" );
    if( !listRequested )
    {
        QQmlProperty::write(root, "lamp_con_id", lamp_id);
        QMetaObject::invokeMethod(root, "lamp_connected"); //light on
        if (lampIsOn)
        {
            startTransfer("1\n");

            QString command = "3";
            command += QString("%1").arg(lightColor.red()  , 3, 10, QChar('0'));
            command += QString("%1").arg(lightColor.green(), 3, 10, QChar('0'));
            command += QString("%1").arg(lightColor.blue() , 3, 10, QChar('0'));
            command += "\n";
            qDebug() << command;
            startTransfer(command.toStdString().c_str());
        }
        else
        {
            light_off();
        }
        if (musicIsOn)
        {
            startTransfer("41\n");
        }
        else
        {
            startTransfer("6\n");
        }
    }
}

//Request for device list
void Transmission::reqDeviceList()
{
    listRequested = true;
    tcpClient.connectToHost(QHostAddress(DEVICE1_IP), 7778 );
    qDebug() << "connecting to " << DEVICE1_IP;
    tof_on_screen( "\nconnecting to " );
    tof_on_screen( DEVICE1_IP );
}

void Transmission::dataReady()
{
    QString recieve = tcpClient.readAll();
    qDebug() << "Received: " << recieve;
    if (listRequested)
    {
        if (recieve.size() < 5)
        {
            int devCount; //device count
            devCount = recieve.toInt();
            if ( upDevices != devCount)//list changed
            {
                upDevices = devCount;
                QQmlProperty::write(root, "lamp_count", upDevices);
                QMetaObject::invokeMethod(root, "lamp_enable"); //update ui device list
            }
            listRequested = false;
            tcpClient.disconnectFromHost();
            tcpClient.waitForBytesWritten(1000);
            tcpClient.abort();
        }
        else
            startTransfer("q\n");//query
    }
}

void Transmission::start(QString IP)
{
    if (tcpClient.isOpen())
    {
        QString command = "5\n";
        startTransfer(command.toStdString().c_str());
        tof_on_screen( "\nclosing connection" );
        QMetaObject::invokeMethod(root, "lamp_disconnected"); //light off
        tcpClient.disconnectFromHost();
        tcpClient.close();
    }
    tcpClient.waitForBytesWritten(1000);
    tcpClient.abort();
    tcpClient.connectToHost(QHostAddress(IP), 7778 );
    qDebug() << "connecting to " << IP;
    tof_on_screen( "\nconnecting to " );
    tof_on_screen( IP );
}

void Transmission::set_lamp(int id)
{
    QString ip_address;
    switch (id)
    {
        case 0:
            ip_address = DEVICE1_IP;
            break;
        case 1:
            ip_address = DEVICE2_IP;
            break;
        case 2:
            ip_address = DEVICE3_IP;
            break;
        case 3:
            ip_address = DEVICE4_IP;
            break;
        default:
            return;
    }
    lamp_id = id;
    start(ip_address);
}

void Transmission::change_color2(QString value)
{
    int r,g,b;
    QString temp;
    QString command = "3";
    temp = value[1];
    temp += value[2];
    r = temp.toInt(0,16);
    temp = value[3];
    temp += value[4];
    g = temp.toInt(0,16);
    temp = value[5];
    temp += value[6];
    b = temp.toInt(0,16);
    command += QString("%1").arg(r  , 3, 10, QChar('0'));
    command += QString("%1").arg(g, 3, 10, QChar('0'));
    command += QString("%1").arg(b , 3, 10, QChar('0'));
    command += "\n";
    qDebug() << command;
    startTransfer(command.toStdString().c_str());
}

void Transmission::change_color(int id, int value)
{
    ColorButtonID button_id = static_cast<ColorButtonID>(id);
    QString command = "3";
    switch (button_id)
    {
        case GREEN_BUTTON:
            lightColor.setGreen(value);
            break;
        case BLUE_BUTTON:
            lightColor.setBlue(value);
            break;
        case RED_BUTTON:
            lightColor.setRed(value);
            break;
    }
    command += QString("%1").arg(lightColor.red()  , 3, 10, QChar('0'));
    command += QString("%1").arg(lightColor.green(), 3, 10, QChar('0'));
    command += QString("%1").arg(lightColor.blue() , 3, 10, QChar('0'));
    command += "\n";
    qDebug() << command;
    startTransfer(command.toStdString().c_str());
}

void Transmission::music_random()
{
    play_music_id = qrand()%MUSIC_MAX;
    QString command = QString("4%1\n").arg(play_music_id);

    startTransfer(command.toStdString().c_str());
}

void Transmission::music_play()
{
    musicIsOn = 1;
    QString command = "2\n";
    startTransfer(command.toStdString().c_str());
}

void Transmission::music_forward()
{
    if (play_music_id < MUSIC_MAX)
        play_music_id++;
    else
        play_music_id = 1;
    QString command = QString("4%1\n").arg(play_music_id);
    startTransfer(command.toStdString().c_str());
    qDebug() << "Next music " << play_music_id;
}

void Transmission::music_prev()
{
    if (play_music_id > 1)
        play_music_id--;
    else
        play_music_id = MUSIC_MAX-1;
    QString command = QString("4%1\n").arg(play_music_id);
    startTransfer(command.toStdString().c_str());
    qDebug() << "Prev music";
}

void Transmission::music_stop()
{
    musicIsOn = 0;
    QString command = "6\n";
    startTransfer(command.toStdString().c_str());
}


void Transmission::light_off()
{
    lampIsOn = 0;
    QString command = "7\n";
    startTransfer(command.toStdString().c_str());
}

void Transmission::light_on()
{
    lampIsOn = 1;
    QString command = "1\n";
    startTransfer(command.toStdString().c_str());
}

void Transmission::rainbow()
{
    QString command = "8\n";
    startTransfer(command.toStdString().c_str());
}


void Transmission::tof_on_screen(QString message)
{
    QQmlProperty::write(root, "message", message);
    QMetaObject::invokeMethod(root, "tof_on_screen"); //show warning to
}
