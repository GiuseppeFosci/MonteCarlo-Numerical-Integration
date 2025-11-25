// --- 1. Librerie Standard C++ (DEVONO essere prime) ---
#include <cstdlib>
#include <cstddef>
#include <vector>
#include <memory>
#include <iostream>

// --- 2. Librerie Qt ---
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtCore/qglobal.h> 

// --- 3. I tuoi Header ---
#include "IntegrationController.h"

int main(int argc, char *argv[]) {
    // Inizializza l'applicazione Qt (Widgets è necessario per Qt Charts)
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Crea il controller
    auto *integrationController = new IntegrationController(&app);

    // Esponi il controller a QML
    engine.rootContext()->setContextProperty("integrationController", integrationController);

    // Carica l'interfaccia
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    
    engine.load(url);

    return app.exec();
}