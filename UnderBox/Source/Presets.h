#pragma once
#include <array>
#include <vector>
#include "Params.h"

struct Preset { juce::String name; int cat = 0; std::array<float, NumParams> v {}; };

const std::vector<Preset>& factoryPresets();
const juce::StringArray& categoryNames();   // la ultima es "User"
