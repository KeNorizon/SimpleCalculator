#pragma once
#include <QFont>
#include <QColor>
#include <unordered_map>
#include "Enums.h"
#include "ExpressionElement.h"
#include <QRect>
#include "ExpressionRect.h"

class VisualMgr
{
public:
	QFont PanelExprFont;
	QFont PanelSubExprFont;

	QColor PanelMainColor;
	QColor PanelSubColor;
	QColor PanelFocusBgColor;
	QColor PanelCursorColor;

	QRect VisibleRect;
	QPoint ExprPosiiton;

	DualHeight PanelExprHeight;
	int BasicCharHeightDelta;
	int SubBasicCharHeightDelta;
	std::unordered_map<TokenType, int> PanelTokenWidth;

	DualHeight PanelSubExprHeight;
	std::unordered_map<TokenType, int> PanelSubTokenWidth;

	int ExprSuperscriptDelta;
	int SubExprSuperscriptDelta;


	VisualMgr();
	~VisualMgr();

	void updateParamCache();
	void updateTokenWidth(TokenType, char, const QFontMetrics &, const QFontMetrics &);
	void ensureCursorInScreen();
	void exprPosLimit();

	void updateVisibleRectPos();
	void updateVisibleRectSize(QSize);
};
