#ifndef EE_UI_KEYBOARDSHORTCUT_HPP
#define EE_UI_KEYBOARDSHORTCUT_HPP

#include <eepp/config.hpp>
#include <eepp/window/keycodes.hpp>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

using namespace EE::Window;
namespace EE { namespace Window {
class Input;
}} // namespace EE::Window

namespace EE { namespace UI {

class UIWidget;

typedef std::map<Uint64, String> ShortcutMap;

class ZeroShared KeyBindings {
  public:
	struct Shortcut {
		Shortcut() {}
		Shortcut( Keycode key, Uint32 mod ) : key( key ), mod( mod ) {}
		Shortcut( const Uint64& code ) :
			key( (Keycode)( code & 0xFFFFFFFF ) ), mod( ( code >> 32 ) & 0xFFFFFFFF ) {}
		Keycode key{ KEY_UNKNOWN };
		Uint32 mod{ 0 };
		Uint64 toUint64() const { return (Uint64)mod << 32 | (Uint64)key; }
		operator Uint64() const { return toUint64(); }
		bool empty() const { return 0 == mod && 0 == key; }
	};

	static KeyBindings::Shortcut sanitizeShortcut( const KeyBindings::Shortcut& shortcut );

	static String keybindFormat( String str );

	static Shortcut toShortcut( const Window::Input* input, const String& keys );

	static String fromShortcut( const Window::Input* input, KeyBindings::Shortcut shortcut,
									 bool format = false );

	KeyBindings( const Window::Input* input );

	void addKeybindsString( const std::map<String, String>& binds );

	void addKeybinds( const std::map<Shortcut, String>& binds );

	void addKeybindsStringUnordered( const std::unordered_map<String, String>& binds );

	void addKeybindsUnordered( const std::unordered_map<Shortcut, String>& binds );

	void addKeybindString( const String& key, const String& command );

	void addKeybind( const Shortcut& key, const String& command );

	/** If the command is already on the list, it will remove the previous keybind. */
	void replaceKeybindString( const String& keys, const String& command );

	/** If the command is already on the list, it will remove the previous keybind. */
	void replaceKeybind( const Shortcut& keys, const String& command );

	Shortcut getShortcutFromString( const String& keys );

	void removeKeybind( const Shortcut& keys );

	bool existsKeybind( const Shortcut& keys );

	void removeCommandKeybind( const String& command );

	void removeCommandsKeybind( const std::vector<String>& command );

	String getCommandFromKeyBind( const Shortcut& keys );

	String getCommandKeybindString( const String& command ) const;

	void reset();

	const ShortcutMap& getShortcutMap() const;

	const std::map<String, Uint64> getKeybindings() const;

	String getShortcutString( Shortcut shortcut, bool format = false ) const;

  protected:
	const Window::Input* mInput;
	ShortcutMap mShortcuts;
	std::map<String, Uint64> mKeybindingsInvert;
};

}} // namespace EE::UI

template <> struct std::hash<EE::UI::KeyBindings::Shortcut> {
	std::size_t operator()( EE::UI::KeyBindings::Shortcut const& s ) const noexcept {
		return s.toUint64();
	}
};

#endif // EE_UI_KEYBOARDSHORTCUT_HPP
