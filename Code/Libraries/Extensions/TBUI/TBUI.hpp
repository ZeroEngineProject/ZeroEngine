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

  void AddView(TBUIView* view)
  {
    mRoot->AddChild(view);
    mViews.PushBack(view);
  }

  void RemoveView(TBUIView* view)
  {
    mRoot->RemoveChild(view);
    mViews.EraseValue(view);
  }

private:
  void OnUiUpdate(UpdateEvent* event);
  void OnUiRenderUpdate(Event* event);

  void OnOsMouseUp(OsMouseEvent* mouseEvent);
  void OnOsMouseDown(OsMouseEvent* mouseEvent);
  void OnOsMouseMoved(OsMouseEvent* mouseEvent);

  void SetupDemo();

private:
  friend class TBUIView;
  OsWindow* mOsWindow = nullptr;

  tb::TBWidget* mRoot;
  tb::TBWindow* mMainWindow;
  TBUIRenderer* mRenderer;

  Array<TBUIView*> mViews;

  bool mDemoSetup = false;
  bool mHookEvents = false;
};

namespace Z
{
extern TBUI* gTBUI;
}

} // namespace Zero