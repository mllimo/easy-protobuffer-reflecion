#pragma once

#include <google/protobuf/message.h>

#include <ProtoReflection/H/TypeWrapper.h>

#include <ProtoReflection_api.h>

namespace easy {
    class PROTOREFLECTION_EXPORT Reflection {
    public:
        Reflection(google::protobuf::Message* message);
        TypeWrapper operator[](const std::string& id);

    private:
        google::protobuf::Message* message_;
    };
}
