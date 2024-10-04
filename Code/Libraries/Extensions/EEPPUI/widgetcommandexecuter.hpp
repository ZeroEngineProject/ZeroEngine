#ifndef EE_UI_WIDGETCOMMANDEXECUTER_HPP
#define EE_UI_WIDGETCOMMANDEXECUTER_HPP

#include <eepp/scene/keyevent.hpp>
#include <eepp/ui/keyboardshortcut.hpp>
#include <functional>

using Zero::Scene;

namespace Zero { namespace UI {

class ZeroShared WidgetCommandExecuter {
  public:
	typedef std::function<void()> CommandCallback;

	WidgetCommandExecuter( const KeyBindings& keybindings ) : mKeyBindings( keybindings ) {}

	void setCommand( const String& name, const CommandCallback& cb ) {
		auto cmdIt = mCommands.find( name );
		if ( cmdIt == mCommands.end() ) {
			mCommands[name] = cb;
			mCommandList.emplace_back( name );
		} else {
			cmdIt->second = cb;
		}
	}

	bool hasCommand( const String& name ) const {
		return mCommands.find( name ) != mCommands.end();
	}

	void execute( const String& command ) {
		auto cmdIt = mCommands.find( command );
		if ( cmdIt != mCommands.end() )
			cmdIt->second();
	}

	size_t commandCount() const { return mCommands.size(); }

	KeyBindings& getKeyBindings() { return mKeyBindings; }

	const Array<String>& getCommandList() const { return mCommandList; }

  protected:
	KeyBindings mKeyBindings;
	std::unordered_map<String, std::function<void()>> mCommands;
	Array<String> mCommandList;

	Uint32 onKeyDown( const KeyEvent& event ) {
		String cmd =
			mKeyBindings.getCommandFromKeyBind( { event.getKeyCode(), event.getMod() } );
		if ( !cmd.empty() ) {
			auto cmdIt = mCommands.find( cmd );
			if ( cmdIt != mCommands.end() ) {
				cmdIt->second();
				return 1;
			}
		}
		return 0;
	}
};

}} // namespace Zero::UI

#endif // EE_UI_WIDGETCOMMANDEXECUTER_HPP
