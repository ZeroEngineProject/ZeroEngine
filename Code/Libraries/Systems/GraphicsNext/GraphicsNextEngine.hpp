// MIT Licensed (see LICENSE.md).

#pragma once

namespace Zero
{

/// System object for graphics.
class GraphicsNextEngine : public System
{
public:
  ZilchDeclareType(GraphicsNextEngine, TypeCopyMode::ReferenceType);

  GraphicsNextEngine();
  ~GraphicsNextEngine();

  cstr GetName() override;
  void Initialize(SystemInitializer& initializer) override;
  void Update(bool debugger) override;

  void OnEngineShutdown(Event* event);

  void AddSpace(GraphicsNextSpace* space);
  void RemoveSpace(GraphicsNextSpace* space);

  void OnOsWindowMinimized(Event* event);
  void OnOsWindowRestored(Event* event);

  void OnProjectCogModified(Event* event);
  void SetVerticalSync(bool verticalSync);

  void OnResourcesAdded(ResourceEvent* event);
  void OnResourcesRemoved(ResourceEvent* event);

  void ProcessModifiedScripts(LibraryRef library);

  typedef InList<GraphicsNextSpace, &GraphicsNextSpace::EngineLink> GraphicsSpaceList;
  GraphicsSpaceList mSpaces;

  

  bool mNewLibrariesCommitted;

  uint mFrameCounter;

 
  bool mVerticalSync;

  bool mEngineShutdown;
  // Renderer thread
  Thread mRendererThread;
};

} // namespace Zero
