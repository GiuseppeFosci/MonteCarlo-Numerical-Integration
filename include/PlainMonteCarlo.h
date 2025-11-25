#pragma once

#include "MonteCarloIntegrator.h"

// Dichiarazione della classe per l'integrazione Monte Carlo standard.
class PlainMonteCarlo : public MonteCarloIntegrator {
public:
    // Implementiamo il metodo "integrate" definito nell'interfaccia.
    // La parola chiave "override" aiuta il compilatore a verificare che stiamo
    // effettivamente sovrascrivendo un metodo della classe base.
    Result integrate(
        const Function& func,
        const std::vector<double>& lower_bounds,
        const std::vector<double>& upper_bounds,
        long long samples
    ) const override;
};