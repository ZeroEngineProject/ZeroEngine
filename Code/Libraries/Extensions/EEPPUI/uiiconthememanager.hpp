#ifndef EE_UI_UIICONTHEMEMANAGER_HPP
#define EE_UI_UIICONTHEMEMANAGER_HPP

#include <eepp/ui/uiicontheme.hpp>
#include <vector>

namespace EE { namespace UI {

class UIThemeManager;

class ZeroShared UIIconThemeManager {
  public:
	static String getIconNameFromFileName( const String& fileName,
												bool retOnlyWithExtension = false );

	static UIIconThemeManager* New();

	~UIIconThemeManager();

	UIIconThemeManager* add( UIIconTheme* iconTheme );

	UIIconTheme* getCurrentTheme() const;

	UIIconThemeManager* setCurrentTheme( UIIconTheme* currentTheme );

	UIIconTheme* getFallbackTheme() const;

	UIIconThemeManager* setFallbackTheme( UIIconTheme* fallbackTheme );

	UIIcon* findIcon( const String& name );

	UIThemeManager* getFallbackThemeManager() const;

	UIIconThemeManager* setFallbackThemeManager( UIThemeManager* fallbackThemeManager );

	void remove( UIIconTheme* iconTheme );

  protected:
	std::vector<UIIconTheme*> mIconThemes;
	UIIconTheme* mCurrentTheme{ nullptr };
	UIIconTheme* mFallbackTheme{ nullptr };
	UIThemeManager* mFallbackThemeManager{ nullptr };

	UIIconThemeManager();

	bool isPresent( UIIconTheme* iconTheme );
};

}} // namespace EE::UI

#endif // EE_UI_UIICONTHEMEMANAGER_HPP
