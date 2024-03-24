// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineType(TBUIContainer, builder, type)
{
  ZeroBindDocumented();
  ZeroBindComponent();
  ZeroBindSetup(SetupMode::DefaultSerialization);
}

void TBUIContainer::Serialize(Serializer& stream)
{
}

void TBUIContainer::Initialize(CogInitializer& initializer)
{
}
} // namespace Zero