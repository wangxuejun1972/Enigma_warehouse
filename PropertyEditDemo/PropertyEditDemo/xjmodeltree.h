#ifndef _H_XJMODELTREE_H__
#define _H_XJMODELTREE_H__

#pragma once
#include <QTreeWidget>
#include <string>
#include <vector>
using std::string;
using std::vector;

#include "../property/fhpropertyitem.h"
#include "../property/fhpropertybasedef.h"
#include "../property/icallback_fhproperty.h"
#include "../property/fhpropertywidget.h"

#include "fh_treemodelbasedef.h"

/*
	@功能：属性编辑窗使用样例配套树窗口代码
		本文件完成 需要进行属性编辑的模型树窗口的层级结点数据结构设计。
		包含模型层级结点，如 区域结点、站结点、一次设备结点、装置层结点【二次设备】。
		本树窗口继承了属性通知回调接口，因此可以直接的与属性编辑框进行编辑互动【属性编辑框编辑对应属性会通过回调反射到本窗口】。
	@author: wangxuejun 
	@date :  2020/1/15
*/

static  const char  *VoltageGradeList[] =
{
	"6KV",
	"10KV",
	"13.8KV",
	"15.75KV",
	"20KV",
	"35KV",
	"66KV",
	"110KV",
	"220KV",
	"330KV",
	"500KV",
	"DC660KV",
	"750KV",
	"DC800KV",
	"1000KV",
};

//属性条目结构定义		--公共结构，所有的属性均可通过这个结构来标识，目前未使用
typedef struct  _PropertyItem
{
	string			propertyName;							//属性名称	
	string			propertyValue;							//属性值
	quint8			propertyType;							//属性数据类型
	string			propertyAlias;							//属性别名
} NPropertyItem, *PTRNPropertyItem;

//结点的层级类型
typedef enum _NODETYPE
{
	unknown_node	= -1,
	area_node		= 0,
	station_node	= 1,
	adevlice_node	= 2,
	equip_node		= 3,
	graph_node		= 4,
}NodeType;

//基本结点属性结构定义
typedef struct _BASE_NODE
{
	quint64			id;										//结点全局编号
	quint64			parent;									//结点父结点编号
	quint8			node_type;								//结点类型
	string			node_name;								//结点名称
	quint16			voltagegrade;							//电压等级编号
	virtual QList<PropertyItemData> getPropertyList() = 0;
	virtual void setComboBoxContent(FHPropertyItem *pItem) {
		if (pItem->indexName().compare(tr_voltage)==0){		//如果是电压等级
			QStringList voltageGradeList;
			voltageGradeList << "6KV" << "10KV" << "13.8KV" << "15.75KV" << "20KV" << "35KV" << "66KV" << "110KV" << "220KV" << "330KV" << "500KV" << "DC660KV" << "750KV" << "DC800KV" << "1000KV" ;
			pItem->setComboxStrList(voltageGradeList);
		}
	}
	//修改属性数据
	virtual bool setMemberData(FHPropertyItem* pItem) {
		if (pItem->indexName().compare(tr_area_name) == 0) {	//修改名称
			node_name = pItem->value().toString().toStdString();
		}
		else if (pItem->indexName().compare(tr_voltage) == 0) {		//电压等级
			voltagegrade = pItem->value().toInt();
		}
		else {
			return false;
		}
		return true;
	}
}BaseNode, *PTRBaseNode;

//区域结点
typedef struct _AREA_NODE_MEMBERS : public  BaseNode
{
	quint8			leavasFlag;								//存在孩子站标志--标识该区域结点下是否存在站结点
	virtual  QList<PropertyItemData> getPropertyList() {
		QList<PropertyItemData> list;

		PropertyItemData area_name;
		area_name.nam = QStringLiteral("区域名称");
		area_name.val = QString(node_name.c_str());
		area_name.indexName = tr_area_name;
		area_name.type = TextProperty;
		list.append(area_name);

		PropertyItemData area_type;
		area_type.nam = QStringLiteral("区域类型");
		area_type.val = node_type;
		area_type.indexName = tr_area_type;
		area_type.type = TextProperty;
		area_type.editFlag = false;
		list.append(area_type);

		PropertyItemData area_id;
		area_id.nam = QStringLiteral("区域编号");
		area_id.val = id;
		area_id.indexName = tr_area_id;
		area_id.type = TextProperty;
		area_id.editFlag = false;
		list.append(area_id);

		PropertyItemData area_parant;
		area_parant.nam = QStringLiteral("父区域编号");
		area_parant.val = parent;
		area_parant.indexName = tr_area_parent;
		area_parant.type = TextProperty;
		area_parant.editFlag = false;
		list.append(area_parant);

		PropertyItemData leavas;
		leavas.nam = QStringLiteral("包含站结点");
		switch (leavasFlag) {
		case 0:
			leavas.val = QStringLiteral("无");
			break;
		case 1:
			leavas.val = QStringLiteral("有");
			break;
		default:
			break;
		}
		leavas.indexName = tr_leavas;
		leavas.type = TextProperty;
		leavas.editFlag = false;
		list.append(leavas);

		PropertyItemData voltageGrade;
		voltageGrade.nam = QStringLiteral("电压等级");
		voltageGrade.val = voltagegrade;
		voltageGrade.indexName = tr_voltage;
		voltageGrade.type = StateProperty;
		voltageGrade.atonceFlg = false;
		list.append(voltageGrade);

		return list;
	}
}AREA_Members,*PTRAREA_Members;

//站结点定义	--对应tb_dev_station_base表单，注释编号对应表单中字段序号
typedef struct _STATION_NODE_MEMBERS : public  BaseNode
{
	quint8			station_type;							//站类型【变电站、换流站、火力发电、水力发电、核能发电电、风能电厂、太阳能电站等】
	quint8			enabledFlag;							//使能标志
	virtual  QList<PropertyItemData> getPropertyList() {
		QList<PropertyItemData> list;

		PropertyItemData station_name;
		station_name.nam = QStringLiteral("站名称");
		station_name.val = QString(node_name.c_str());
		station_name.indexName = tr_station_name;
		station_name.type = TextProperty;
		list.append(station_name);

		PropertyItemData nodetype;
		nodetype.nam = QStringLiteral("结点类型");
		nodetype.val = node_type;
		nodetype.indexName = tr_station_type;
		nodetype.type = TextProperty;
		nodetype.editFlag = false;
		list.append(nodetype);

		PropertyItemData station_id;
		station_id.nam = QStringLiteral("子站编号");
		station_id.val = id;
		station_id.indexName = tr_station_id;
		station_id.type = TextProperty;
		station_id.editFlag = false;
		list.append(station_id);

		PropertyItemData station_parant;
		station_parant.nam = QStringLiteral("父区域编号");
		station_parant.val = parent;
		station_parant.indexName = tr_station_parent;
		station_parant.type = TextProperty;
		station_parant.editFlag = false;
		list.append(station_parant);

		PropertyItemData station_nodetype;
		station_nodetype.nam = QStringLiteral("站类型");
		station_nodetype.val = station_type;
		station_nodetype.indexName = tr_station_type;
		station_nodetype.type = TextProperty;
		station_nodetype.editFlag = false;
		list.append(station_nodetype);

		PropertyItemData enabled;
		enabled.nam = QStringLiteral("使能标志");
		enabled.val = enabledFlag;
		enabled.indexName = tr_enabled;
		enabled.type = BoolProperty;
		enabled.atonceFlg = false;				//设置是否即刻提交【false是不即刻提交】
		list.append(enabled);

		PropertyItemData voltageGrade;
		voltageGrade.nam = QStringLiteral("电压等级");
		voltageGrade.val = voltagegrade;
		voltageGrade.indexName = tr_voltage;
		voltageGrade.type = StateProperty;
		//voltageGrade.atonceFlg = false;				//设置是否即刻提交【false是不即刻提交】
		list.append(voltageGrade);

		return list;
	}
	//修改属性数据
	virtual bool setMemberData(FHPropertyItem* pItem) {
		if (BaseNode::setMemberData(pItem))
			return true;
		if (pItem->indexName().compare(tr_enabled) == 0) {		//使能标志
			enabledFlag = pItem->value().toInt();
		}
		else {
			return false;
		}
		return true;
	}
} STATION_Members, *PTRSTATION_Members;

//一次设备结点定义
typedef struct _ADEVICE_NODE_MEMBERS : public  BaseNode
{
	quint8			adevice_type;							//一次设备类型
	quint16			Breaker_NO;								//开关编号
	quint16         SendFilter;                             //配置过滤
	string          LineFlag;                               //线路标志
	virtual  QList<PropertyItemData> getPropertyList() {
		QList<PropertyItemData> list;

		PropertyItemData adevice_name;
		adevice_name.nam = QStringLiteral("一次设备名称");
		adevice_name.val = QString(node_name.c_str());
		adevice_name.indexName = tr_device_name;
		adevice_name.type = TextProperty;
		list.append(adevice_name);

		PropertyItemData nodetype;
		nodetype.nam = QStringLiteral("结点类型");
		nodetype.val = node_type; 
		nodetype.indexName = tr_device_type;
		nodetype.type = TextProperty;
		nodetype.editFlag = false;
		list.append(nodetype);

		PropertyItemData station_id;
		station_id.nam = QStringLiteral("一次设备编号");
		station_id.val = id;
		station_id.indexName = tr_device_id;
		station_id.type = TextProperty;
		station_id.editFlag = false;
		list.append(station_id);

		PropertyItemData adevice_parant;
		adevice_parant.nam = QStringLiteral("所属站编号");
		adevice_parant.val = parent;
		adevice_parant.indexName = tr_device_parent;
		adevice_parant.type = TextProperty;
		adevice_parant.editFlag = false;
		list.append(adevice_parant);

		PropertyItemData adevice_nodetype;
		adevice_nodetype.nam = QStringLiteral("一次设备类型");		//【刀闸、断路器、电抗、变压器等】
		adevice_nodetype.val = adevice_type;
		adevice_nodetype.indexName = tr_device_type;
		adevice_nodetype.type = TextProperty;
		adevice_nodetype.editFlag = false;
		list.append(adevice_nodetype);

		PropertyItemData voltageGrade;
		voltageGrade.nam = QStringLiteral("电压等级");
		voltageGrade.val = voltagegrade;
		voltageGrade.indexName = tr_voltage;
		voltageGrade.type = StateProperty;
		//voltageGrade.atonceFlg = false;
		list.append(voltageGrade);

		PropertyItemData adevice_breakNo;
		adevice_breakNo.nam = QStringLiteral("断路器编号");		
		adevice_breakNo.val = Breaker_NO;
		adevice_breakNo.indexName = tr_Breaker_no;
		adevice_breakNo.type = TextProperty;
		adevice_breakNo.editFlag = false;
		list.append(adevice_breakNo);

		PropertyItemData FilterFlag;
		FilterFlag.nam = QStringLiteral("调度过滤标志");
		FilterFlag.val = SendFilter;					//【发送过滤，16位，代表针对16路调度主站是否发送过滤】
		FilterFlag.indexName = tr_leavas;
		FilterFlag.type = IntProperty;
		FilterFlag.editFlag = false;
		list.append(FilterFlag);

		return list;
	}
}ADEVICE_Members, *PTRADEVICE_Members;

//二次设备结点定义	--对应tb_secdev_base表单  
typedef struct _EQUIP_NODE_MEMBERS : public  BaseNode
{
	quint8			equipgroup_type;						// 3 装置类型分组类型-- ==
	quint8			equipsub_type;							// 4 装置子类型类型  ==
	quint8			run_status;								// 5 投运状态
	quint8			commcard_id;							// 6 通信卡编号
	quint8			leadcomm_id;							// 7 前置通信程序模块编号
	quint8			commtype_id;							// 8 通信链路类型编号
	quint8			comprotocald;							// 9 规约库编号
	quint8			manufacturer_id;						// 10 生产厂家编号

	quint32			checkinterval;							// 12 检查设备时间间隔
	quint32			settinginterval;						// 13 定值巡检时间间隔
	quint32			sampleinterval;							// 14 采样值巡检时间间隔
	quint32			switchinterval;							// 15 开关量巡检时间间隔
	quint32			curvetableinterval;						// 16 扰动表巡检时间间隔
	quint32			gpspairinterval;						// 17 gsp对时巡检间隔
	quint16			sendfilterlevel;						// 18 转发主站过滤级别
	quint64			softmaketime;							// 19 软件设计实现时间
	quint32			settingverify_interval;					// 23定值自动校核间隔

	quint8			current_channel;						// 11 当前使用通道【主、备】
	
	string			EQUIPVersion;							// 20 装置软件版本
	string			EQUIPVerify;							// 21 装置软件校验码
	virtual  QList<PropertyItemData> getPropertyList() {
		QList<PropertyItemData> list;

		return list;
	}
}Equip_Members, *PTREquip_Members;

//图形节点
typedef struct _DRAW_NODE_MEMBERS : public  BaseNode
{
	quint8          drawing_type;                            //画面类型【地理图，主接线图、状态图、模拟量图、设备工况图】
	int				sceneHeight;
	int				sceneWidth;
	bool			imageValidate;
	QColor			backColor;
	virtual  QList<PropertyItemData> getPropertyList() {
		QList<PropertyItemData> list;

		return list;
	}
} Draw_Members, *PTRDraw_Members;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CustomTreeItem : public QTreeWidgetItem
{
public:
	CustomTreeItem(QTreeWidgetItem *_parent=NULL, NodeType _nodetype= unknown_node);
	CustomTreeItem(QTreeWidget *_treeview);
	virtual ~CustomTreeItem();
public:
	//获取树结点的结点类型
	NodeType getNodeType() { return m_NodeType; }
	
	//设置树结点的结点数据
	void setNodeData(PTRBaseNode _pNodedat) { m_pNodeData = _pNodedat;	}
	
	//设置状态类型属性编辑时需要使用的下拉框条目内容
	void setComboBoxContent(FHPropertyItem* pItem) {
		if (m_pNodeData)
			m_pNodeData->setComboBoxContent(pItem);
	}

	//获取结点属性列表
	QList<PropertyItemData> getPropertyList();

	//设置【修改】树结点的结点属性的值
	bool setMemberData(FHPropertyItem* pItem) {
		if (pItem->indexName().compare(tr_area_name) == 0) {		//修改名称
			this->setText(0, pItem->value().toString());
		}
		else if (pItem->indexName().compare(tr_voltage) == 0) {		//电压等级
			this->setText(1, VoltageGradeList[pItem->value().toInt()]);
		}
		else if (pItem->indexName().compare(tr_enabled) == 0) {		//使能标志
			this->setBackgroundColor(0, pItem->value().toInt() ? QColor(0, 255,0): QColor(255, 0, 0));
		}
		if (m_pNodeData)
			return m_pNodeData->setMemberData(pItem);
		return false;
	}
private:
	NodeType			m_NodeType;							//结点类型
	PTRBaseNode			m_pNodeData;						//结点数据
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FHPropertyWidget;
class XJModelTree : public QTreeWidget , public ICallBack_PropertyVary
{
	Q_OBJECT
public:
	XJModelTree(QWidget *parent=NULL);
	~XJModelTree();
public:
	void init_Test();
	void setCurSelItem(CustomTreeItem* _selItem) {
		m_pCurSelItem = _selItem;
	}
	//将自己设置到属性框中，以便于在属性被编辑后的反射回调调用
	void setToCallBack(FHPropertyWidget* _pPropertyWidget)
	{
		if (_pPropertyWidget)
		{
			_pPropertyWidget->setICallBack(this);
		}
	}
	//回调反射处理器
	virtual void operatorProperty(FHPropertyItem * itemData) {

		if (m_pCurSelItem)
			m_pCurSelItem->setMemberData(itemData);
	}
protected:
	void mouseReleaseEvent(QMouseEvent * e);
private:
	CustomTreeItem*					m_pCurSelItem;

};





#endif  //_H_XJMODELTREE_H__