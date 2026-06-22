#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ollama = new OllamaClient(this);
    logger = new Logger();

    connect(ollama, &OllamaClient::rispostaRicevuta, this, &MainWindow::gestisciRispostaIA);

    connect(ollama, &OllamaClient::erroreRicevuto, this, [this](QString modello, QString messaggio){
        ui->textChat->append("<tr><td><b style='color:red;'>Errore [" + modello + "]:</b> " + messaggio + "</td></tr>");
        elaboraProssimoModello();
    });

    //Inserimento dei test
    ui->comboBoxTest->addItem("Test 0: Manuale", 0);
    ui->comboBoxTest->addItem("Test 1: Base", 1);
    ui->comboBoxTest->addItem("Test 2: Dettagliato", 2);
    ui->comboBoxTest->addItem("Test 3: OCR", 3);
    ui->comboBoxTest->addItem("Test 4: Emozioni", 4);

    ui->inputNomeFile->setPlaceholderText("session_log");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnCarica_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Seleziona Immagini"), "", tr("Immagini (*.png *.jpg *.jpeg *.gif)"));

    if (!fileNames.isEmpty()) {
        listaPercorsiImmagini = fileNames;
        indiceImmagineAttuale = 0;
        //ui->textChat->clear(); per poter cancellare il testo precedente ad ogni caricamento di una nuova immagine
        matriceContesti.clear();
        if(listaPercorsiImmagini.size() == 1)
        {
            QString imageHtml = QString(
                                    "<table width='100%'><tr><td align='right'>"
                                    "  <img src='%1' width='300' />"
                                    "  <br><i style='color:gray;'>Immagine caricata</i>"
                                    "</table>"
                                    ).arg(QUrl::fromLocalFile(fileNames[indiceImmagineAttuale]).toString());

            ui->textChat->append(imageHtml);
        }
        else
        {
            QString imageHtml = QString("<table width='100%'><tr><td align='right'>");
            for(int i = 0; i < listaPercorsiImmagini.size(); i++)
            {
                imageHtml += QString("<img src='%1' width='300' />").arg(QUrl::fromLocalFile(fileNames[i]).toString());

            }
            imageHtml += QString("<br><i style='color:gray;'>Immagini caricate</i></table>");
            ui->textChat->append(imageHtml);
        }
        aggiornaStatoInvia();
    }
}

void MainWindow::aggiornaStatoInvia() {
    bool hasText = !ui->inputManuale->text().trimmed().isEmpty();
    bool anyModel = ui->checkLlava->isChecked() || ui->checkMoondream->isChecked() || ui->checkMinicpm->isChecked() || ui->checkBakllava->isChecked() || ui->checkLlama->isChecked();
    bool hasImage = !listaPercorsiImmagini.isEmpty();
    ui->btnInvia->setEnabled(hasText && anyModel && hasImage);
}

void MainWindow::on_comboBoxTest_currentIndexChanged(int index)
{
    if (index == 0) {
        ui->inputManuale->clear();
        ui->inputManuale->setEnabled(true);
        ui->inputManuale->setPlaceholderText("Scrivi qui la tua domanda...");
    } else {
        ui->inputManuale->setEnabled(false);
        switch(index) {
            case 1: ui->inputManuale->setText(promptBase); break;
            case 2: ui->inputManuale->setText(promptDettagliato); break;
            case 3: ui->inputManuale->setText(promptOCR); break;
            case 4: ui->inputManuale->setText(promptEmozioni); break;
        }
    }
    aggiornaStatoInvia();
}

void MainWindow::on_checkLlava_checkStateChanged(const Qt::CheckState &arg1)
{
    aggiornaStatoInvia();
}


void MainWindow::on_checkMoondream_checkStateChanged(const Qt::CheckState &arg1)
{
    aggiornaStatoInvia();
}


void MainWindow::on_checkMinicpm_checkStateChanged(const Qt::CheckState &arg1)
{
    aggiornaStatoInvia();
}


void MainWindow::on_checkBakllava_checkStateChanged(const Qt::CheckState &arg1)
{
    aggiornaStatoInvia();
}


void MainWindow::on_checkLlama_checkStateChanged(const Qt::CheckState &arg1)
{
    aggiornaStatoInvia();
}

void MainWindow::on_inputManuale_textChanged(const QString &arg1)
{
    aggiornaStatoInvia();
}


void MainWindow::on_btnInvia_clicked()
{
    caricaModelli();
    indiceImmagineAttuale = 0;
    ui->btnInvia->setEnabled(false);
    ui->btnCarica->setEnabled(false);
    ui->groupBoxModelli->setEnabled(false);
    ui->comboBoxTest->setEnabled(false);
    ui->inputManuale->setEnabled(false);
    ui->inputNomeFile->setEnabled(false);

    ui->textChat->append("<table width='100%'><tr><td align='right'>"
                         "<span style='color:#00ff00;'><b>Tu:</b> " + ui->inputManuale->text() + "</span>"
                                                      "</td></tr></table>");
    if(listaPercorsiImmagini.size() > 1)
    {
        ui->textChat->append(QString("<hr><center><b style='color:#7f8c8d;'>--- Analisi Immagine %1 di %2 ---</b></center>")
                                 .arg(indiceImmagineAttuale + 1)
                                 .arg(listaPercorsiImmagini.size()));
    }
    elaboraProssimoModello();
}

void MainWindow::elaboraProssimoModello()
{
    if(!modelliInCoda.isEmpty())
    {
        QString modelloAttuale = modelliInCoda.takeFirst();
        ui->textChat->append("<table width='100%'><tr><td align='left' style='margin-left: 10px; color: #666;'><i>⏳ " + modelloAttuale + " sta pensando...</i></td></tr>");
        ollama->inviaPrompt(modelloAttuale, ui->inputManuale->text() + " (rispondi in italiano)", listaPercorsiImmagini[indiceImmagineAttuale], matriceContesti[listaPercorsiImmagini[indiceImmagineAttuale]][modelloAttuale]);
        return;
    }
    indiceImmagineAttuale++;
    if(indiceImmagineAttuale < listaPercorsiImmagini.size())
    {
        ui->textChat->append(QString("<hr><center><b style='color:#7f8c8d;'>--- Analisi Immagine %1 di %2 ---</b></center>")
                                 .arg(indiceImmagineAttuale + 1)
                                 .arg(listaPercorsiImmagini.size()));
        caricaModelli();
        elaboraProssimoModello();
    }
    else
    {
        ui->btnInvia->setEnabled(true);
        ui->btnCarica->setEnabled(true);
        ui->groupBoxModelli->setEnabled(true);
        ui->comboBoxTest->setEnabled(true);
        ui->inputNomeFile->setEnabled(true);
        if(ui->comboBoxTest->currentIndex()==0)
        {
            ui->inputManuale->clear();
            ui->inputManuale->setPlaceholderText("Scrivi qui la tua domanda...");
            ui->inputManuale->setEnabled(true);
        }
        ui->textChat->append("<tr><td><i style='color:gray;'>--- Fine analisi ---</i></td></tr></table>");
    }

}

void MainWindow::gestisciRispostaIA(QString modello, QString risposta, double tempo, int tokens, double vram, QJsonArray nuovoContesto)
{
    matriceContesti[listaPercorsiImmagini[indiceImmagineAttuale]][modello] = nuovoContesto;
    QMap<QString, QString> colori;
    colori["llava"] = "#ffff00"; //giallo
    colori["moondream"] = "#ffa500"; //arancione
    colori["minicpm-v"] = "#00ffff"; //ciano
    colori["bakllava"] = "#800080"; //viola
    colori["llama3.2-vision"] = "#a52a2a"; //marrone

    QString coloreModello = colori.value(modello.toLower(), "#ffffff");

    QString html = QString(
                       "<tr><td style='margin-top: 10px; margin-bottom: 5px;'>"
                       "  <b style='color:%1; font-size: 15px;'>● [%2]</b><br>"
                       "  <div style='color: %3; font-size: 14px; margin-left: 10px;'>"
                       "    %4"
                       "  </div>"
                       "  <div style='color: gray; font-size: 10px; margin-left: 10px; margin-top: 4px;'>"
                       "    <i>Tempo: %5s | Tokens: %6 | VRAM: %7 GB</i>"
                       "  </div>"
                       "</td></tr>"
                       ).arg(coloreModello, modello, coloreModello, risposta, QString::number(tempo, 'f', 2), QString::number(tokens), QString::number(vram, 'f', 2));

    ui->textChat->append(html);
    aggiornaDimensioneFont();

    QString percorsoFile;
    if(!ui->inputNomeFile->text().trimmed().isEmpty())
    {
        percorsoFile = ui->inputNomeFile->text().trimmed();
    }
    else
    {
        percorsoFile = "session_log";
    }
    QDir dir;
    if(!dir.exists("logs"))
    {
        dir.mkdir("logs");
    }
    QString percorsoCompleto = "logs/" + percorsoFile;
    logger->salvaLogCSV(percorsoCompleto, modello, listaPercorsiImmagini[indiceImmagineAttuale], ui->comboBoxTest->currentText(), ui->inputManuale->text(), tempo, tokens, vram, risposta);
    logger->salvaLogTXT(percorsoCompleto, modello, listaPercorsiImmagini[indiceImmagineAttuale], ui->comboBoxTest->currentText(), ui->inputManuale->text(), tempo, tokens, vram, risposta);
    logger->salvaLogJSON(percorsoCompleto, modello, listaPercorsiImmagini[indiceImmagineAttuale], ui->comboBoxTest->currentText(), ui->inputManuale->text(), tempo, tokens, vram, risposta);


    elaboraProssimoModello();
}

void MainWindow::on_inputManuale_returnPressed()
{
    if(ui->btnInvia->isEnabled())
    {
        on_btnInvia_clicked();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        QFont font = ui->textChat->font();
        int currentSize = font.pointSize();

        if (event->key() == Qt::Key_Plus)
        {
            font.setPointSize(currentSize + 1);
            ui->textChat->setFont(font);
            ui->textChat->selectAll();
            ui->textChat->setFontPointSize(currentSize + 1);
            ui->textChat->moveCursor(QTextCursor::End);
        }
        else if (event->key() == Qt::Key_Minus)
        {
            if (currentSize > 6)
            {
                font.setPointSize(currentSize - 1);
                ui->textChat->setFont(font);
                ui->textChat->selectAll();
                ui->textChat->setFontPointSize(currentSize - 1);
                ui->textChat->moveCursor(QTextCursor::End);
            }
        }
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::aggiornaDimensioneFont()
{
    QFont font = ui->textChat->font();
    int currentSize = font.pointSize();
    font.setPointSize(currentSize);
    ui->textChat->setFont(font);
    ui->textChat->selectAll();
    ui->textChat->setFontPointSize(currentSize);
    ui->textChat->moveCursor(QTextCursor::End);
}

void MainWindow::caricaModelli()
{
    modelliInCoda.clear();

    if (ui->checkLlava->isChecked()) {
        modelliInCoda.append("llava");
    }
    if (ui->checkMoondream->isChecked()) {
        modelliInCoda.append("moondream");
    }
    if (ui->checkMinicpm->isChecked()) {
        modelliInCoda.append("minicpm-v");
    }
    if (ui->checkBakllava->isChecked()) {
        modelliInCoda.append("bakllava");
    }
    if (ui->checkLlama->isChecked()) {
        modelliInCoda.append("llama3.2-vision");
    }
}

