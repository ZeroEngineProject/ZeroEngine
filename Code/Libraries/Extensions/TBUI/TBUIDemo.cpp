#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineType(TBUIDemo, builder, type)
{
  ZeroBindDocumented();
}

TBUIDemo::TBUIDemo(TBUIView* view)
{
  mMainWindow = MakeUnique<tb::TBWindow>();

  view->AddChild(mMainWindow);

  tb::TBNode node;
  if (node.ReadFile("@TBUI/demo/ui_resources/test_ui.tb.txt"))
  {
    tb::g_widgets_reader->LoadNodeTree(mMainWindow, &node);

    // Get title from the WindowInfo section (or use "" if not specified)
    mMainWindow->SetText(node.GetValueString("WindowInfo>title", ""));

    const tb::TBRect parent_rect(0, 0, mMainWindow->GetParent()->GetRect().w, mMainWindow->GetParent()->GetRect().h);
    const tb::TBDimensionConverter* dc = tb::g_tb_skin->GetDimensionConverter();
    tb::TBRect window_rect = mMainWindow->GetResizeToFitContentRect();

    // Use specified size or adapt to the preferred content size.
    tb::TBNode* tmp = node.GetNode("WindowInfo>size");
    if (tmp && tmp->GetValue().GetArrayLength() == 2)
    {
      window_rect.w = dc->GetPxFromString(tmp->GetValue().GetArray()->GetValue(0)->GetString(), window_rect.w);
      window_rect.h = dc->GetPxFromString(tmp->GetValue().GetArray()->GetValue(1)->GetString(), window_rect.h);
    }

    // Use the specified position or center in parent.
    tmp = node.GetNode("WindowInfo>position");
    if (tmp && tmp->GetValue().GetArrayLength() == 2)
    {
      window_rect.x = dc->GetPxFromString(tmp->GetValue().GetArray()->GetValue(0)->GetString(), window_rect.x);
      window_rect.y = dc->GetPxFromString(tmp->GetValue().GetArray()->GetValue(1)->GetString(), window_rect.y);
    }
    else
      window_rect = window_rect.CenterIn(parent_rect);

    // Make sure the window is inside the parent, and not larger.
    window_rect = window_rect.MoveIn(parent_rect).Clip(parent_rect);

    mMainWindow->SetRect(window_rect);

    // mainWindow->SetOpacity(1);
  }
}

TBUIDemo::~TBUIDemo()
{
  if (mMainWindow)
  {
    //mMainWindow->RemoveFromParent();
    //mMainWindow.Reset();
  }
}
} // namespace Zero