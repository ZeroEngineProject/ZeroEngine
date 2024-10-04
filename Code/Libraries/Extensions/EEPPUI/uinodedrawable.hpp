#ifndef EE_UI_UINODEDRAWABLE_HPP
#define EE_UI_UINODEDRAWABLE_HPP

#include <eepp/graphics/drawable.hpp>
#include <eepp/math/ease.hpp>
#include <eepp/scene/action.hpp>
#include <eepp/ui/uibackgrounddrawable.hpp>
#include <map>

using Zero::Graphics;
using Zero::Scene;

namespace Zero { namespace UI {

class UINode;

class ZeroShared UINodeDrawable : public Drawable {
  public:
	enum Repeat { RepeatXY, RepeatX, RepeatY, NoRepeat };

	static Repeat repeatFromText( const String& text );

	class ZeroShared LayerDrawable : public Drawable {
	  public:
		static LayerDrawable* New( UINodeDrawable* container );

		LayerDrawable( UINodeDrawable* container );

		virtual ~LayerDrawable();

		bool isStateful() { return false; }

		virtual void draw();

		virtual void draw( const Vector2f& position );

		virtual void draw( const Vector2f& position, const Sizef& size );

		virtual Sizef getSize();

		virtual Sizef getPixelsSize();

		virtual void setSize( const Sizef& size );

		Drawable* getDrawable() const;

		const String& getDrawableRef() const;

		void setDrawable( Drawable* drawable, const bool& ownIt );

		void setDrawable( const String& drawableRef );

		void setOffset( const Vector2f& offset );

		const Vector2f& getOffset() const;

		String getOffsetEq();

		void setPositionEq( const String& offset );

		void setSizeEq( const String& size );

		const String& getSizeEq() const;

		const Repeat& getRepeat() const;

		void setRepeat( const Repeat& repeat );

		void invalidate();

		const Sizef& getDrawableSize() const;

		void setDrawableSize( const Sizef& drawableSize );

		Sizef calcDrawableSize( const String& drawableSizeEq );

		Vector2f calcPosition( String positionXEq, String positionYEq );

		const String& getPositionX() const;

		void setPositionX( const String& positionX );

		const String& getPositionY() const;

		void setPositionY( const String& positionY );

	  protected:
		UINodeDrawable* mContainer;
		Sizef mSize;
		Sizef mDrawableSize;
		Vector2f mOffset;
		String mPositionX;
		String mPositionY;
		String mSizeEq;
		bool mNeedsUpdate{ false };
		bool mOwnsDrawable{ false };
		bool mColorWasSet{ false };
		Drawable* mDrawable;
		String mDrawableRef;
		Uint32 mResourceChangeCbId;
		Repeat mRepeat;

		virtual void onPositionChange();

		virtual void onColorFilterChange();

		void update();

		Drawable* createDrawable( const String& value, const Sizef& size, bool& ownIt );
	};

	static UINodeDrawable* New( UINode* owner );

	UINodeDrawable( UINode* owner );

	virtual ~UINodeDrawable();

	virtual Sizef getSize();

	virtual Sizef getPixelsSize();

	virtual void setSize( const Sizef& size );

	virtual void draw();

	virtual void draw( const Vector2f& position );

	virtual void draw( const Vector2f& position, const Sizef& size );

	void draw( const Vector2f& position, const Sizef& size, const Uint32& alpha );

	virtual bool isStateful() { return false; }

	void clearDrawables();

	void setBorderRadius( const Uint32& radius );

	Uint32 getBorderRadius() const;

	bool layerExists( int index );

	LayerDrawable* getLayer( int index );

	void setDrawable( int index, Drawable* drawable, bool ownIt );

	void setDrawable( int index, const String& drawable );

	void setDrawablePositionX( int index, const String& positionX );

	void setDrawablePositionY( int index, const String& positionY );

	void setDrawableRepeat( int index, const String& repeatRule );

	void setDrawableSize( int index, const String& sizeEq );

	void setDrawableColor( int index, const Color& color );

	void setBackgroundColor( const Color& color );

	Color getBackgroundColor() const;

	bool getClipEnabled() const;

	void setClipEnabled( bool clipEnabled );

	void invalidate();

	UINode* getOwner() const;

	UIBackgroundDrawable& getBackgroundDrawable();

	bool isSmooth() const;

	void setSmooth( bool smooth );

  protected:
	UINode* mOwner;
	UIBackgroundDrawable mBackgroundColor;
	std::map<int, LayerDrawable*> mGroup;
	Sizef mSize;
	bool mNeedsUpdate{ true };
	bool mClipEnabled{ false };
	bool mSmooth{ false };

	virtual void onPositionChange();

	virtual void onSizeChange();

	void update();
};

}} // namespace Zero::UI

#endif
