#include <ProtoReflection/H/Reflection.h>

namespace easy {
    Reflection::Reflection(google::protobuf::Message* message)
        : message_(message)
    {
        assert(message_);
    }

    bool Reflection::Contains(const std::string& id) const
    {
        const auto* message_descriptor = message_->GetDescriptor();
        const auto* field_descriptor = message_descriptor->FindFieldByName(id);

        return field_descriptor;
    }


    TypeWrapper Reflection::operator[](const std::string& id)
    {
        const auto* message_descriptor = message_->GetDescriptor();
        const auto* field_descriptor = message_descriptor->FindFieldByName(id);

        assert(field_descriptor);

        return TypeWrapper(message_, field_descriptor);
    }
}
