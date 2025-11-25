#pragma once

#include <functional>
#include <vector>

// Definiamo un tipo "Function" che rappresenta una funzione matematica
// che prende un punto N-dimensionale (vector<double>) e restituisce un valore (double).
using Function = std::function<double(const std::vector<double>&)>;