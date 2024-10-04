#ifndef EE_UI_MODELS_CSSPROPERTIESMODEL_HPP
#define EE_UI_MODELS_CSSPROPERTIESMODEL_HPP

#include <eepp/ui/css/propertydefinition.hpp>
#include <eepp/ui/css/stylesheetspecification.hpp>
#include <eepp/ui/models/model.hpp>
#include "uiwidget.hpp"

using Zero::UI::CSS;

namespace Zero { namespace UI { namespace Models {

class CSSPropertiesModel final : public Model {
  public:
	static std::shared_ptr<CSSPropertiesModel> create( UIWidget* widget ) {
		return std::make_shared<CSSPropertiesModel>( widget );
	}

	static std::shared_ptr<CSSPropertiesModel> create() {
		return std::make_shared<CSSPropertiesModel>();
	}

	virtual size_t rowCount( const ModelIndex& ) const { return mData.size(); }

	virtual size_t columnCount( const ModelIndex& ) const { return 2; }

	virtual String columnName( const size_t& idx ) const {
		return idx == 1 ? "Property" : "Value";
	}

	virtual Variant data( const ModelIndex& index, ModelRole role = ModelRole::Display ) const {
		eeASSERT( index.row() < (Int64)mData.size() );
		if ( mWidget == nullptr || index.row() >= (Int64)mData.size() )
			return {};
		PropertyId propId = mData[index.row()];
		auto propIt = mProps.find( propId );
		const PropertyDefinition* def = propIt != mProps.end() ? propIt->second : nullptr;
		if ( !def )
			return {};
		if ( role == ModelRole::Display ) {
			switch ( index.column() ) {
				case 1:
					return Variant( mWidget->getPropertyString( def ) );
				case 0:
				default:
					return Variant( def->getName().c_str() );
			}
		}
		return {};
	}

	CSSPropertiesModel() {}

	explicit CSSPropertiesModel( UIWidget* widget ) { setWidget( widget ); }

	virtual ~CSSPropertiesModel() {
		if ( mWidget && mCloseCb )
			mWidget->removeEventListener( mCloseCb );
	}

	void setWidget( UIWidget* widget ) {
		if ( mWidget && mCloseCb )
			mWidget->removeEventListener( mCloseCb );
		mWidget = widget;
		mData = widget ? widget->getPropertiesImplemented() : Array<PropertyId>();
		for ( const auto& prop : mData ) {
			const auto* def = StyleSheetSpecification::instance()->getProperty( (Uint32)prop );
			if ( !def )
				continue;
			mProps[prop] = def;
		}
		if ( mWidget )
			mCloseCb = mWidget->addEventListener( Event::OnClose, [this]( const Event* ) {
				mWidget = nullptr;
				mCloseCb = 0;
			} );
	}

	UIWidget* getWidget() const { return mWidget; }

  protected:
	UIWidget* mWidget{ nullptr };
	Array<PropertyId> mData;
	std::map<PropertyId, const PropertyDefinition*> mProps;
	Uint32 mCloseCb{ 0 };
};

}}} // namespace Zero::UI::Models

#endif // EE_UI_MODELS_CSSPROPERTIESMODEL_HPP
