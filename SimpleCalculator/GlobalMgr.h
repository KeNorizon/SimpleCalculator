#pragma once
#include "HorizontalExpression.h"
#include "CursorMgr.h"
#include "VisualMgr.h"
#include <QPoint>
#include "ArithmeticPanel.h"

class GlobalMgr
{
private:
	GlobalMgr();
	bool ExprDirtyFlag = false;
	bool EnsureCursorInScreenFlag = false;
	bool RequireComputeFlag = false;
public:
	static void init();
	HorizontalExpression *RootExpr = nullptr;
	CursorMgr Cursor;
	VisualMgr Visual;

	ComputeResult ExprResult;

	bool isExprDirty();
	void markExprDirty();
	void clearExprDirtyFlag();

	bool isEnsureCursorInScreen();
	void markEnsureCursorInScreen();
	void clearEnsureCursorInScreenFlag();

	bool isRequireCompute();
	void markRequireCompute();
	void clearRequireComputeFlag();

	void repaintExpr();
	void updateResult();
	~GlobalMgr();
};

extern GlobalMgr *g_Data;