#ifndef _H_FHPROPERTYTREEVIEW_H__
#define _H_FHPROPERTYTREEVIEW_H__

#include <QTreeView>
#include <QModelIndex>
#pragma once
class FHPropertyDelegate;

// Ù–‘±‡º≠ ˜ ”Õº
class FHPropertyTreeView : public QTreeView
{
	Q_OBJECT
public:
	FHPropertyTreeView(QWidget *parent = 0);
	~FHPropertyTreeView();
public:
	void setRecItemDelegate(FHPropertyDelegate	* ItemDelegate) { m_itemDelegate = ItemDelegate; }
protected:
	void currentChanged(const QModelIndex &current, const QModelIndex &previous);
protected slots:

signals:
	void clicked(const QModelIndex & index);
private:
	FHPropertyDelegate			*m_itemDelegate;
	QModelIndex					m_current;
	QMouseEvent					*m_event;
};

#endif //_H_FHPROPERTYTREEVIEW_H__