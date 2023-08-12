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
        bool Contains(const std::string& id) const;

        operator float() const;
        operator double() const;
        operator int32_t() const;
        operator int64_t() const;
        operator uint32_t() const;
        operator uint64_t() const;
        operator bool() const;
        operator const std::string& () const;
        operator google::protobuf::Message* ();
        template <typename T> operator google::protobuf::MutableRepeatedFieldRef<T>();
        template <typename T> operator google::protobuf::RepeatedFieldRef<T>() const;

        TypeWrapper& operator=(TypeWrapper&&) = default;
        TypeWrapper& operator=(float);
        TypeWrapper& operator=(double);
        TypeWrapper& operator=(int32_t);
        TypeWrapper& operator=(int64_t);
        TypeWrapper& operator=(uint32_t);
        TypeWrapper& operator=(uint64_t);
        TypeWrapper& operator=(bool);


        /**
         * Use the move operator to do a copyless assignment. Otherwise the function will make a copy.
         */
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
         *  Utilize the move operator to perform a copyless assignment. Otherwise, the function will create a copy.
         *  For the Message type, you need to pass a google::protobuf::Message*, and the function will assume ownership.
         *  If you want a copy, you must allocate memory and generate a copy of the vector.
         */
        template <typename T> inline TypeWrapper& operator=(std::vector<T>);
        template <>           inline TypeWrapper& operator=(std::vector<std::string>);
        template <>           inline TypeWrapper& operator=(std::vector<google::protobuf::Message*>);

        /**
         * Given a string, this function attempts to match a field and returns a TypeWrapper for that field.
         */
        TypeWrapper At(const std::string& id);

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

    template <typename T>
    inline TypeWrapper& TypeWrapper::operator=(std::vector<T> value)
    {
        if (not descriptor_->is_repeated())
            throw std::bad_typeid();

        google::protobuf::MutableRepeatedFieldRef<T> repeated = reflection_->GetMutableRepeatedFieldRef<T>(message_, descriptor_);

        repeated.Clear();
        for (auto& element : value) {
            repeated.Add(element);
        }

        return *this;
    }

    template <>
    inline TypeWrapper& TypeWrapper::operator=(std::vector<std::string> value)
    {
        if (not descriptor_->is_repeated())
            throw std::bad_typeid();

        reflection_->ClearField(message_, descriptor_);
        for (auto& str : value) {
            reflection_->AddString(message_, descriptor_, std::move(str));
        }

        return *this;
    }

    template <>
    inline TypeWrapper& TypeWrapper::operator=(std::vector<google::protobuf::Message*> value)
    {
        if (not descriptor_->is_repeated())
            throw std::bad_typeid();

        reflection_->ClearField(message_, descriptor_);
        for (auto& proto : value) {
            reflection_->AddAllocatedMessage(message_, descriptor_, std::move(proto));
        }

        return *this;
    }
}
