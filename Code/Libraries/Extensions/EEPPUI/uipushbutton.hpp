#ifndef EE_UICUIPUSHBUTTON_HPP
#define EE_UICUIPUSHBUTTON_HPP

#include <eepp/ui/uiimage.hpp>
#include <eepp/ui/uitextview.hpp>
#include "uiwidget.hpp"

namespace Zero { namespace UI {

enum class InnerWidgetOrientation {
	WidgetIconTextBox,
	WidgetTextBoxIcon,
	IconTextBoxWidget,
	IconWidgetTextBox,
	TextBoxIconWidget,
	TextBoxWidgetIcon,
};

class ZeroShared UIPushButton : public UIWidget {
  public:
	static InnerWidgetOrientation innerWidgetOrientationFromString( String iwo );

	static String innerWidgetOrientationToString( const InnerWidgetOrientation& orientation );

	static UIPushButton* New();

	static UIPushButton* NewWithTag( const String& tag );

	static UIPushButton*
	NewWithOpt( const String& tag,
				const std::function<UITextView*( UIPushButton* )>& newTextViewCb );

	virtual ~UIPushButton();

	virtual Uint32 getType() const;

	virtual bool isType( const Uint32& type ) const;

	virtual void setTheme( UITheme* Theme );

	virtual UIPushButton* setIcon( Drawable* icon, bool ownIt = false );

	virtual UIImage* getIcon();

	bool hasIcon() const;

	virtual UIPushButton* setText( const String& text );

	virtual const String& getText() const;

	UITextView* getTextBox() const;

	void setIconMinimumSize( const Sizei& minIconSize );

	const Sizei& getIconMinimumSize() const;

	virtual bool applyProperty( const StyleSheetProperty& attribute );

	virtual String getPropertyString( const PropertyDefinition* propertyDef,
										   const Uint32& propertyIndex = 0 ) const;

	virtual Array<PropertyId> getPropertiesImplemented() const;

	UIWidget* setTextAlign( const Uint32& align );

	virtual Sizef getContentSize() const;

	const InnerWidgetOrientation& getInnerWidgetOrientation() const;

	void setInnerWidgetOrientation( const InnerWidgetOrientation& innerWidgetOrientation );

	virtual UIWidget* getExtraInnerWidget() const;

	UIWidget* getFirstInnerItem() const;

	virtual Sizef updateLayout();

	bool isTextAsFallback() const;

	void setTextAsFallback( bool textAsFallback );

  protected:
	UIImage* mIcon;
	UITextView* mTextBox;
	Sizei mIconMinSize;
	InnerWidgetOrientation mInnerWidgetOrientation{ InnerWidgetOrientation::IconTextBoxWidget };
	bool mTextAsFallback{ false };

	UIPushButton();

	explicit UIPushButton( const String& tag );

	explicit UIPushButton( const String& tag,
						   const std::function<UITextView*( UIPushButton* )>& cb );

	virtual Rectf calculatePadding() const;

	virtual void onSizeChange();

	virtual void onAlphaChange();

	virtual void onStateChange();

	virtual void onAlignChange();

	virtual void onThemeLoaded();

	virtual void onAutoSize();

	virtual void onPaddingChange();

	virtual Uint32 onKeyDown( const KeyEvent& Event );

	virtual Uint32 onKeyUp( const KeyEvent& Event );

	void updateTextBox();

	Vector2f packLayout( const Array<UIWidget*>& widgets, const Rectf& padding );

	Vector2f calcLayoutSize( const Array<UIWidget*>& widgets, const Rectf& padding ) const;
};

}} // namespace Zero::UI

#endif
