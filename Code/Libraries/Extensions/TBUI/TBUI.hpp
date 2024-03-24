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

private:
  void OnUiUpdate(UpdateEvent* event);
  void OnUiRenderUpdate(Event* event);

  void OnOsMouseUp(OsMouseEvent* mouseEvent);
  void OnOsMouseDown(OsMouseEvent* mouseEvent);
  void OnOsMouseMoved(OsMouseEvent* mouseEvent);

private:
  tb::TBWidget* mRoot;
  TBUIRenderer* mRenderer;
};

namespace Z
{
extern TBUI* gTBUI;
}

} // namespace Zero