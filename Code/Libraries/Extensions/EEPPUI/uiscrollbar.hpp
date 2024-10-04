#ifndef EE_UICUISCROLLBAR_HPP
#define EE_UICUISCROLLBAR_HPP

#include <eepp/ui/uislider.hpp>
#include "uiwidget.hpp"

namespace Zero { namespace UI {

class ZeroShared UIScrollBar : public UIWidget {
  public:
	enum ScrollBarType { TwoButtons, NoButtons };

	static UIScrollBar* New();

	static UIScrollBar* NewHorizontal();

	static UIScrollBar* NewVertical();

	static UIScrollBar* NewWithTag( const String& tag );

	static UIScrollBar* NewHorizontalWithTag( const String& tag );

	static UIScrollBar* NewVerticalWithTag( const String& tag );

	explicit UIScrollBar( const String& tag = "scrollbar",
						  const UIOrientation& orientation = UIOrientation::Vertical );

	virtual ~UIScrollBar();

	virtual Uint32 getType() const;

	virtual bool isType( const Uint32& type ) const;

	virtual void setValue( Float val, const bool& emmitEvent = true );

	const Float& getValue() const;

	virtual void setMinValue( const Float& MinVal );

	const Float& getMinValue() const;

	virtual void setMaxValue( const Float& MaxVal );

	const Float& getMaxValue() const;

	virtual void setClickStep( const Float& step );

	const Float& getClickStep() const;

	Float getPageStep() const;

	void setPageStep( const Float& pageStep );

	virtual void setTheme( UITheme* Theme );

	bool isVertical() const;

	UISlider* getSlider() const;

	UINode* getButtonUp() const;

	UINode* getButtonDown() const;

	UIOrientation getOrientation() const;

	UINode* setOrientation( const UIOrientation& orientation );

	ScrollBarType getScrollBarType() const;

	void setScrollBarStyle( const ScrollBarType& scrollBarType );

	bool getExpandBackground() const;

	void setExpandBackground( bool expandBackground );

	virtual bool applyProperty( const StyleSheetProperty& attribute );

	virtual String getPropertyString( const PropertyDefinition* propertyDef,
										   const Uint32& propertyIndex = 0 ) const;

	virtual Array<PropertyId> getPropertiesImplemented() const;

	bool isDragging() const;

  protected:
	ScrollBarType mScrollBarStyle;
	UISlider* mSlider;
	UIWidget* mBtnUp;
	UIWidget* mBtnDown;

	virtual void onSizeChange();

	virtual void onAutoSize();

	void adjustChilds();

	void onValueChangeCb( const Event* Event );

	virtual void onAlphaChange();

	virtual Uint32 onMessage( const NodeMessage* Msg );

	virtual void onPaddingChange();
};

}} // namespace Zero::UI

#endif
