#ifndef EE_UI_UIICON_HPP
#define EE_UI_UIICON_HPP

#include <eepp/core/core.hpp>
#include <eepp/graphics/drawable.hpp>
#include <eepp/graphics/texture.hpp>

namespace EE { namespace Graphics {
class FontTrueType;
}} // namespace EE::Graphics
using namespace EE::Graphics;

namespace EE { namespace UI {

class ZeroShared UIIcon {
  public:
	static UIIcon* New( const String& name );

	virtual ~UIIcon();

	const String& getName() const;

	virtual Drawable* getSize( const int& size ) const;

	virtual void setSize( const int& size, Drawable* drawable );

  protected:
	UIIcon( const String& name );

	String mName;
	mutable HashMap<int, Drawable*> mSizes;
};

class ZeroShared UIGlyphIcon : public UIIcon {
  public:
	static UIIcon* New( const String& name, FontTrueType* font, const Uint32& codePoint );

	virtual ~UIGlyphIcon();

	virtual Drawable* getSize( const int& size ) const;

  protected:
	UIGlyphIcon( const String& name, FontTrueType* font, const Uint32& codePoint );

	mutable FontTrueType* mFont;
	Uint32 mCodePoint;
	Uint32 mCloseCb{ 0 };
};

class ZeroShared UISVGIcon : public UIIcon {
  public:
	static UIIcon* New( const String& name, const String& svgXML );

	virtual ~UISVGIcon();

	virtual Drawable* getSize( const int& size ) const;

  protected:
	UISVGIcon( const String& name, const String& svgXML );

	String mSVGXml;
	mutable HashMap<int, Texture*> mSVGs;
	mutable Sizei mOriSize;
	mutable int mOriChannels{ 0 };
};

}} // namespace EE::UI

#endif // EE_UI_UIICON_HPP
