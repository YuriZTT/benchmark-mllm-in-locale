#include "logger.h"

Logger::Logger() {}

void Logger::salvaLogCSV(QString percorsoCompleto, QString modello, QString percorso, QString test, QString domanda, double tempo, int tokens, double vram, QString risposta) {
    QFile file(percorsoCompleto + ".csv");
    bool exists = file.exists();

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        if (!exists) {
            out << "Modello;Percorso;Tipo_Test;Domanda;Timestamp;Tempo_Totale;Token_Generati;VRAM_Allocata;Risposta\n";
        }
        QString rispostaPulita = risposta.trimmed().simplified().replace(";", ",");
        out << modello << ";"
            << percorso << ";"
            << test << ";"
            << domanda << ";"
            << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << ";"
            << tempo << ";"
            << tokens << ";"
            << vram << ";"
            << "\"" << rispostaPulita << "\"" << "\n";

        file.close();
    }
}

void Logger::salvaLogTXT(QString percorsoCompleto, QString modello, QString percorso, QString test, QString domanda, double tempo, int tokens, double vram, QString risposta) {
    QFile file(percorsoCompleto + ".txt");

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Immagine: " << percorso << Qt::endl;
        out << "Modello: " << modello << Qt::endl;
        out << "Test scelto: " << test << Qt::endl;
        out << "Domanda: " << domanda << Qt::endl;
        out << "Risposta: " << risposta << Qt::endl;
        out << "Tempo totale: " << tempo << " s" << Qt::endl;
        out << "Tokens totali: " << tokens << Qt::endl;
        out << "Utilizzo VRAM (GPU): " << vram << Qt::endl;
        out << "------------------------------------" << Qt::endl;
        file.close();
    }
}

void Logger::salvaLogJSON(QString percorsoCompleto, QString modello, QString percorso, QString test, QString domanda, double tempo, int tokens, double vram, QString risposta) {
    QFile file(percorsoCompleto + ".json");
    QJsonArray logArray;

    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            if (doc.isArray()) {
                logArray = doc.array();
            }
            file.close();
        }
    }

    QJsonObject nuovoLog;
    nuovoLog["modello"] = modello;
    nuovoLog["percorso_immagine"] = percorso;
    nuovoLog["tipo_test"] = test;
    nuovoLog["domanda"] = domanda;
    nuovoLog["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    nuovoLog["tempo_totale_sec"] = tempo;
    nuovoLog["tokens_totali"] = tokens;
    nuovoLog["vram_allocata"] = vram;
    nuovoLog["risposta_modello"] = risposta;

    logArray.append(nuovoLog);

    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument saveDoc(logArray);
        file.write(saveDoc.toJson(QJsonDocument::Indented));
        file.close();
    }


}
