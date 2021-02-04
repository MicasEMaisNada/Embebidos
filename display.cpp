#include "display.h"
using namespace std;
Display::Display(QObject *parent):QObject(parent)
{
      //manager = new QNetworkAccessManager(this);
      connect(&manager, &QNetworkAccessManager::finished, this, &Display::replyFinished);
      connect(&manager, &QNetworkAccessManager::authenticationRequired, this, &Display::replyAuthenticationRequired);
      connect(&manager, &QNetworkAccessManager::sslErrors, this, &Display::replyErrors);
      connect(&manager, &QNetworkAccessManager::encrypted, this, &Display::replyencrypted);
      connect(&manager, &QNetworkAccessManager::preSharedKeyAuthenticationRequired, this, &Display::replypreSharedKeyAuthenticationRequired);
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
{
    QNetworkReply * reply = manager.get(QNetworkRequest(QUrl("https://api.thingspeak.com/channels/1295410/fields/1.json?results=1")));
    connect(reply, &QNetworkReply::readyRead, this, &Display::readyRead);
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
    /*if(reply) {
         QString strReply = (QString)reply->readAll();
         //parse json
         qDebug() << "Response:" << strReply;
         QByteArray data = reply->readAll();;
         QJsonDocument temp = QJsonDocument::fromJson(strReply.toUtf8());
         QJsonObject jsob = temp.object();
         QString next = jsob["channel"].toString();
         int next1 = jsob["id"].toInt();
         qDebug()<<next;
         qDebug()<<next1;
         */
   /* QStringList propertyNames;
    QStringList propertyKeys;
    QString strReply = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonObject second = jsonObject
    QJsonValue jsonValue = jsonObject.take("channel");
    qDebug() << "latitude:" << jsonObject["latitude"] ;
    QJsonArray jsonArray = jsonObject["channel"].toArray();

    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject();
        propertyNames.append(obj["latitude"].toString());
        propertyKeys.append(obj["key"].toString());
    }
         qDebug() << "encoding:" << propertyNames;
    */
         QByteArray rawData = reply->readAll();
         /*QJsonDocument doc(QJsonDocument::fromJson(rawData));
         QJsonObject json = doc.object();
         QJsonValue agentsArrayValue = json.value("channel");
         QJsonObject second = agentsArrayValue.toObject();
         QJsonDocument doc1 = QJsonDocument(second);
         QJsonObject json1 = doc.object();
         QJsonValue agentsArrayValue1 = json1.value("latitude");
         QJsonValue agentsArrayValue_ = second.value("latitude");
         QJsonArray jsonArray = json["channel"].toArray();*/
         QJsonDocument document = QJsonDocument::fromJson(rawData);
         QJsonObject getjson = document.object();
         QJsonValue siteValue = getjson.value("channel");
         QJsonObject siteObject = siteValue.toObject();
         QJsonValue siteValue1 = siteObject.value("field1");
         qDebug() << siteValue1.toString();
  //           Person p;
             // the QJsonValue encapsulates the person QJsonObject
    //         p.read(jsonPerson.toObject());
      //       this->persons.append(p);

        // QJsonArray agentsArray = agentsArrayValue.toArray();
        //foreach(const QJsonValue & v, agentsArray) {
            // QJsonValue value = json.value(key);
             //qDebug() << "Key = " << key << ", Value = " << value.toString();


   /*      QJsonObject rootObject = loadDoc.object();
         qDebug() << "Response 1:" << rootObject;
         qDebug() << "PM2_5:" << QVariant(rootObject["pm2.5"].toString());
         qDebug() << "Motor:" <<  QVariant(rootObject["motor"].toString());*/
         /*QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
         QJsonArray json_array = jsonResponse.array();
         foreach (const QJsonValue &value, json_array) {
         QJsonObject json_obj = value.toObject();
         qDebug() << "latitude:" << json_obj["latitude"].toString();
         qDebug() << "longitude:" << json_obj["longitude"].toString();*/
 /*        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
         QJsonObject jsonObj = jsonResponse.object();

         qDebug() << "latitude:" << jsonObj["username"].toString();
         qDebug() << "longitude:" << jsonObj["password"].toString();*/



    //else qDebug() << "Error reading!";


/*         QJsonParseError jsonError;
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
