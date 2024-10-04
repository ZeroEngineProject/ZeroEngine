#ifndef EE_UI_UIICONTHEME_HPP
#define EE_UI_UIICONTHEME_HPP

#include <eepp/graphics/drawable.hpp>
#include <eepp/ui/uiicon.hpp>
#include <unordered_map>

using namespace EE::Graphics;

namespace EE { namespace UI {

class ZeroShared UIIconTheme {
  public:
	static UIIconTheme* New( const String& name );

	~UIIconTheme();

	UIIconTheme* add( UIIcon* icon );

	UIIconTheme* add( const std::unordered_map<String, UIIcon*>& icons );

	const String& getName() const;

	UIIcon* getIcon( const String& name ) const;

  protected:
	String mName;
	std::unordered_map<String, UIIcon*> mIcons;

	UIIconTheme( const String& name );
};

}} // namespace EE::UI

#endif // EE_UI_UIICONTHEME_HPP
