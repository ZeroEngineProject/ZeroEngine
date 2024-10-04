#include <eepp/ui/keyboardshortcut.hpp>
#include <eepp/window/input.hpp>

using Zero::Window;

namespace Zero { namespace UI {

KeyBindings::Shortcut KeyBindings::sanitizeShortcut( const KeyBindings::Shortcut& shortcut ) {
	KeyBindings::Shortcut sanitized( shortcut.key, 0 );
	if ( shortcut.mod & KEYMOD_CTRL )
		sanitized.mod |= KEYMOD_CTRL;
	if ( shortcut.mod & KEYMOD_SHIFT )
		sanitized.mod |= KEYMOD_SHIFT;
	if ( shortcut.mod & KEYMOD_META )
		sanitized.mod |= KEYMOD_META;
	if ( shortcut.mod & KEYMOD_LALT )
		sanitized.mod |= KEYMOD_LALT;
	if ( shortcut.mod & KEYMOD_RALT )
		sanitized.mod |= KEYMOD_RALT;
	return sanitized;
}

KeyBindings::KeyBindings( const Window::Input* input ) : mInput( input ) {}

void KeyBindings::addKeybindsString( const std::map<String, String>& binds ) {
	for ( auto& bind : binds ) {
		addKeybindString( bind.first, bind.second );
	}
}

void KeyBindings::addKeybinds( const std::map<KeyBindings::Shortcut, String>& binds ) {
	for ( auto& bind : binds ) {
		addKeybind( bind.first, bind.second );
	}
}

void KeyBindings::addKeybindsStringUnordered(
	const std::unordered_map<String, String>& binds ) {
	for ( auto& bind : binds ) {
		addKeybindString( bind.first, bind.second );
	}
}

void KeyBindings::addKeybindsUnordered(
	const std::unordered_map<KeyBindings::Shortcut, String>& binds ) {
	for ( auto& bind : binds ) {
		addKeybind( bind.first, bind.second );
	}
}

void KeyBindings::addKeybindString( const String& key, const String& command ) {
	addKeybind( getShortcutFromString( key ), command );
}

void KeyBindings::addKeybind( const KeyBindings::Shortcut& key, const String& command ) {
	mShortcuts[sanitizeShortcut( key )] = command;
	mKeybindingsInvert[command] = sanitizeShortcut( key );
}

void KeyBindings::replaceKeybindString( const String& keys, const String& command ) {
	replaceKeybind( getShortcutFromString( keys ), command );
}

void KeyBindings::replaceKeybind( const KeyBindings::Shortcut& keys, const String& command ) {
	bool erased;
	do {
		erased = false;
		auto it = mShortcuts.find( sanitizeShortcut( keys ) );
		if ( it != mShortcuts.end() ) {
			mShortcuts.erase( it );
			mKeybindingsInvert.erase( it->second );
			erased = true;
		}
	} while ( erased );
	mShortcuts[sanitizeShortcut( keys )] = command;
	mKeybindingsInvert[command] = sanitizeShortcut( keys );
}

KeyBindings::Shortcut KeyBindings::toShortcut( const Window::Input* input,
											   const String& keys ) {
	Shortcut shortcut;
	Uint32 mod = 0;
	auto keysSplit = String::split( keys, '+' );
	if ( keysSplit.size() == 1 && KeyMod::getKeyMod( keysSplit[0] ) && keys.find( "++" ) )
		keysSplit.emplace_back( "+" );
	if ( keysSplit.size() == 2 && KeyMod::getKeyMod( keysSplit[0] ) &&
		 keys.find( " +" ) != String::npos )
		keysSplit[1] += "+";
	for ( auto& part : keysSplit ) {
		if ( ( mod = KeyMod::getKeyMod( part ) ) ) {
			shortcut.mod |= mod;
		} else {
			shortcut.key = input->getKeyFromName( part );
		}
	}
	return shortcut;
}

KeyBindings::Shortcut KeyBindings::getShortcutFromString( const String& keys ) {
	return toShortcut( mInput, keys );
}

void KeyBindings::removeKeybind( const KeyBindings::Shortcut& keys ) {
	auto it = mShortcuts.find( keys.toUint64() );
	if ( it != mShortcuts.end() ) {
		mShortcuts.erase( it );
	}
}

bool KeyBindings::existsKeybind( const KeyBindings::Shortcut& keys ) {
	return mShortcuts.find( keys.toUint64() ) != mShortcuts.end();
}

void KeyBindings::removeCommandKeybind( const String& command ) {
	auto kbIt = mKeybindingsInvert.find( command );
	if ( kbIt != mKeybindingsInvert.end() ) {
		removeKeybind( kbIt->second );
		mKeybindingsInvert.erase( command );
	}
}

void KeyBindings::removeCommandsKeybind( const Array<String>& commands ) {
	for ( auto& cmd : commands )
		removeCommandKeybind( cmd );
}

String KeyBindings::getCommandFromKeyBind( const KeyBindings::Shortcut& keys ) {
	auto it = mShortcuts.find( sanitizeShortcut( keys ) );
	if ( it != mShortcuts.end() ) {
		return it->second;
	}
	return "";
}

String KeyBindings::keybindFormat( String str ) {
	if ( !str.empty() ) {
		String::replace( str, "mod", KeyMod::getDefaultModifierString() );
		str[0] = std::toupper( str[0] );
		size_t found = str.find_first_of( '+' );
		while ( found != String::npos ) {
			if ( found + 1 < str.size() ) {
				str[found + 1] = std::toupper( str[found + 1] );
			}
			found = str.find_first_of( '+', found + 1 );
		}
		return str;
	}
	return "";
}

String KeyBindings::getCommandKeybindString( const String& command ) const {
	auto it = mKeybindingsInvert.find( command );
	if ( it == mKeybindingsInvert.end() )
		return "";
	return keybindFormat( getShortcutString( Shortcut( it->second ) ) );
}

void KeyBindings::reset() {
	mShortcuts.clear();
	mKeybindingsInvert.clear();
}

const ShortcutMap& KeyBindings::getShortcutMap() const {
	return mShortcuts;
}

const std::map<String, Uint64> KeyBindings::getKeybindings() const {
	return mKeybindingsInvert;
}

String KeyBindings::fromShortcut( const Window::Input* input, KeyBindings::Shortcut shortcut,
									   bool format ) {
	Array<String> mods;
	String keyname( String::toLower( input->getKeyName( shortcut.key ) ) );
	const auto& MOD_MAP = KeyMod::getModMap();
	if ( shortcut.mod & MOD_MAP.at( "mod" ) )
		mods.emplace_back( "mod" );
	if ( ( shortcut.mod & KEYMOD_CTRL ) && KEYMOD_CTRL != MOD_MAP.at( "mod" ) )
		mods.emplace_back( "ctrl" );
	if ( ( shortcut.mod & KEYMOD_SHIFT ) && KEYMOD_SHIFT != MOD_MAP.at( "mod" ) )
		mods.emplace_back( "shift" );
	if ( ( shortcut.mod & KEYMOD_LALT ) && KEYMOD_LALT != MOD_MAP.at( "mod" ) )
		mods.emplace_back( "alt" );
	if ( ( shortcut.mod & KEYMOD_RALT ) && KEYMOD_RALT != MOD_MAP.at( "mod" ) )
		mods.emplace_back( "altgr" );
	if ( ( shortcut.mod & KEYMOD_META ) && KEYMOD_META != MOD_MAP.at( "mod" ) )
		mods.emplace_back( "meta" );
	if ( mods.empty() )
		return format ? keybindFormat( keyname ) : keyname;
	auto ret = String::join( mods, '+' ) + "+" + keyname;
	return format ? keybindFormat( ret ) : ret;
}

String KeyBindings::getShortcutString( KeyBindings::Shortcut shortcut, bool format ) const {
	return fromShortcut( mInput, shortcut, format );
}

}} // namespace Zero::UI
