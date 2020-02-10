#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_propertyeditdemo.h"

class FHPropertyWidget;
class QMdiArea;
class QTreeWidgetItem;
class XJModelTree;

/*
	���Ա༭��ʹ����������
	�������࣬����ģ������������Ա���������Ա༭����Ա
*/
class PropertyEditDemo : public QMainWindow
{
	Q_OBJECT

public:
	PropertyEditDemo(QWidget *parent = Q_NULLPTR);
private:
	void init_ui(void);
//protected slots:
	void TreeItemClicked(QTreeWidgetItem* curItem, int _col);
	
private:
	Ui::PropertyEditDemoClass		ui;
	FHPropertyWidget*				m_pPropertyWidget;					//���Ա༭����
	QMdiArea*						m_pWorkspace;						//�ര��ģʽ�������
	
	XJModelTree*					m_pTreeWidget;						//��ģ�ʹ���
};
