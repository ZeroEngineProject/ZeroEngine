#ifndef EE_UI_CSS_TIMINGFUNCTION_HPP
#define EE_UI_CSS_TIMINGFUNCTION_HPP

#include <eepp/core/string.hpp>
#include <eepp/math/ease.hpp>

using Zero;
using Zero::Math;

namespace Zero { namespace UI { namespace CSS {

class ZeroShared TimingFunction {
  public:
	static TimingFunction parse( String timingFunction );

	Ease::Interpolation interpolation{ Ease::None };
	Array<double> parameters;
};

}}} // namespace Zero::UI::CSS

#endif // EE_UI_CSS_TIMINGFUNCTION_HPP
