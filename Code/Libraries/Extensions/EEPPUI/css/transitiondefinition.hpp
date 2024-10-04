#ifndef EE_UI_CSS_TRANSITIONDEFINITION_HPP
#define EE_UI_CSS_TRANSITIONDEFINITION_HPP

#include <eepp/math/ease.hpp>
#include <eepp/system/time.hpp>
#include <eepp/ui/css/stylesheetproperty.hpp>
#include <map>
#include <vector>

using Zero::Math;
using Zero::System;

namespace Zero { namespace UI { namespace CSS {

class ZeroShared TransitionDefinition {
  public:
	static HashMap<String, TransitionDefinition>
	parseTransitionProperties( const Array<const StyleSheetProperty*>& styleSheetProperties );

	TransitionDefinition() : timingFunction( Ease::Linear ) {}

	const String& getProperty() const { return property; }

	Ease::Interpolation getTimingFunction() const { return timingFunction; }

	Array<double> getTimingFunctionParameters() const { return timingFunctionParameters; }

	const Time& getDelay() const { return delay; }

	const Time& getDuration() const { return duration; }

	String property;
	Ease::Interpolation timingFunction = Ease::Interpolation::Linear;
	Array<double> timingFunctionParameters{};
	Time delay = Time::Zero;
	Time duration = Time::Zero;
};

typedef HashMap<String, TransitionDefinition> TransitionsMap;

}}} // namespace Zero::UI::CSS

#endif
