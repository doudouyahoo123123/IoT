#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_client = new QMQTT::Client(QHostAddress::LocalHost, 1883, this);
    connect(m_client, SIGNAL(received(QMQTT::Message)), this, SLOT(slot_mqtt_recvived(QMQTT::Message)));
/*doudou*/
//    m_client->setHostName("192.168.237.133");
//    m_client->setPort(1883);
//    m_client->setClientId("MQTT_FX_Client");
//    m_client->setUsername("doudou");
//    m_client->setPassword("ww123123");
//    m_client->setVersion(QMQTT::V3_1_0);
//    m_client->connectToHost();
//    if(m_client->isConnectedToHost() == false)
//    {
//        qDebug() << "connection failed";
//    }
/*doudou*/
    ui->cbxQosSubscribe->addItem("0");
    ui->cbxQosSubscribe->addItem("1");
    ui->cbxQosSubscribe->addItem("2");

    ui->cbxQosPublish->addItem("0");
    ui->cbxQosPublish->addItem("1");
    ui->cbxQosPublish->addItem("2");

    m_timer1s = new QTimer(this);
    m_timer1s->start(1000);
    connect(this->m_timer1s, &QTimer::timeout, [this](void){
        if(this->m_client->isConnectedToHost() == false){
            ui->pbConnectToHost->setText("连接");
            ui->cbxSubscribedTopic->clear();
        }
    });


    //读取本地配置
    QFile f(QCoreApplication::applicationDirPath() + "/config.ini");
    if(f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open file";
        ui->leHostaddr->setText(get_cfg_str(f.readLine()));
        ui->lePort->setText(get_cfg_str(f.readLine()));
        ui->leClientId->setText(get_cfg_str(f.readLine()));
        ui->leUsernam->setText(get_cfg_str(f.readLine()));
        ui->lePassword->setText(get_cfg_str(f.readLine()));
        ui->leKeepAlive->setText(get_cfg_str(f.readLine()));
        f.close();
    }
}


Widget::~Widget()
{
    if(m_client->isConnectedToHost())
    {
        for(int i = 0; i != ui->cbxSubscribedTopic->count(); i++)
        {
            m_client->unsubscribe(ui->cbxSubscribedTopic->itemText(i));
        }
        m_client->disconnected();
    }
    delete ui;
}




QString Widget::get_cfg_str(const QString &str)
{
    std::string rawstr = str.trimmed().toStdString();
    int index = rawstr.find("=") + 1;

    if(index > 0)
    {
        std::string cfgstr = rawstr.substr(index, rawstr.size() - 1);
        return QString(cfgstr.c_str());
    }
    return NULL;
}



void Widget::on_pbConnectToHost_clicked()
{
    if(m_client->isConnectedToHost() == false)
    {
        connect(m_client, &QMQTT::Client::connected, [this](void){ui->pbConnectToHost->setText("断开连接");});
        m_client->setHostName(ui->leHostaddr->text());
        m_client->setPort(ui->lePort->text().toInt());
        m_client->setClientId(ui->leClientId->text());
        m_client->setUsername(ui->leUsernam->text());
        m_client->setPassword(QByteArray(ui->lePassword->text().toStdString().c_str()));
        m_client->setVersion(QMQTT::V3_1_1);

        qDebug() << ui->leHostaddr->text()<< ui->lePort->text().toInt() << ui->leClientId->text()<<ui->leUsernam->text()<<ui->lePassword->text().toStdString().c_str();

        if(ui->leKeepAlive->text().size() != 0)
            m_client->setKeepAlive(ui->leKeepAlive->text().toInt());
        m_client->cleanSession();
        m_client->connectToHost();
    }else {
        for(int i = 0; i != ui->cbxSubscribedTopic->count(); i++)
        {
            m_client->unsubscribe(ui->cbxSubscribedTopic->itemText(i));
        }
        m_client->disconnectFromHost();
        ui->pbConnectToHost->setText("连接");
    }
    if(m_client->isConnectedToHost() == false)
    {
       // qDebug() << m_client->
    }
}

void Widget::on_pcSubscribeTopic_clicked()
{
    if(m_client->isConnectedToHost())
    {
        m_client->subscribe(ui->leSubscribeTopic->text(), ui->cbxQosSubscribe->currentText().toUInt());

        QString topic = ui->leSubscribeTopic->text();

        if(ui->cbxSubscribedTopic->findText(topic) < 0)
            ui->cbxSubscribedTopic->addItem(topic);
    }
}

void Widget::on_pbUnsubscribeTopic_clicked()
{
    if(m_client->isConnectedToHost())
    {
        m_client->unsubscribe(ui->cbxSubscribedTopic->currentText());
        ui->cbxSubscribedTopic->removeItem(ui->cbxSubscribedTopic->currentIndex());
    }
}

void Widget::on_pbClearRecv_clicked()
{
    ui->teRecvData->clear();
}

void Widget::on_pbClearPublish_clicked()
{
    ui->tePublicData->clear();
}

void Widget::on_pbPublishTopic_clicked()
{
    if(m_client->isConnectedToHost())
    {
        QMQTT::Message msg(0, ui->lePublicTopic->text(), QByteArray(ui->tePublicData->toPlainText().toStdString().c_str()), ui->cbxQosPublish->currentIndex());
        m_client->publish(msg);
    }
}

void Widget::slot_mqtt_recvived(const QMQTT::Message &msg)
{
    ui->teRecvData->append("【主题】：" + msg.topic());
    ui->teRecvData->append("【数据】：" + QString(msg.payload()));
}
