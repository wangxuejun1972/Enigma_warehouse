#include "fhpropertywidget.h"

#include "../memLeak/memdebug.h"

#include <QVBoxLayout>
#include <string>
#include <QDebug>
#include <QHeaderView>
#include "fhpropertyitem.h"
#include "fhpropertymodel.h"
#include "fhpropertydelegate.h"
#include "fhpropertytreeview.h"
using std::string;


FHPropertyWidget::FHPropertyWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    m_delegate = NULL;

    m_treeView = new FHPropertyTreeView(this);
    mainLayout->addWidget(m_treeView);
    mainLayout->setMargin(1);

    m_model = new FHPropertyModel("", this);
    m_delegate = new FHPropertyDelegate(this);
    m_delegate->setModel(m_model);

    m_treeView->setModel(m_model);
    m_treeView->setItemDelegate(m_delegate);		//设置视图类的实际使用代理
    m_treeView->setRecItemDelegate(m_delegate);		//将自定义代理指针传递到视图对象中记录起来
    m_treeView->setAlternatingRowColors(true);		//
    m_treeView->setUpdatesEnabled(1);			//
    m_treeView->expandAll();						//设置树全部展开
    m_treeView->header()->setSectionResizeMode(QHeaderView::Stretch);   //表头自适应内容长度
//    connect(m_treeView, SIGNAL(pressed(const QModelIndex &)), this, SLOT(pressed(const QModelIndex &)));

    setWindowTitle("属性编辑窗");

    m_multipleTopNode = false;
}

FHPropertyWidget::~FHPropertyWidget()
{
}

void FHPropertyWidget::setICallBack(ICallBack_PropertyVary * icalback)
{
    if (m_delegate)
        m_delegate->setICallBack(icalback);
}

void FHPropertyWidget::setCustomItemHeight(int _height)
{ 
    if (m_delegate)
        m_delegate->setCustomItemHeight(_height);
}

FHPropertyItem * FHPropertyWidget::addItemNoTop(QString &name,string &indexName, QVariant &data, PropertyNodeType type)
{
    FHPropertyItem * item = m_model->addItemNoTop(name, indexName, data, type);
    return item;
}

void FHPropertyWidget::flushView()
{
    m_treeView->reset();
    m_treeView->expandAll();						//设置树全部展开
}

void  FHPropertyWidget::clearView()
{
    m_model->clearAll();
}

void FHPropertyWidget::setMultipleTopnode(bool _isMultipleTopNode)
{
    Q_UNUSED(_isMultipleTopNode);
}

FHPropertyItem * FHPropertyWidget::addTopItem(QString &name)
{
    FHPropertyItem * item = m_model->addTopItem(name);
    return	item;
}

FHPropertyItem * FHPropertyWidget::addItem(FHPropertyItem* parentItem, QString &name, string &indexName, QVariant &data, PropertyNodeType type)
{
    FHPropertyItem * item = m_model->addItem(parentItem, name, indexName, data, type);
    return	item;
}
