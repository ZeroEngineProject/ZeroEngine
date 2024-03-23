#include "Precompiled.hpp"

#include "RendererBase/RendererBaseStandard.hpp"

namespace Zero
{
TBUIBatch::TBUIBatch(const BlendSettings& blendSettings,
                     const IntRect& clipRect,
                     const Texture* texture,
                     Array<StreamedVertex>* vertices) :
    mBlendSettings(blendSettings), 
    mClipRect(clipRect), 
    mTexture(texture), 
    mVertices(vertices), 
    mVerticesStart(vertices->Size()),
    mVerticesEnd(vertices->Size())
{
}

bool AreEqual(const BlendSettings& left, const BlendSettings& right)
{
  if (left.mBlendEquation != right.mBlendEquation || left.mBlendEquationAlpha != right.mBlendEquationAlpha ||
      left.mBlendMode != right.mBlendMode || left.mDestFactor != right.mDestFactor ||
      left.mDestFactorAlpha != right.mDestFactorAlpha || left.mSourceFactor != right.mSourceFactor ||
      left.mSourceFactorAlpha != right.mSourceFactorAlpha)
    return false;
  return true;
}

bool TBUIBatch::Merge(const TBUIBatch& batch)
{
  if (!AreEqual(mBlendSettings,mBlendSettings) ||
      !(mClipRect.X == batch.mClipRect.X && mClipRect.Y == batch.mClipRect.Y && mClipRect.SizeX == batch.mClipRect.SizeX &&
        mClipRect.SizeY == batch.mClipRect.SizeY) 
      || mTexture != batch.mTexture)
    return false;

  mVertices->Append(batch.mVertices->All());

  return true;
}
void TBUIBatch::AddOrMerge(const TBUIBatch& batch, Array<TBUIBatch>& batches)
{
  if (batch.mVertices->Size() == 0)
    return;

  if (!batches.Empty() && batches.Back().Merge(batch))
    return;

  batches.Append(batch);
}
} // namespace Zero