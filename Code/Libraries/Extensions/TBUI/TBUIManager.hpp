// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

class TBUIWidget;

class TBUIManager : public ExplicitSingleton<TBUIManager, EventObject>, private tb::TBWidgetListener
{
  friend class TBUIView;

public:
  ZilchDeclareType(TBUIManager, TypeCopyMode::ReferenceType);

  TBUIManager();
  ~TBUIManager();

  void OnEngineUpdate(UpdateEvent* event);
  void OnShutdown(Event* event);

  ZeroForceInline TBUIRenderer* GetRenderer()
  {
    return mRenderer;
  }

  ZeroForceInline tb::TBWidget* GetRootWidget()
  {
    return mRoot;
  }

  ZeroForceInline TBUIView* GetFocusedView()
  {
    return mFocusedView;
  }

private:
  void OnUiUpdate(UpdateEvent* event);
  void OnUiRenderUpdate(Event* event);

  void OnOsMouseUp(OsMouseEvent* mouseEvent);
  void OnOsMouseDown(OsMouseEvent* mouseEvent);
  void OnOsMouseMoved(OsMouseEvent* mouseEvent);

  

  void AddView(TBUIView* view);
  void RemoveView(TBUIView* view);
  void SetFocusedView(TBUIView* view);


  virtual void OnWidgetDelete(tb::TBWidget* widget) override
  {
  }

  virtual bool OnWidgetDying(tb::TBWidget* widget) override
  {
    return false;
  }

  virtual void OnWidgetAdded(tb::TBWidget* parent, tb::TBWidget* child) override
  {
  }

  virtual void OnWidgetRemove(tb::TBWidget* parent, tb::TBWidget* child) override
  {
  }

  virtual void OnWidgetFocusChanged(tb::TBWidget* widget, bool focused) override
  {
  }

  virtual bool OnWidgetInvokeEvent(tb::TBWidget* widget, const tb::TBWidgetEvent& ev) override
  {
    return false;
  }

private:
  tb::TBWidget* mRoot;
  TBUIRenderer* mRenderer;
  Array<TBUIView*> mViews;
  TBUIView* mFocusedView = nullptr;
};

namespace Z
{
extern TBUIManager* gTBUIManager;
}

} // namespace Zero