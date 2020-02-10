#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_propertyeditdemo.h"

class FHPropertyWidget;
class QMdiArea;
class QTreeWidgetItem;
class XJModelTree;

/*
	属性编辑框使用样例代码
	主窗口类，包含模型树样例窗成员、包含属性编辑窗成员
*/
class PropertyEditDemo : public QMainWindow
{
	Q_OBJECT

public:
	PropertyEditDemo(QWidget *parent = Q_NULLPTR);
private:
	void init_ui(void);
//protected slots:
	void TreeItemClicked(QTreeWidgetItem* curItem, int _col);
	
private:
	Ui::PropertyEditDemoClass		ui;
	FHPropertyWidget*				m_pPropertyWidget;					//属性编辑窗口
	QMdiArea*						m_pWorkspace;						//多窗口模式管理对象
	
	XJModelTree*					m_pTreeWidget;						//树模型窗口
};
