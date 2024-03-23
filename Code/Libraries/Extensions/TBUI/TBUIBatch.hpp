// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
class TBUIBatch
{
public:
  TBUIBatch() = default;
  TBUIBatch(const BlendSettings& blendSettings,
            const IntRect& clipRect,
            const Texture* texture,
            Array<StreamedVertex>* vertices);

  bool Merge(const TBUIBatch& batch);

  static void AddOrMerge(const TBUIBatch& batch, Array<TBUIBatch>& batches);

  BlendSettings mBlendSettings;
  IntRect mClipRect;
  const Texture* mTexture = nullptr;
  Array<StreamedVertex>* mVertices;
  u32 mVerticesStart;
  u32 mVerticesEnd;
};
} // namespace Zero