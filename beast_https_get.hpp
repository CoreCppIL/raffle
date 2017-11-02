#pragma once
#include <string>
#include "json.hpp"

nlohmann::json https_get_json(std::string const& host, std::string const& target);

