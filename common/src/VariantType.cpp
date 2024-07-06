#include "VariantType.hpp"

std::string VariantToString(const VariantType &var) {
	return std::visit([](const auto& value) -> std::string {
		std::ostringstream oss;
		if constexpr (std::is_same_v<std::decay_t<decltype(value)>, void*>) {
			oss << "Pointer: " << value;
		} else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, bool>) {
			oss << (value ? "true" : "false");
		} else {
			oss << value;
		}
		return oss.str();
	}, var);
}

std::string VariantTypeName(const VariantType &var) {
	return std::visit([](const auto& value) -> std::string {
		using T = std::decay_t<decltype(value)>;
		if      constexpr (std::is_same_v<T, uint8_t>)      return "uint8_t";
		else if constexpr (std::is_same_v<T, uint16_t>)     return "uint16_t";
		else if constexpr (std::is_same_v<T, uint32_t>)     return "uint32_t";
		else if constexpr (std::is_same_v<T, uint64_t>)     return "uint64_t";
		else if constexpr (std::is_same_v<T, int8_t>)       return "int8_t";
		else if constexpr (std::is_same_v<T, int16_t>)      return "int16_t";
		else if constexpr (std::is_same_v<T, int32_t>)      return "int32_t";
		else if constexpr (std::is_same_v<T, int64_t>)      return "int64_t";
		else if constexpr (std::is_same_v<T, float>)        return "float";
		else if constexpr (std::is_same_v<T, double>)       return "double";
		else if constexpr (std::is_same_v<T, bool>)         return "bool";
		else if constexpr (std::is_same_v<T, std::string>)  return "std::string";
		else if constexpr (std::is_same_v<T, void*>)        return "void*";
		else return "unknown";
	}, var);
}