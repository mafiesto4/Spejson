#pragma once

template<class T>
// Clamps value withing given range
const T& clamp(const T& val, const T& min, const T& max)
{
	return val < min ? min : val > max ? max : val;
}
