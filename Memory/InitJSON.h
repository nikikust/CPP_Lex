#pragma once
#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;

class Coin;
void to_json_edt2(json& j, const std::shared_ptr<Coin>& p);
void from_json_edt2(const json& j, std::shared_ptr<Coin>& p);

namespace nlohmann
{
	template <> struct adl_serializer<std::shared_ptr<Coin>> {
		static void from_json(const json& j, std::shared_ptr<Coin>& ptr) {
			from_json_edt2(j, ptr);
		}
		static void to_json(json& j, const std::shared_ptr<Coin>& ptr) {
			to_json_edt2(j, ptr);
		}
	};
	template <typename T> struct adl_serializer<std::shared_ptr<T>> {
		static void from_json(const json& j, std::shared_ptr<T>& ptr) {
			if (j.is_null()) {
				ptr = nullptr;
			}
			else {
				ptr = std::make_shared<T>(j.get<T>());
			}
		}
		static void to_json(json& j, const std::shared_ptr<T>& ptr) {
			if (ptr.get()) {
				j = *ptr;
			}
			else {
				j = nullptr;
			}
		}
	};
}