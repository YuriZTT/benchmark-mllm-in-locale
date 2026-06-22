#ifndef LOGGER_H
#define LOGGER_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class Logger
{
public:
    Logger();

    void salvaLogCSV(QString percorsoCompleto, QString modello, QString percorso, QString test, QString domanda, double tempo, int tokens, double vram, QString risposta);
    void salvaLogTXT(QString percorsoCompleto, QString modello, QString percorso, QString test, QString domanda, double tempo, int tokens, double vram, QString risposta);
    void salvaLogJSON(QString percorsoCompleto, QString modello, QString percorso, QString test, QString domanda, double tempo, int tokens, double vram, QString risposta);
};

#endif // LOGGER_H
