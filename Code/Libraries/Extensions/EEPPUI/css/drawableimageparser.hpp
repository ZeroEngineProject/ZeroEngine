#ifndef EE_UI_CSS_DRAWABLEIMAGEPARSER_HPP
#define EE_UI_CSS_DRAWABLEIMAGEPARSER_HPP

#include <eepp/core.hpp>
#include <eepp/math/size.hpp>
#include <eepp/system/functionstring.hpp>
#include <functional>
#include <map>

namespace EE { namespace Graphics {
class Drawable;
}} // namespace EE::Graphics
namespace EE { namespace UI {
class UINode;
}} // namespace EE::UI

using namespace EE::Graphics;
using namespace EE::Math;
using namespace EE::System;

namespace EE { namespace UI { namespace CSS {

typedef std::function<Drawable*( const FunctionString& functionType, const Sizef& size, bool& ownIt,
								 UINode* node )>
	DrawableImageParserFunc;

class ZeroShared DrawableImageParser {
  public:
	DrawableImageParser();

	bool exists( const String& name ) const;

	Drawable* createDrawable( const String& value, const Sizef& size, bool& ownIt,
							  UINode* node );

	void addParser( const String& name, const DrawableImageParserFunc& func );

  protected:
	std::map<String, DrawableImageParserFunc> mFuncs;

	void registerBaseParsers();
};

}}} // namespace EE::UI::CSS

#endif // EE_UI_CSS_DRAWABLEIMAGEPARSER_HPP
