#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLayout>
#include "ArithmeticPanel.h"
#include "Enums.h"
#include "KeyboardButton.h"

class MainWindowUI
{
public:
	QMenuBar *MenuBar;
	QWidget *CentralWidget;

	// Layouts
	QVBoxLayout *LayoutY;
	QGridLayout *LayoutButtons;

	// Arithmetic Panel
	ArithmeticPanel *FrameArithmetic;

	// Calc Keyboard Buttons
	KeyboardButton *Buttons[KbButtonName::_Last];

	MainWindowUI();
	void setupUi(QMainWindow *);
	KeyboardButton *getButton(KbButtonName);
	~MainWindowUI();

private:
	void addButton(const char *, KbButtonName, KbButtonType);
};

