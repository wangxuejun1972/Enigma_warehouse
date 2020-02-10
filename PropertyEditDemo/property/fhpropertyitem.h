#ifndef H_FHPROPERTYITEMDATA_H__
#define H_FHPROPERTYITEMDATA_H__

#pragma once

#include <limits>
#include <QVariant>
#include <string>

#include "fhrange.h"

#include "fhpropertybasedef.h"

using std::string;

//属性条目数据   -- 动态库需要导出
class  FHPropertyItem
{
public:
        FHPropertyItem(const QString &name = "", const string &indexName = "", QVariant &value = QVariant(), PropertyNodeType type = UnknownProperty, FHPropertyItem *parentItem = nullptr);
        ~FHPropertyItem();
public:
        PropertyNodeType type() { return m_itemType; }                                          //返回条目数据类型
        void setType(PropertyNodeType type);

		QString  nameStr() { return m_name; }													//返回属性名称【中文】
        void setNameStr(const QString& name) { m_name = name; }

        QVariant value() { return m_value; }													//返回数据
        void setValue(QVariant &data);															//设置数据
        void setChildValue(QString &name, QVariant &data);                                      //设置孩子节点的数据

		string indexName() { return m_indexName; }												//获取索引名称--用于快速查询【英文】
        void setIndexName(const string& indexName) { m_indexName = indexName; }

        FHPropertyItem* parentItem() {return m_pParentItem;}                                    //返回父节点
        void setParentItem(FHPropertyItem* pParentItem){ m_pParentItem = pParentItem;}

        const QList<ComboBoxItem>& comboxItmList() { return m_comboxItems; }                     //获取下拉框内容条目
        void setComboxItmList(const QList<ComboBoxItem> &comboxStrList);                        //设置下拉框内容条目
		void setComboxStrList( const QList<QString> &strlist);									//这个是按字符串列表直接设置

        bool isEditEnable() { return m_editFlag; }                                              //返回编辑标志
        void setEditEnable(bool flag) { m_editFlag = flag; }                                    //设置编辑标志

        void setIntRange(int down, int up);
        void setDoubleRange(double down, double up);
        FHRange* mRange() { return &m_range; }													//获取本属性的值范围--当本属性是属于数字类型的时候有效

        int row() const;																		//返回自己在父节点中的行
		void appendChild(FHPropertyItem *child);												//加入孩子节点
        FHPropertyItem *child(int row);															//返回指定行孩子节点
        int childCount() const;																	//返回孩子个数
        void clearSubItem();                                                                    //清除所有孩子
        void clearComBoxItem();

		//[wxj]2020/2/9
		void setAtonceVal(bool _flg) { m_bAtonce = _flg; }
		bool atonceVal() { return m_bAtonce; }
		
private:
        FHPropertyItem*                 m_pParentItem;			//父节点
        QString							m_name;					//属性的名字
        string							m_indexName;			//设置属性时的查找名称
        QVariant						m_value;				//属性值
        PropertyNodeType                m_itemType;				//属性条目类型
        bool							m_editFlag;				//编辑标志 1可编辑，0不可编辑
        QList<FHPropertyItem*>          m_childItems;			//本属性包含的孩子节点列表
        FHRange							m_range;				//值范围
        QList<ComboBoxItem>				m_comboxItems;			//如果是状态属性，这个list用来保存特定的下拉框字符串集
        bool							m_isTopItem;			//本结点是否为顶层结点
		bool							m_bAtonce;				//在属性被修改后立即提交的标志[wxj]2020/2/9
};

#endif	//H_FHPROPERTYITEMDATA_H__
