#include "pch.h"
#include "NumUtil.h"

int NumUtil::Wrap(int const currentValue, int const upperBound)
{
	int range = upperBound;
	int wrappedValue = (currentValue % range);
	if (wrappedValue < 0)
		return upperBound + wrappedValue;
	else
		return wrappedValue;
}