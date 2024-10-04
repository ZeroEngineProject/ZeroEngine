#ifndef EE_UIUIWIDGET_HPP
#define EE_UIUIWIDGET_HPP

#include <eepp/ui/css/propertydefinition.hpp>
#include <eepp/ui/css/stylesheetproperty.hpp>
#include <eepp/ui/css/stylesheetselector.hpp>
#include <eepp/ui/uinode.hpp>

namespace pugi {
class xml_node;
}

namespace EE { namespace UI { namespace CSS {
class PropertyDefinition;
}}} // namespace EE::UI::CSS

using namespace EE::UI::CSS;

namespace EE { namespace UI {

class UITooltip;
class UIStyle;

class ZeroShared UIWidget : public UINode {
  public:
	static UIWidget* New();

	static UIWidget* NewWithTag( const String& tag );

	UIWidget();

	virtual ~UIWidget();

	virtual Uint32 getType() const;

	virtual bool isType( const Uint32& type ) const;

	virtual UINode* setFlags( const Uint32& flags );

	virtual UINode* unsetFlags( const Uint32& flags );

	virtual UIWidget* setAnchors( const Uint32& flags );

	virtual void setTheme( UITheme* Theme );

	virtual UINode* setThemeSkin( const String& skinName );

	virtual UINode* setThemeSkin( UITheme* Theme, const String& skinName );

	virtual Node* setId( const String& id );

	virtual bool acceptsDropOfWidget( const UIWidget* widget );

	UIWidget* acceptsDropOfWidgetInTree( const UIWidget* widget );

	UITooltip* getTooltip();

	void tooltipRemove();

	UIWidget* setTooltipText( const String& text );

	String getTooltipText();

	void updateAnchorsDistances();

	const Rectf& getLayoutMargin() const;

	const Rectf& getLayoutPixelsMargin() const;

	UIWidget* setLayoutMargin( const Rectf& margin );

	UIWidget* setLayoutMarginLeft( const Float& marginLeft );

	UIWidget* setLayoutMarginRight( const Float& marginRight );

	UIWidget* setLayoutMarginTop( const Float& marginTop );

	UIWidget* setLayoutMarginBottom( const Float& marginBottom );

	UIWidget* setLayoutPixelsMargin( const Rectf& margin );

	UIWidget* setLayoutPixelsMarginLeft( const Float& marginLeft );

	UIWidget* setLayoutPixelsMarginRight( const Float& marginRight );

	UIWidget* setLayoutPixelsMarginTop( const Float& marginTop );

	UIWidget* setLayoutPixelsMarginBottom( const Float& marginBottom );

	Float getLayoutWeight() const;

	UIWidget* setLayoutWeight( const Float& weight );

	Uint32 getLayoutGravity() const;

	UIWidget* setLayoutGravity( const Uint32& layoutGravity );

	const SizePolicy& getLayoutWidthPolicy() const;

	UIWidget* setLayoutWidthPolicy( const SizePolicy& widthPolicy );

	const SizePolicy& getLayoutHeightPolicy() const;

	UIWidget* setLayoutHeightPolicy( const SizePolicy& heightPolicy );

	UIWidget* setLayoutSizePolicy( const SizePolicy& widthPolicy, const SizePolicy& heightPolicy );

	UIWidget* setLayoutPositionPolicy( const PositionPolicy& layoutPositionPolicy, UIWidget* of );

	UIWidget* getLayoutPositionPolicyWidget() const;

	PositionPolicy getLayoutPositionPolicy() const;

	virtual void loadFromXmlNode( const pugi::xml_node& node );

	void notifyLayoutAttrChange();

	void notifyLayoutAttrChangeParent();

	void setStyleSheetInlineProperty( const String& name, const String& value,
									  const Uint32& specificity = UINT32_MAX -
																  1 /*SpecificityInline*/ );

	virtual bool applyProperty( const StyleSheetProperty& attribute );

	const Rectf& getPadding() const;

	const Rectf& getPixelsPadding() const;

	UIWidget* setPadding( const Rectf& padding );

	UIWidget* setPaddingLeft( const Float& paddingLeft );

	UIWidget* setPaddingRight( const Float& paddingRight );

	UIWidget* setPaddingTop( const Float& paddingTop );

	UIWidget* setPaddingBottom( const Float& paddingBottom );

	UIWidget* setPaddingPixels( const Rectf& padding );

	UIWidget* setPaddingPixelsLeft( const Float& paddingLeft );

	UIWidget* setPaddingPixelsRight( const Float& paddingRight );

	UIWidget* setPaddingPixelsTop( const Float& paddingTop );

	UIWidget* setPaddingPixelsBottom( const Float& paddingBottom );

	const String& getStyleSheetTag() const;

	const String& getStyleSheetId() const;

	const std::vector<String>& getStyleSheetClasses() const;

	UIWidget* getStyleSheetParentElement() const;

	UIWidget* getStyleSheetPreviousSiblingElement() const;

	UIWidget* getStyleSheetNextSiblingElement() const;

	const std::vector<String>& getStyleSheetPseudoClasses() const;

	UIWidget* resetClass();

	/** Resets all classes and assign a class */
	UIWidget* setClass( const String& cls );

	/** Resets all classes and assign vector of classes */
	UIWidget* setClasses( const std::vector<String>& classes );

	UIWidget* addClass( const String& cls );

	UIWidget* addClasses( const std::vector<String>& classes );

	UIWidget* removeClass( const String& cls );

	UIWidget* removeClasses( const std::vector<String>& classes );

	bool hasClass( const String& cls ) const;

	void toggleClass( const String& cls );

	void setElementTag( const String& tag );

	const std::vector<String> getClasses() const;

	const String& getElementTag() const;

	virtual void pushState( const Uint32& State, bool emitEvent = true );

	virtual void popState( const Uint32& State, bool emitEvent = true );

	UIStyle* getUIStyle() const;

	void reloadStyle( const bool& reloadChilds = true, const bool& disableAnimations = false,
					  const bool& reportStateChange = true,
					  const bool& forceReApplyProperties = false );

	void beginAttributesTransaction();

	void endAttributesTransaction();

	const Uint32& getStyleState() const;

	const Uint32& getStylePreviousState() const;

	std::vector<UIWidget*> findAllByClass( const String& className );

	std::vector<UIWidget*> findAllByTag( const String& tag );

	UIWidget* findByClass( const String& className );

	template <typename T> T* findByClass( const String& className ) {
		return reinterpret_cast<T*>( findByClass( className ) );
	}

	UIWidget* findByTag( const String& tag );

	template <typename T> T* findByTag( const String& tag ) {
		return reinterpret_cast<T*>( findByTag( tag ) );
	}

	UIWidget* querySelector( const CSS::StyleSheetSelector& selector );

	UIWidget* querySelector( const String& selector );

	template <typename T> T* querySelector( const String& selector ) {
		return reinterpret_cast<T*>( querySelector( selector ) );
	}

	std::vector<UIWidget*> querySelectorAll( const CSS::StyleSheetSelector& selector );

	std::vector<UIWidget*> querySelectorAll( const String& selector );

	String getPropertyString( const String& property ) const;

	virtual String getPropertyString( const PropertyDefinition* propertyDef,
										   const Uint32& propertyIndex = 0 ) const;

	virtual std::vector<PropertyId> getPropertiesImplemented() const;

	bool isSceneNodeLoading() const;

	void reportStyleStateChangeRecursive( bool disableAnimations = false,
										  bool forceReApplyStyles = false );

	UITooltip* createTooltip();

	bool isTabStop() const;

	void setTabStop();

	void unsetTabStop();

	bool isTabFocusable() const;

	void setTabFocusable();

	void unsetTabFocusable();

	UIWidget* getPrevTabWidget() const;

	UIWidget* getNextTabWidget() const;

	bool hasPseudoClass( const String& pseudoCls ) const;

	bool isTooltipEnabled() const;

	void setTooltipEnabled( bool enabled );

	UIWidget* getPrevWidget() const;

	UIWidget* getNextWidget() const;

	String getTranslatorString( const String& str );

	String getTranslatorString( const String& str, const String& defaultValue );

	String i18n( const String& str );

	String i18n( const String& str, const String& defaultValue );

  protected:
	friend class UIManager;
	friend class UISceneNode;
	friend class UIEventDispatcher;

	String mTag;
	UITheme* mTheme;
	UIStyle* mStyle;
	UITooltip* mTooltip;
	Rect mDistToBorder;
	Rectf mLayoutMargin;
	Rectf mLayoutMarginPx;
	Rectf mPadding;
	Rectf mPaddingPx;
	Float mLayoutWeight;
	Uint32 mLayoutGravity;
	SizePolicy mWidthPolicy;
	SizePolicy mHeightPolicy;
	PositionPolicy mLayoutPositionPolicy;
	UIWidget* mLayoutPositionPolicyWidget;
	int mAttributesTransactionCount;
	String mSkinName;
	std::vector<String> mClasses;
	std::vector<String> mPseudoClasses;
	String mTooltipText;

	explicit UIWidget( const String& tag );

	void updatePseudoClasses();

	virtual void onChildCountChange( Node* child, const bool& removed );

	virtual Uint32 onKeyDown( const KeyEvent& event );

	virtual Uint32 onMouseMove( const Vector2i& Pos, const Uint32& Flags );

	virtual Uint32 onMouseOver( const Vector2i& Pos, const Uint32& Flags );

	virtual Uint32 onMouseLeave( const Vector2i& Pos, const Uint32& Flags );

	virtual void onParentSizeChange( const Vector2f& sizeChange );

	virtual void onPositionChange();

	virtual void onVisibilityChange();

	virtual void onSizeChange();

	virtual void onAutoSize();

	virtual void onWidgetCreated();

	virtual void onPaddingChange();

	virtual void onMarginChange();

	virtual void onThemeLoaded();

	virtual void onParentChange();

	virtual void onClassChange();

	virtual void onTagChange();

	virtual void onFocusPrevWidget();

	virtual void onFocusNextWidget();

	virtual Uint32 onFocus( NodeFocusReason reason );

	virtual Uint32 onFocusLoss();

	void updateAnchors( const Vector2f& sizeChange );

	void alignAgainstLayout();

	void reportStyleStateChange( bool disableAnimations = false, bool forceReApplyStyles = false );

	String getLayoutWidthPolicyString() const;

	String getLayoutHeightPolicyString() const;

	String getLayoutGravityString() const;

	String getGravityString() const;

	String getFlagsString() const;

	bool checkPropertyDefinition( const StyleSheetProperty& property );

	Vector2f getTooltipPosition();

	void createStyle();

	void enableCSSAnimations();

	void disableCSSAnimations();

	void reloadFontFamily();
};

}} // namespace EE::UI

#endif
