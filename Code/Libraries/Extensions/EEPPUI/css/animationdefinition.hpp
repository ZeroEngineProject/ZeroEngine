#ifndef EE_UI_CSS_ANIMATIONDEFINITION_HPP
#define EE_UI_CSS_ANIMATIONDEFINITION_HPP

#include <eepp/math/ease.hpp>
#include <eepp/system/time.hpp>
#include "stylesheetproperty.hpp"
#include <vector>

using Zero::Math;
using Zero::System;

namespace Zero { namespace UI { namespace CSS {

class ZeroShared AnimationDefinition {
  public:
	static HashMap<String, AnimationDefinition>
	parseAnimationProperties( const Array<const StyleSheetProperty*>& stylesheetProperties );

	/* https://developer.mozilla.org/en-US/docs/Web/CSS/animation-direction */
	enum AnimationDirection {
		Normal = String::hash( "normal" ),
		Reverse = String::hash( "reverse" ),
		Alternate = String::hash( "alternate" ),
		AlternateReverse = String::hash( "alternate-reverse" )
	};

	static bool isDirectionString( const String str );

	static AnimationDirection directionFromString( String str );

	/* https://developer.mozilla.org/en-US/docs/Web/CSS/animation-fill-mode */
	enum AnimationFillMode {
		None = String::hash( "none" ),
		Forwards = String::hash( "forwards" ),
		Backwards = String::hash( "backwards" ),
		Both = String::hash( "both" )
	};

	static bool isAnimationFillModeString( const String& str );

	static AnimationFillMode fillModeFromString( String str );

	AnimationDefinition();

	const AnimationDirection& getDirection() const;

	const bool& isPaused() const;

	const Int32& getIterations() const;

	const String& getName() const;

	const Time& getDelay() const;

	const Time& getDuration() const;

	const Ease::Interpolation& getTimingFunction() const;

	const Array<double>& getTimingFunctionParameters() const;

	const AnimationFillMode& getFillMode() const;

	void setName( const String& value );

	void setDelay( const Time& value );

	void setDuration( const Time& value );

	void setIterations( const Int32& value );

	void setTimingFunction( const Ease::Interpolation& value );

	void setTimingFunctionParameters( const Array<double>& timingFunctionParameters );

	void setDirection( const AnimationDirection& value );

	void setFillMode( const AnimationFillMode& value );

	void setPaused( bool value );

	const String::HashType& getId() const;

  protected:
	String::HashType mId;
	String mName;
	Time mDelay = Time::Zero;
	Time mDuration = Time::Zero;
	Int32 mIterations = 1; /* -1 == "infinite" */
	Ease::Interpolation mTimingFunction = Ease::Interpolation::Linear;
	Array<double> mTimingFunctionParameters{};
	AnimationDirection mDirection = Normal;
	AnimationFillMode mFillMode = None;
	bool mPaused = false;
};

inline bool operator==( const AnimationDefinition& a, const AnimationDefinition& b ) {
	return a.getDuration() == b.getDuration() && a.getTimingFunction() == b.getTimingFunction() &&
		   a.getDelay() == b.getDelay() && a.getDirection() == b.getDirection() &&
		   a.isPaused() == b.isPaused() && a.getIterations() == b.getIterations() &&
		   a.getName() == b.getName() &&
		   a.getTimingFunctionParameters() == b.getTimingFunctionParameters();
}

inline bool operator!=( const AnimationDefinition& a, const AnimationDefinition& b ) {
	return !( a == b );
}

typedef HashMap<String, AnimationDefinition> AnimationsMap;

}}} // namespace Zero::UI::CSS

#endif // EE_UI_CSS_ANIMATIONDEFINITION_HPP
