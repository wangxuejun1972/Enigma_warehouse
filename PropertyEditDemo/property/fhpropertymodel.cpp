#include "fhpropertymodel.h"

#include <QDebug>
#include "fhpropertyitem.h"
///////////////////////////////////////////////////////////////////////
FHPropertyModel::FHPropertyModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    Q_UNUSED(data);
    m_rootNode = new FHPropertyItem(PROPERTY_GBKStr("属性"));
    createPropertyItem(10);
}

FHPropertyModel::~FHPropertyModel()
{
    delete m_rootNode;

    int curUsedCount = m_useItemQueue.count();
    for(int i = 0; i < curUsedCount; i++){
        delete m_useItemQueue.dequeue();
    }

    int curUnUsedCount = m_unUseItemQueue.count();
    for(int i = 0; i < curUnUsedCount; i++){
        delete m_unUseItemQueue.dequeue();
    }
}

void FHPropertyModel::setMultipleTopnode(bool _isMultipleTopNode)
{
    m_multipleTopNode = _isMultipleTopNode;
}

QVariant FHPropertyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    FHPropertyItem *itemData = static_cast<FHPropertyItem*>(index.internalPointer());
    if (index.column() == 0)
        return itemData->nameStr();
    return itemData->value();
}

Qt::ItemFlags FHPropertyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant FHPropertyModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (m_multipleTopNode)
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            switch (section) {
            case 0:
                return PROPERTY_GBKStr("分组属性");
            case 1:
                return PROPERTY_GBKStr("属性值");
            case 2:
                return PROPERTY_GBKStr("ss属性值");
            default:
                return QVariant();
            }
        }
    }
    else
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            switch (section) {
            case 0:
                return PROPERTY_GBKStr("属性名");
            case 1:
                return PROPERTY_GBKStr("属性值");
            default:
                return QVariant();
            }
        }
    }
    return QVariant();
}

QModelIndex FHPropertyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FHPropertyItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootNode;
    else
        parentItem = static_cast<FHPropertyItem*>(parent.internalPointer());

    FHPropertyItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex FHPropertyModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    FHPropertyItem *childItem = static_cast<FHPropertyItem*>(index.internalPointer());
    FHPropertyItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootNode)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FHPropertyModel::rowCount(const QModelIndex &parent) const
{
    FHPropertyItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootNode;
    else
        parentItem = static_cast<FHPropertyItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int FHPropertyModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 2;
    else
        return 2;
}

FHPropertyItem * FHPropertyModel::addItem(FHPropertyItem* parentItem, QString &name, string &indexName, QVariant &value, PropertyNodeType type)
{
    if (parentItem)
    {
        FHPropertyItem* itemdata = getUnUsePropertyItem();
        if(nullptr != itemdata)
        {
            itemdata->setNameStr(name);
            itemdata->setIndexName(indexName);
            itemdata->setValue(value);
            itemdata->setType(type);
            itemdata->setParentItem(parentItem);
        }
        parentItem->appendChild(itemdata);
        return itemdata;
    }
    return NULL;
}

FHPropertyItem * FHPropertyModel::addItemNoTop(QString &name, string &indexName, QVariant &data, PropertyNodeType type)
{
    FHPropertyItem * itemdata = new FHPropertyItem(name, indexName, data, type, m_rootNode);
    m_rootNode->appendChild(itemdata);
    return itemdata;
}

FHPropertyItem * FHPropertyModel::addTopItem(QString &name)
{
    FHPropertyItem * itemdata = getUnUsePropertyItem();
    if(nullptr != itemdata)
    {
        itemdata->setNameStr(name);
        itemdata->setValue(QVariant(""));
        itemdata->setType(TopNodeProperty);
        itemdata->setParentItem(m_rootNode);
        itemdata->setEditEnable(false);
    }

    m_rootNode->appendChild(itemdata);
    m_multipleTopNode = true;
    return itemdata;
}

bool FHPropertyModel::isTopItem(FHPropertyItem *topItem)
{
    bool ret = false;
    if(topItem->parentItem() == m_rootNode){
        ret = true;
    }
    return ret;
}

void FHPropertyModel::clearAll()
{
    m_rootNode->clearSubItem();
    int curCount = m_useItemQueue.count();
    FHPropertyItem* pPropertyItem = nullptr;
    for(int i = 0; i < curCount; i++)
    {
        pPropertyItem = m_useItemQueue.dequeue();
        pPropertyItem->clearSubItem();
        m_unUseItemQueue.enqueue(pPropertyItem);
    }
}

void FHPropertyModel::flushValue(string &indexName, QVariant &data)
{
    Q_UNUSED(data);
    Q_UNUSED(indexName);
    if (m_multipleTopNode)
    {

    }
    else
    {

    }
}

FHPropertyItem* FHPropertyModel::getUnUsePropertyItem()
{
    FHPropertyItem* propertyItem = nullptr;
    if(!m_unUseItemQueue.isEmpty()){
       propertyItem = m_unUseItemQueue.dequeue();
       propertyItem->clearComBoxItem();
    }
    else{
        propertyItem = new FHPropertyItem();
    }
    m_useItemQueue.enqueue(propertyItem);

    return propertyItem;
}

void FHPropertyModel::createPropertyItem(int itemCount)
{
    for(int i = 0; i < itemCount; i++)
    {
        m_unUseItemQueue.enqueue(new FHPropertyItem());
    }
}
