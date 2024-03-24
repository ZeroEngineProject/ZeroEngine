// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineType(TBUIRenderer, builder, type)
{
  ZeroBindDocumented();
}
tb::TBBitmap* TBUIRenderer::CreateBitmap(int width, int height, uint32* data)
{
  TBUIBitmap* bitmap = new TBUIBitmap(this);
  if (!bitmap->Init(width, height, data))
  {
    delete bitmap;
    return nullptr;
  }

  return bitmap;
}
void TBUIRenderer::RenderBatch(Batch* tbBatch)
{
  if (tbBatch->vertex_count == 0)
    return;

  Texture* texture = nullptr;
  if (tbBatch->bitmap != nullptr)
  {
    TBUIBitmap* bitmap = (TBUIBitmap*)tbBatch->bitmap;
    texture = reinterpret_cast<Texture*>(bitmap->GetTexture());
  }

  BlendSettings blendSettings;
  blendSettings.SetBlendAlpha();
  TBUIBatch batch(blendSettings, mClipRect, texture, mVertices);

  size_t begin = batch.mVertices->Size();
  batch.mVertices->Resize(begin + tbBatch->vertex_count);
  batch.mVerticesEnd = batch.mVertices->Size();

  for (int i = 0; i < tbBatch->vertex_count; i++)
  {
    tb::TBRendererBatcher::Vertex tbVertex = tbBatch->vertex[i];
    StreamedVertex vertex =
        StreamedVertex(Vec3(tbVertex.x, tbVertex.y, 0), Vec2(tbVertex.u, tbVertex.v), ToFloatColor(tbVertex.col));
    (*batch.mVertices)[begin + i] = vertex;
  }

  TBUIBatch::AddOrMerge(batch, *mBatches);
}
void TBUIRenderer::SetClipRect(const tb::TBRect& rect)
{
  mClipRect = IntRect(rect.x, rect.y, rect.w, rect.h);
}
} // namespace Zero