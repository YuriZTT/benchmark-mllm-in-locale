# MLLM Local Benchmarking Tool

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Qt](https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white)
![Ollama](https://img.shields.io/badge/Ollama-FFFFFF?style=for-the-badge&logo=Ollama&logoColor=black)

Applicazione desktop sviluppata in **C++** e **Qt** per l'esecuzione e il benchmarking locale di Multimodal Large Language Models (MLLM). 
Questo progetto è stato realizzato come parte integrante del lavoro di tesi in Informatica presso l'**Università degli Studi di Milano-Bicocca**.

## 🎯 Panoramica del Progetto
Il software fornisce un'interfaccia grafica intuitiva e asincrona per interrogare modelli IA multimodali eseguiti interamente in locale tramite l'infrastruttura **Ollama**. È progettato per valutare la fattibilità dell'IA generativa su hardware *consumer*, estraendo metriche critiche senza dipendere da API cloud esterne.

### ✨ Funzionalità Principali
* **Esecuzione Locale:** Totale privacy dei dati, funzionamento offline e assenza di costi per le API.
* **Benchmarking Hardware:** Tracciamento in tempo reale del tempo totale di inferenza e dei picchi di allocazione VRAM della GPU.
* **Gestione Asincrona:** Interfaccia utente fluida che non si blocca durante il caricamento dei modelli o l'elaborazione neurale.
* **Elaborazione Visiva:** Codifica automatica delle immagini in formato Base64 per l'analisi e il *Visual Question Answering* (VQA).
* **Gestione del Contesto:** Mantenimento della memoria conversazionale (fino al limite architetturale dei token) per interazioni prolungate.

## 🧠 Modelli Supportati
L'applicativo è configurato per interfacciarsi con i seguenti modelli visivo-linguistici:
* **[Llama 3.2 Vision](https://ollama.com/library/llama3.2-vision):** Modello multimodale ad alte prestazioni di Meta.
* **[MiniCPM-V](https://ollama.com/library/minicpm-v):** Ottime prestazioni semantiche, ragionamento spaziale e capacità di OCR avanzato in the wild.
* **[LLaVA](https://ollama.com/library/llava):** Modello compatto con latenza ridotta.
* **[BakLLaVA](https://ollama.com/library/bakllava):** Alternativa architetturale per task generici.
* **[Moondream](https://ollama.com/library/moondream):** Modello ultra-leggero, ottimizzato per impronte in VRAM minime (~1.12 GB).

## ⚙️ Requisiti di Sistema
Per compilare ed eseguire il progetto, è necessario disporre di:
1. **Ollama:** Installato e in esecuzione in background (default su `localhost:11434`).
2. I modelli scaricati all'interno di Ollama (es. tramite il comando `ollama pull minicpm-v`).
3. **Qt Framework** e **Qt Creator** installati nel sistema.
4. Un compilatore C++ compatibile (GCC, Clang o MSVC).

## 🚀 Compilazione e Avvio
1. Clona il repository in locale:
   ```bash
   git clone https://github.com/YuriZTT/benchmark-mllm-in-locale.git
2. Apri il file di configurazione del progetto (.pro o CMakeLists.txt) all'interno di Qt Creator.

3. Configura il progetto selezionando il kit desktop appropriato.

4. Compila ed avvia l'applicazione (tramite l'interfaccia di Qt Creator o usando Ctrl+R / Cmd+R).
