// MIT Licensed (see LICENSE.md).
#pragma once

#include "Engine/EngineStandard.hpp"
#include "Graphics/GraphicsStandard.hpp"

namespace Zero
{
// Forward declarations

// Widget library
class ZeroNoImportExport EEPPUILibrary : public Zilch::StaticLibrary
{
public:
  ZilchDeclareStaticLibraryInternals(EEPPUILibrary, "ZeroEngine");

  static void Initialize();
  static void Shutdown();
};

} // namespace Zero

#include "uicheckbox.hpp"
#include "uicodeeditor.hpp"
#include "uicombobox.hpp"
#include "uiconsole.hpp"
#include "uidropdownlist.hpp"
#include "uifiledialog.hpp"
#include "uigridlayout.hpp"
#include "uiiconthememanager.hpp"
#include "uiimage.hpp"
#include "uilinearlayout.hpp"
#include "uilistbox.hpp"
#include "uilistboxitem.hpp"
#include "uiloader.hpp"
#include "uimenu.hpp"
#include "uimenubar.hpp"
#include "uimenucheckbox.hpp"
#include "uimenuitem.hpp"
#include "uimenuradiobutton.hpp"
#include "uimenuseparator.hpp"
#include "uimessagebox.hpp"
#include "uinode.hpp"
#include "uipopupmenu.hpp"
#include "uiprogressbar.hpp"
#include "uipushbutton.hpp"
#include "uiradiobutton.hpp"
#include "uirelativelayout.hpp"
#include "uiscenenode.hpp"
#include "uiscrollbar.hpp"
#include "uiscrollview.hpp"
#include "uiselectbutton.hpp"
#include "uiskin.hpp"
#include "uislider.hpp"
#include "uispinbox.hpp"
#include "uisplitter.hpp"
#include "uisprite.hpp"
#include "uistacklayout.hpp"
#include "uistyle.hpp"
#include "uitab.hpp"
#include "uitableview.hpp"
#include "uitabwidget.hpp"
#include "uitextedit.hpp"
#include "uitextinput.hpp"
#include "uitextinputpassword.hpp"
#include "uitextureregion.hpp"
#include "uitextview.hpp"
#include "uitheme.hpp"
#include "uithememanager.hpp"
#include "uitouchdraggablewidget.hpp"
#include "uitreeview.hpp"
#include "uiviewpager.hpp"
#include "uiwidget.hpp"
#include "uiwidgetcreator.hpp"
#include "uiwidgettable.hpp"
#include "uiwidgettablerow.hpp"
#include "uiwindow.hpp"
#include "widgetcommandexecuter.hpp"

#include "css/propertydefinition.hpp"
#include "css/propertyidset.hpp"
#include "css/propertyspecification.hpp"
#include "css/stylesheet.hpp"
#include "css/stylesheetparser.hpp"
#include "css/stylesheetpropertiesparser.hpp"
#include "css/stylesheetproperty.hpp"
#include "css/stylesheetselector.hpp"
#include "css/stylesheetselectorparser.hpp"
#include "css/stylesheetstyle.hpp"

#include "doc/syntaxdefinitionmanager.hpp"
#include "doc/textdocument.hpp"

#include "models/csspropertiesmodel.hpp"
#include "models/filesystemmodel.hpp"
#include "models/model.hpp"
#include "models/modelselection.hpp"
#include "models/sortingproxymodel.hpp"
#include "models/widgettreemodel.hpp"

#include "uidatabind.hpp"

#include "undostack.hpp"

#include "uiapplication.hpp"
