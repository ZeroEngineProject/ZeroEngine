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
  void UpdateBatches();
  void GetBatches(Array<TBUIBatch>& batches, Array<StreamedVertex>& vertices, const IntRect& clipRect);
  void Render(const Array<TBUIBatch>& batches, u32 batchStart, u32 batchEnd);

  void RenderBatch(const TBUIBatch& batch);

    void CreateRenderData(ViewBlock& viewBlock,
                        FrameBlock& frameBlock,
                        const IntRect& clipRect,
                        Array<StreamedVertex>& vertices,
                        const Texture* texture,
                        PrimitiveType::Enum primitiveType);
  ViewNode& AddRenderNodes(ViewBlock& viewBlock, FrameBlock& frameBlock, const IntRect& clipRect, const Texture* texture);

private:
  friend class TBUIManager;

  Vec3 mTranslation;
  float mAngle;

  Mat4 mWorldTx = Mat4::cIdentity;

  Array<TBUIBatch> mBatches;
  Array<StreamedVertex> mVertices;

  TBUIManager* mUIManager = nullptr;
  TBUIRenderer* mRenderer = nullptr;

  Texture* mRenderTexture = nullptr;

  bool mAutoFocus = false;
  bool mHandleInput = false;
};
} // namespace Zero