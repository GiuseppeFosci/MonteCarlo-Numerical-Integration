#pragma once

// Una struttura per contenere il risultato di un'integrazione:
// il valore stimato e una stima dell'errore.
struct Result {
    double estimate = 0.0;
    double error = 0.0;
};