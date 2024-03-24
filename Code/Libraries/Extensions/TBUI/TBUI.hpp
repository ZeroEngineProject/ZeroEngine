// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

class TBUIContainer;

class TBUI : public ExplicitSingleton<TBUI, EventObject>
{
public:
  ZilchDeclareType(TBUI, TypeCopyMode::ReferenceType);

  TBUI();
  ~TBUI();

  void OnEngineUpdate(UpdateEvent* event);
  void OnInitialize(Event* event);
  void OnShutdown(Event* event);

  ZeroForceInline tb::TBWidget* GetRootWidget()
  {
    return mRoot;
  }

  ZeroForceInline TBUIRenderer* GetRenderer()
  {
    return mRenderer;
  }

  void AddView(TBUIView* view);
  void RemoveView(TBUIView* view);

private:
  void OnUiUpdate(UpdateEvent* event);
  void OnUiRenderUpdate(Event* event);

  void Render();

  void OnOsMouseUp(OsMouseEvent* mouseEvent);
  void OnOsMouseDown(OsMouseEvent* mouseEvent);
  void OnOsMouseMoved(OsMouseEvent* mouseEvent);

  void InitializeDemo();

private:
  UniquePointer<tb::TBWidget> mRoot;
  TBUIRenderer* mRenderer;

  Array<TBUIView*> mViews;

  bool mDemoInitialized = false;
  UniquePointer<TBUIView> mDemoView;
};

namespace Z
{
extern TBUI* gTBUI;
}

} // namespace Zero