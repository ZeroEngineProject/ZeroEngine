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

  void Render();

private:
  void Render(const Array<TBUIBatch>& batches, u32 batchStart, u32 batchEnd);

  void UpdateBatches();
  void GetBatches(Array<TBUIBatch>& batches, Array<StreamedVertex>& vertices, const IntRect& clipRect);

  void RenderBatch(ViewBlock& viewBlock, FrameBlock& frameBlock, const TBUIBatch& batch, const IntRect& clipRect);
  void CreateRenderData(ViewBlock& viewBlock,
                        FrameBlock& frameBlock,
                        const IntRect& clipRect,
                        Array<StreamedVertex>& vertices,
                        const Texture* texture,
                        PrimitiveType::Enum primitiveType);
  ViewNode&
  AddRenderNodes(ViewBlock& viewBlock, FrameBlock& frameBlock, const IntRect& clipRect, const Texture* texture);

private:
  friend class TBUI;

  TBUI* mUI = nullptr;
  TBUIRenderer* mRenderer = nullptr;
  Array<TBUIBatch> mBatches;
  Array<StreamedVertex> mVertices;

  Vec3 mTranslation;
  float mAngle;

  Mat4 mWorldTx = Mat4::cIdentity;
  Texture* mFontTexture = nullptr;
};
} // namespace Zero