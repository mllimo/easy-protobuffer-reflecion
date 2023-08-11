#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_test_macros.hpp>

#include <protos/simple.pb.h>

#include <ProtoReflection/H/Reflection.h>

TEST_CASE("Reflection find field can be casted", "[Reflection]") {

    int integer_expected = 10;
    double double_expected = 1.4;
    std::string string_expected = "expected ;)";
    std::string string_embedded_expected = "expected x2 ;)";

    Simple simple;
    simple.set_decimal(double_expected);
    simple.set_int_(integer_expected);
    simple.set_str(string_expected);

    easy::Reflection reflection(&simple);

    SECTION("First level double") {
        double value = reflection["decimal"];
        REQUIRE_THAT(value, Catch::Matchers::WithinRel(double_expected, 0.001));
    }

    SECTION("First level double") {
        int32_t value = reflection["int"];
        REQUIRE(value == integer_expected);
    }

    SECTION("First level string") {
        std::string value = reflection["str"];
        REQUIRE(value == string_expected);
    }
}

TEST_CASE("Reflection find field can be assigned", "[Reflection]") {

    int integer_expected = 10;
    double double_expected = 1.4;
    std::string string_expected = "expected ;)";
    std::string string_embedded_expected = "expected x2 ;)";

    Simple simple;
    simple.set_decimal(0);
    simple.set_int_(0);
    simple.set_str("");
    simple.mutable_embedded()->set_str("");

    easy::Reflection reflection(&simple);

    SECTION("First level Double field Assigned with double") {
        reflection["decimal"] = double_expected;
        double value = reflection["decimal"];
        REQUIRE_THAT(value, Catch::Matchers::WithinRel(double_expected, 0.001));
    }

    SECTION("First level field Assigned with int32") {
        reflection["int"] = integer_expected;
        int32_t value = reflection["int"];
        REQUIRE(value == integer_expected);
    }

    SECTION("First level field Assigned with string") {
        reflection["str"] = string_expected;
        std::string value = reflection["str"];
        REQUIRE(value == string_expected);
    }

    SECTION("Second level string field Assigned with string") {
        reflection["embedded"]["str"] = string_expected;
        std::string value = reflection["str"];
        REQUIRE(value == string_expected);
    }

    SECTION("Second level string field Assigned with int") {
        auto e = [&]() { reflection["embedded"]["str"] = 1; };
        REQUIRE_THROWS(e());
    }
}