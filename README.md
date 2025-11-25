# Monte Carlo Numerical Integration Suite

![Language](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=c%2B%2B)
![Framework](https://img.shields.io/badge/Qt-6.x-41CD52?style=flat-square&logo=qt)
![License](https://img.shields.io/badge/License-MIT-blue?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20(WSL)-lightgrey?style=flat-square)

Un'applicazione desktop ad alte prestazioni per il calcolo numerico di integrali definiti (quadratura) mediante metodi stocastici. Sviluppata in **C++17** con interfaccia grafica moderna in **Qt Quick (QML)**, l'applicazione integra un parser matematico dinamico e sistemi di visualizzazione dati in tempo reale.



##  Indice

- [Caratteristiche Principali](#-caratteristiche-principali)
- [Architettura Software](#-architettura-software)
- [Requisiti di Sistema](#-requisiti-di-sistema)
- [Installazione e Compilazione](#-installazione-e-compilazione)
- [Guida all'Uso](#-guida-alluso)
- [Cenni Teorici](#-cenni-teorici)
- [Risoluzione Problemi (WSL)](#-risoluzione-problemi-wsl)


##  Caratteristiche Principali

*   **Valutazione Dinamica delle Espressioni:** Utilizzo della libreria `muParser` per il parsing e la valutazione runtime di espressioni matematiche arbitrarie definite dall'utente.
*   **Algoritmi Stocastici Multipli:**
    *   **Plain Monte Carlo:** Campionamento casuale uniforme (Standard).
    *   **Stratified Monte Carlo:** Tecnica di riduzione della varianza mediante suddivisione del dominio in strati equiprobabili.
*   **Rendering Grafico Avanzato:** Visualizzazione interattiva della funzione integranda e dell'area di calcolo tramite il modulo **Qt Charts**.
*   **Calcolo Asincrono:** Esecuzione dei task computazionali su thread secondari (via `QtConcurrent`) per garantire la non-bloccabilità dell'interfaccia utente (GUI Responsiveness).
*   **Modern UI/UX:** Interfaccia QML basata su controlli vettoriali, adattabile e con supporto per font di sistema nativi.

##  Architettura Software

Il progetto segue rigorosi principi di ingegneria del software orientata agli oggetti (OOP):

1.  **Strategy Pattern:** Implementato per la gestione degli algoritmi di integrazione. La classe astratta `MonteCarloIntegrator` definisce l'interfaccia, permettendo di scambiare a runtime le implementazioni concrete (`PlainMonteCarlo`, `StratifiedMonteCarlo`).
2.  **Controller Bridge:** La classe `IntegrationController` funge da mediatore tra il core computazionale (C++) e il livello di presentazione (QML/JavaScript), gestendo il ciclo di vita dei thread e la conversione dei dati.
3.  **Resource Management:** Utilizzo intensivo di smart pointers (`std::unique_ptr`) e container STL standard per una gestione sicura ed efficiente della memoria.

##  Requisiti di Sistema

*   **Compilatore C++:** GCC 9+, Clang 10+ o MSVC 2019+ (Supporto C++17 richiesto).
*   **Build System:** CMake 3.16 o superiore.
*   **Qt Framework:** Versione 6.2 o superiore (Moduli richiesti: Core, Gui, Quick, Qml, Charts, Concurrent, Widgets).
*   **Librerie Terze Parti:** `libmuparser`.

##  Installazione e Compilazione

### Ubuntu / Debian / WSL (Windows Subsystem for Linux)

Per configurare l'ambiente di sviluppo e installare tutte le dipendenze runtime QML necessarie:

```bash
# 1. Aggiornamento repository e installazione toolchain
sudo apt update
sudo apt install build-essential cmake pkg-config

# 2. Installazione librerie di sviluppo
sudo apt install qt6-base-dev qt6-declarative-dev libqt6charts6-dev libqt6opengl6-dev libmuparser-dev

# 3. Installazione moduli runtime QML (Cruciale per l'esecuzione)
sudo apt install qml6-module-qtquick \
                 qml6-module-qtquick-controls \
                 qml6-module-qtquick-layouts \
                 qml6-module-qtcharts \
                 qml6-module-qtqml-workerscript \
                 qml6-module-qtquick-templates \
                 qml6-module-qtquick-window \
                 qml6-module-qtquick-nativestyle
