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
void TBUIView::UpdateBatches()
{
}
} // namespace Zero