#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include "ollamaclient.h"
#include "logger.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnCarica_clicked();

    void on_comboBoxTest_currentIndexChanged(int index);

    void on_checkLlava_checkStateChanged(const Qt::CheckState &arg1);

    void on_checkMoondream_checkStateChanged(const Qt::CheckState &arg1);

    void on_checkMinicpm_checkStateChanged(const Qt::CheckState &arg1);

    void on_checkBakllava_checkStateChanged(const Qt::CheckState &arg1);

    void on_checkLlama_checkStateChanged(const Qt::CheckState &arg1);

    void on_inputManuale_textChanged(const QString &arg1);

    void on_btnInvia_clicked();

    void elaboraProssimoModello();

    void gestisciRispostaIA(QString modello, QString risposta, double tempo, int tokens, double vram, QJsonArray nuovoContesto);

    void on_inputManuale_returnPressed();

    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    OllamaClient *ollama;
    Logger *logger;
    QStringList listaPercorsiImmagini;
    int indiceImmagineAttuale;
    void aggiornaStatoInvia();
    QString promptBase = "Descrivi l'immagine in modo sintetico."; //Global Understanding
    QString promptDettagliato = "Analizza l'immagine passo dopo passo. Identifica gli oggetti, la loro posizione spaziale e i colori dominanti."; //Reasoning & Spatial Awareness
    QString promptOCR = "Identifica e trascrivi tutto il testo visibile nell'immagine, mantenendo l'ordine di lettura."; //Optical Character Recognition
    QString promptEmozioni = "Quali emozioni trasmette questa immagine?"; //High-level Semantics/Interpretation
    QStringList modelliInCoda;
    QMap<QString, QMap<QString, QJsonArray>> matriceContesti;
    void aggiornaDimensioneFont();
    void caricaModelli();

};
#endif // MAINWINDOW_H
