// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
class TBUIWidget : public Component
{
public:
  /// Meta Initialization.
  ZilchDeclareType(TBUIWidget, TypeCopyMode::ReferenceType);
  
  /// Component Interface.
  void Serialize(Serializer& stream) override;
  void Initialize(CogInitializer& initializer) override;

  TBUIView* GetView() const
  {
    return mUIView;
  }

private:
  TBUIView* mUIView = nullptr;
};
} // namespace Zero