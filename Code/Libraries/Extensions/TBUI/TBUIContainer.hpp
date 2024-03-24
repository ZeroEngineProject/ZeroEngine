// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
class TBUIContainer : public Component
{
public:
  /// Meta Initialization.
  ZilchDeclareType(TBUIContainer, TypeCopyMode::ReferenceType);
  
  /// Component Interface.
  void Serialize(Serializer& stream) override;
  void Initialize(CogInitializer& initializer) override;
};
} // namespace Zero