#ifndef EE_GRAPHICS_UILAYOUT_HPP
#define EE_GRAPHICS_UILAYOUT_HPP

#include "uiwidget.hpp"

namespace Zero { namespace UI {

class ZeroShared UILayout : public UIWidget {
  public:
	virtual Uint32 getType() const;

	virtual bool isType( const Uint32& type ) const;

	virtual const Sizef& getSize() const;

	virtual void updateLayout();

	bool isGravityOwner() const;

	void setGravityOwner( bool gravityOwner );

	bool isPacking() const { return mPacking; }

  protected:
	friend class UISceneNode;

	UnorderedSet<UILayout*> mLayouts;
	bool mDirtyLayout{ false };
	bool mPacking{ false };
	bool mGravityOwner{ false };

	explicit UILayout( const String& tag );

	virtual void onSizeChange();

	virtual void onPaddingChange();

	virtual void onParentSizeChange( const Vector2f& SizeChange );

	virtual void onChildCountChange( Node* child, const bool& removed );

	virtual void onLayoutUpdate();

	virtual void tryUpdateLayout();

	virtual void updateLayoutTree();

	void setLayoutDirty();

	Sizef getSizeFromLayoutPolicy();

	Float getMatchParentWidth() const;

	Float getMatchParentHeight() const;
};

}} // namespace Zero::UI

#endif
