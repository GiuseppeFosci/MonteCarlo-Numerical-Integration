#pragma once

#include "MonteCarloIntegrator.h"

// Dichiarazione della classe per l'integrazione Monte Carlo stratificata.
class StratifiedMonteCarlo : public MonteCarloIntegrator {
public:
    Result integrate(
        const Function& func,
        const std::vector<double>& lower_bounds,
        const std::vector<double>& upper_bounds,
        long long samples
    ) const override;
};