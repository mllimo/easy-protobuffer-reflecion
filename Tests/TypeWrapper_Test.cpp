#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_test_macros.hpp>

#include <protos/simple.pb.h>

#include <ProtoReflection/H/TypeWrapper.h>

TEST_CASE("TypeWrappers can be casted", "[TypeWrapper]") {

    int integer_expected = 10;
    double double_expected = 1.4;
    std::string string_expected = "expected ;)";
    std::string string_embedded_expected = "expected x2 ;)";
    std::vector<int> integers_expected = { 1, 2, 3 };

    Simple simple;
    simple.set_decimal(double_expected);
    simple.set_int_(integer_expected);
    simple.set_str(string_expected);
    simple.add_integers(1);
    simple.add_integers(2);
    simple.add_integers(3);
    simple.mutable_embedded()->set_str(string_embedded_expected);


    const auto* descriptor = simple.GetDescriptor();

    SECTION("double to double") {
        const auto* field_descriptor = descriptor->FindFieldByName("decimal");
        easy::TypeWrapper wrapper(&simple, field_descriptor);
        double value = wrapper;
        REQUIRE_THAT(value, Catch::Matchers::WithinRel(double_expected, 0.001));
    }

    SECTION("double to int") {
        const auto* field_descriptor = descriptor->FindFieldByName("decimal");
        easy::TypeWrapper wrapper(&simple, field_descriptor);
        auto e = [&]() { int value = wrapper; };
        REQUIRE_THROWS(e());
    }

    SECTION("int to int") {
        const auto* field_descriptor = descriptor->FindFieldByName("int");
        easy::TypeWrapper wrapper(&simple, field_descriptor);
        int value = wrapper;
        REQUIRE(value == integer_expected);
    }

    SECTION("int to double") {
        const auto* field_descriptor = descriptor->FindFieldByName("int");
        easy::TypeWrapper wrapper(&simple, field_descriptor);
        auto e = [&]() { double value = wrapper; };
        REQUIRE_THROWS(e());
    }

    SECTION("message to message") {
        const auto* field_descriptor = descriptor->FindFieldByName("embedded");
        easy::TypeWrapper wrapper(&simple, field_descriptor);
        google::protobuf::Message* value = wrapper;
        auto* casted = dynamic_cast<const Simple::Level2*>(value);
        CHECK(casted);
        REQUIRE(casted->str() == string_embedded_expected);
    }

    SECTION("repeated int to repeated int") {
        const auto* field_descriptor = descriptor->FindFieldByName("integers");
        easy::TypeWrapper wrapper(&simple, field_descriptor);
        google::protobuf::RepeatedFieldRef<int32_t> value = wrapper;

        CHECK(value.size()  == integers_expected.size());
        
        for (int i = 0; i < value.size(); ++i) {
            REQUIRE(value.Get(i) == integers_expected.at(i));
        }
    }

    SECTION("repeated int to other type") {
        const auto* field_descriptor = descriptor->FindFieldByName("integers");
        easy::TypeWrapper wrapper(&simple, field_descriptor);
        auto e = [&]() {google::protobuf::RepeatedFieldRef<float> value = wrapper; };
        REQUIRE_THROWS(e());
    }

}

TEST_CASE("TypeWrappers can be assigned", "[TypeWrapper]") {

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

    const auto* descriptor = simple.GetDescriptor();

    SECTION("Double field Assigned with double") {
        const auto* field_descriptor = descriptor->FindFieldByName("decimal");
        easy::TypeWrapper wrapper(&simple, field_descriptor);
        wrapper = double_expected;
        double value = wrapper;
        REQUIRE_THAT(value, Catch::Matchers::WithinRel(double_expected, 0.001));
    }

    SECTION("Int32 field Assigned with int32") {
        const auto* field_descriptor = descriptor->FindFieldByName("int");
        easy::TypeWrapper wrapper(&simple, field_descriptor);
        wrapper = integer_expected;
        int32_t value = wrapper;
        REQUIRE(value == integer_expected);
    }

    SECTION("String field Assigned with string") {
        const auto* field_descriptor = descriptor->FindFieldByName("str");
        easy::TypeWrapper wrapper(&simple, field_descriptor);
        wrapper = string_expected;
        std::string value = wrapper;
        REQUIRE(value == string_expected);
    }

    SECTION("Message field Assigned with message") {
        Simple::Level2* expected = new Simple::Level2();
        expected->set_str(string_embedded_expected);
        const auto* field_descriptor = descriptor->FindFieldByName("embedded");

        easy::TypeWrapper wrapper(&simple, field_descriptor);
        wrapper = std::move(expected);
        google::protobuf::Message* value = wrapper;
        auto value_cast = dynamic_cast<Simple::Level2*>(value);

        CHECK(value_cast);
        REQUIRE(value_cast->str() == string_embedded_expected);
    }

    SECTION("Message field Assigned with other field") {
        Simple::Level2* expected = new Simple::Level2();
        expected->set_str(string_embedded_expected);
        const auto* field_descriptor = descriptor->FindFieldByName("embedded");

        easy::TypeWrapper wrapper(&simple, field_descriptor);
        wrapper["embedded"]["str"] = string_embedded_expected;

        std::string value = wrapper["embedded"]["str"];

        REQUIRE(value == string_embedded_expected);
    }

    SECTION("Int Repeated field Assigned") {
        Simple::Level2* expected = new Simple::Level2();
        expected->set_str(string_embedded_expected);
        const auto* field_descriptor = descriptor->FindFieldByName("integers");

        easy::TypeWrapper wrapper(&simple, field_descriptor);
   
        google::protobuf::MutableRepeatedFieldRef<int> value = wrapper;

        CHECK(value.size() == 0);

        for (int i = 0; i < (int)integers_expected.size(); ++i) {
            value.Add(integers_expected[i]);
        }

        CHECK(value.size() == integers_expected.size());

        for (int i = 0; i < value.size(); ++i) {
            REQUIRE(value.Get(i) == integers_expected[i]);
        }

    }
}