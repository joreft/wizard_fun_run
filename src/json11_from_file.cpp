#include "json11_from_file.h"

#include "log.h"

#include <json11/json11.hpp>
#include <fstream>
#include <sstream>

namespace jeagle
{

std::optional<json11::Json> json11_from_file(std::string const& filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        return std::nullopt;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string json_parse_error {};
    auto const as_json = json11::Json::parse(buffer.str(), json_parse_error);

    if (!json_parse_error.empty())
    {
        LOG_WARNING("Failed to parse json from file {} with {}", filename, json_parse_error);

        return std::nullopt;
    }

    return as_json;
}

} // namespace jeagle