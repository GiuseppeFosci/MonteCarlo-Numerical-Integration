#include "IntegrationController.h"
#include "PlainMonteCarlo.h"
#include "StratifiedMonteCarlo.h"
#include "Function.h"
#include "Result.h"
#include <muParser.h>
#include <QPointF>
#include <QtConcurrent> // Per il multithreading facile

IntegrationController::IntegrationController(QObject *parent) : QObject(parent) {
    // Connettiamo il segnale del watcher ai nostri segnali QML
    connect(&watcher, &QFutureWatcher<Result>::finished, this, [this]() {
        try {
            Result res = watcher.result(); // Recupera il risultato
            emit calculationDone(res.estimate, res.error);
        } catch (const std::exception& e) {
            emit calculationError(QString(e.what()));
        }
    });
}

void IntegrationController::calculate(const QString& formula, double lower, double upper, int samples, bool useStratified) {
    
    // Usiamo QtConcurrent::run per eseguire il lavoro in un thread separato.
    // Passiamo tutti i parametri per valore (copia) per thread-safety.
    QFuture<Result> future = QtConcurrent::run([=]() -> Result {
        
        // 1. Creazione dell'Integratore (LOCALE al thread)
        std::unique_ptr<MonteCarloIntegrator> integrator;
        if (useStratified) {
            integrator = std::make_unique<StratifiedMonteCarlo>();
        } else {
            integrator = std::make_unique<PlainMonteCarlo>();
        }

        std::vector<double> lower_bounds = {lower};
        std::vector<double> upper_bounds = {upper};

        // 2. Setup muParser (Thread-safe perché locale)
        mu::Parser parser;
        double x_var = 0;
        try {
            parser.DefineVar("x", &x_var);
            parser.SetExpr(formula.toStdString());
        } catch (const mu::Parser::exception_type &e) {
            // Rilanciamo come std::runtime_error per catturarlo nel watcher
            throw std::runtime_error("Errore parser: " + e.GetMsg());
        }

        Function user_function = [&](const std::vector<double>& point) -> double {
            x_var = point[0];
            return parser.Eval(); // Eval può lanciare eccezioni
        };

        // 3. Esecuzione integrazione
        try {
            return integrator->integrate(user_function, lower_bounds, upper_bounds, samples);
        } catch (const std::exception& e) {
            throw std::runtime_error(e.what());
        }
    });

    // Impostiamo il future nel watcher
    watcher.setFuture(future);
}

QVariantList IntegrationController::generateFunctionPoints(const QString& formula, double lower, double upper, int numPoints) {
    QVariantList points;
    if (formula.isEmpty() || lower >= upper || numPoints <= 1) {
        return points;
    }

    try {
        mu::Parser parser;
        double x_var = 0;
        parser.DefineVar("x", &x_var);
        parser.SetExpr(formula.toStdString());

        double step = (upper - lower) / (numPoints - 1);

        for (int i = 0; i < numPoints; ++i) {
            x_var = lower + i * step;
            double y_val = parser.Eval();
            // Nota: QPointF in QVariant richiede che il lato QML sappia gestirlo
            points.append(QVariant::fromValue(QPointF(x_var, y_val)));
        }
    } catch (const mu::Parser::exception_type &e) {
        emit calculationError("Errore grafico: " + QString::fromStdString(e.GetMsg()));
        return {};
    }

    return points;
}