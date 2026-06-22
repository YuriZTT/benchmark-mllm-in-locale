#ifndef OLLAMACLIENT_H
#define OLLAMACLIENT_H
#include <QString>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>

class OllamaClient : public QObject
{
    Q_OBJECT

public:
    explicit OllamaClient(QObject *parent = nullptr);

    void inviaPrompt(const QString &modello, const QString &prompt, const QString &percorso, const QJsonArray &context);
    void richiediStatisticheVRAM(QString modello, QString risposta, double tempo, int tokens, QJsonArray contesto);

signals:
    void rispostaRicevuta(QString modello, QString risposta, double tempo, int tokens, double vram, QJsonArray nuovoContesto);
    void erroreRicevuto(QString modello, QString messaggio);

private:
    QString imageToBase64(const QString &percorso);
    QNetworkAccessManager *manager;
};

#endif // OLLAMACLIENT_H
