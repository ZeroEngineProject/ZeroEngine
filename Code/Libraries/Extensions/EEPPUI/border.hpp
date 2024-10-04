#ifndef EE_BORDER_HPP
#define EE_BORDER_HPP

#include <eepp/config.hpp>
#include <eepp/math/size.hpp>
#include <eepp/system/color.hpp>

Zero { namespace Graphics {
class VertexBuffer;
}} // namespace Zero::Graphics
using Zero::Graphics;
using Zero::System;
using Zero::Math;

namespace Zero { namespace UI {
class UINode;

enum class BorderType : Uint32 { Inside, Outside, Outline };

struct ZeroShared Border {
	int width = 0;
	Color color;
	Color realColor;
};

struct ZeroShared BorderRadiuses {
	Sizef topLeft;
	Sizef topRight;
	Sizef bottomLeft;
	Sizef bottomRight;
};

struct ZeroShared Borders {
	Border left;
	Border top;
	Border right;
	Border bottom;
	BorderRadiuses radius;

	static String fromBorderType( const BorderType& borderType );

	static BorderType toBorderType( const String& borderType );

	static Sizef radiusFromString( const UINode* node, const String& val );

	/** Creates the border geometry into the VertexBuffer provided. The VertexBuffer must be a
	 * a EE::Graphics::PrimitiveType::PRIMITIVE_TRIANGLE_STRIP with VERTEX_FLAGS_PRIMITIVE flags. */
	static void createBorders( VertexBuffer* vbo, const Borders& borders, const Vector2f& pos,
							   const Sizef& size );

	/** Creates a rounded rectangle to use as background of a UI node. The VertexBuffer must be a
	 * a EE::Graphics::PrimitiveType::PRIMITIVE_TRIANGLE_FAN with VERTEX_FLAGS_PRIMITIVE flags. */
	static void createBackground( VertexBuffer* vbo, const BorderRadiuses& radius,
								  const Vector2f& pos, const Sizef& size, const Color& color );
};

struct BorderRadiuseStr {
	String topLeft;
	String topRight;
	String bottomLeft;
	String bottomRight;
};

struct BorderStr {
	struct {
		String left;
		String right;
		String top;
		String bottom;
	} width;
	BorderRadiuseStr radius;
};

}} // namespace Zero::UI

#endif // EE_BORDER_HPP
