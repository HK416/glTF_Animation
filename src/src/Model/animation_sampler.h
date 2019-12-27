/************************************/
/*  FILE NAME: animation_sampler.h  */
/************************************/
#ifndef _ANIMATION_SAMPLER_H_
#define _ANIMATION_SAMPLER_H_

/**************/
/*  INCLUDES  */
/**************/
#include <vector>
#include <vector.hpp>

/*****************************************/
/*  ENUM CLASS NAME: INTERPOLATION_TYPE  */
/*****************************************/
enum class INTERPOLATION_TYPE
{
    UNKNOWN,
    LINEAR,
    STEP,
    CUBICSPLINE
}; // enum class INTERPOLATION_TYPE

/**********************************/
/*  CLASS NAME: AnimationSampler  */
/**********************************/
class AnimationSampler
{
public:
    AnimationSampler();
    AnimationSampler(const AnimationSampler& other);

public:
    INTERPOLATION_TYPE interpolation;
    std::vector<float> inputs;
    std::vector<orca::vec4<float>> outputs;
}; // class AnimationSampler
#endif // !_ANIMATION_SAMPLER_H_