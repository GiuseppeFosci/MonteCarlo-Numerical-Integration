#include "StratifiedMonteCarlo.h"
#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>
#include <algorithm> // Per std::min

Result StratifiedMonteCarlo::integrate(
    const Function& func,
    const std::vector<double>& lower_bounds,
    const std::vector<double>& upper_bounds,
    long long samples) const
{
    if (lower_bounds.empty() || lower_bounds.size() != upper_bounds.size()) {
        throw std::invalid_argument("I limiti di integrazione non sono validi.");
    }
    if (samples <= 0) {
        throw std::invalid_argument("Il numero di campioni deve essere positivo.");
    }

    const size_t dimensions = lower_bounds.size();

    // --- Strategia di Stratificazione ---
    // Euristica: proviamo 10 divisioni per dimensione.
    long long strata_per_dim = 10; 
    long long total_strata = static_cast<long long>(std::pow(strata_per_dim, dimensions));

    // CORREZIONE: Adattamento dinamico se i campioni sono pochi
    if (total_strata > samples) {
        // Se abbiamo pochi campioni, riduciamo gli strati al minimo (1 strato = Plain MC)
        // oppure calcoliamo il massimo numero di strati possibile.
        // Qui facciamo fallback su un approccio più semplice:
        strata_per_dim = 1; 
        total_strata = 1;
        // (Nota: in un codice reale si potrebbe calcolare strata_per_dim = floor(pow(samples, 1/dim)))
    }

    const long long samples_per_stratum = samples / total_strata;
    
    std::random_device rd;
    std::mt19937 gen(rd());

    double total_estimate = 0.0;
    double total_variance_sum = 0.0;
    
    std::vector<double> point(dimensions);
    std::vector<long long> stratum_index(dimensions);

    for (long long i = 0; i < total_strata; ++i) {
        long long temp_id = i;
        // Decodifica indice lineare in coordinate dello strato
        for (int d = dimensions - 1; d >= 0; --d) {
            stratum_index[d] = temp_id % strata_per_dim;
            temp_id /= strata_per_dim;
        }

        double stratum_volume = 1.0;
        std::vector<std::uniform_real_distribution<>> dists;
        dists.reserve(dimensions);

        for (size_t d = 0; d < dimensions; ++d) {
            double dim_width = (upper_bounds[d] - lower_bounds[d]) / strata_per_dim;
            double stratum_lower = lower_bounds[d] + stratum_index[d] * dim_width;
            
            stratum_volume *= dim_width;
            dists.emplace_back(stratum_lower, stratum_lower + dim_width);
        }

        double sum_in_stratum = 0.0;
        double sum_sq_in_stratum = 0.0;

        for (long long j = 0; j < samples_per_stratum; ++j) {
            for (size_t d = 0; d < dimensions; ++d) {
                point[d] = dists[d](gen);
            }
            
            double val = func(point);
            sum_in_stratum += val;
            sum_sq_in_stratum += val * val;
        }

        double mean = sum_in_stratum / samples_per_stratum;
        double mean_sq = sum_sq_in_stratum / samples_per_stratum;
        
        // Varianza campionaria dello strato
        double variance = mean_sq - mean * mean;

        total_estimate += mean * stratum_volume;
        
        // La varianza dell'estimatore stratificato è la somma delle varianze ponderate
        // Var(F) = Sum ( (Vol_i / Vol_tot)^2 * (Var_i / N_i) ) * Vol_tot^2
        // Semplificato: Sum ( Vol_i^2 * Var_i / N_i )
        if (samples_per_stratum > 0) {
            total_variance_sum += (stratum_volume * stratum_volume * variance) / samples_per_stratum;
        }
    }

    Result result;
    result.estimate = total_estimate;
    result.error = std::sqrt(total_variance_sum);
    
    return result;
}