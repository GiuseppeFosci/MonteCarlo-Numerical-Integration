#pragma once

#include "Function.h"
#include "Result.h"
#include <vector>

// Classe base astratta per tutti gli integratori Monte Carlo.
// Definisce l'interfaccia comune.
class MonteCarloIntegrator {
public:
    // Un distruttore virtuale è essenziale nelle classi base con funzioni virtuali.
    virtual ~MonteCarloIntegrator() = default;

    // Funzione virtuale pura. Obbliga le classi derivate a implementarla.
    virtual Result integrate(
        const Function& func,
        const std::vector<double>& lower_bounds,
        const std::vector<double>& upper_bounds,
        long long samples
    ) const = 0;
};