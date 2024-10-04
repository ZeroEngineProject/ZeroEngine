#ifndef EE_UI_UIDATABIND_HPP
#define EE_UI_UIDATABIND_HPP

#include <eepp/system/log.hpp>
#include "uiwidget.hpp"
#include <memory>
#include <set>
#include <variant>

namespace Zero { namespace UI {

template <typename T> class UIDataBind {
  public:
	struct Converter {
		Converter() {}

		Converter( std::function<bool( const UIDataBind<T>*, T&, const String& )> toVal,
				   std::function<bool( const UIDataBind<T>*, String&, const T& )> fromVal ) :
			toVal( toVal ), fromVal( fromVal ) {}

		std::function<bool( const UIDataBind<T>*, T&, const String& )> toVal;
		std::function<bool( const UIDataBind<T>*, String&, const T& )> fromVal;
	};

	static Converter converterDefault() {
		return Converter( []( const UIDataBind<T>*, T& val,
							  const String& str ) { return String::fromString( val, str ); },
						  []( const UIDataBind<T>*, String& str, const T& val ) {
							  str = String::toString( val );
							  return true;
						  } );
	}

	static Converter converterString() {
		return Converter(
			[]( const UIDataBind<T>*, T& val, const String& str ) {
				val = str;
				return true;
			},
			[]( const UIDataBind<T>*, String& str, const T& val ) {
				str = val;
				return true;
			} );
	}

	static Converter converterBool() {
		return Converter(
			[]( const UIDataBind<T>* databind, T& val, const String& str ) -> bool {
				val = StyleSheetProperty( databind->getPropertyDefinition(), str ).asBool();
				return true;
			},
			[]( const UIDataBind<T>*, String& str, const T& val ) -> bool {
				str = val ? "true" : "false";
				return true;
			} );
	}

	static std::unique_ptr<UIDataBind<T>>
	New( T* t, const std::set<UIWidget*>& widgets,
		 const Converter& converter = UIDataBind<T>::converterDefault(),
		 const String& valueKey = "value",
		 const Event::EventType& eventType = Event::OnValueChange ) {
		return std::unique_ptr<UIDataBind<T>>(
			new UIDataBind<T>( t, widgets, converter, valueKey, eventType ) );
	}

	static std::unique_ptr<UIDataBind<T>>
	New( T* t, UIWidget* widget, const Converter& converter = UIDataBind<T>::converterDefault(),
		 const String& valueKey = "value",
		 const Event::EventType& eventType = Event::OnValueChange ) {
		return std::unique_ptr<UIDataBind<T>>(
			new UIDataBind<T>( t, widget, converter, valueKey, eventType ) );
	}

	UIDataBind() {}

	UIDataBind( T* t, const std::set<UIWidget*>& widgets,
				const Converter& converter = UIDataBind<T>::converterDefault(),
				const String& valueKey = "value",
				const Event::EventType& eventType = Event::OnValueChange ) {
		init( t, widgets, converter, valueKey, eventType );
	}

	UIDataBind( T* t, UIWidget* widget,
				const Converter& converter = UIDataBind<T>::converterDefault(),
				const String& valueKey = "value",
				const Event::EventType& eventType = Event::OnValueChange ) {
		init( t, { widget }, converter, valueKey, eventType );
	}

	void init( T* t, const std::set<UIWidget*>& widgets,
			   const Converter& converter = UIDataBind<T>::converterDefault(),
			   const String& valueKey = "value",
			   const Event::EventType& eventType = Event::OnValueChange ) {
		data = t;
		this->widgets = widgets;
		this->property = StyleSheetSpecification::instance()->getProperty( valueKey );
		this->converter = converter;
		this->eventType = eventType;
		for ( auto widget : widgets )
			bindListeners( widget );
		set( *data );
	}

	void set( const T& t ) {
		inSetValue = true;
		*data = t;
		setValueChange();
		inSetValue = false;
		if ( onValueChangeCb )
			onValueChangeCb( t );
	}

	const T& get() const { return *data; }

	void reset() {
		for ( auto widget : widgets ) {
			widget->removeEventListener( valueCbs[widget] );
			widget->removeEventListener( closeCbs[widget] );
		}
		widgets.clear();
		valueCbs.clear();
		closeCbs.clear();
		converter = Converter();
		inSetValue = false;
		property = nullptr;
		data = nullptr;
	}

	void bind( UIWidget* widget ) {
		bindListeners( widget );
		widgets.insert( widget );
		inSetValue = true;
		widget->applyProperty( StyleSheetProperty( property, String::toString( data ) ) );
		inSetValue = false;
	}

	void unbind( UIWidget* widget ) {
		if ( widgets.find( widget ) == widgets.end() )
			return;
		widget->removeEventListener( valueCbs[widget] );
		widget->removeEventListener( closeCbs[widget] );
		valueCbs.erase( widget );
		closeCbs.erase( widget );
		widgets.erase( widget );
	}

	~UIDataBind() { reset(); }

	const PropertyDefinition* getPropertyDefinition() const { return property; }

	std::function<void( const T& newVal )> onValueChangeCb;

	const std::set<UIWidget*>& getWidgets() const { return widgets; }

  protected:
	T* data{ nullptr };
	std::set<UIWidget*> widgets;
	std::unordered_map<UIWidget*, Uint32> valueCbs;
	std::unordered_map<UIWidget*, Uint32> closeCbs;
	bool inSetValue{ false };
	const PropertyDefinition* property{ nullptr };
	Converter converter;
	Event::EventType eventType{ Event::OnValueChange };

	void bindListeners( UIWidget* widget ) {
		valueCbs[widget] = widget->addEventListener( eventType, [this]( const Event* event ) {
			processValueChange( event->getNode()->asType<UIWidget>() );
		} );
		closeCbs[widget] = widget->addEventListener( Event::OnClose, [this]( const Event* event ) {
			closeCbs.erase( event->getNode()->asType<UIWidget>() );
			this->widgets.erase( event->getNode()->asType<UIWidget>() );
		} );
	}

	String dataToString() const {
		String str;
		if ( !converter.fromVal( this, str, *data ) ) {
			Log::error( "UIDataBind::dataToString converter::fromVal: unable to convert value "
						"to string." );
		}
		return str;
	}

	void processValueChange( UIWidget* emitter ) {
		if ( inSetValue )
			return;
		bool success = false;
		T val;
		success = converter.toVal( this, val, emitter->getPropertyString( property ) );

		if ( success ) {
			*data = val;
			StyleSheetProperty prop( property, dataToString(), 0, false );
			inSetValue = true;
			for ( auto widget : widgets ) {
				if ( widget != emitter )
					widget->applyProperty( prop );
			}
			inSetValue = false;
			if ( onValueChangeCb )
				onValueChangeCb( val );
		}
	}

	void setValueChange() {
		StyleSheetProperty prop( property, dataToString(), 0, false );
		for ( auto widget : widgets )
			widget->applyProperty( prop );
	}
};

class UIDataBindBool {
  public:
	using Ptr = std::unique_ptr<UIDataBind<bool>>;

	static Ptr
	New( bool* t, const std::set<UIWidget*>& widgets,
		 const UIDataBind<bool>::Converter& converter = UIDataBind<bool>::converterBool(),
		 const String& valueKey = "value" ) {
		return UIDataBind<bool>::New( t, widgets, converter, valueKey );
	}

	static Ptr
	New( bool* t, UIWidget* widget,
		 const UIDataBind<bool>::Converter& converter = UIDataBind<bool>::converterBool(),
		 const String& valueKey = "value" ) {
		return UIDataBind<bool>::New( t, widget, converter, valueKey );
	}
};

class UIDataBindString {
  public:
	using Ptr = std::unique_ptr<UIDataBind<String>>;

	static Ptr New( String* t, const std::set<UIWidget*>& widgets,
					const UIDataBind<String>::Converter& converter =
						UIDataBind<String>::converterString(),
					const String& valueKey = "text",
					const Event::EventType& eventType = Event::OnTextChanged ) {
		return UIDataBind<String>::New( t, widgets, converter, valueKey, eventType );
	}

	static Ptr New( String* t, UIWidget* widget,
					const UIDataBind<String>::Converter& converter =
						UIDataBind<String>::converterString(),
					const String& valueKey = "text",
					const Event::EventType& eventType = Event::OnTextChanged ) {
		return UIDataBind<String>::New( t, widget, converter, valueKey, eventType );
	}
};

template <typename... Ts> class UIDataBindHolder {
  public:
	using UIDataBindVariant = std::variant<std::unique_ptr<UIDataBind<Ts>>...>;

	UIDataBindHolder& hold( UIDataBindVariant&& ptr ) {
		mHolder.emplace_back( std::move( ptr ) );
		return *this;
	}

	UIDataBindHolder& operator+=( UIDataBindVariant&& ptr ) {
		mHolder.emplace_back( std::move( ptr ) );
		return *this;
	}

	void clear() { mHolder.clear(); }

  protected:
	Array<UIDataBindVariant> mHolder;
};

template <typename... Ts> class UIDataBindHolderKV {
  public:
	using UIDataBindVariant = std::variant<std::unique_ptr<UIDataBind<Ts>>...>;

	UIDataBindHolderKV& hold( String key, UIDataBindVariant&& ptr ) {
		mHolder[std::move( key )] = std::move( ptr );
		return *this;
	}

	UIDataBindHolderKV& operator+=( std::pair<String, UIDataBindVariant&&> pair ) {
		mHolder[std::move( pair.first )] = std::move( pair.second );
		return *this;
	}

	void clear() { mHolder.clear(); }

  protected:
	HashMap<String, UIDataBindVariant> mHolder;
};

}} // namespace Zero::UI

#endif // EE_UI_UIDATABIND_HPP
