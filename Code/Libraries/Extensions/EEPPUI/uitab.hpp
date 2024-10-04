#ifndef EE_UICUITAB_HPP
#define EE_UICUITAB_HPP

#include <eepp/ui/uiselectbutton.hpp>

namespace Zero { namespace UI {

class UITabWidget;

class ZeroShared UITab : public UISelectButton {
  public:
	static UITab* New();

	UITab();

	Node* getOwnedWidget() const;

	void setOwnedWidget( Node* ownedWidget );

	void setTabSelected();

	virtual ~UITab();

	virtual Uint32 getType() const;

	virtual bool isType( const Uint32& type ) const;

	virtual void setTheme( UITheme* Theme );

	virtual const String& getText() const;

	virtual UIPushButton* setText( const String& text );

	virtual bool applyProperty( const StyleSheetProperty& attribute );

	virtual String getPropertyString( const PropertyDefinition* propertyDef,
										   const Uint32& propertyIndex = 0 ) const;

	virtual Array<PropertyId> getPropertiesImplemented() const;

	UITabWidget* getTabWidget() const;

	virtual UIWidget* getExtraInnerWidget() const;

	void removeTab( bool destroyOwnedNode = true, bool immediateClose = false );

	UIWidget* getCloseButton() const;

  protected:
	friend class UITabWidget;

	Node* mOwnedWidget{ nullptr };
	String mText;
	String mOwnedName;
	mutable UIWidget* mCloseButton{ nullptr };
	Float mDragTotalDiff{ 0.f };
	UITabWidget* mTabWidget{ nullptr };
	UIWidget* mCurDropWidget{ nullptr };
	bool mWasToolipEnabled{ true };

	Uint32 onDrag( const Vector2f& position, const Uint32& flags, const Sizef& dragDiff );

	Uint32 onDragStart( const Vector2i& position, const Uint32& flags );

	Uint32 onDragStop( const Vector2i& position, const Uint32& flags );

	virtual Uint32 onMouseUp( const Vector2i& position, const Uint32& flags );

	virtual Uint32 onMessage( const NodeMessage* message );

	virtual void onStateChange();

	virtual void onAutoSize();

	virtual void onParentChange();

	virtual void onSizeChange();

	virtual bool onCreateContextMenu( const Vector2i& position, const Uint32& flags );

	void setOwnedNode();

	void updateTab();
};

}} // namespace Zero::UI

#endif
