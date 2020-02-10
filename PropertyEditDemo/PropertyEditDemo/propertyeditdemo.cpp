#include "propertyeditdemo.h"
#include "../property/fhpropertywidget.h"
#include "xjmodeltree.h"

#include <QMdiArea>
#include <QMdiSubWindow>
#include <QDebug>
#include <QSignalMapper>
#include <QDockWidget>
#include <QTreeWidget>
#include <QTextEdit>
#include <QDateTime>


PropertyEditDemo::PropertyEditDemo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle(QStringLiteral("属性编辑演示模块主窗口"));
	init_ui();
}

void PropertyEditDemo::init_ui(void)
{
	m_pWorkspace = new QMdiArea;
	m_pWorkspace->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_pWorkspace->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_pWorkspace->setMinimumSize(width() / 3, height() / 3);
	setCentralWidget(m_pWorkspace);
	
	//创建属性编辑窗口
	QDockWidget *propertyDock = new QDockWidget(QStringLiteral("属性编辑窗"), this);
	propertyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_pPropertyWidget = new FHPropertyWidget();
	m_pPropertyWidget->setCustomItemHeight(24);

	propertyDock->setWidget(m_pPropertyWidget);
	addDockWidget(Qt::RightDockWidgetArea, propertyDock);

	//创建演示树模型窗
	QDockWidget* treeModelDock = new QDockWidget(QStringLiteral("模型窗口"), this);
	treeModelDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_pTreeWidget = new XJModelTree();
	treeModelDock->setWidget(m_pTreeWidget);
	addDockWidget(Qt::LeftDockWidgetArea, treeModelDock);

	//这是qt4方式的connect
	//connect(m_pTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(TreeItemClicked(QTreeWidgetItem*, int)));

	//qt5对于槽函数的方式已经被改变,针对信号通过基类选择，槽函数变化为任意的函数,但是注意信号与槽参数一定要一一对应
	connect(m_pTreeWidget, &QTreeWidget::itemClicked, this, &PropertyEditDemo::TreeItemClicked);

	m_pTreeWidget->init_Test();
}

void PropertyEditDemo::TreeItemClicked(QTreeWidgetItem* curItem, int _col)
{
	if (NULL == curItem)
			return;
	CustomTreeItem * treeItem = static_cast<CustomTreeItem*> (curItem);
	QList<PropertyItemData> items = treeItem->getPropertyList();			//获取选中对象的属性列表

	if(m_pPropertyWidget)
	{
		//为属性框设置回调通知对象
		if (m_pTreeWidget)
		{
			m_pTreeWidget->setCurSelItem(treeItem);
			m_pTreeWidget->setToCallBack(m_pPropertyWidget);
		}
			
		//清空属性框的显示
		m_pPropertyWidget->clearView();
		int propertyCount = items.count();
		if (0 != propertyCount)
		{
			QString topName = QStringLiteral("基本属性");
			FHPropertyItem* pTopItem = m_pPropertyWidget->addTopItem(topName);
			PropertyItemData data;
			for (int i = 0; i < items.count(); i++)
			{
				data = items.at(i);
				FHPropertyItem* pPropertyItem = m_pPropertyWidget->addItem(pTopItem, data.nam, data.indexName, data.val, data.type);	//增加属性
				if (nullptr == pPropertyItem)
				{
					return;
				}
				pPropertyItem->setEditEnable(data.editFlag);
				pPropertyItem->setAtonceVal(data.atonceFlg);		//设置属性被编辑后是否即刻提交标志
				if (StateProperty == data.type)						//如果是状态类型的属性，则需要调用对应对象的设置状态属性ComboBox框组件的内容
				{
					treeItem->setComboBoxContent(pPropertyItem);
				}
			}
		}
		m_pPropertyWidget->flushView();
	}

}