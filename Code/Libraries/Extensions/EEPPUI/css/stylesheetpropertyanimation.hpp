#ifndef EE_UI_CSS_STYLESHEETPROPERTYANIMATION_HPP
#define EE_UI_CSS_STYLESHEETPROPERTYANIMATION_HPP

#include <eepp/math/ease.hpp>
#include <eepp/scene/action.hpp>
#include <eepp/ui/css/animationdefinition.hpp>
#include <eepp/ui/css/keyframesdefinition.hpp>
#include <eepp/ui/css/propertydefinition.hpp>

using Zero::Math;
using Zero::Scene;

namespace Zero { namespace UI {
class UIWidget;
}} // namespace Zero::UI

namespace Zero { namespace UI { namespace CSS {

enum class AnimationOrigin : uint8_t { User, Animation, Transition };

class ZeroShared StyleSheetPropertyAnimation : public Action {
  public:
	static constexpr String::HashType ID = String::hash( "StyleSheetPropertyAnimation" );

	static void tweenProperty( UIWidget* widget, const Float& normalizedProgress,
							   const PropertyDefinition* property, const String& startValue,
							   const String& endValue,
							   const Ease::Interpolation& timingFunction,
							   const Array<double> timingFunctionParameters,
							   const Uint32& propertyIndex, const bool& isDone );

	static StyleSheetPropertyAnimation* fromAnimationKeyframes(
		const AnimationDefinition& animation, const KeyframesDefinition& keyframes,
		const PropertyDefinition* propertyDef, UIWidget* widget, const Uint32& propertyIndex,
		const AnimationOrigin& animationOrigin = AnimationOrigin::Animation );

	static bool animationSupported( const PropertyType& type );

	static StyleSheetPropertyAnimation*
	New( const AnimationDefinition& animation, const PropertyDefinition* propertyDef,
		 Array<String> states, Array<Float> animationStepsTime,
		 const Uint32& propertyIndex, const AnimationOrigin& animationOrigin );

	static StyleSheetPropertyAnimation*
	New( const PropertyDefinition* property, const String& startValue,
		 const String& endValue, const Uint32& propertyIndex, const Time& duration,
		 const Time& delay, const Ease::Interpolation& timingFunction,
		 const Array<double>& timingFunctionParameters,
		 const AnimationOrigin& animationOrigin );

	void start() override;

	void stop() override;

	void update( const Time& time ) override;

	bool isDone() override;

	Float getCurrentProgress() override;

	Time getTotalTime() override;

	Action* clone() const override;

	Action* reverse() const override;

	const Uint32& getPropertyIndex() const;

	const String& getStartValue() const;

	const String& getEndValue() const;

	const Time& getElapsed() const;

	void setElapsed( const Time& elapsed );

	const AnimationOrigin& getAnimationOrigin() const;

	void setRunning( const bool& running );

	void setPaused( const bool& paused );

	void notifyClose();

	const AnimationDefinition& getAnimation() const;

  protected:
	AnimationDefinition mAnimation;
	const PropertyDefinition* mPropertyDef;
	Array<String> mStates;
	Array<Float> mAnimationStepsTime;
	Time mRealElapsed;
	Time mElapsed;
	Int32 mPendingIterations;
	Uint32 mPropertyIndex;
	String mFillModeValue;
	AnimationOrigin mAnimationOrigin;
	bool mPaused;

	StyleSheetPropertyAnimation( const AnimationDefinition& animation,
								 const PropertyDefinition* propertyDef,
								 Array<String> states,
								 Array<Float> animationStepsTime, const Uint32& propertyIndex,
								 const AnimationOrigin& animationOrigin );

	void onStart() override;

	void onUpdate( const Time& time ) override;

	void onTargetChange() override;

	void prepareDirection();

	void reverseAnimation();
};

}}} // namespace Zero::UI::CSS

#endif // EE_UI_CSS_STYLESHEETPROPERTYANIMATION_HPP
