#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

TEST_CASE("Verifying that catch functions")
{
    SECTION("Arithmetics")
    {
        REQUIRE(1 + 1 == 2);
    }
}
