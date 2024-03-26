// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

ZilchDefineType(TBUI, builder, type)
{
}

TBUI::TBUI() : mRoot(nullptr)
{
  Z::gTBUI = this;

  mRenderer = new TBUIRenderer();

  ConnectThisTo(Z::gEngine, Events::EngineUpdate, OnEngineUpdate);
  ConnectThisTo(Z::gEngine, Events::EngineDebuggerUpdate, OnEngineUpdate);
  ConnectThisTo(Z::gEngine, Events::EngineInitialized, OnInitialize);
  ConnectThisTo(Z::gEngine, Events::EngineShutdown, OnShutdown);
}

TBUI::~TBUI()
{
  if (mRoot != nullptr)
  {
    delete mRoot;
    mRoot = nullptr;
  }
  delete mRenderer;
}

void TBUI::OnUiUpdate(UpdateEvent* event)
{
  IntVec2 clientSize = mOsWindow->GetClientSize();
  IntVec2 clientPos = mOsWindow->GetMonitorClientPosition();

  mRoot->SetRect(tb::TBRect(clientPos.x, clientPos.y, clientSize.x, clientSize.y));
  tb::TBAnimationManager::Update();
  mRoot->InvokeProcessStates();
  mRoot->InvokeProcess();

  tb::TBMessageHandler::ProcessMessages();
}

void TBUI::OnUiRenderUpdate(Event* event)
{
  forRange (TBUIView* view, mViews.All())
  {
    view->UpdateBatches();
  }

  forRange (TBUIView* view, mViews.All())
  {
    view->RenderBatches();
  }
}

void TBUI::OnEngineUpdate(UpdateEvent* event)
{
  if (mHookEvents)
  {
    ConnectThisTo(Z::gEngine->has(TimeSystem), "UiUpdate", OnUiUpdate);
    ConnectThisTo(Z::gEngine->has(GraphicsEngine), "UiRenderUpdate", OnUiRenderUpdate);

    mHookEvents = false;
  }
}

void TBUI::OnInitialize(Event* event)
{
  OsShell* shell = Z::gEngine->has(OsShell);
  mOsWindow = shell->GetWindow(0);

  if (!tb::tb_core_is_initialized())
  {
    if (tb::tb_core_init(mRenderer))
    {
      // Load language file
      tb::g_tb_lng->Load("@TBUI/resources/language/lng_en.tb.txt");

      // Load the default skin
      tb::g_tb_skin->Load("@TBUI/resources/default_skin/skin.tb.txt", "@TBUI/demo/skin/skin.tb.txt");

      // Register font renderers.
#ifdef TB_FONT_RENDERER_TBBF
      void register_tbbf_font_renderer();
      register_tbbf_font_renderer();
#endif

#ifdef TB_FONT_RENDERER_STB
      void register_stb_font_renderer();
      register_stb_font_renderer();
#endif
#ifdef TB_FONT_RENDERER_FREETYPE
      void register_freetype_font_renderer();
      register_freetype_font_renderer();
#endif

      // Add fonts we can use to the font manager.
#if defined(TB_FONT_RENDERER_STB) || defined(TB_FONT_RENDERER_FREETYPE)
      tb::g_font_manager->AddFontInfo("@TBUI/resources/vera.ttf", "Vera");
#endif
#ifdef TB_FONT_RENDERER_TBBF
      tb::g_font_manager->AddFontInfo("@TBUI/resources/default_font/segoe_white_with_shadow.tb.txt", "Segoe");
#endif

      // Set the default font description for widgets to one of the fonts we just added
      tb::TBFontDescription fd;
#ifdef TB_FONT_RENDERER_TBBF
      fd.SetID(TBIDC("Segoe"));
#else
      fd.SetID(TBIDC("Vera"));
#endif
      fd.SetSize(tb::g_tb_skin->GetDimensionConverter()->DpToPx(14));
      tb::g_font_manager->SetDefaultFontDescription(fd);

      // Create the font now.
      tb::TBFontFace* font = tb::g_font_manager->CreateFontFace(tb::g_font_manager->GetDefaultFontDescription());

      // Render some glyphs in one go now since we know we are going to use them. It would work fine
      // without this since glyphs are rendered when needed, but with some extra updating of the glyph bitmap.
      if (font)
        font->RenderGlyphs(" !\"#$%&'()*+,-./"
                           "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~•·åäöÅÄÖ");

      IntVec2 clientSize = mOsWindow->GetClientSize();
      IntVec2 clientPos = mOsWindow->GetMonitorClientPosition();

      mRoot = new tb::TBWidget();
      mRoot->SetRect(tb::TBRect(clientPos.x, clientPos.y, clientSize.x, clientSize.y));
      // mRoot->SetOpacity(0.1);
      SetupDemo();

      // Give the root widget a background skin
      mRoot->SetSkinBg(TBIDC("background"));

      tb::TBWidgetsAnimationManager::Init();

      // ConnectThisTo(osWindow, Events::OsResized, OnOsResize);
      ConnectThisTo(mOsWindow, Events::OsMouseDown, OnOsMouseDown);
      ConnectThisTo(mOsWindow, Events::OsMouseUp, OnOsMouseUp);
      ConnectThisTo(mOsWindow, Events::OsMouseMove, OnOsMouseMoved);

      // ConnectThisTo(mOsWindow, Events::OsWindowBorderHitTest, OnOsWindowBorderHitTest);

      // ConnectThisTo(mOsWindow, Events::OsMouseScroll, OnOsMouseScroll);

      // ConnectThisTo(mOsWindow, Events::OsKeyTyped, OnOsKeyTyped);
      // ConnectThisTo(mOsWindow, Events::OsKeyRepeated, OnOsKeyDown);
      // ConnectThisTo(mOsWindow, Events::OsKeyDown, OnOsKeyDown);
      // ConnectThisTo(mOsWindow, Events::OsKeyUp, OnOsKeyUp);

      // ConnectThisTo(mOsWindow, Events::OsFocusGained, OnOsFocusGained);
      // ConnectThisTo(mOsWindow, Events::OsFocusLost, OnOsFocusLost);

      // ConnectThisTo(mOsWindow, Events::OsMouseFileDrop, OnOsMouseDrop);
      // ConnectThisTo(mOsWindow, Events::OsPaint, OnOsPaint);

      // ConnectThisTo(mOsWindow, Events::OsClose, OnClose);

      // ConnectThisTo(Z::gEngine->has(TimeSystem), "UiUpdate", OnUiUpdate);
      // ConnectThisTo(Z::gEngine->has(GraphicsEngine), "UiRenderUpdate", OnUiRenderUpdate);
    }
  }
}

void TBUI::OnShutdown(Event* event)
{
  if (tb::tb_core_is_initialized())
  {
    tb::TBWidgetsAnimationManager::Shutdown();
    tb::tb_core_shutdown();
  }
}

tb::MODIFIER_KEYS GetModifierKeys(OsMouseEvent* mouseEvent)
{
  tb::MODIFIER_KEYS modifierKeys = tb::MODIFIER_KEYS::TB_MODIFIER_NONE;

  if (mouseEvent->AltPressed)
    modifierKeys |= tb::MODIFIER_KEYS::TB_ALT;

  if (mouseEvent->ShiftPressed)
    modifierKeys |= tb::MODIFIER_KEYS::TB_SHIFT;

  if (mouseEvent->CtrlPressed)
    modifierKeys |= tb::MODIFIER_KEYS::TB_CTRL;

  return modifierKeys;
}

void TBUI::OnOsMouseUp(OsMouseEvent* mouseEvent)
{
  if (mouseEvent->mTerminated)
    return;

  tb::MODIFIER_KEYS modifierKeys = GetModifierKeys(mouseEvent);

  if (mRoot->InvokePointerUp(mouseEvent->ClientPosition.x, mouseEvent->ClientPosition.y, modifierKeys, false))
    mouseEvent->Terminate();
}

void TBUI::OnOsMouseDown(OsMouseEvent* mouseEvent)
{
  if (mouseEvent->mTerminated)
    return;

  tb::MODIFIER_KEYS modifierKeys = GetModifierKeys(mouseEvent);

  if (mRoot->InvokePointerDown(mouseEvent->ClientPosition.x, mouseEvent->ClientPosition.y, 1, modifierKeys, false))
    mouseEvent->Terminate();
}

void TBUI::OnOsMouseMoved(OsMouseEvent* mouseEvent)
{
  tb::MODIFIER_KEYS modifierKeys = GetModifierKeys(mouseEvent);

  mRoot->InvokePointerMove(mouseEvent->ClientPosition.x, mouseEvent->ClientPosition.y, modifierKeys, false);
}

void TBUI::SetupDemo()
{
  if (mDemoSetup)
    return;

  TBUIView* view = new TBUIView();
  mMainWindow = new tb::TBWindow();

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

  mDemoSetup = true;
  mHookEvents = true;
}

namespace Z
{
TBUI* gTBUI = nullptr;
}

} // namespace Zero