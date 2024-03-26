// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
void Build2dTransform(Mat4& m, Vec3Param t, float r);

class TBUIRenderer : public tb::TBRendererBatcher
{
public:
  /// Meta Initialization.
  ZilchDeclareType(TBUIRenderer, TypeCopyMode::ReferenceType);

  tb::TBBitmap* CreateBitmap(int width, int height, uint32* data) override;
  void RenderBatch(Batch* batch) override;
  void SetClipRect(const tb::TBRect& rect) override;

private:

private:
  friend class TBUIView;

  IntRect mClipRect;
  Array<TBUIBatch>* mBatches = nullptr;

  Texture* mFontTexture = nullptr;
  int mCurrentUpdateFrame = -1;
  int mCurrentRenderFrame = -1;
};
} // namespace Zero