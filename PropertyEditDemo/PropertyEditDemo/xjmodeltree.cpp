#include "xjmodeltree.h"
#include <QStyleFactory>
#include <QMouseEvent>
#include "../memLeak/memdebug.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CustomTreeItem::CustomTreeItem(QTreeWidgetItem *_parent, NodeType _nodetype)
	: QTreeWidgetItem(_parent), m_NodeType(_nodetype), m_pNodeData(NULL)
{
}
CustomTreeItem::CustomTreeItem(QTreeWidget *_treeview)
	: QTreeWidgetItem(_treeview)
{
}
CustomTreeItem::~CustomTreeItem()
{
	if (m_pNodeData)
		delete m_pNodeData;
	m_pNodeData = NULL;
}

QList<PropertyItemData> CustomTreeItem::getPropertyList()
{
	if (m_pNodeData)
		return m_pNodeData->getPropertyList();
	QList<PropertyItemData> list;
	return list;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
XJModelTree::XJModelTree(QWidget *parent)
	: QTreeWidget(parent), m_pCurSelItem(NULL)
{
	
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	this->setExpandsOnDoubleClick(false); //屏蔽双击展开树节点
	this->setStyle(QStyleFactory::create("windowsxp"));
	//this->setHeaderHidden(true);
	this->setColumnCount(2);
	QStringList labels = QStringList() << QStringLiteral("结点名称") << QStringLiteral("电压等级");
	this->setHeaderLabels(labels);
	this->setColumnWidth(0, 220);
	this->setColumnWidth(1, 120);

}

XJModelTree::~XJModelTree()
{

}

void XJModelTree::init_Test()
{
	//模拟测试树---实际中应该读取数据库或者文件中的模型记录来建立模型树
	QTreeWidgetItem* pRootItem = this->invisibleRootItem();
	CustomTreeItem * pRootAreaItem = new CustomTreeItem(pRootItem, area_node);
	//建立区域根结点
	pRootAreaItem->setText(0, QStringLiteral("测试根区域结点"));
	PTRAREA_Members  pNodeData = new AREA_Members();
	pNodeData->id = 1;
	pNodeData->node_name = QStringLiteral("测试根区域结点").toStdString ();
	pNodeData->node_type = area_node;
	pNodeData->parent = 0;
	pNodeData->voltagegrade = 7;
	pRootAreaItem->setText(1, VoltageGradeList[pNodeData->voltagegrade]);
	pNodeData->leavasFlag = 0;
	pRootAreaItem->setNodeData(pNodeData);

	//建立区域第一层0号结点
	CustomTreeItem * itemArea0 = new CustomTreeItem(pRootAreaItem, area_node);
	itemArea0->setText(0, QStringLiteral("测试区域一层结点"));
	pNodeData = new AREA_Members();
	pNodeData->id = 2;
	pNodeData->node_name = QStringLiteral("测试区域一层结点").toStdString();
	pNodeData->node_type = area_node;
	pNodeData->parent = 1;
	pNodeData->voltagegrade = 6;
	itemArea0->setText(1, VoltageGradeList[pNodeData->voltagegrade]);
	pNodeData->leavasFlag = 1;
	itemArea0->setNodeData(pNodeData);
	
	//建立0号站结点
	CustomTreeItem * itemStation = new CustomTreeItem(itemArea0, station_node);
	itemStation->setText(0, QStringLiteral("测试站结点0"));
	PTRSTATION_Members pStationNodeData = new STATION_Members();
	pStationNodeData->id = 3;
	pStationNodeData->node_name = QStringLiteral("测试站结点").toStdString();
	pStationNodeData->node_type = station_node;
	pStationNodeData->parent = 2;
	pStationNodeData->voltagegrade = 6;
	itemStation->setText(1, VoltageGradeList[pNodeData->voltagegrade]);
	pStationNodeData->station_type = 2;
	pStationNodeData->enabledFlag = true;
	itemStation->setNodeData(pStationNodeData);

	//建立0号一次设备
	CustomTreeItem * itemAdevice = new CustomTreeItem(itemStation, station_node);
	itemAdevice->setText(0, QStringLiteral("测试一次设备结点0"));
	PTRADEVICE_Members pAdeviceNodeData = new ADEVICE_Members();
	pAdeviceNodeData->id = 4;
	pAdeviceNodeData->node_name = QStringLiteral("测试一次设备结点").toStdString();
	pAdeviceNodeData->node_type = adevlice_node;
	pAdeviceNodeData->parent = 3;
	pAdeviceNodeData->voltagegrade = 6;
	itemAdevice->setText(1, VoltageGradeList[pNodeData->voltagegrade]);
	pAdeviceNodeData->adevice_type = 2;
	pAdeviceNodeData->Breaker_NO = 901;
	pAdeviceNodeData->LineFlag = "rcs931";
	pAdeviceNodeData->SendFilter = 12345;
	itemAdevice->setNodeData(pAdeviceNodeData);

	this->expandAll();
}

void XJModelTree::mouseReleaseEvent(QMouseEvent * e)
{
	if (e->button() == Qt::RightButton)
	{
	}
	else  //左键击中
	{
		QTreeWidgetItem * item = itemAt(e->pos());
		if (item)
		{
			emit itemClicked(item, 0);
		}
		else
		{
			emit itemClicked(item, 0);
		}
	}
}