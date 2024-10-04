#ifndef EE_UI_CSS_TRANSITIONDEFINITION_HPP
#define EE_UI_CSS_TRANSITIONDEFINITION_HPP

#include <eepp/math/ease.hpp>
#include <eepp/system/time.hpp>
#include <eepp/ui/css/stylesheetproperty.hpp>
#include <map>
#include <vector>

using namespace EE::Math;
using namespace EE::System;

namespace EE { namespace UI { namespace CSS {

class ZeroShared TransitionDefinition {
  public:
	static HashMap<String, TransitionDefinition>
	parseTransitionProperties( const std::vector<const StyleSheetProperty*>& styleSheetProperties );

	TransitionDefinition() : timingFunction( Ease::Linear ) {}

	const String& getProperty() const { return property; }

	Ease::Interpolation getTimingFunction() const { return timingFunction; }

	std::vector<double> getTimingFunctionParameters() const { return timingFunctionParameters; }

	const Time& getDelay() const { return delay; }

	const Time& getDuration() const { return duration; }

	String property;
	Ease::Interpolation timingFunction = Ease::Interpolation::Linear;
	std::vector<double> timingFunctionParameters{};
	Time delay = Time::Zero;
	Time duration = Time::Zero;
};

typedef HashMap<String, TransitionDefinition> TransitionsMap;

}}} // namespace EE::UI::CSS

#endif
