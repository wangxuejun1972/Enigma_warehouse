#ifndef H_FHPROPERTYWIDGET_H__
#define H_FHPROPERTYWIDGET_H__

#pragma once
#include <QWidget>
#include <QItemDelegate>
#include <QModelIndex>
#include "fhpropertybasedef.h"

class FHPropertyItem;
class ICallBack_PropertyVary;
class FHPropertyModel;
class FHPropertyDelegate;
class FHPropertyTreeView;

//外部使用的属性编辑窗口 -- 动态库需要导出
class FHPropertyWidget : public QWidget
{
	Q_OBJECT
public:
	FHPropertyWidget(QWidget *parent = 0);
	~FHPropertyWidget();
public:
	//设置策略对象
	void setICallBack(ICallBack_PropertyVary * icalback);//策略对象-这里由策略对象提供内部变化到外部的操纵接口 
	
	//增加顶层条目
	FHPropertyItem * addTopItem(QString &name);
	//增加属性条目--不使用顶层结点
	FHPropertyItem * addItemNoTop(QString &name, string &indexName, QVariant &data, PropertyNodeType type);
	//增加属性条目--按有顶层结点使用
	FHPropertyItem * addItem(FHPropertyItem* parentItem, QString &name, string &indexName, QVariant &data, PropertyNodeType type);

	//刷新属性view窗口
	void flushView();
	//清空视图上属性条目
	void clearView();
	//设置条目项的高度
	void setCustomItemHeight(int _height);
public:					
	//设置是否是多顶层结点模式
	void setMultipleTopnode(bool _isMultipleTopNode);

private:
	FHPropertyTreeView			*m_treeView;						//属性编辑树视图窗口
	FHPropertyModel				*m_model;							//属性编辑模型
	FHPropertyDelegate			*m_delegate;						//属性编辑树代理对象
    bool						m_multipleTopNode;					//属性框是否为多顶层结点方式【存在多个顶层结点+每个顶层结点的孩子】，单顶层结点则默认不会显示顶层结点
};

#endif	//H_FHPROPERTYWIDGET_H__
