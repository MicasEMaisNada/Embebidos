#ifndef DISPLAY_H
#define DISPLAY_H
#include <QObject>
#include <QString>
#include <QtNetwork>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
struct carInformation {
    Q_GADGET
    Q_PROPERTY(bool pm2_5 MEMBER pm2_5)
    Q_PROPERTY(bool airFilterState MEMBER airFilterState)
public:
    float pm2_5;
    bool airFilterState;
};
class Display : public QObject
{
    Q_OBJECT
public:
    explicit Display(QObject *parent = nullptr);
    void get_info_database();
signals:
    void valueChanged(QString s);
public slots:
    void changeValue(int a);
    void replyFinished(QNetworkReply *);
    void replyErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void replyAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    void replyencrypted(QNetworkReply *reply);
    void replypreSharedKeyAuthenticationRequired(QNetworkReply *reply, QSslPreSharedKeyAuthenticator *authenticator);
    void replyproxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void readyRead();
    void requestError(QNetworkReply::NetworkError);
private:
    QNetworkAccessManager manager;
    carInformation *car;
};

#endif // DISPLAY_H
