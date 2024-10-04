#ifndef EE_UI_UIBACKGROUNDDRAWABLE_HPP
#define EE_UI_UIBACKGROUNDDRAWABLE_HPP

#include <eepp/graphics/drawable.hpp>
#include <eepp/ui/border.hpp>

using Zero::Graphics;

namespace Zero { namespace UI {

class UINode;

class ZeroShared UIBackgroundDrawable : public Drawable {
  public:
	static UIBackgroundDrawable* New( const UINode* owner );

	UIBackgroundDrawable( const UINode* owner );

	virtual ~UIBackgroundDrawable();

	void setSize( const Sizef& size );

	virtual Sizef getSize();

	virtual Sizef getPixelsSize();

	virtual void draw();

	virtual void draw( const Vector2f& position );

	virtual void draw( const Vector2f& position, const Sizef& size );

	virtual bool isStateful();

	const BorderRadiuses& getRadiuses() const;

	bool hasRadius() const;

	void setRadiuses( const BorderRadiuses& radiuses );

	void setRadius( const Uint32& radius );

	Int32 getRadius() const;

	void invalidate();

	void setTopWidth( const String& topWidth );

	void setBottomWidth( const String& bottomWidth );

	void setTopLeftRadius( const String& radius );

	void setTopRightRadius( const String& radius );

	void setBottomLeftRadius( const String& radius );

	void setBottomRightRadius( const String& radius );

	bool isSmooth() const;

	void setSmooth( bool smooth );

  protected:
	const UINode* mOwner;
	BorderRadiuseStr mRadiusesStr;
	VertexBuffer* mVertexBuffer;
	Sizef mSize;
	BorderRadiuses mRadiuses;
	bool mNeedsUpdate;
	bool mNeedsRadiusUpdate;
	bool mColorNeedsUpdate;
	bool mSmooth{ false };

	virtual void onAlphaChange();

	virtual void onColorFilterChange();

	virtual void onPositionChange();

	void update();

	void updateRadiuses();
};

}} // namespace Zero::UI

#endif // EE_UI_UIBACKGROUNDDRAWABLE_HPP
