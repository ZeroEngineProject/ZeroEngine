// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineType(TBUIView, builder, type)
{
  ZeroBindDocumented();
}
TBUIView::TBUIView()
{
  mUI = TBUI::GetInstance();
  mRenderer = mUI->GetRenderer();

  // Set gravity all so we resize correctly
  SetGravity(tb::WIDGET_GRAVITY_ALL);

  // Set initial size for view
  tb::TBRect rect = mUI->GetRootWidget()->GetRect();
  SetSize(rect.w, rect.h);

  mUI->AddView(this);
}
void TBUIView::Render()
{
  Render(mBatches, 0, mBatches.Size());
}

void TBUIView::Render(const Array<TBUIBatch>& batches, u32 batchStart, u32 batchEnd)
{
  if (batches.Empty())
  {
    return;
  }

  OsShell* shell = Z::gEngine->has(OsShell);
  OsWindow* osWindow = shell->GetWindow(0);
  IntVec2 clientSize = osWindow->GetClientSize();
  //// IntVec2 clientPos = osWindow->GetMonitorClientPosition();

    ColorTransform colorTx = {Vec4(1, 1, 1, 1)};
  Mat4 localTx;
  this->mTranslation = Vec3{0, 0, 0};
  this->mAngle = 0;
  Build2dTransform(localTx, this->mTranslation, this->mAngle);
  mWorldTx = localTx * Mat4::cIdentity;

  IntVec2 size = IntVec2{clientSize.x, clientSize.y};
  //IntVec2 pos = IntVec2{clientPos.x, clientPos.y};
  IntVec2 pos = IntVec2{0, 0};

  IntRect clipRect = IntRect(pos.x, pos.y, size.x, size.y);

  Vec4 clearColor = Vec4{0.5f, 0.7f, 0.3f, 0.1f};

  GraphicsEngine* graphics = Z::gEngine->has(GraphicsEngine);
  RenderTasks& renderTasks = *graphics->mRenderTasksBack;
  RenderQueues& renderQueues = *graphics->mRenderQueuesBack;
  renderQueues.mRenderTasks = &renderTasks;

  FrameBlock& frameBlock = renderQueues.mFrameBlocks.PushBack();
  ViewBlock& viewBlock = renderQueues.mViewBlocks.PushBack();
  frameBlock.mRenderQueues = &renderQueues;

  Mat4 translation;
  translation.Translate(size.x * -0.5f, size.y * -0.5f, 0.0f);
  Mat4 scale;
  scale.Scale(1.0f, -1.0f, 1.0f);
  viewBlock.mWorldToView = scale * translation;
  BuildOrthographicTransformZero(viewBlock.mViewToPerspective, size.y, size.x / size.y, -1.0f, 1.0f);

  Mat4 apiPerspective;
  Z::gRenderer->BuildOrthographicTransform(apiPerspective, size.y, size.x / size.y, -1.0f, 1.0f);
  viewBlock.mZeroPerspectiveToApiPerspective = apiPerspective * viewBlock.mViewToPerspective.Inverted();

  for (u32 i = batchStart; i < batchEnd; i++)
  {
    const TBUIBatch& batch = batches[i];
    if (batch.mVerticesStart == batch.mVerticesEnd)
      continue;

    RenderBatch(viewBlock, frameBlock, batch, clipRect);
  }

  IndexRange& indexRange = viewBlock.mRenderGroupRanges.PushBack();
  indexRange.start = 0;
  indexRange.end = viewBlock.mViewNodes.Size();

  RenderTaskRange& renderTaskRange = renderTasks.mRenderTaskRanges.PushBack();
  renderTaskRange.mFrameBlockIndex = renderQueues.mFrameBlocks.Size() - 1;
  renderTaskRange.mViewBlockIndex = renderQueues.mViewBlocks.Size() - 1;
  renderTaskRange.mTaskIndex = renderTasks.mRenderTaskBuffer.mCurrentIndex;
  renderTaskRange.mTaskCount = 0;

  HandleOf<RenderTarget> renderTarget =
      Z::gEngine->has(GraphicsEngine)->GetRenderTarget((uint)size.x, (uint)size.y, TextureFormat::RGBA8);

  GraphicsRenderSettings renderSettings;
  renderSettings.SetColorTarget(renderTarget);
  renderSettings.mBlendSettings[0].SetBlendAlpha();
  renderSettings.mScissorMode = ScissorMode::Enabled;

  BoundType* defaultRenderPass = MetaDatabase::GetInstance()->FindType("ColorOutput");
  ReturnIf(defaultRenderPass == nullptr, , "We expected to have a type defined called ColorOutput");

  HandleOf<MaterialBlock> renderPassHandle = ZilchAllocate(MaterialBlock, defaultRenderPass);
  MaterialBlock& renderPass = renderPassHandle;

  Material* spriteMaterial = MaterialManager::FindOrNull("AlphaSprite");
  uint shaderInputsId = 0;

  {
    Pair<u64, uint> key((u64)spriteMaterial->mResourceId, shaderInputsId);
    IndexRange range = spriteMaterial->AddShaderInputs(renderTasks.mShaderInputs, renderTasks.mShaderInputsVersion);
    renderTasks.mShaderInputRanges.Insert(key, range);
  }
  {
    Pair<u64, uint> key(cFragmentShaderInputsId, shaderInputsId);
    IndexRange range = renderPass.AddShaderInputs(renderTasks.mShaderInputs);
    renderTasks.mShaderInputRanges.Insert(key, range);
  }

  RenderTaskHelper helper(renderTasks.mRenderTaskBuffer);
  // helper.AddRenderTaskClearTarget(renderSettings, clearColor, 0, 0, 0xFF);
  helper.AddRenderTaskRenderPass(renderSettings, 0, defaultRenderPass->Name, shaderInputsId);

  ScreenViewport viewport = {pos.x, pos.y, (int)size.x, (int)size.y};
  helper.AddRenderTaskBackBufferBlit(renderTarget, viewport);

  renderTaskRange.mTaskCount = 2;

  Z::gEngine->has(GraphicsEngine)->ClearRenderTargets();
}

void TBUIView::UpdateBatches()
{
  mBatches.Clear();
  mVertices.Clear();

  tb::TBRect rect = GetRect();

  GetBatches(mBatches, mVertices, IntRect(/*rect.x*/ 0, /*rect.y*/ 0, rect.w, rect.h));
}
void TBUIView::GetBatches(Array<TBUIBatch>& batches, Array<StreamedVertex>& vertices, const IntRect& clipRect)
{
  tb::g_renderer->BeginPaint(clipRect.SizeX, clipRect.SizeY);

  mRenderer->mClipRect = clipRect;
  mRenderer->mBatches = &batches;
  mRenderer->mVertices = &vertices;
  //this->InvokePaint(tb::TBWidget::PaintProps());
  mUI->GetRootWidget()->InvokePaint(tb::TBWidget::PaintProps());

  tb::g_renderer->EndPaint();
}

void TBUIView::RenderBatch(ViewBlock& viewBlock,
                           FrameBlock& frameBlock,
                           const TBUIBatch& batch,
                           const IntRect& clipRect)
{
  CreateRenderData(viewBlock, frameBlock, clipRect, *(batch.mVertices), batch.mTexture, PrimitiveType::Triangles);
}

void TBUIView::CreateRenderData(ViewBlock& viewBlock,
                                FrameBlock& frameBlock,
                                const IntRect& clipRect,
                                Array<StreamedVertex>& vertices,
                                const Texture* texture,
                                PrimitiveType::Enum primitiveType)
{
  if (vertices.Empty())
    return;

  StreamedVertexArray& streamedVertices = frameBlock.mRenderQueues->mStreamedVertices;

  ViewNode& viewNode = AddRenderNodes(viewBlock, frameBlock, clipRect, texture);

  for (size_t i = 0; i < vertices.Size(); ++i)
  {
    StreamedVertex vertex = vertices[i];
    vertex.mPosition = Math::TransformPoint(viewNode.mLocalToView, vertex.mPosition);
    streamedVertices.PushBack(vertex);
  }

  viewNode.mStreamedVertexType = primitiveType;
  viewNode.mStreamedVertexCount = streamedVertices.Size() - viewNode.mStreamedVertexStart;

  // viewNode.mStreamedVertexStart = offset;
  // viewNode.mStreamedVertexCount = count;
}

ViewNode&
TBUIView::AddRenderNodes(ViewBlock& viewBlock, FrameBlock& frameBlock, const IntRect& clipRect, const Texture* texture)
{
  FrameNode& frameNode = frameBlock.mFrameNodes.PushBack();
  ViewNode& viewNode = viewBlock.mViewNodes.PushBack();

  frameNode.mGraphicalEntry = nullptr;
  viewNode.mGraphicalEntry = nullptr;

  frameNode.mBorderThickness = 1.0f;
  frameNode.mBlendSettingsOverride = false;
  frameNode.mRenderingType = RenderingType::Streamed;
  frameNode.mCoreVertexType = CoreVertexType::Streamed;
  frameNode.mLocalToWorld = mWorldTx.Transposed();
  viewNode.mFrameNodeIndex = frameBlock.mFrameNodes.Size() - 1;
  viewNode.mLocalToView = viewBlock.mWorldToView * frameNode.mLocalToWorld;

  frameNode.mClip = Vec4(clipRect.Left(), clipRect.Top(), clipRect.Right(), clipRect.Bottom());

  // maybe cache this lookup on root
  Material* spriteMaterial = nullptr;

  spriteMaterial = MaterialManager::FindOrNull("AlphaSprite");

  frameNode.mMeshRenderData = nullptr;
  frameNode.mMaterialRenderData = spriteMaterial->mRenderData;
  frameNode.mTextureRenderData = texture->mRenderData;

  // default setup for adding streamed data
  viewNode.mStreamedVertexType = PrimitiveType::Triangles;
  viewNode.mStreamedVertexStart = frameBlock.mRenderQueues->mStreamedVertices.Size();
  viewNode.mStreamedVertexCount = 0;

  return viewNode;
}
} // namespace Zero