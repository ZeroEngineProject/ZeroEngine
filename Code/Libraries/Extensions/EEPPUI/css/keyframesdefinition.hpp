#ifndef EE_UI_CSS_KEYFRAMESDEFINITION_HPP
#define EE_UI_CSS_KEYFRAMESDEFINITION_HPP

#include <eepp/math/ease.hpp>
#include <eepp/system/time.hpp>
#include <eepp/ui/css/stylesheetproperty.hpp>
#include <eepp/ui/css/stylesheetstyle.hpp>
#include <map>
#include <vector>

using namespace EE::Math;
using namespace EE::System;

namespace EE { namespace UI { namespace CSS {

class ZeroShared KeyframesDefinition {
  public:
	struct KeyframeBlock {
		Float normalizedTime;
		StyleSheetProperties properties;
	};

	static KeyframesDefinition
	parseKeyframes( const String& name,
					const std::vector<std::shared_ptr<StyleSheetStyle>>& keyframeBlocks );

	const String& getName() const;

	const std::map<Float, KeyframeBlock>& getKeyframeBlocks() const;

	/** @return A map containing all the properties involved in the keyframes definition. */
	std::map<PropertyId, const PropertyDefinition*> getPropertyDefinitionList() const;

	const Uint32& getMarker() const;

	void setMarker( const Uint32& marker );

	Uint32 marker{ 0 };
	String name;
	std::map<Float, KeyframeBlock> keyframeBlocks;
};

typedef std::map<String, KeyframesDefinition> KeyframesDefinitionMap;

}}} // namespace EE::UI::CSS

#endif // EE_UI_CSS_KEYFRAMESDEFINITION_HPP
