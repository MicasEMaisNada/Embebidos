#include "display.h"
using namespace std;
Display::Display(QObject *parent):QObject(parent)
{
      //manager = new QNetworkAccessManager(this);
      connect(&manager, &QNetworkAccessManager::finished, this, &Display::replyFinished);
      connect(&manager, &QNetworkAccessManager::authenticationRequired, this, &Display::replyAuthenticationRequired);
      //connect(&manager, &QNetworkAccessManager::sslErrors, this, &Display::replyErrors);
      //connect(&manager, &QNetworkAccessManager::encrypted, this, &Display::replyencrypted);
      //connect(&manager, &QNetworkAccessManager::preSharedKeyAuthenticationRequired, this, &Display::replypreSharedKeyAuthenticationRequired);
      connect(&manager, &QNetworkAccessManager::proxyAuthenticationRequired, this, &Display::replyproxyAuthenticationRequired);
      //connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));
      //QNetworkRequest request;
      //request.setUrl(QUrl("http://google.com"));
      //QNetworkReply *reply = this->manager->get(request);
      //manager.get(QNetworkRequest(QUrl("https://api.thingspeak.com/channels/1295410/fields/1.json?results=2")));
      //manager->get(QNetworkRequest(QUrl("https://api.thingspeak.com/channels/1280253/fields/3/last.json?api_key=BDJ0QE8EP5FWXBIR&results=2")));
      //connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
      qDebug() << "updated successfully";
}

void Display::get_info_database()
{   qDebug() << "in get_info_database";
    QNetworkReply * reply = manager.get(QNetworkRequest(QUrl("http://api.thingspeak.com/channels/1280253/feeds.json?api_key=BDJ0QE8EP5FWXBIR&results=2")));
    connect(reply, &QNetworkReply::readyRead, this, &Display::readyRead);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(requestError(QNetworkReply::NetworkError)));
}

void Display::changeValue(int a)
{
    if(a==1)
        valueChanged("values is 1,From c++");

}

void Display::replyFinished(QNetworkReply *reply)
{   qDebug() << "in reply";
/*    QByteArray response;
     // Checks if the reply has no erros
     if(reply->error() == QNetworkReply::NoError) {
         response = reply->readAll();
     }

     else {
         qDebug() << "Error reading!";
     }

     QJsonParseError jsonError;
     QJsonDocument loadDoc = QJsonDocument::fromJson(response, &jsonError); // Parse and capture the error flag

     if(jsonError.error != QJsonParseError::NoError) {
         qDebug() << "Error: " << jsonError.errorString();
     }

     //Parses the json
     QJsonObject rootObject = loadDoc.object();
     car->pm2_5 = QVariant(rootObject["pm2.5"].toString()).toFloat();
     car->airFilterState = QVariant(rootObject["motor"].toString()).toBool();
     //emit done(DISPLAY_CONTENT);
*/
}

void Display::replyErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
     qDebug() << "Error: ";
}
void Display::replyAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    qDebug() << "Authentication: ";
}

void Display::replyencrypted(QNetworkReply *reply)
{

    qDebug() << "replyencrypted: ";
}

void Display::replypreSharedKeyAuthenticationRequired(QNetworkReply *reply, QSslPreSharedKeyAuthenticator *authenticator)
{
      qDebug() << "replypreSharedKeyAuthenticationRequired: ";
}

void Display::replyproxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    qDebug() << "replyproxyAuthenticationRequired: ";
}

void Display::readyRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    qDebug() << "readyRead:"<<reply;
    if(reply) {
        QByteArray data = reply->readAll();
        QString strReply = (QString)reply->readAll();
        qDebug() << "Response:" << strReply;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonObject root = jsonDoc.object();
        QJsonArray dataObject = root.value("feeds").toArray();
        QJsonValue  value = dataObject.at(1);
        int  valueMotor = value["field5"].toInt();
        qDebug() << "Motor:" <<valueMotor;
        int  valuePM2_5 = value["field4"].toInt();
        qDebug() << "PM2.5 :" <<valuePM2_5;}

    else qDebug() << "Error reading!";
}

void Display::requestError(QNetworkReply::NetworkError e)
{
    qDebug() << "Error in reply!";
    qDebug()<< "is:"<< e;
}
