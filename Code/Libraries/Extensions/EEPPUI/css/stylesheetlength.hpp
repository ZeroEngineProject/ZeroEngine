#ifndef EE_UI_CSS_STYLESHEETLENGTH_HPP
#define EE_UI_CSS_STYLESHEETLENGTH_HPP

#include <eepp/config.hpp>
#include <eepp/math/size.hpp>
#include <eepp/scene/scenenode.hpp>
#include <string>

using namespace EE::Math;

namespace EE { namespace UI { namespace CSS {

class ZeroShared StyleSheetLength {
  public:
	enum Unit {
		Percentage,
		In,
		Cm,
		Mm,
		Em,
		Ex,
		Pt,
		Pc,
		Px,
		Dpi,
		Dp,
		Dpcm,
		Vw,
		Vh,
		Vmin,
		Vmax,
		Rem,
		Dprd,
		Dpru,
		Dpr,
	};

	static Unit unitFromString( String unitStr );

	static String unitToString( const Unit& unit );

	static bool isLength( const String& unitStr );

	static StyleSheetLength fromString( const String& str, const Float& defaultValue = 0 );

	StyleSheetLength();

	StyleSheetLength( const Float& val, const Unit& unit );

	StyleSheetLength( const String& val, const Float& defaultValue = 0 );

	StyleSheetLength( const StyleSheetLength& val );

	void setValue( const Float& val, const Unit& units );

	const Float& getValue() const;

	const Unit& getUnit() const;

	Float asPixels( const Float& parentSize, const Sizef& viewSize, const Float& displayDpi,
					const Float& elFontSize = 12, const Float& globalFontSize = 12 ) const;

	Float asDp( const Float& parentSize, const Sizef& viewSize, const Float& displayDpi,
				const Float& elFontSize = 12, const Float& globalFontSize = 12 ) const;

	bool operator==( const StyleSheetLength& val ) const;

	bool operator!=( const StyleSheetLength& val ) const;

	StyleSheetLength& operator=( const StyleSheetLength& val );

	StyleSheetLength& operator=( const Float& val );

	String toString() const;

  protected:
	Unit mUnit;
	Float mValue;
};

}}} // namespace EE::UI::CSS

#endif
