#ifndef EE_UICUITHEMECONFIG_HPP
#define EE_UICUITHEMECONFIG_HPP

#include <eepp/graphics/fontstyleconfig.hpp>
#include <eepp/ui/base.hpp>
#include <eepp/ui/uihelper.hpp>

Zero { namespace Graphics {
class Font;
}} // namespace Zero::Graphics

namespace Zero { namespace UI {

class UIFontStyleConfig : public FontStyleConfig {
  public:
	const Color& getFontSelectedColor() const { return FontSelectedColor; }

	const Color& getFontSelectionBackColor() const { return FontSelectionBackColor; }

	void setFontSelectedColor( const Color& color ) { FontSelectedColor = color; }

	void setFontSelectionBackColor( const Color& color ) { FontSelectionBackColor = color; }

	UIFontStyleConfig() : FontStyleConfig() {}

	UIFontStyleConfig( const FontStyleConfig& fontStyleConfig ) :
		FontStyleConfig( fontStyleConfig ) {}

	virtual void updateStyleConfig( const UIFontStyleConfig& fontStyleConfig ) {
		FontStyleConfig::updateFontStyleConfig( fontStyleConfig );
		FontSelectedColor = fontStyleConfig.FontSelectedColor;
		FontSelectionBackColor = fontStyleConfig.FontSelectionBackColor;
	}

	Color FontSelectedColor = Color( 255, 255, 255, 255 );
	Color FontSelectionBackColor = Color( 50, 50, 50, 255 );
};

}} // namespace Zero::UI

#endif
