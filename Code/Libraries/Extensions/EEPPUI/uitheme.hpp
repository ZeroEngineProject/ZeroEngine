#ifndef EE_UICUITHEME_HPP
#define EE_UICUITHEME_HPP

#include <eepp/system/resourcemanager.hpp>
#include <eepp/ui/base.hpp>
#include <eepp/ui/css/stylesheet.hpp>
#include <eepp/ui/uifontstyleconfig.hpp>
#include <eepp/ui/uihelper.hpp>
#include <eepp/ui/uiskin.hpp>

namespace EE { namespace Graphics {
class Sprite;
class TextureAtlas;
class Font;
class Drawable;
}} // namespace EE::Graphics

namespace EE { namespace UI {

class UIIcon;
class UIIconTheme;

class ZeroShared UITheme : protected ResourceManagerMulti<UISkin> {
  public:
	using ResourceManagerMulti<UISkin>::getById;
	using ResourceManagerMulti<UISkin>::getByName;
	using ResourceManagerMulti<UISkin>::exists;
	using ResourceManagerMulti<UISkin>::existsId;

	static UITheme* New( const String& name, const String& abbr,
						 Graphics::Font* defaultFont = NULL );

	static UITheme* load( const String& name, const String& abbr,
						  const String& textureAtlasPath, Graphics::Font* defaultFont,
						  const String& styleSheetPath );

	static UITheme* loadFromTextureAtlas( UITheme* tTheme,
										  Graphics::TextureAtlas* getTextureAtlas );

	static UITheme* loadFromTextureAtlas( Graphics::TextureAtlas* getTextureAtlas,
										  const String& Name, const String& NameAbbr );

	static UITheme* loadFromDirectroy( UITheme* tTheme, const String& Path,
									   const Float& pixelDensity = 1 );

	static UITheme* loadFromDirectroy( const String& Path, const String& Name,
									   const String& NameAbbr, const Float& pixelDensity = 1 );

	virtual ~UITheme();

	const String& getName() const;

	void setName( const String& name );

	const String::HashType& getId() const;

	const String& getAbbr() const;

	virtual UISkin* add( UISkin* Resource );

	Graphics::TextureAtlas* getTextureAtlas() const;

	UIIcon* getIconByName( const String& name );

	UISkin* getSkin( const String& widgetName );

	Font* getDefaultFont() const;

	void setDefaultFont( Font* font );

	CSS::StyleSheet& getStyleSheet();

	const CSS::StyleSheet& getStyleSheet() const;

	void setStyleSheet( const CSS::StyleSheet& styleSheet );

	const Float& getDefaultFontSize() const;

	void setDefaultFontSize( const Float& defaultFontSize );

	UIIconTheme* getIconTheme() const;

	const String& getStyleSheetPath() const;

	void setStyleSheetPath( const String& styleSheetPath );

	bool reloadStyleSheet();

  protected:
	String mName;
	String::HashType mNameHash;
	String mAbbr;
	Graphics::TextureAtlas* mTextureAtlas;
	Font* mDefaultFont;
	Float mDefaultFontSize;
	CSS::StyleSheet mStyleSheet;
	String mStyleSheetPath;
	UIIconTheme* mIconTheme;

	void setTextureAtlas( Graphics::TextureAtlas* SG );

	UITheme( const String& name, const String& abbr, Graphics::Font* defaultFont = NULL );
};

}} // namespace EE::UI

#endif
