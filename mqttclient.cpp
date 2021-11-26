//MqttClient.pp
#include "mqttclient.h"

MqttClient::MqttClient(QObject *parent) : QObject(parent)
{

}
//mqtt 初始化
void MqttClient::mqttInit(QString domainName, quint16 Port)
{
    QHostInfo info = QHostInfo::fromName(domainName);
    QString host = info.addresses().first().toString(); // 代理服务器 IP
    qDebug() << host;

//    mqttclient = new QMQTT::Client(QHostAddress(host),Port);
    mqttclient.setKeepAlive(120);
    mqttclient.setHost(QHostAddress(host));
    mqttclient.setPort(Port);
    mqttclient.setClientId(CLIENTID);
    mqttclient.setUsername(USERNAME);
    mqttclient.setPassword(PASSWORD);
    mqttclient.cleanSession();
    mqttclient.setVersion(QMQTT::MQTTVersion::V3_1_1); // 设置mqtt版本
    connect(&mqttclient,SIGNAL(received(QMQTT::Message)),this,SLOT(onMQTT_Received(QMQTT::Message)));
}


//链接到华为物联网平台
void MqttClient::connectTOHuaWeiIOT(QString domainName, quint16 Port)
{
    //初始化 MQTT
    mqttInit(domainName,Port);
    mqttclient.connectToHost();
    qDebug()<<"connect to host success!!";
}

//断开与平台的链接
void MqttClient::disConnectTOHuaWeiIOT()
{
    mqttclient.disconnectFromHost();
    qDebug()<<"disconnect huaweiIOT!!";
}

//接收消息的槽函数
void MqttClient::onMQTT_Received(QMQTT::Message message)
{
    QString str(message.payload());
    qDebug()<<"onMQTT_Received: "<<str;
    QMQTT::Message message();

}

//发送消息的槽函数
void MqttClient::MQTT_SendMessage(QMQTT::Message msg)
{
    mqttclient.publish(msg);
}
