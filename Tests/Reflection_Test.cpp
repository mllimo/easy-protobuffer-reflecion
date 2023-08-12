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
        double value = reflection.At("decimal");
        REQUIRE_THAT(value, Catch::Matchers::WithinRel(double_expected, 0.001));
    }

    SECTION("First level int") {
        int32_t value = reflection.At("int");
        REQUIRE(value == integer_expected);
    }

    SECTION("First level string") {
        std::string value = reflection.At("str");
        REQUIRE(value == string_expected);
    }
}

TEST_CASE("Reflection find field can be assigned", "[Reflection]") {

    int integer_expected = 10;
    double double_expected = 1.4;
    std::string string_expected = "expected ;)";
    std::string string_embedded_expected = "expected x2 ;)";
    std::vector<int> integers_expected = { 1, 2, 3 };

    Simple simple;
    simple.set_decimal(0);
    simple.set_int_(0);
    simple.set_str("");
    simple.mutable_embedded()->set_str("");

    easy::Reflection reflection(&simple);

    SECTION("First level Double field Assigned with double") {
        reflection.At("decimal") = double_expected;
        double value = reflection.At("decimal");
        REQUIRE_THAT(value, Catch::Matchers::WithinRel(double_expected, 0.001));
    }

    SECTION("First level field Assigned with int32") {
        reflection.At("int") = integer_expected;
        int32_t value = reflection.At("int");
        REQUIRE(value == integer_expected);
    }

    SECTION("First level field Assigned with string") {
        reflection.At("str") = string_expected;
        std::string value = reflection.At("str");
        REQUIRE(value == string_expected);
    }

    SECTION("Second level string field Assigned with string") {
        reflection.At("embedded").At("str") = string_expected;
        std::string value = reflection.At("str");
        REQUIRE(value == string_expected);
    }

    SECTION("Second level string field Assigned with int") {
        auto e = [&]() { reflection.At("embedded").At("str") = 1; };
        REQUIRE_THROWS(e());
    }

    SECTION("First level repeated int field Assigned std::vector<int>") {
        reflection.At("integers") = integers_expected;
        google::protobuf::RepeatedFieldRef<int> repeated = reflection.At("integers");

        CHECK(repeated.size() == integers_expected.size());

        for (int i = 0; i < repeated.size(); ++i) {
            REQUIRE(repeated.Get(i) == integers_expected[i]);
        }

        for (int i = 0; i < simple.integers().size(); ++i) {
            REQUIRE(simple.integers().at(i) == integers_expected[i]);
        }
    }
}