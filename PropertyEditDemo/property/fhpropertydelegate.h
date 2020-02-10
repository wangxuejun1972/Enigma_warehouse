#ifndef FHPROPERTYDELEGATE_H
#define FHPROPERTYDELEGATE_H

#include <QItemDelegate>
#include <QStyledItemDelegate>

class FHPropertyModel;
class ICallBack_PropertyVary;

//属性编辑代理
class FHPropertyDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	FHPropertyDelegate(QWidget *parent = 0);
	~FHPropertyDelegate();
public:
	//设置策略对象
	void setICallBack(ICallBack_PropertyVary * icalback);		//策略对象-这里由策略对象提供内部变化到外部的操纵接口 

	//设置模型
	void setModel(FHPropertyModel *model);

	//设置指定条目为当前条目
	void setCurrentItem(QModelIndex & index) { m_currentIndex = index; }

	//设置条目项的高度
	void setCustomItemHeight(int _height) { m_itemHeight = _height; }

	//设置是否是多顶层结点模式
	void setMultipleTopnode(bool _isMultipleTopNode);

protected:
	//绘制条目
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
	//设置编辑数据
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	//设置模型数据
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	//更新widget的大小
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	//建立编辑属性的widget
	QWidget *createEditor(QWidget *parent,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const;
	//设置条目的高度
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		QSize size = QStyledItemDelegate::sizeHint(option, index);
		return QSize(size.width(), m_itemHeight);
	}

//2019/12/02 去掉槽函数，在setModel中改为直接调用
/* private slots :
	//列表框变化
	void currentIndexChanged(int index);
	//文本编辑完成
	void textEditFinished();
	//spinbox完成
	void intEditFinished();
	//double spinbox 完成
	void doubleEditFinished();
    //datetime编辑完成
    void dateTimeEditFinished();
*/

private:
        ICallBack_PropertyVary*							m_ICallBack;							//回调策略对象
        QModelIndex										m_currentIndex;							//当前条目索引
        FHPropertyModel*								m_model;								//编辑操作针对的数据模型
        int												m_itemHeight;							//条目的高度
};

#endif // FHPROPERTYDELEGATE_H
