#pragma once

// Header Standard
#include <memory>
#include <vector>

// Header Qt
#include <QObject>
#include <QString>
#include <QVariantList>
#include <QFutureWatcher> 

// Tuoi Header
#include "Result.h"

class IntegrationController : public QObject {
    Q_OBJECT
public:
    explicit IntegrationController(QObject *parent = nullptr);

    Q_INVOKABLE void calculate(const QString& formula, double lower, double upper, int samples, bool useStratified);
    Q_INVOKABLE QVariantList generateFunctionPoints(const QString& formula, double lower, double upper, int numPoints = 200);

signals:
    void calculationDone(double estimate, double error);
    void calculationError(const QString& errorMessage);

private:
    QFutureWatcher<Result> watcher;
};