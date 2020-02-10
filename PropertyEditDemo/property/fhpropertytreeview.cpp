#include "fhpropertytreeview.h"
#include "fhpropertydelegate.h"
///////////////////////////////////////////////////////////////////////
FHPropertyTreeView::FHPropertyTreeView(QWidget *parent)
	: QTreeView(parent)
{
	m_itemDelegate = NULL;
	m_event = NULL;
}

FHPropertyTreeView::~FHPropertyTreeView()
{
}

void FHPropertyTreeView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);
	m_current = current;
	if (m_itemDelegate)
		m_itemDelegate->setCurrentItem(m_current);
}
///////////////////////////////////////////////////////////////////////