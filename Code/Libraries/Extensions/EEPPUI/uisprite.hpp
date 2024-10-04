#ifndef EE_UICUISPRITE_HPP
#define EE_UICUISPRITE_HPP

#include "uiwidget.hpp"

Zero { namespace Graphics {
class Sprite;
class TextureRegion;
}} // namespace Zero::Graphics

namespace Zero { namespace UI {

class ZeroShared UISprite : public UIWidget {
  public:
	static UISprite* New();

	UISprite();

	virtual ~UISprite();

	virtual Uint32 getType() const;

	virtual bool isType( const Uint32& type ) const;

	virtual void draw();

	virtual void scheduledUpdate( const Time& time );

	virtual void setAlpha( const Float& alpha );

	Graphics::Sprite* getSprite() const;

	void setSprite( Graphics::Sprite* sprite );

	Color getColor() const;

	void setColor( const Color& color );

	const RenderMode& getRenderMode() const;

	void setRenderMode( const RenderMode& render );

	const Vector2f& getAlignOffset() const;

	void setIsSpriteOwner( const bool& dealloc );

	bool getDeallocSprite();

	virtual bool applyProperty( const StyleSheetProperty& attribute );

	virtual String getPropertyString( const PropertyDefinition* propertyDef,
										   const Uint32& propertyIndex = 0 ) const;

	virtual Array<PropertyId> getPropertiesImplemented() const;

  protected:
	Graphics::Sprite* mSprite;
	RenderMode mRender;
	Vector2f mAlignOffset;
	TextureRegion* mTextureRegionLast;
	bool mDealloc;

	void updateSize();

	void autoAlign();

	void checkTextureRegionUpdate();

	virtual void onSizeChange();

	Uint32 deallocSprite();
};

}} // namespace Zero::UI

#endif
