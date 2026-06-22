#include "ollamaclient.h"

OllamaClient::OllamaClient(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
}

QString OllamaClient::imageToBase64(const QString &percorso) {
    QFile file(percorso);
    if (!file.open(QIODevice::ReadOnly)) {
        return "";
    }
    QByteArray fileData = file.readAll();
    return fileData.toBase64();
}

void OllamaClient::inviaPrompt(const QString &modello, const QString &prompt, const QString &percorso, const QJsonArray &context) {
    QUrl url("http://localhost:11434/api/generate");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QString immagine = imageToBase64(percorso);
    QJsonObject json;
    json["model"] = modello;
    json["prompt"] = prompt;
    json["stream"] = false;
    json["options"] = QJsonObject{
        {"temperature", 0.3},
        {"num_predict", 10000}
    };

    QJsonArray imagesArray;
    imagesArray.append(immagine);
    json["images"] = imagesArray;

    if (!context.isEmpty()) {
        json["context"] = context;
    }

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, [this, reply, modello]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
            QJsonObject obj = responseDoc.object();

            QString risposta = obj.value("response").toString();
            QJsonArray nuovoContesto = obj.value("context").toArray();
            double tempo = obj.value("total_duration").toVariant().toDouble() / 1000000000.0;
            int tokens = obj.value("eval_count").toInt() + obj.value("prompt_eval_count").toInt();

            richiediStatisticheVRAM(modello, risposta, tempo, tokens, nuovoContesto);
        } else {
            emit erroreRicevuto(modello, reply->errorString());
        }
        reply->deleteLater();
    });
}


void OllamaClient::richiediStatisticheVRAM(QString modello, QString risposta, double tempo, int tokens, QJsonArray contesto) {
    QUrl url("http://localhost:11434/api/ps");
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, modello, risposta, tempo, tokens, contesto]() {
        double vram = 0.0;
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument statsDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray models = statsDoc.object().value("models").toArray();

            for (int i = 0; i < models.size(); ++i) {
                QJsonObject m = models.at(i).toObject();
                if (m.value("name").toString().contains(modello)) {
                    vram = m.value("size_vram").toVariant().toDouble() / (1024.0 * 1024.0 * 1024.0);
                    break;
                }
            }
        }
        emit rispostaRicevuta(modello, risposta, tempo, tokens, vram, contesto);
        reply->deleteLater();
    });

}

