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
  mUIManager = TBUIManager::GetInstance();
  mRenderer = mUIManager->GetRenderer();

  // Set gravity all so we resize correctly
  SetGravity(tb::WIDGET_GRAVITY_ALL);

  // Set initial size for view
  tb::TBRect rect = mUIManager->GetRootWidget()->GetRect();
  SetSize(rect.w, rect.h);

  mUIManager->AddView(this);
}
void TBUIView::Render()
{
  Render(mBatches, 0, mBatches.Size());
}
void TBUIView::UpdateBatches()
{
  mBatches.Clear();
  mVertices.Clear();

  tb::TBRect rect = GetRect();

  IntRect clipRect = IntRect(rect.x, rect.y, rect.w, rect.h);
  GetBatches(mBatches, mVertices, clipRect);
}
void TBUIView::GetBatches(Array<TBUIBatch>& batches, Array<StreamedVertex>& vertices, const IntRect& clipRect)
{
  tb::g_renderer->BeginPaint(clipRect.SizeX, clipRect.SizeY);

  mRenderer->mClipRect = clipRect;
  mRenderer->mBatches = &batches;
  mRenderer->mVertices = &vertices;
  InvokePaint(tb::TBWidget::PaintProps());

  tb::g_renderer->EndPaint();
}
void TBUIView::Render(const Array<TBUIBatch>& batches, u32 batchStart, u32 batchEnd)
{
  if (batches.Empty())
  {
    return;
  }

  IntVec2 size;

  if (mRenderTexture != nullptr)
  {
    size.x = mRenderTexture->mWidth;
    size.y = mRenderTexture->mHeight;
  }
  else
  {
    OsShell* shell = Z::gEngine->has(OsShell);
    OsWindow* osWindow = shell->GetWindow(0);
    IntVec2 clientSize = osWindow->GetClientSize();
    IntVec2 clientPos = osWindow->GetMonitorClientPosition();
    size.x = clientSize.x;
    size.y = clientSize.y;
  }

  for (u32 i = batchStart; i < batchEnd; i++)
  {
    RenderBatch(batches[i]);
  }
}

void TBUIView::RenderBatch(const TBUIBatch& batch)
{
  ColorTransform colorTx = {Vec4(1, 1, 1, 1)};
  Mat4 localTx;
  this->mTranslation = Vec3{0, 0, 0};
  this->mAngle = 0;
  Build2dTransform(localTx, this->mTranslation, this->mAngle);
  mWorldTx = localTx * Mat4::cIdentity;

  //tb::TBRect rect = 

  //Vec2 size = Vec2{(real)rect.w, (real)rect.h};
  //Vec2 pos = Vec2{(real)rect.x, (real)rect.y};

  IntRect clipRect = batch.mClipRect;

  Vec4 clearColor = Vec4{0.5f, 0.7f, 0.3f, 0.1f};

  GraphicsEngine* graphics = Z::gEngine->has(GraphicsEngine);
  RenderTasks& renderTasks = *graphics->mRenderTasksBack;
  RenderQueues& renderQueues = *graphics->mRenderQueuesBack;
  renderQueues.mRenderTasks = &renderTasks;

  FrameBlock& frameBlock = renderQueues.mFrameBlocks.PushBack();
  ViewBlock& viewBlock = renderQueues.mViewBlocks.PushBack();
  frameBlock.mRenderQueues = &renderQueues;

  Mat4 translation;
  translation.Translate(clipRect.SizeX * -0.5f, clipRect.SizeY * -0.5f, 0.0f);
  Mat4 scale;
  scale.Scale(1.0f, -1.0f, 1.0f);
  viewBlock.mWorldToView = scale * translation;
  BuildOrthographicTransformZero(
      viewBlock.mViewToPerspective, clipRect.SizeY, clipRect.SizeX / clipRect.SizeY, -1.0f, 1.0f);

  Mat4 apiPerspective;
  Z::gRenderer->BuildOrthographicTransform(
      apiPerspective, clipRect.SizeY, clipRect.SizeX / clipRect.SizeY, -1.0f, 1.0f);
  viewBlock.mZeroPerspectiveToApiPerspective = apiPerspective * viewBlock.mViewToPerspective.Inverted();

  //
  CreateRenderData(viewBlock, frameBlock, clipRect, *(batch.mVertices), batch.mTexture, PrimitiveType::Triangles);
  //

  IndexRange& indexRange = viewBlock.mRenderGroupRanges.PushBack();
  indexRange.start = 0;
  indexRange.end = viewBlock.mViewNodes.Size();

  RenderTaskRange& renderTaskRange = renderTasks.mRenderTaskRanges.PushBack();
  renderTaskRange.mFrameBlockIndex = renderQueues.mFrameBlocks.Size() - 1;
  renderTaskRange.mViewBlockIndex = renderQueues.mViewBlocks.Size() - 1;
  renderTaskRange.mTaskIndex = renderTasks.mRenderTaskBuffer.mCurrentIndex;
  renderTaskRange.mTaskCount = 0;

  HandleOf<RenderTarget> renderTarget =
      Z::gEngine->has(GraphicsEngine)
          ->GetRenderTarget((uint)clipRect.SizeX, (uint)clipRect.SizeY, TextureFormat::RGBA8);

  GraphicsRenderSettings renderSettings;
  renderSettings.SetColorTarget(renderTarget);
  renderSettings.mBlendSettings[0] = batch.mBlendSettings;
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

  ScreenViewport viewport = {clipRect.X, clipRect.Y, clipRect.SizeX, clipRect.SizeY};
  helper.AddRenderTaskBackBufferBlit(renderTarget, viewport);

  renderTaskRange.mTaskCount = 2;

  if (renderTarget && renderTarget->mTexture)
  {
    Texture* texture = renderTarget->mTexture;
    graphics->WriteTextureToFile(texture, "output2.png");
  }

  Z::gEngine->has(GraphicsEngine)->ClearRenderTargets();
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

  for (uint i = 0; i < vertices.Size(); ++i)
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

  // frameNode.mClip = Vec4(clipRect.GetLeft(), clipRect.GetTop(), clipRect.GetRight(), clipRect.GetBottom());
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