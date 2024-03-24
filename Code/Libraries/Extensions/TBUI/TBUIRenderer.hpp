// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
void Build2dTransform(Mat4& m, Vec3Param t, float r);

class TBUIRenderer : public tb::TBRendererBatcher
{
public:
  friend class TBUIView;

  /// Meta Initialization.
  ZilchDeclareType(TBUIRenderer, TypeCopyMode::ReferenceType);

  tb::TBBitmap* CreateBitmap(int width, int height, uint32* data) override;
  void RenderBatch(Batch* batch) override;
  void SetClipRect(const tb::TBRect& rect) override;

private:
  Array<TBUIBatch>* mBatches = nullptr;
  Array<StreamedVertex>* mVertices = nullptr;
  IntRect mClipRect;
};
} // namespace Zero