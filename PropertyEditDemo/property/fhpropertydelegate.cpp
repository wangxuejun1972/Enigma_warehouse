#include "fhpropertydelegate.h"
#include <QPainter>
#include <QPainter>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDateTimeEdit>
#include "fhpropertyitem.h"
#include "icallback_fhproperty.h"
#include "fhpropertymodel.h"

///////////////////////////////////////////////////////////////////////
FHPropertyDelegate::FHPropertyDelegate(QWidget *parent)
    :QStyledItemDelegate(parent), m_ICallBack(NULL), m_itemHeight(18)
{
}

FHPropertyDelegate::~FHPropertyDelegate()
{
}

void FHPropertyDelegate::setICallBack(ICallBack_PropertyVary * icalback)
{
    m_ICallBack = icalback;
}

//根据实际的属性值 绘制属性框的第一列
void FHPropertyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    FHPropertyItem *selItem = static_cast<FHPropertyItem*>(index.internalPointer());
    if (index.column() == 0)
    {
        painter->setPen(QColor(0, 0, 0));
        QStyledItemDelegate::paint(painter, option, index);
    }
    else if (index.column() == 1)		//第1列
    {
        PropertyNodeType		typeVal = selItem->type();
        switch (typeVal)
        {
        case ColorProperty:			//颜色类型属性
        {
            painter->setPen(selItem->value().value<QColor>());
            painter->setBrush(selItem->value().value<QColor>());
            painter->drawRect(option.rect);
        }break;
        case BoolProperty:			//布尔类型
        {
            painter->setPen(QColor(0, 0, 0));
            int indx = index.model()->data(index, Qt::DisplayRole).toInt();
            /*QRectF rt = option.rect;
                        int height = option.rect.height();
                        if (m_itemHeight > height)
                        {
                                int yoff = m_itemHeight - height;
                                rt.setTop(rt.top()-10);
                        }*/
            if (indx == 1)
            {
				QTextOption textOption;
				textOption.setAlignment(Qt::AlignVCenter);
				QRect rect(option.rect.x() + 5, option.rect.y(), option.rect.width(), option.rect.height());    //填写范围矩形
                painter->drawText(rect, PROPERTY_GBKStr("是"), textOption);
            }
            else
            {
				QTextOption textOption;
				textOption.setAlignment(Qt::AlignVCenter);
				QRect rect(option.rect.x() + 5, option.rect.y(), option.rect.width(), option.rect.height());    //填写范围矩形
                painter->drawText(rect, PROPERTY_GBKStr("否"), textOption);
            }
        }break;
        case LineTypeProperty:		//画笔线类型
        {
            painter->setPen(QColor(0, 0, 0));
            int indx = index.model()->data(index, Qt::DisplayRole).toInt();

			QRect rect(option.rect.x() + 5, option.rect.y(), option.rect.width(), option.rect.height());    //填写范围矩形
            switch (indx)
            {
            case 1:
            {
				QTextOption textOption;
				textOption.setAlignment(Qt::AlignVCenter);
                painter->drawText(rect, QString("___________"), textOption);
            }break;
            case 2:
            {
				QTextOption textOption;
				textOption.setAlignment(Qt::AlignVCenter);
                painter->drawText(rect, QString("_ _ _ _ _ _"), textOption);
            }break;
            case 3:
            {
				QTextOption textOption;
				textOption.setAlignment(Qt::AlignVCenter);
                painter->drawText(rect, QString("..........."), textOption);
            }break;
            case 4:
            {
				QTextOption textOption;
				textOption.setAlignment(Qt::AlignVCenter);
                painter->drawText(rect, QString("_._._._._._."), textOption);
            }break;
            case 5:
            {
				QTextOption textOption;
				textOption.setAlignment(Qt::AlignVCenter);
                painter->drawText(rect, QString("_.._.._.._.."), textOption);
            }break;
            default:
            {
				QTextOption textOption;
				textOption.setAlignment(Qt::AlignVCenter);
                painter->drawText(rect, QString("___________"), textOption);
            }break;
            }
        }break;
        case StateProperty:			//状态类型
        {
            painter->setPen(QColor(0, 0, 0));
            int indx = index.model()->data(index, Qt::DisplayRole).toInt();
            const QList<ComboBoxItem>& lis = selItem->comboxItmList();
			if (lis.count())
			{
				QTextOption textOption;
				textOption.setAlignment(Qt::AlignVCenter);
				QRect rect(option.rect.x() + 5, option.rect.y(), option.rect.width(), option.rect.height());      //填写范围矩形
				painter->drawText(rect, lis.at(indx).text, textOption);
			}
        }break;
        case DateTypeProperty:
        {
            painter->setPen(QColor(0, 0, 0));
            QDateTime dateTime;
            dateTime.setTime_t(index.data(Qt::DisplayRole).toULongLong());
            QString value = dateTime.toString("yyyy-MM-dd hh:mm:ss");
			QTextOption textOption;
			textOption.setAlignment(Qt::AlignVCenter);
			QRect rect(option.rect.x() + 5, option.rect.y(), option.rect.width(), option.rect.height());       //填写范围矩形
            painter->drawText(rect, value, textOption);
        }break;
        default:
        {
            painter->setPen(QColor(0, 0, 0));
            QStyledItemDelegate::paint(painter, option, index);
        }
        }
    }
}

//设置模型
void FHPropertyDelegate::setModel(FHPropertyModel *model)
{
    m_model = model;
}

//建立条目编辑需要的widget
QWidget *FHPropertyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    FHPropertyItem  * selItem = static_cast<FHPropertyItem*>(index.internalPointer());

    int  col = index.column();
    switch (col)
    {
    case 0:
    {
        return 0;
    }break;
    case 1:
    {
        switch (selItem->type())
        {
        case TopNodeProperty:
        case TextProperty:		//文本
        {
            QLineEdit * editor = new QLineEdit(parent);
            if (!selItem->isEditEnable())
                editor->setReadOnly(true);
            return editor;
        }break;
        case StateProperty:		//状态量
        {
            QComboBox * editor = new QComboBox(parent);
            const QList<ComboBoxItem>&  lists = selItem->comboxItmList();
            for (int i = 0; i < lists.size(); ++i)
            {
                editor->addItem(lists.at(i).text);
                if(false == lists.at(i).enabled){
                    QModelIndex index = editor->model()->index(i, 0);
                    editor->model()->setData(index, QVariant(0), Qt::UserRole - 1);
                }
            }
            return editor;
        }break;
        case ColorProperty:		//颜色
        {
            QLabel * editor = new QLabel(parent);
            QColor colorII = selItem->value().value<QColor>();
            editor->setPalette(QPalette(colorII));
            return editor;
        }break;
        case FontProperty:		//字体
        {
            QLabel * editor = new QLabel(parent);
            return editor;
        }break;
        case IntProperty:		//整形
        {
            QSpinBox * editor = new QSpinBox(parent);
            if (!selItem->isEditEnable())
                editor->setReadOnly(true);
            if (selItem->mRange()->type())
                editor->setRange(selItem->mRange()->down_int(), selItem->mRange()->up_int());
            else
                editor->setRange(-10000000, 10000000);
            return editor;
        }break;
        case FloatProperty:		//浮点数
        {
            QDoubleSpinBox * editor = new QDoubleSpinBox(parent);
            if (!selItem->isEditEnable())
                editor->setReadOnly(true);
            if (selItem->mRange()->type())
                editor->setRange(selItem->mRange()->down_real(), selItem->mRange()->up_real());
            else
                editor->setRange(-100000000.00, 100000000000.00);
            return editor;
        }break;
        case BoolProperty:		//bool型
        {
            QComboBox * editor = new QComboBox(parent);
            const QList<ComboBoxItem>&  lists = selItem->comboxItmList();
            for (int i = 0; i < lists.size(); ++i)
            {
                editor->addItem(lists.at(i).text);
            }
            return editor;
        }break;
        case LineTypeProperty:	//线型对象
        {
            QComboBox * editor = new QComboBox(parent);
            const QList<ComboBoxItem>& lists = selItem->comboxItmList();
            for (int i = 0; i < lists.size(); ++i)
            {
                editor->addItem(lists.at(i).text);
            }
            return editor;
        }break;
        case DateTypeProperty:
        {
            QDateTimeEdit* edit = new QDateTimeEdit(parent);
            return edit;
        }
        }
    default:
            break;
        }break;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

//初始化编辑数据到编辑窗口控件
void FHPropertyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
    case 0:
    {

    }break;
    case 1:
    {
        FHPropertyItem *selItem = static_cast<FHPropertyItem*>(index.internalPointer());
        switch (selItem->type())
        {
        case TextProperty:
        {
            QString				dispValue;
            QLineEdit * widgetEditor = qobject_cast<QLineEdit *>(editor);
            dispValue = index.model()->data(index, Qt::DisplayRole).toString();
            widgetEditor->setText(QString("%1").arg(dispValue));
            return;
        }break;
        case StateProperty:
        {
            QComboBox			*widgetEditor;
            widgetEditor = qobject_cast<QComboBox *>(editor);
            int sel = index.model()->data(index, Qt::DisplayRole).toInt();
            widgetEditor->setCurrentIndex(sel);
            return;
        }break;
        case ColorProperty:
        {
            QColor colorII = selItem->value().value<QColor>();
            QColor color = QColorDialog::getColor(colorII, 0);
            if (color.isValid()) {
                selItem->setValue(QVariant(color));
            }
            //通知外部颜色值变化了
            if (m_ICallBack){
                m_ICallBack->operatorProperty(selItem);
            }
            return;
        }break;
        case FontProperty:
        {
            QFont fontII = selItem->value().value<QFont>();
            bool ok;
            QFont font = QFontDialog::getFont(&ok, fontII, 0);
            if (ok) {
                selItem->setValue(QVariant(font));
            }
            //通知外部字体值变化了
            if (m_ICallBack) {
                m_ICallBack->operatorProperty(selItem);
            }
            return;
        }break;
        case IntProperty:		//int
        {
            int val = selItem->value().value<int>();
            QSpinBox			*widgetEditor;
            widgetEditor = qobject_cast<QSpinBox *>(editor);
            widgetEditor->setValue(val);
            return;
        }break;
        case FloatProperty:		//double
        {
            qreal				dispValue;
            QDoubleSpinBox * widgetEditor = qobject_cast<QDoubleSpinBox *>(editor);
            dispValue = index.model()->data(index, Qt::DisplayRole).toDouble();
            widgetEditor->setValue(dispValue);
            return;
        }break;
        case BoolProperty:
        {
            QComboBox			*widgetEditor;
            int					i;
            widgetEditor = qobject_cast<QComboBox *>(editor);
            i = index.model()->data(index, Qt::DisplayRole).toInt();
            widgetEditor->setCurrentIndex(i);
            return;
        }break;
        case LineTypeProperty:
        {
            QComboBox			*widgetEditor;
            int					i;
            widgetEditor = qobject_cast<QComboBox *>(editor);
            i = index.model()->data(index, Qt::DisplayRole).toInt();
            widgetEditor->setCurrentIndex(i - 1);
            return;
        }break;
        case DateTypeProperty:
        {
            QDateTimeEdit* edit = qobject_cast<QDateTimeEdit*>(editor);
            QDateTime dateTime;
            dateTime.setTime_t(index.data(Qt::DisplayRole).toULongLong());
            edit->setDateTime(dateTime);
            return;
        }
        default:
        {}
        }
    }break;
    default:
    {}
    }

    return QStyledItemDelegate::setEditorData(editor, index);
}

//设置模型数据
void FHPropertyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(model);
    int col = index.column();
    switch (col)
    {
    case 0:
    {
    }break;
    case 1:
    {
        FHPropertyItem *selItem = static_cast<FHPropertyItem*>(index.internalPointer());

		switch (selItem->type())
        {
        case TextProperty:
        {
            QString				dispValue;
            QLineEdit * widgetEditor = qobject_cast<QLineEdit *>(editor);
            dispValue = widgetEditor->text();
            if(QVariant(dispValue) != selItem->value()){
				if (!selItem->atonceVal()) {	//不是即刻提交
					QString tipmsg = QStringLiteral("属性:【") + selItem->nameStr() + QStringLiteral("】值被修改，") + QStringLiteral("确认提交修改信息？");
					int result = QMessageBox::information(nullptr, QStringLiteral("提示"), tipmsg, QMessageBox::Yes, QMessageBox::No);
					if(QMessageBox::No == result) return;
				}
                selItem->setValue(QVariant(dispValue));
				if (m_ICallBack) m_ICallBack->operatorProperty(selItem);
            }
        }break;
        case StateProperty:
        {
            QComboBox			*widgetEditor;
            int					index;
            widgetEditor = qobject_cast<QComboBox *>(editor);
            index = widgetEditor->currentIndex();
            if(selItem->value() != QVariant(index)){
				if (!selItem->atonceVal()) {	//不是即刻提交
					QString tipmsg = QStringLiteral("属性:【") + selItem->nameStr() + QStringLiteral("】值被修改，") + QStringLiteral("确认提交修改信息？");
					int result = QMessageBox::information(nullptr, QStringLiteral("提示"), tipmsg, QMessageBox::Yes, QMessageBox::No);
					if(QMessageBox::No == result) return;
				}
                selItem->setValue(QVariant(index));
				if (m_ICallBack) m_ICallBack->operatorProperty(selItem);
            }
        }break;
        case IntProperty:
        {
            QSpinBox			*widgetEditor;
            widgetEditor = qobject_cast<QSpinBox *>(editor);
            int val = widgetEditor->value();
            if(selItem->value() != QVariant(val)){
				if (!selItem->atonceVal()) {	//不是即刻提交
					QString tipmsg = QStringLiteral("属性:【") + selItem->nameStr() + QStringLiteral("】值被修改，") + QStringLiteral("确认提交修改信息？");
					int result = QMessageBox::information(nullptr, QStringLiteral("提示"), tipmsg, QMessageBox::Yes, QMessageBox::No);
					if(QMessageBox::No == result) return;
				}
                selItem->setValue(QVariant(val));
				if (m_ICallBack) m_ICallBack->operatorProperty(selItem);
            }
            return;
        }break;
        case FloatProperty:
        {
            qreal				dispValue;
            QDoubleSpinBox * widgetEditor = qobject_cast<QDoubleSpinBox *>(editor);
            dispValue = widgetEditor->value();
            if(QVariant(dispValue) != selItem->value()){
				if (!selItem->atonceVal()) {	//不是即刻提交
					QString tipmsg = QStringLiteral("属性:【") + selItem->nameStr() + QStringLiteral("】值被修改，") + QStringLiteral("确认提交修改信息？");
					int result = QMessageBox::information(nullptr, QStringLiteral("提示"), tipmsg, QMessageBox::Yes, QMessageBox::No);
					if(QMessageBox::No == result) return;
				}
                selItem->setValue(QVariant(dispValue));
				if (m_ICallBack) m_ICallBack->operatorProperty(selItem);
            }
            return;
        }break;
        case BoolProperty:
        {
            QComboBox			*widgetEditor;
            int					index;
            widgetEditor = qobject_cast<QComboBox *>(editor);
            index = widgetEditor->currentIndex();
			if (QVariant(index) != selItem->value())
			{
				if (!selItem->atonceVal()) {	//不是即刻提交
					QString tipmsg = QStringLiteral("属性:【") + selItem->nameStr() + QStringLiteral("】值被修改，") + QStringLiteral("确认提交修改信息？");
					int result = QMessageBox::information(nullptr, QStringLiteral("提示"), tipmsg, QMessageBox::Yes, QMessageBox::No);
					if (QMessageBox::No == result) return;
				}
				selItem->setValue(QVariant(index));
				if (m_ICallBack) m_ICallBack->operatorProperty(selItem);
			}

        }break;
        case LineTypeProperty:
        {
            QComboBox			*widgetEditor;
            int					index;
            widgetEditor = qobject_cast<QComboBox *>(editor);
            index = widgetEditor->currentIndex();
            if(selItem->value() != QVariant(index)){
				if (!selItem->atonceVal()) {	//不是即刻提交
					QString tipmsg = QStringLiteral("属性:【") + selItem->nameStr() + QStringLiteral("】值被修改，") + QStringLiteral("确认提交修改信息？");
					int result = QMessageBox::information(nullptr, QStringLiteral("提示"), tipmsg, QMessageBox::Yes, QMessageBox::No);
					if(QMessageBox::No == result) return;
				}
                selItem->setValue(QVariant(index));
				if (m_ICallBack) m_ICallBack->operatorProperty(selItem);
            }
        }break;
        case DateTypeProperty:
        {
            QDateTimeEdit* widgetEditor = qobject_cast<QDateTimeEdit*>(editor);
            QDateTime dateValue = widgetEditor->dateTime();
            if(selItem->value() != dateValue.toTime_t()){
				if (!selItem->atonceVal()) {	//不是即刻提交
					QString tipmsg = QStringLiteral("属性:【") + selItem->nameStr() + QStringLiteral("】值被修改，") + QStringLiteral("确认提交修改信息？");
					int result = QMessageBox::information(nullptr, QStringLiteral("提示"), tipmsg, QMessageBox::Yes, QMessageBox::No);
					if (QMessageBox::No == result) return;
				}
                selItem->setValue(QVariant(dateValue.toTime_t()));
				if (m_ICallBack) m_ICallBack->operatorProperty(selItem);
            }
        }break;
        default:{}
        }
    }break;
    default:{}
    }
}

//更新widget大小和位置
void FHPropertyDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void FHPropertyDelegate::setMultipleTopnode(bool _isMultipleTopNode)
{
    if (m_model){
        m_model->setMultipleTopnode(_isMultipleTopNode);
    }
}

