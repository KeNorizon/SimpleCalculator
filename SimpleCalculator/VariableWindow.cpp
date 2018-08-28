#include "VariableWindow.h"
#include "GlobalMgr.h"
#include "Util.h"
#include "ArithmeticPanel.h"
#include "MainWindow.h"
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <sstream>

#pragma execution_character_set("utf-8")

void VariableWindow::eventMemorize()
{
	QListWidgetItem *selected = VariableList->currentItem();
	if (selected != nullptr)
	{
		std::string varName = getVariableName(selected);
		if (g_Data->Variable.modifiable(varName))
		{
			if (g_Data->ExprResult.good())
			{
				g_Data->Variable.set(varName, g_Data->ExprResult.Value);
				refreshItem(selected);
			}
		}
		else
		{
			QMessageBox::warning(this, "����ʧ��", "�������ܱ��޸ġ�");
		}
	}
}

void VariableWindow::refreshItem(QListWidgetItem *item)
{
	std::string varName = getVariableName(item);
	item->setText(QString::asprintf("%s\n= %g", varName.c_str(), g_Data->Variable.get(varName)));
}

std::string VariableWindow::getVariableName(QListWidgetItem *item)
{
	return item->data(ListItemVarNameRole).toString().toStdString();
}

void VariableWindow::showItems()
{
	VariableList->clear();
	for (auto iter = g_Data->Variable.Constant.cbegin(); iter != g_Data->Variable.Constant.cend(); iter++)
	{
		addVariableItem((*iter).first, (*iter).second);
	}
	for (auto iter = g_Data->Variable.Variable.cbegin(); iter != g_Data->Variable.Variable.cend(); iter++)
	{
		addVariableItem((*iter).first, (*iter).second);
	}
}

void VariableWindow::addVariableItem(const std::string &str, double value)
{
	QListWidgetItem *item = new QListWidgetItem(QString::asprintf("%s\n= %g", str.c_str(), value));
	item->setData(ListItemVarNameRole, str.c_str());
	VariableList->addItem(item);
	VariableList->setCurrentItem(item);
}

void VariableWindow::beforeClose()
{
	if (g_Data->Config.AutoCompute)
	{
		g_Data->doCompute();
		g_Data->repaintExpr();
	}
}

void VariableWindow::reject()
{
	beforeClose();
	QDialog::reject();
}

void VariableWindow::closeEvent(QCloseEvent * event)
{
	beforeClose();
	QDialog::closeEvent(event);
}

int VariableWindow::exec()
{
	showItems();
	return QDialog::exec();
}

void VariableWindow::eventGetVariable()
{
	QListWidgetItem *selected = VariableList->currentItem();
	if (selected != nullptr)
	{
		eventVariableDblClick(selected);
	}
}

void VariableWindow::eventSetValue()
{
	QListWidgetItem *selected = VariableList->currentItem();
	if (selected != nullptr)
	{
		std::string varName = getVariableName(selected);
		if (g_Data->Variable.modifiable(varName))
		{
			bool ok;
			QString valueStr = QInputDialog::getText(this, "������ֵ", "������Ҫ�趨����ֵ", QLineEdit::Normal, QString(), &ok);
			if (ok)
			{
				std::stringstream ss;
				double value;
				ss << valueStr.toStdString();
				ss >> value;
				if (ss.fail())
				{
					QMessageBox::warning(this, "����ʧ��", "�������ֵ��ʽ����");
				}
				else
				{
					g_Data->Variable.set(varName, value);
					refreshItem(selected);
				}
			}
		}
		else
		{
			QMessageBox::warning(this, "����ʧ��", "�������ܱ��޸ġ�");
		}
	}
	
}

void VariableWindow::eventClear()
{
	QListWidgetItem *selected = VariableList->currentItem();
	if (selected != nullptr)
	{
		std::string varName = getVariableName(selected);
		if (g_Data->Variable.modifiable(varName))
		{
			g_Data->Variable.set(varName, 0.0);
			refreshItem(selected);
		}
		else
		{
			QMessageBox::warning(this, "����ʧ��","�������ܱ��޸ġ�");
		}
	}
}

void VariableWindow::eventClearAll()
{
	g_Data->Variable.clearAll();
	showItems();
}

void VariableWindow::eventAddVariable()
{
	bool ok;
	QString newVarName = QInputDialog::getText(this, "��������", "������Ҫ�����ı�����", QLineEdit::Normal, QString(), &ok);
	if (ok)
	{
		std::string newVarNameStd = newVarName.toStdString();
		if (g_Data->Variable.has(newVarNameStd))
		{
			QMessageBox::warning(this, "�������ظ�", "Ҫ�����ı������Ѵ��ڡ�");
		}
		else if (!VariableMgr::variableNameAvailable(newVarName))
		{
			QMessageBox::warning(this, "�����������Ϲ淶", "�����ֻ���з���[ + - * / ( ) ]���ı�������Ϊ��������");
		}
		else
		{
			g_Data->Variable.set(newVarNameStd, 0.0);
			addVariableItem(newVarNameStd, 0.0);
		}
	}
}

void VariableWindow::eventDeleteVariable()
{
	QListWidgetItem *selected = VariableList->currentItem();
	if (selected != nullptr)
	{
		std::string varName = getVariableName(selected);
		if (g_Data->Variable.modifiable(varName))
		{
			g_Data->Variable.del(varName);
			VariableList->removeItemWidget(selected);
			delete selected;
		}
		else
		{
			QMessageBox::warning(this, "ɾ��ʧ��", "�������ܱ�ɾ����");
		}
	}
}

void VariableWindow::eventVariableDblClick(QListWidgetItem *item)
{
	std::string varName = getVariableName(item);
	const Cursor &cursor = g_Data->Cursor.get();
	cursor.FocusdExpr->insertVariable(cursor.Pos, varName);
	MainWindow::getInstance()->afterInput(true);
	close();
}

void VariableWindow::eventOpenFile()
{
	QString fileName = QFileDialog::getOpenFileName(
		this, "��������ļ�", QString(),
		VariableFileFilter);
	if (!fileName.isEmpty()) 
	{
		if (g_Data->Variable.loadFromFile(fileName.toStdWString()))
		{
			showItems();
		}
		else
		{
			QMessageBox::information(this, "�����ļ�ʧ��", "��ѡ����ļ����𻵻���ָ����ʽ��");
		}
	}
}

void VariableWindow::eventSaveFile()
{
	QString fileName = QFileDialog::getSaveFileName(
		this, "��������ļ�", QString(),
		VariableFileFilter);
	if (!fileName.isEmpty())
	{
		if (!g_Data->Variable.saveToFile(fileName.toStdWString()))
		{
			QMessageBox::information(this, "�����ļ�ʧ��", "�ļ���ռ�û�û��д��Ȩ�ޡ�");
		}
	}
}

VariableWindow::VariableWindow(QWidget *parent)
	: QDialog(parent), Singleton<VariableWindow>()
{
	this->setWindowTitle("���� & ����");
	this->resize(550, 400);
	setQtWindowIcon(winId());

	LayoutY = new QVBoxLayout(this);
	LayoutButtons = new QHBoxLayout();
	LayoutButtonsSecond = new QHBoxLayout();

	VariableList = new QListWidget(this);
	LayoutY->addWidget(VariableList);

	ButtonMemorize = new QPushButton("���뵱ǰ���", this);
	ButtonSetValue = new QPushButton("������ֵ", this);
	ButtonGetVariable = new QPushButton("ȡ����", this);
	ButtonClear = new QPushButton("����", this);
	ButtonClearAll = new QPushButton("��������", this);

	ButtonAddVariable = new QPushButton("��ӱ���...", this);
	ButtonDeleteVariable = new QPushButton("ɾ������", this);

	ButtonOpenFile = new QPushButton("���ļ�����...", this);
	ButtonSaveFile = new QPushButton("���浽�ļ�...", this);

	LayoutButtonsSecond->addWidget(ButtonOpenFile);
	LayoutButtonsSecond->addWidget(ButtonSaveFile);
	LayoutButtonsSecond->addStretch();
	LayoutButtonsSecond->addWidget(ButtonAddVariable);
	LayoutButtonsSecond->addWidget(ButtonDeleteVariable);

	LayoutButtons->addWidget(ButtonClearAll);
	LayoutButtons->addStretch();
	LayoutButtons->addWidget(ButtonMemorize);
	LayoutButtons->addWidget(ButtonSetValue);
	LayoutButtons->addWidget(ButtonClear);
	LayoutButtons->addWidget(ButtonGetVariable);

	LayoutY->addLayout(LayoutButtonsSecond);
	LayoutY->addLayout(LayoutButtons);

	VariableList->setStyleSheet("QListWidget{outline:0px; border:1px solid gray; color:black; font-family:\"Microsoft YaHei UI\"; font-size:18px;}"
		"QListWidget::Item{border:none; padding-top:5px; padding-bottom:5px;}"
		"QListWidget::Item:hover{background:#c2c2c2;}"
		"QListWidget::item:selected{background:#a6d8ff;}"
		"QListWidget::item:selected:!active{color:black;}"
		"QListWidget::item:selected:active{color:black;}"
	);
	this->setStyleSheet("QPushButton {font-family:\"Microsoft YaHei UI\";}");
	showItems();

	connect(ButtonAddVariable, SIGNAL(clicked()), this, SLOT(eventAddVariable()));
	connect(ButtonDeleteVariable, SIGNAL(clicked()), this, SLOT(eventDeleteVariable()));
	connect(ButtonMemorize, SIGNAL(clicked()), this, SLOT(eventMemorize()));
	connect(ButtonGetVariable, SIGNAL(clicked()), this, SLOT(eventGetVariable()));
	connect(ButtonSetValue, SIGNAL(clicked()), this, SLOT(eventSetValue()));
	connect(ButtonClear, SIGNAL(clicked()), this, SLOT(eventClear()));
	connect(ButtonClearAll, SIGNAL(clicked()), this, SLOT(eventClearAll()));
	connect(ButtonOpenFile, SIGNAL(clicked()), this, SLOT(eventOpenFile()));
	connect(ButtonSaveFile, SIGNAL(clicked()), this, SLOT(eventSaveFile()));

	connect(VariableList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(eventVariableDblClick(QListWidgetItem *)));
}


VariableWindow::~VariableWindow()
{
}
