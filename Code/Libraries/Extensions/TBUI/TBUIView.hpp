// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
class TBUIView : public tb::TBWidget
{
public:
  /// Meta Initialization.
  ZilchDeclareType(TBUIView, TypeCopyMode::ReferenceType);

  TBUIView();

private:
  void UpdateBatches();

private:
  TBUI* mUI = nullptr;
  TBUIRenderer* mRenderer = nullptr;
  Array<TBUIBatch> mBatches;
};
} // namespace Zero