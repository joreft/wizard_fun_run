#include <json11/json11.hpp>

#include <optional>

namespace jeagle
{

std::optional<json11::Json> json11_from_file(std::string const& filename);

}