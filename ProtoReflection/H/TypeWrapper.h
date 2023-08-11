#pragma once

#include <string>

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>
#include <google/protobuf/repeated_field.h>

#include <ProtoReflection_api.h>

namespace easy {
    class PROTOREFLECTION_EXPORT TypeWrapper {
    public:
        TypeWrapper(TypeWrapper&&) = default;

        /**
         * Constructs a TypeWrapper that represents a field of the message with the provided descriptor.
         */
        TypeWrapper(google::protobuf::Message* message, const google::protobuf::FieldDescriptor* descriptor);

        google::protobuf::FieldDescriptor::Type Type() const;

        operator double() const;
        operator int32_t() const;
        operator const std::string& () const;
        operator google::protobuf::Message* ();
        template <typename T> operator google::protobuf::MutableRepeatedFieldRef<T>();
        template <typename T> operator google::protobuf::RepeatedFieldRef<T>() const;

        TypeWrapper& operator=(TypeWrapper&&) = default;
        TypeWrapper& operator=(double);
        TypeWrapper& operator=(int32_t);
        TypeWrapper& operator=(std::string);

        /**
         * It assigns ownership of the given message to the internal message.
         */
        TypeWrapper& operator=(google::protobuf::Message*&&);

        /**
         *  The internal message receives a copy of the provided message.
         */
        TypeWrapper& operator=(const google::protobuf::Message&);

        /**
         * Given a string, this function attempts to match a field and returns a TypeWrapper for that field.
         */
        TypeWrapper operator[](const std::string& id);

    private:
        google::protobuf::Message* message_;
        const google::protobuf::FieldDescriptor* descriptor_;
        const google::protobuf::Reflection* reflection_;
    };

    template <typename T>
    TypeWrapper::operator google::protobuf::MutableRepeatedFieldRef<T>()
    {
        return reflection_->GetMutableRepeatedFieldRef<T>(message_, descriptor_);
    }

    template <typename T> 
    TypeWrapper::operator google::protobuf::RepeatedFieldRef<T>() const
    {
        return reflection_->GetRepeatedFieldRef<T>(*message_, descriptor_);
    }
}
