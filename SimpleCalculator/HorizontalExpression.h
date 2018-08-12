#pragma once
#include "ExpressionBase.h"
#include "ExpressionElement.h"
#include "Enums.h"
#include "ValidateResult.h"
#include <vector>

class HorizontalExpression : public ExpressionBase
{
protected:
	ValidateResult validateInternal(int fromIdx, int toIdx);
	int findMatchingRightBracket(int leftBracketIdx, int maxIdx);
	bool IsSubExpr = false;
	int getBasicWidth();
public:
	std::vector<ExpressionElement> Elements;

	HorizontalExpression(ExpressionBase *parent);
	~HorizontalExpression();

	double computeValue();
	void computeSize();
	void computePosition(AnchoredPoint);
	ValidateResult validate();
	int findChildPosition(ExpressionBase *);
	int getLength();

	bool input(KbButtonName, int pos);
	void draw(QPainter *);
	bool getIsSubExpr();
	void setIsSubExpr(bool);
	void drawToken(QPainter *, QPoint, const ExpressionElement *);
	void drawEmptyRect(QPainter *, QPoint );
	const DualHeight &getBasicHeight();
	QPoint pointAt(int, AnchorType anchor = AnchorType::Left);
};

