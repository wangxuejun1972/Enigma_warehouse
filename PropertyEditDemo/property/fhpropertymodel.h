#ifndef FHPROPERTYMODEL_H
#define FHPROPERTYMODEL_H
#include <QQueue>
#include <QAbstractItemModel>

#include "fhpropertybasedef.h"
//属性树视图 数据模型处理类
class FHPropertyItem;
class FHPropertyModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	FHPropertyModel(const QString &data, QObject *parent = 0);
	~FHPropertyModel();
public:
	QVariant data(const QModelIndex &index, int role) const;			//返回指定索引的数据
	Qt::ItemFlags flags(const QModelIndex &index) const;				//返回指定索引的项的操作状态

	QVariant headerData(int section, Qt::Orientation orientation,		//返回显示列名称
		int role = Qt::DisplayRole) const;

	QModelIndex index(int row, int column,								//返回指定行列条目索引
		const QModelIndex &parent = QModelIndex()) const;

	QModelIndex parent(const QModelIndex &index) const;					//返回指定索引条目的父条目

	int rowCount(const QModelIndex &parent = QModelIndex()) const;		//返回行数
	int columnCount(const QModelIndex &parent = QModelIndex()) const;	//返回列数

	//增加属性条目
	FHPropertyItem * addItem(FHPropertyItem* parentItem, QString &name, string &indexName, QVariant &data, PropertyNodeType type);
	//没有顶层结点的增加条目方式
	FHPropertyItem * addItemNoTop(QString &name, string &indexName, QVariant &data, PropertyNodeType type);//不使用多层增加属性条目
	//增加顶层条目
	FHPropertyItem * addTopItem(QString &name);

    bool isTopItem(FHPropertyItem* topItem);

	//外部刷新全部属性值
        //void flushValues();

	//外部更新指定属性值
	void flushValue(string &indexName, QVariant &data);

	//清除模型中全部条目
	void clearAll();													

	//设置是否是多顶层结点模式
	void setMultipleTopnode(bool _isMultipleTopNode);

private:
    void createPropertyItem(int itemCount);
    FHPropertyItem* getUnUsePropertyItem();

private:
	FHPropertyItem									*m_rootNode;						//模型的根结点
    bool                                            m_multipleTopNode;					//属性框是否为多顶层结点方式【存在多个顶层结点+每个顶层结点的孩子】，单顶层结点则默认不会显示顶层结点
    QQueue<FHPropertyItem*>                         m_useItemQueue;
    QQueue<FHPropertyItem*>                         m_unUseItemQueue;
};

#endif // FHPROPERTYMODEL_H
