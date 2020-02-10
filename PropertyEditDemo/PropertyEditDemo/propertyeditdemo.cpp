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
	this->setWindowTitle(QStringLiteral("���Ա༭��ʾģ��������"));
	init_ui();
}

void PropertyEditDemo::init_ui(void)
{
	m_pWorkspace = new QMdiArea;
	m_pWorkspace->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_pWorkspace->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_pWorkspace->setMinimumSize(width() / 3, height() / 3);
	setCentralWidget(m_pWorkspace);
	
	//�������Ա༭����
	QDockWidget *propertyDock = new QDockWidget(QStringLiteral("���Ա༭��"), this);
	propertyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_pPropertyWidget = new FHPropertyWidget();
	m_pPropertyWidget->setCustomItemHeight(24);

	propertyDock->setWidget(m_pPropertyWidget);
	addDockWidget(Qt::RightDockWidgetArea, propertyDock);

	//������ʾ��ģ�ʹ�
	QDockWidget* treeModelDock = new QDockWidget(QStringLiteral("ģ�ʹ���"), this);
	treeModelDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_pTreeWidget = new XJModelTree();
	treeModelDock->setWidget(m_pTreeWidget);
	addDockWidget(Qt::LeftDockWidgetArea, treeModelDock);

	//����qt4��ʽ��connect
	//connect(m_pTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(TreeItemClicked(QTreeWidgetItem*, int)));

	//qt5���ڲۺ����ķ�ʽ�Ѿ����ı�,����ź�ͨ������ѡ�񣬲ۺ����仯Ϊ����ĺ���,����ע���ź���۲���һ��Ҫһһ��Ӧ
	connect(m_pTreeWidget, &QTreeWidget::itemClicked, this, &PropertyEditDemo::TreeItemClicked);

	m_pTreeWidget->init_Test();
}

void PropertyEditDemo::TreeItemClicked(QTreeWidgetItem* curItem, int _col)
{
	if (NULL == curItem)
			return;
	CustomTreeItem * treeItem = static_cast<CustomTreeItem*> (curItem);
	QList<PropertyItemData> items = treeItem->getPropertyList();			//��ȡѡ�ж���������б�

	if(m_pPropertyWidget)
	{
		//Ϊ���Կ����ûص�֪ͨ����
		if (m_pTreeWidget)
		{
			m_pTreeWidget->setCurSelItem(treeItem);
			m_pTreeWidget->setToCallBack(m_pPropertyWidget);
		}
			
		//������Կ����ʾ
		m_pPropertyWidget->clearView();
		int propertyCount = items.count();
		if (0 != propertyCount)
		{
			QString topName = QStringLiteral("��������");
			FHPropertyItem* pTopItem = m_pPropertyWidget->addTopItem(topName);
			PropertyItemData data;
			for (int i = 0; i < items.count(); i++)
			{
				data = items.at(i);
				FHPropertyItem* pPropertyItem = m_pPropertyWidget->addItem(pTopItem, data.nam, data.indexName, data.val, data.type);	//��������
				if (nullptr == pPropertyItem)
				{
					return;
				}
				pPropertyItem->setEditEnable(data.editFlag);
				pPropertyItem->setAtonceVal(data.atonceFlg);		//�������Ա��༭���Ƿ񼴿��ύ��־
				if (StateProperty == data.type)						//�����״̬���͵����ԣ�����Ҫ���ö�Ӧ���������״̬����ComboBox�����������
				{
					treeItem->setComboBoxContent(pPropertyItem);
				}
			}
		}
		m_pPropertyWidget->flushView();
	}

}