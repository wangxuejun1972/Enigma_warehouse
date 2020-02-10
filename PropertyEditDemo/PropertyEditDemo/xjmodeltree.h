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
	@���ܣ����Ա༭��ʹ���������������ڴ���
		���ļ���� ��Ҫ�������Ա༭��ģ�������ڵĲ㼶������ݽṹ��ơ�
		����ģ�Ͳ㼶��㣬�� �����㡢վ��㡢һ���豸��㡢װ�ò��㡾�����豸����
		�������ڼ̳�������֪ͨ�ص��ӿڣ���˿���ֱ�ӵ������Ա༭����б༭���������Ա༭��༭��Ӧ���Ի�ͨ���ص����䵽�����ڡ���
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

//������Ŀ�ṹ����		--�����ṹ�����е����Ծ���ͨ������ṹ����ʶ��Ŀǰδʹ��
typedef struct  _PropertyItem
{
	string			propertyName;							//��������	
	string			propertyValue;							//����ֵ
	quint8			propertyType;							//������������
	string			propertyAlias;							//���Ա���
} NPropertyItem, *PTRNPropertyItem;

//���Ĳ㼶����
typedef enum _NODETYPE
{
	unknown_node	= -1,
	area_node		= 0,
	station_node	= 1,
	adevlice_node	= 2,
	equip_node		= 3,
	graph_node		= 4,
}NodeType;

//����������Խṹ����
typedef struct _BASE_NODE
{
	quint64			id;										//���ȫ�ֱ��
	quint64			parent;									//��㸸�����
	quint8			node_type;								//�������
	string			node_name;								//�������
	quint16			voltagegrade;							//��ѹ�ȼ����
	virtual QList<PropertyItemData> getPropertyList() = 0;
	virtual void setComboBoxContent(FHPropertyItem *pItem) {
		if (pItem->indexName().compare(tr_voltage)==0){		//����ǵ�ѹ�ȼ�
			QStringList voltageGradeList;
			voltageGradeList << "6KV" << "10KV" << "13.8KV" << "15.75KV" << "20KV" << "35KV" << "66KV" << "110KV" << "220KV" << "330KV" << "500KV" << "DC660KV" << "750KV" << "DC800KV" << "1000KV" ;
			pItem->setComboxStrList(voltageGradeList);
		}
	}
	//�޸���������
	virtual bool setMemberData(FHPropertyItem* pItem) {
		if (pItem->indexName().compare(tr_area_name) == 0) {	//�޸�����
			node_name = pItem->value().toString().toStdString();
		}
		else if (pItem->indexName().compare(tr_voltage) == 0) {		//��ѹ�ȼ�
			voltagegrade = pItem->value().toInt();
		}
		else {
			return false;
		}
		return true;
	}
}BaseNode, *PTRBaseNode;

//������
typedef struct _AREA_NODE_MEMBERS : public  BaseNode
{
	quint8			leavasFlag;								//���ں���վ��־--��ʶ�����������Ƿ����վ���
	virtual  QList<PropertyItemData> getPropertyList() {
		QList<PropertyItemData> list;

		PropertyItemData area_name;
		area_name.nam = QStringLiteral("��������");
		area_name.val = QString(node_name.c_str());
		area_name.indexName = tr_area_name;
		area_name.type = TextProperty;
		list.append(area_name);

		PropertyItemData area_type;
		area_type.nam = QStringLiteral("��������");
		area_type.val = node_type;
		area_type.indexName = tr_area_type;
		area_type.type = TextProperty;
		area_type.editFlag = false;
		list.append(area_type);

		PropertyItemData area_id;
		area_id.nam = QStringLiteral("������");
		area_id.val = id;
		area_id.indexName = tr_area_id;
		area_id.type = TextProperty;
		area_id.editFlag = false;
		list.append(area_id);

		PropertyItemData area_parant;
		area_parant.nam = QStringLiteral("��������");
		area_parant.val = parent;
		area_parant.indexName = tr_area_parent;
		area_parant.type = TextProperty;
		area_parant.editFlag = false;
		list.append(area_parant);

		PropertyItemData leavas;
		leavas.nam = QStringLiteral("����վ���");
		switch (leavasFlag) {
		case 0:
			leavas.val = QStringLiteral("��");
			break;
		case 1:
			leavas.val = QStringLiteral("��");
			break;
		default:
			break;
		}
		leavas.indexName = tr_leavas;
		leavas.type = TextProperty;
		leavas.editFlag = false;
		list.append(leavas);

		PropertyItemData voltageGrade;
		voltageGrade.nam = QStringLiteral("��ѹ�ȼ�");
		voltageGrade.val = voltagegrade;
		voltageGrade.indexName = tr_voltage;
		voltageGrade.type = StateProperty;
		voltageGrade.atonceFlg = false;
		list.append(voltageGrade);

		return list;
	}
}AREA_Members,*PTRAREA_Members;

//վ��㶨��	--��Ӧtb_dev_station_base����ע�ͱ�Ŷ�Ӧ�����ֶ����
typedef struct _STATION_NODE_MEMBERS : public  BaseNode
{
	quint8			station_type;							//վ���͡����վ������վ���������硢ˮ�����硢���ܷ���硢���ܵ糧��̫���ܵ�վ�ȡ�
	quint8			enabledFlag;							//ʹ�ܱ�־
	virtual  QList<PropertyItemData> getPropertyList() {
		QList<PropertyItemData> list;

		PropertyItemData station_name;
		station_name.nam = QStringLiteral("վ����");
		station_name.val = QString(node_name.c_str());
		station_name.indexName = tr_station_name;
		station_name.type = TextProperty;
		list.append(station_name);

		PropertyItemData nodetype;
		nodetype.nam = QStringLiteral("�������");
		nodetype.val = node_type;
		nodetype.indexName = tr_station_type;
		nodetype.type = TextProperty;
		nodetype.editFlag = false;
		list.append(nodetype);

		PropertyItemData station_id;
		station_id.nam = QStringLiteral("��վ���");
		station_id.val = id;
		station_id.indexName = tr_station_id;
		station_id.type = TextProperty;
		station_id.editFlag = false;
		list.append(station_id);

		PropertyItemData station_parant;
		station_parant.nam = QStringLiteral("��������");
		station_parant.val = parent;
		station_parant.indexName = tr_station_parent;
		station_parant.type = TextProperty;
		station_parant.editFlag = false;
		list.append(station_parant);

		PropertyItemData station_nodetype;
		station_nodetype.nam = QStringLiteral("վ����");
		station_nodetype.val = station_type;
		station_nodetype.indexName = tr_station_type;
		station_nodetype.type = TextProperty;
		station_nodetype.editFlag = false;
		list.append(station_nodetype);

		PropertyItemData enabled;
		enabled.nam = QStringLiteral("ʹ�ܱ�־");
		enabled.val = enabledFlag;
		enabled.indexName = tr_enabled;
		enabled.type = BoolProperty;
		enabled.atonceFlg = false;				//�����Ƿ񼴿��ύ��false�ǲ������ύ��
		list.append(enabled);

		PropertyItemData voltageGrade;
		voltageGrade.nam = QStringLiteral("��ѹ�ȼ�");
		voltageGrade.val = voltagegrade;
		voltageGrade.indexName = tr_voltage;
		voltageGrade.type = StateProperty;
		//voltageGrade.atonceFlg = false;				//�����Ƿ񼴿��ύ��false�ǲ������ύ��
		list.append(voltageGrade);

		return list;
	}
	//�޸���������
	virtual bool setMemberData(FHPropertyItem* pItem) {
		if (BaseNode::setMemberData(pItem))
			return true;
		if (pItem->indexName().compare(tr_enabled) == 0) {		//ʹ�ܱ�־
			enabledFlag = pItem->value().toInt();
		}
		else {
			return false;
		}
		return true;
	}
} STATION_Members, *PTRSTATION_Members;

//һ���豸��㶨��
typedef struct _ADEVICE_NODE_MEMBERS : public  BaseNode
{
	quint8			adevice_type;							//һ���豸����
	quint16			Breaker_NO;								//���ر��
	quint16         SendFilter;                             //���ù���
	string          LineFlag;                               //��·��־
	virtual  QList<PropertyItemData> getPropertyList() {
		QList<PropertyItemData> list;

		PropertyItemData adevice_name;
		adevice_name.nam = QStringLiteral("һ���豸����");
		adevice_name.val = QString(node_name.c_str());
		adevice_name.indexName = tr_device_name;
		adevice_name.type = TextProperty;
		list.append(adevice_name);

		PropertyItemData nodetype;
		nodetype.nam = QStringLiteral("�������");
		nodetype.val = node_type; 
		nodetype.indexName = tr_device_type;
		nodetype.type = TextProperty;
		nodetype.editFlag = false;
		list.append(nodetype);

		PropertyItemData station_id;
		station_id.nam = QStringLiteral("һ���豸���");
		station_id.val = id;
		station_id.indexName = tr_device_id;
		station_id.type = TextProperty;
		station_id.editFlag = false;
		list.append(station_id);

		PropertyItemData adevice_parant;
		adevice_parant.nam = QStringLiteral("����վ���");
		adevice_parant.val = parent;
		adevice_parant.indexName = tr_device_parent;
		adevice_parant.type = TextProperty;
		adevice_parant.editFlag = false;
		list.append(adevice_parant);

		PropertyItemData adevice_nodetype;
		adevice_nodetype.nam = QStringLiteral("һ���豸����");		//����բ����·�����翹����ѹ���ȡ�
		adevice_nodetype.val = adevice_type;
		adevice_nodetype.indexName = tr_device_type;
		adevice_nodetype.type = TextProperty;
		adevice_nodetype.editFlag = false;
		list.append(adevice_nodetype);

		PropertyItemData voltageGrade;
		voltageGrade.nam = QStringLiteral("��ѹ�ȼ�");
		voltageGrade.val = voltagegrade;
		voltageGrade.indexName = tr_voltage;
		voltageGrade.type = StateProperty;
		//voltageGrade.atonceFlg = false;
		list.append(voltageGrade);

		PropertyItemData adevice_breakNo;
		adevice_breakNo.nam = QStringLiteral("��·�����");		
		adevice_breakNo.val = Breaker_NO;
		adevice_breakNo.indexName = tr_Breaker_no;
		adevice_breakNo.type = TextProperty;
		adevice_breakNo.editFlag = false;
		list.append(adevice_breakNo);

		PropertyItemData FilterFlag;
		FilterFlag.nam = QStringLiteral("���ȹ��˱�־");
		FilterFlag.val = SendFilter;					//�����͹��ˣ�16λ���������16·������վ�Ƿ��͹��ˡ�
		FilterFlag.indexName = tr_leavas;
		FilterFlag.type = IntProperty;
		FilterFlag.editFlag = false;
		list.append(FilterFlag);

		return list;
	}
}ADEVICE_Members, *PTRADEVICE_Members;

//�����豸��㶨��	--��Ӧtb_secdev_base��  
typedef struct _EQUIP_NODE_MEMBERS : public  BaseNode
{
	quint8			equipgroup_type;						// 3 װ�����ͷ�������-- ==
	quint8			equipsub_type;							// 4 װ������������  ==
	quint8			run_status;								// 5 Ͷ��״̬
	quint8			commcard_id;							// 6 ͨ�ſ����
	quint8			leadcomm_id;							// 7 ǰ��ͨ�ų���ģ����
	quint8			commtype_id;							// 8 ͨ����·���ͱ��
	quint8			comprotocald;							// 9 ��Լ����
	quint8			manufacturer_id;						// 10 �������ұ��

	quint32			checkinterval;							// 12 ����豸ʱ����
	quint32			settinginterval;						// 13 ��ֵѲ��ʱ����
	quint32			sampleinterval;							// 14 ����ֵѲ��ʱ����
	quint32			switchinterval;							// 15 ������Ѳ��ʱ����
	quint32			curvetableinterval;						// 16 �Ŷ���Ѳ��ʱ����
	quint32			gpspairinterval;						// 17 gsp��ʱѲ����
	quint16			sendfilterlevel;						// 18 ת����վ���˼���
	quint64			softmaketime;							// 19 ������ʵ��ʱ��
	quint32			settingverify_interval;					// 23��ֵ�Զ�У�˼��

	quint8			current_channel;						// 11 ��ǰʹ��ͨ������������
	
	string			EQUIPVersion;							// 20 װ������汾
	string			EQUIPVerify;							// 21 װ�����У����
	virtual  QList<PropertyItemData> getPropertyList() {
		QList<PropertyItemData> list;

		return list;
	}
}Equip_Members, *PTREquip_Members;

//ͼ�νڵ�
typedef struct _DRAW_NODE_MEMBERS : public  BaseNode
{
	quint8          drawing_type;                            //�������͡�����ͼ��������ͼ��״̬ͼ��ģ����ͼ���豸����ͼ��
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
	//��ȡ�����Ľ������
	NodeType getNodeType() { return m_NodeType; }
	
	//���������Ľ������
	void setNodeData(PTRBaseNode _pNodedat) { m_pNodeData = _pNodedat;	}
	
	//����״̬�������Ա༭ʱ��Ҫʹ�õ���������Ŀ����
	void setComboBoxContent(FHPropertyItem* pItem) {
		if (m_pNodeData)
			m_pNodeData->setComboBoxContent(pItem);
	}

	//��ȡ��������б�
	QList<PropertyItemData> getPropertyList();

	//���á��޸ġ������Ľ�����Ե�ֵ
	bool setMemberData(FHPropertyItem* pItem) {
		if (pItem->indexName().compare(tr_area_name) == 0) {		//�޸�����
			this->setText(0, pItem->value().toString());
		}
		else if (pItem->indexName().compare(tr_voltage) == 0) {		//��ѹ�ȼ�
			this->setText(1, VoltageGradeList[pItem->value().toInt()]);
		}
		else if (pItem->indexName().compare(tr_enabled) == 0) {		//ʹ�ܱ�־
			this->setBackgroundColor(0, pItem->value().toInt() ? QColor(0, 255,0): QColor(255, 0, 0));
		}
		if (m_pNodeData)
			return m_pNodeData->setMemberData(pItem);
		return false;
	}
private:
	NodeType			m_NodeType;							//�������
	PTRBaseNode			m_pNodeData;						//�������
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
	//���Լ����õ����Կ��У��Ա��������Ա��༭��ķ���ص�����
	void setToCallBack(FHPropertyWidget* _pPropertyWidget)
	{
		if (_pPropertyWidget)
		{
			_pPropertyWidget->setICallBack(this);
		}
	}
	//�ص����䴦����
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