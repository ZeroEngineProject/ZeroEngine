#ifndef EE_UI_CSS_DRAWABLEIMAGEPARSER_HPP
#define EE_UI_CSS_DRAWABLEIMAGEPARSER_HPP

#include <eepp/core.hpp>
#include <eepp/math/size.hpp>
#include <eepp/system/functionstring.hpp>
#include <functional>
#include <map>

Zero { namespace Graphics {
class Drawable;
}} // namespace Zero::Graphics
namespace Zero { namespace UI {
class UINode;
}} // namespace Zero::UI

using Zero::Graphics;
using Zero::Math;
using Zero::System;

namespace Zero { namespace UI { namespace CSS {

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

}}} // namespace Zero::UI::CSS

#endif // EE_UI_CSS_DRAWABLEIMAGEPARSER_HPP
