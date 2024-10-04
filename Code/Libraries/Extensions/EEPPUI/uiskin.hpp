#ifndef EE_UICUISKIN_HPP
#define EE_UICUISKIN_HPP

#include <eepp/graphics/statelistdrawable.hpp>
#include <eepp/ui/base.hpp>

namespace EE { namespace UI {

class ZeroShared UISkin : public StateListDrawable {
  public:
	static UISkin* New( const String& name = "" );

	explicit UISkin( const String& name = "" );

	virtual ~UISkin();

	virtual Sizef getSize( const Uint32& state );

	virtual Sizef getPixelsSize( const Uint32& state );

	virtual Sizef getSize();

	virtual Sizef getPixelsSize();

	virtual UISkin* clone();

	virtual UISkin* clone( const String& NewName );

	virtual Rectf getBorderSize( const Uint32& state );

	virtual Rectf getBorderSize();
};

}} // namespace EE::UI

#endif
