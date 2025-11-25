#include "PlainMonteCarlo.h"
#include <random>
#include <stdexcept>
#include <cmath>

Result PlainMonteCarlo::integrate(
    const Function& func,
    const std::vector<double>& lower_bounds,
    const std::vector<double>& upper_bounds,
    long long samples) const
{
    if (lower_bounds.empty() || lower_bounds.size() != upper_bounds.size()) {
        throw std::invalid_argument("I limiti di integrazione non sono validi.");
    }

    const size_t dimensions = lower_bounds.size();
    
    // Calcola il volume del dominio di integrazione
    double volume = 1.0;
    for (size_t i = 0; i < dimensions; ++i) {
        volume *= (upper_bounds[i] - lower_bounds[i]);
    }

    // Setup del generatore di numeri casuali
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::uniform_real_distribution<>> dists;
    dists.reserve(dimensions);
    for (size_t i = 0; i < dimensions; ++i) {
        dists.emplace_back(lower_bounds[i], upper_bounds[i]);
    }

    double sum = 0.0;
    double sum_sq = 0.0; // Somma dei quadrati per il calcolo dell'errore
    std::vector<double> point(dimensions);

    for (long long i = 0; i < samples; ++i) {
        // Genera un punto casuale nel dominio
        for (size_t j = 0; j < dimensions; ++j) {
            point[j] = dists[j](gen);
        }
        
        double val = func(point);
        sum += val;
        sum_sq += val * val;
    }

    double mean = sum / samples;
    double mean_sq = sum_sq / samples;

    double variance = mean_sq - mean * mean;
    double std_error = volume * std::sqrt(variance / samples);

    Result result;
    result.estimate = mean * volume;
    result.error = std_error;
    
    return result;
}