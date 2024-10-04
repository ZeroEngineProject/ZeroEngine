#ifndef EE_UI_UIWIDGETCREATOR_HPP
#define EE_UI_UIWIDGETCREATOR_HPP

#include <eepp/core.hpp>
#include "uiwidget.hpp"

namespace Zero { namespace UI {

class ZeroShared UIWidgetCreator {
  public:
	typedef std::function<UIWidget*( const String& )> CustomWidgetCb;
	typedef std::function<UIWidget*()> RegisterWidgetCb;

	typedef std::unordered_map<String, UIWidgetCreator::CustomWidgetCb> WidgetCallbackMap;
	typedef std::unordered_map<String, UIWidgetCreator::RegisterWidgetCb>
		RegisteredWidgetCallbackMap;

	static UIWidget* createFromName( const String& widgetName );

	static void addCustomWidgetCallback( const String& widgetName, const CustomWidgetCb& cb );

	static void removeCustomWidgetCallback( const String& widgetName );

	static bool existsCustomWidgetCallback( const String& widgetName );

	static void registerWidget( const String& widgetName, const RegisterWidgetCb& cb );

	static void unregisterWidget( const String& widgetName );

	static bool isWidgetRegistered( const String& widgetName );

	static const RegisteredWidgetCallbackMap& getRegisteredWidgets();

	static Array<String> getWidgetNames();

  protected:
	static RegisteredWidgetCallbackMap registeredWidget;

	static WidgetCallbackMap widgetCallback;

	static void createBaseWidgetList();
};

}} // namespace Zero::UI

#endif
