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
	this->setExpandsOnDoubleClick(false); //����˫��չ�����ڵ�
	this->setStyle(QStyleFactory::create("windowsxp"));
	//this->setHeaderHidden(true);
	this->setColumnCount(2);
	QStringList labels = QStringList() << QStringLiteral("�������") << QStringLiteral("��ѹ�ȼ�");
	this->setHeaderLabels(labels);
	this->setColumnWidth(0, 220);
	this->setColumnWidth(1, 120);

}

XJModelTree::~XJModelTree()
{

}

void XJModelTree::init_Test()
{
	//ģ�������---ʵ����Ӧ�ö�ȡ���ݿ�����ļ��е�ģ�ͼ�¼������ģ����
	QTreeWidgetItem* pRootItem = this->invisibleRootItem();
	CustomTreeItem * pRootAreaItem = new CustomTreeItem(pRootItem, area_node);
	//������������
	pRootAreaItem->setText(0, QStringLiteral("���Ը�������"));
	PTRAREA_Members  pNodeData = new AREA_Members();
	pNodeData->id = 1;
	pNodeData->node_name = QStringLiteral("���Ը�������").toStdString ();
	pNodeData->node_type = area_node;
	pNodeData->parent = 0;
	pNodeData->voltagegrade = 7;
	pRootAreaItem->setText(1, VoltageGradeList[pNodeData->voltagegrade]);
	pNodeData->leavasFlag = 0;
	pRootAreaItem->setNodeData(pNodeData);

	//���������һ��0�Ž��
	CustomTreeItem * itemArea0 = new CustomTreeItem(pRootAreaItem, area_node);
	itemArea0->setText(0, QStringLiteral("��������һ����"));
	pNodeData = new AREA_Members();
	pNodeData->id = 2;
	pNodeData->node_name = QStringLiteral("��������һ����").toStdString();
	pNodeData->node_type = area_node;
	pNodeData->parent = 1;
	pNodeData->voltagegrade = 6;
	itemArea0->setText(1, VoltageGradeList[pNodeData->voltagegrade]);
	pNodeData->leavasFlag = 1;
	itemArea0->setNodeData(pNodeData);
	
	//����0��վ���
	CustomTreeItem * itemStation = new CustomTreeItem(itemArea0, station_node);
	itemStation->setText(0, QStringLiteral("����վ���0"));
	PTRSTATION_Members pStationNodeData = new STATION_Members();
	pStationNodeData->id = 3;
	pStationNodeData->node_name = QStringLiteral("����վ���").toStdString();
	pStationNodeData->node_type = station_node;
	pStationNodeData->parent = 2;
	pStationNodeData->voltagegrade = 6;
	itemStation->setText(1, VoltageGradeList[pNodeData->voltagegrade]);
	pStationNodeData->station_type = 2;
	pStationNodeData->enabledFlag = true;
	itemStation->setNodeData(pStationNodeData);

	//����0��һ���豸
	CustomTreeItem * itemAdevice = new CustomTreeItem(itemStation, station_node);
	itemAdevice->setText(0, QStringLiteral("����һ���豸���0"));
	PTRADEVICE_Members pAdeviceNodeData = new ADEVICE_Members();
	pAdeviceNodeData->id = 4;
	pAdeviceNodeData->node_name = QStringLiteral("����һ���豸���").toStdString();
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
	else  //�������
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