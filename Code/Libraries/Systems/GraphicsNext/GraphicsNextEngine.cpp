// MIT Licensed (see LICENSE.md).

#include "Precompiled.hpp"

namespace Zero
{

System* CreateGraphicsNextSystem()
{
  return new GraphicsNextEngine();
}

Memory::Pool* gShaderPool = nullptr;

ZilchDefineType(GraphicsNextEngine, builder, type)
{
}

GraphicsNextEngine::GraphicsNextEngine() : mNewLibrariesCommitted(false)
{
  mEngineShutdown = false;
}

GraphicsNextEngine::~GraphicsNextEngine()
{
}

cstr GraphicsNextEngine::GetName()
{
  return "GraphicsNext";
}

void GraphicsNextEngine::Initialize(SystemInitializer& initializer)
{

  ConnectThisTo(Z::gEngine, Events::EngineShutdown, OnEngineShutdown);

  ConnectThisTo(Z::gResources, Events::ResourcesLoaded, OnResourcesAdded);
  ConnectThisTo(Z::gResources, Events::ResourcesUnloaded, OnResourcesRemoved);

  
  gShaderPool = new Memory::Pool("Shaders", Memory::GetRoot(), sizeof(Shader), 1024);

  mFrameCounter = 0;
  mVerticalSync = false;
}

void GraphicsNextEngine::Update(bool debugger)
{
  // Do not try to run rendering while this job is going.
  if (ThreadingEnabled)
    return;

  // Do any deferred tasks
  if (!ThreadingEnabled)
  {
    //RendererThreadMain(mRendererJobQueue);
  }

  ZilchManager::GetInstance()->mDebugger.DoNotAllowBreakReason =
      "Cannot currently break within the graphics engine because it must "
      "continue running in editor";

  ProfileScopeTree("GraphicsNext", "Engine", Color::Blue);

  ++mFrameCounter;

  {
    ProfileScopeTree("UiRenderUpdate", "Graphics", Color::DarkOliveGreen);
    // add ui render task range after sorting so that everything else renders
    // before it
    Event event;
    DispatchEvent("UiRenderUpdate", &event);
  }

  if (gDebugDraw->MaxCountExceeded())
    DoNotifyWarning("Max debug object count exceeded.",
                    "To edit the max count, open the Select menu and choose "
                    "'Select Project'. "
                    "Expand the component 'DebugSettings' (or add it) and "
                    "modify 'MaxDebugObjects'.");

  gDebugDraw->ClearObjects();

  ZilchManager::GetInstance()->mDebugger.DoNotAllowBreakReason.Clear();
}

void GraphicsNextEngine::OnEngineShutdown(Event* event)
{
  
  mEngineShutdown = true;
}

void GraphicsNextEngine::AddSpace(GraphicsNextSpace* space)
{
  mSpaces.PushBack(space);
}

void GraphicsNextEngine::RemoveSpace(GraphicsNextSpace* space)
{
  mSpaces.Erase(space);
}

void GraphicsNextEngine::OnOsWindowMinimized(Event* event)
{
  //Z::gRenderer->mThreadLock.Lock();
  //Z::gRenderer->mBackBufferSafe = false;
  //Z::gRenderer->mThreadLock.Unlock();
}

void GraphicsNextEngine::OnOsWindowRestored(Event* event)
{
  //Z::gRenderer->mThreadLock.Lock();
  //Z::gRenderer->mBackBufferSafe = true;
  //Z::gRenderer->mThreadLock.Unlock();
}

void GraphicsNextEngine::OnProjectCogModified(Event* event)
{
  //if (FrameRateSettings* frameRate = mProjectCog.has(FrameRateSettings))
  //  SetVerticalSync(frameRate->mVerticalSync && !frameRate->mLimitFrameRate);
  //else
  //  SetVerticalSync(false);

  //if (DebugSettings* debugSettings = mProjectCog.has(DebugSettings))
  //  gDebugDraw->SetMaxDebugObjects(debugSettings->GetMaxDebugObjects());
  //else
  //  gDebugDraw->SetMaxDebugObjects();
}

void GraphicsNextEngine::SetVerticalSync(bool verticalSync)
{
  if (verticalSync == mVerticalSync)
    return;

  mVerticalSync = verticalSync;

  //SetVSyncJob* setVSyncJob = new SetVSyncJob();
  //setVSyncJob->mVSync = mVerticalSync;
  //AddRendererJob(setVSyncJob);
}

/*
void GraphicsNextEngine::CreateRenderer(OsWindow* mainWindow)
{
  OsHandle mainWindowHandle = mainWindow->GetWindowHandle();

  CreateRendererJob* rendererJob = new CreateRendererJob();
  rendererJob->mMainWindowHandle = mainWindowHandle;
  AddRendererJob(rendererJob);
  rendererJob->WaitOnThisJob();

  if (rendererJob->mError.Empty() == false)
    FatalEngineError(rendererJob->mError.c_str());

  delete rendererJob;

  gIntelGraphics = Z::gRenderer->mDriverSupport.mIntel;

  ConnectThisTo(mainWindow, Events::OsWindowMinimized, OnOsWindowMinimized);
  ConnectThisTo(mainWindow, Events::OsWindowRestored, OnOsWindowRestored);
}

void GraphicsNextEngine::DestroyRenderer()
{
  DestroyRendererJob* rendererJob = new DestroyRendererJob();
  rendererJob->mRendererJobQueue = mRendererJobQueue;
  AddRendererJob(rendererJob);
  rendererJob->WaitOnThisJob();
  delete rendererJob;
}
*/

void GraphicsNextEngine::ProcessModifiedScripts(LibraryRef library)
{
  /*
  forRange (BoundType* type, library->BoundTypes.Values())
  {
    String typeName = type->Name;

    // If already an entry for this type then inputs might have changed
    bool oldEntries = mComponentShaderProperties.ContainsKey(typeName);
    mComponentShaderProperties.Erase(typeName);

    // Only look for inputs on component types
    if (type->IsA(ZilchTypeId(Component)))
    {

      forRange (Property* metaProperty, type->GetProperties())
      {
        forRange (MetaShaderInput* shaderInput, metaProperty->HasAll<MetaShaderInput>())
        {
          ShaderMetaProperty shaderProperty;
          shaderProperty.mMetaPropertyName = metaProperty->Name;
          shaderProperty.mFragmentName = shaderInput->mFragmentName;
          shaderProperty.mInputName = shaderInput->mInputName;

          mComponentShaderProperties[typeName].PushBack(shaderProperty);
        }
      }
    }

    bool newEntries = mComponentShaderProperties.ContainsKey(typeName);

    // Forward to Graphicals if any relevant changes
    if (oldEntries || newEntries)
    {
      ShaderInputsEvent event;
      event.mType = type;
      DispatchEvent(Events::ShaderInputsModified, &event);
    }
  }
  */
}
} // namespace Zero
