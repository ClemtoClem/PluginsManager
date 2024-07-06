#ifndef VARIANT_TYPE_HPP
#define VARIANT_TYPE_HPP

#include <cstdint>
#include <iostream>
#include <variant>
#include <string>
#include <sstream>
#include <type_traits>

// Définir VariantType
using VariantType = std::variant<uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, float, double, bool, std::string, void*>;

// Fonction pour convertir un VariantType en chaîne de caractères
std::string VariantToString(const VariantType& var);

// Fonction pour obtenir le type d'un VariantType en chaîne de caractères
std::string VariantTypeName(const VariantType& var);

#endif // VARIANT_TYPE_HPP