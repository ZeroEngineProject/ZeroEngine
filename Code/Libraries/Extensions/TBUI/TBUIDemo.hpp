// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
class TBUIDemo
{
public:
  /// Meta Initialization.
  ZilchDeclareType(TBUIDemo, TypeCopyMode::ReferenceType);

  TBUIDemo(TBUIView* view);

  ~TBUIDemo();

private:
  UniquePointer<tb::TBWindow> mMainWindow = nullptr;
};
} // namespace Zero