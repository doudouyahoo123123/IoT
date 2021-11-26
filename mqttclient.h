//MqttClient.h
#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <QObject>
#include <macroinclude.h>
#include <mqtt/qmqtt.h>

class MqttClient : public QObject
{
    Q_OBJECT
public:
    explicit MqttClient(QObject *parent = 0);
    void mqttInit(QString domainName, quint16 Port);
signals:

public slots:
    void onMQTT_Received(QMQTT::Message message);
    void connectTOHuaWeiIOT(QString domainName, quint16 Port);
    void disConnectTOHuaWeiIOT();
    void MQTT_SendMessage(QMQTT::Message msg);
private:
    QMQTT::Client mqttclient;
};

#endif // MQTTCLIENT_H
