#include <set>

#include <ProtoReflection/H/TypeWrapper.h>


namespace easy {
    TypeWrapper::TypeWrapper(google::protobuf::Message* message, const google::protobuf::FieldDescriptor* descriptor)
        : message_(message)
        , descriptor_(descriptor)
        , reflection_(message->GetReflection())
    {
        assert(message_);
        assert(descriptor_);
        assert(reflection_);
    }

    google::protobuf::FieldDescriptor::Type TypeWrapper::Type() const
    {
        return descriptor_->type();
    }

    bool TypeWrapper::Contains(const std::string& id) const
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
            return false;

        const auto* message_descriptor = message_->GetDescriptor();
        const auto* field_descriptor = message_descriptor->FindFieldByName(id);

        return field_descriptor;
    }

    static bool IsNumericType(google::protobuf::FieldDescriptor::Type type)
    {
        using Type = google::protobuf::FieldDescriptor::Type;
        static const std::set<Type> KCheck = {
            Type::TYPE_DOUBLE,
            Type::TYPE_FLOAT,
            Type::TYPE_FIXED32,
            Type::TYPE_FIXED64,
            Type::TYPE_INT32,
            Type::TYPE_INT64,
            Type::TYPE_SFIXED32,
            Type::TYPE_SFIXED64,
            Type::TYPE_SINT32,
            Type::TYPE_SINT64,
            Type::TYPE_UINT32,
            Type::TYPE_UINT64,
        };

        auto it = KCheck.find(type);
        return it != KCheck.end();
    }

    TypeWrapper::operator float() const
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_FLOAT)
            throw std::bad_cast();
        return reflection_->GetFloat(*message_, descriptor_);
    }

    TypeWrapper::operator double() const
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE)
            throw std::bad_cast();
        return reflection_->GetDouble(*message_, descriptor_);
    }

    TypeWrapper::operator int32_t() const
    {
        if (descriptor_->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT32)
            return reflection_->GetInt32(*message_, descriptor_);
        if (descriptor_->type() == google::protobuf::FieldDescriptor::Type::TYPE_ENUM)
            throw std::bad_typeid();
        return reflection_->GetEnumValue(*message_, descriptor_);
    }

    TypeWrapper::operator int64_t() const
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_INT64)
            throw std::bad_cast();
        return reflection_->GetInt64(*message_, descriptor_);
    }

    TypeWrapper::operator uint32_t() const
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_UINT32)
            throw std::bad_cast();
        return reflection_->GetUInt32(*message_, descriptor_);
    }

    TypeWrapper::operator uint64_t() const
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_UINT64)
            throw std::bad_cast();
        return reflection_->GetUInt64(*message_, descriptor_);
    }

    TypeWrapper::operator bool() const
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_BOOL)
            throw std::bad_cast();
        return reflection_->GetBool(*message_, descriptor_);
    }

    TypeWrapper::operator const std::string& () const
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_STRING)
            throw std::bad_cast();
        return reflection_->GetStringReference(*message_, descriptor_, {});
    }

    TypeWrapper::operator google::protobuf::Message*()
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
            throw std::bad_cast();
        return reflection_->MutableMessage(message_, descriptor_);
    }

    TypeWrapper& TypeWrapper::operator=(float value)
    {
        if (not IsNumericType(descriptor_->type()))
            throw std::bad_typeid();
        reflection_->SetFloat(message_, descriptor_, value);
        return *this;
    }

    TypeWrapper& TypeWrapper::operator=(double value)
    {
        if (not IsNumericType(descriptor_->type()))
            throw std::bad_typeid();
        reflection_->SetDouble(message_, descriptor_, value);
        return *this;
    }

    TypeWrapper& TypeWrapper::operator=(int32_t value)
    {
        if (not IsNumericType(descriptor_->type()))
            throw std::bad_typeid();
        reflection_->SetInt32(message_, descriptor_, value);
        return *this;
    }

    TypeWrapper& TypeWrapper::operator=(int64_t value)
    {
        if (not IsNumericType(descriptor_->type()))
            throw std::bad_typeid();
        reflection_->SetInt64(message_, descriptor_, value);
        return *this;
    }

    TypeWrapper& TypeWrapper::operator=(uint32_t value)
    {
        if (not IsNumericType(descriptor_->type()) || descriptor_->type() == google::protobuf::FieldDescriptor::Type::TYPE_ENUM)
            throw std::bad_typeid();
        
        if (descriptor_->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT32)
            reflection_->SetUInt32(message_, descriptor_, value);
        else
            reflection_->SetEnumValue(message_, descriptor_, value);

        return *this;
    }

    TypeWrapper& TypeWrapper::operator=(uint64_t value)
    {
        if (not IsNumericType(descriptor_->type()))
            throw std::bad_typeid();
        reflection_->SetUInt64(message_, descriptor_, value);
        return *this;
    }

    TypeWrapper& TypeWrapper::operator=(bool value)
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_BOOL)
            throw std::bad_typeid();
        reflection_->SetBool(message_, descriptor_, value);
        return *this;
    }

    TypeWrapper& TypeWrapper::operator=(std::string value)
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_STRING)
            throw std::bad_typeid();
        reflection_->SetString(message_, descriptor_, std::move(value));
        return *this;
    }

    TypeWrapper& TypeWrapper::operator=(google::protobuf::Message*&& value)
    {
        assert(value);
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
            throw std::bad_typeid();
        reflection_->SetAllocatedMessage(message_, std::move(value), descriptor_);
        return *this;
    }

    TypeWrapper& TypeWrapper::operator=(const google::protobuf::Message& value)
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
            throw std::bad_typeid();

        google::protobuf::Message* copy = value.New();
        copy->CopyFrom(value);

        reflection_->SetAllocatedMessage(message_, std::move(copy), descriptor_);
        return *this;
    }

    TypeWrapper TypeWrapper::At(const std::string& id)
    {
        if (descriptor_->type() != google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
            throw std::bad_typeid();

        const auto* message_descriptor = message_->GetDescriptor();
        const auto* field_descriptor = message_descriptor->FindFieldByName(id);
        
        assert(field_descriptor);

        const auto* message_reflection = message_->GetReflection();
        
        return field_descriptor->type() == google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE
            ? TypeWrapper(message_reflection->MutableMessage(message_, descriptor_), field_descriptor)
            : TypeWrapper(message_, field_descriptor);
    }

}
