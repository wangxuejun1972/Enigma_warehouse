#include "fhpropertyitem.h"
#include <QDebug>
///////////////////////////////////////////////////////////////////////
//==========================================================================================================
FHPropertyItem::FHPropertyItem(const QString &name, const string &indexName, QVariant &value, PropertyNodeType itemType, FHPropertyItem *parentItem)
    : m_name(name), m_indexName(indexName), m_value(value), m_itemType(itemType), m_pParentItem(parentItem),m_bAtonce (true)
{
    if (m_itemType == TopNodeProperty){
        m_isTopItem = true;
        m_editFlag = false;
    }
    else{
        m_isTopItem = false;
        m_editFlag = true;
    }
}

FHPropertyItem::~FHPropertyItem()
{
//    for(int i = 0; i < m_childItems.count(); i++) //内存不在item内部释放，外部统一释放
//    {
//        delete m_childItems.at(i);
//    }
}

void FHPropertyItem::setType(PropertyNodeType type)
{
    m_itemType = type;
    QStringList strList;
    switch (m_itemType)
    {
    case BoolProperty:
    {
        strList << PROPERTY_GBKStr("否") << PROPERTY_GBKStr("是");
    }break;
    case LineTypeProperty:
    {
        strList << "___________" << "_ _ _ _ _ _" << "..........."
                << "_._._._._._." << "_.._.._.._..";
    }break;
    default:{
        return;
    }
    }
	m_comboxItems.clear();
    for(int i = 0; i < strList.count(); i++){
        ComboBoxItem item;
        item.text = strList.at(i);
		m_comboxItems.append(item);
    }
}

void FHPropertyItem::appendChild(FHPropertyItem *item)
{
    m_childItems.append(item);
}

FHPropertyItem *FHPropertyItem::child(int row)
{
    return m_childItems.value(row);
}

int FHPropertyItem::childCount() const
{
    return m_childItems.count();
}

void FHPropertyItem::clearSubItem()
{
    if(0 != m_childItems.count())
        m_childItems.clear();
}

void FHPropertyItem::clearComBoxItem()
{
	m_comboxItems.clear();
}

int FHPropertyItem::row() const
{
    if (m_pParentItem)
        return m_pParentItem->m_childItems.indexOf(const_cast<FHPropertyItem*>(this));
    return 0;
}

void FHPropertyItem::setValue(QVariant &data)
{
    m_value = data;
}

void FHPropertyItem::setChildValue(QString &name, QVariant &data)
{
    QString  str;
    for (int i = 0; i<m_childItems.count(); i++)
    {
        str = m_childItems.at(i)->nameStr();
        if (str.compare(name) == 0)
        {
            m_childItems.at(i)->setValue(data);
            return;
        }
    }
}

void FHPropertyItem::setComboxItmList(const QList<ComboBoxItem>& comboxStrList)
{
	m_comboxItems.clear();
	m_comboxItems = comboxStrList;
}

void FHPropertyItem::setComboxStrList(const QList<QString> &strlist)
{
	m_comboxItems.clear();
	QList<ComboBoxItem> comboxStrList;
	for (int i = 0; i < strlist.count(); i++)
	{
		ComboBoxItem item;
		item.text = strlist.at(i);
		item.enabled = true;
		m_comboxItems.append(item);
	}
}

void FHPropertyItem::setIntRange(int down, int up)
{
    m_range.setType(0);
    m_range.setDown_int(down);
    m_range.setUp_int(up);
}

void FHPropertyItem::setDoubleRange(double down, double up)
{
    m_range.setType(1);
    m_range.setDown_real(down);
    m_range.setUp_real(up);
}
