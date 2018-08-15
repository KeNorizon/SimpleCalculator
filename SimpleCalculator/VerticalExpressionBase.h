#pragma once
#include "ExpressionBase.h"
#include <vector>
class VerticalExpressionBase :
	public ExpressionBase
{
public:
	HorizontalExpression **ChildrenArray;
	VerticalExpressionBase(ExpressionBase *parent, int childrenCount);
	void remove(ExpressionBase *, bool moveCursor);
	~VerticalExpressionBase();
};

