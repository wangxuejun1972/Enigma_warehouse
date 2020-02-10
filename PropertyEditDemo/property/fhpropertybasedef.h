#ifndef H_FHPROPERTYBASEDEF_H__
#define H_FHPROPERTYBASEDEF_H__

#include <QVariant>
#include <string>
using namespace std;

//属性结点的类型枚举定义
typedef enum _PropertyTypeDef
{
        TopNodeProperty = 0,			//顶层结点
        TextProperty = 1,				//文本
        StateProperty = 2,				//状态量
        ColorProperty = 3,				//颜色
        FontProperty = 4,				//字体
        IntProperty = 5,				//整形
        FloatProperty = 6,				//浮点
        BoolProperty = 7,				//布尔
        LineTypeProperty = 8,			//画笔线类型
        DateTypeProperty = 9,			//日期类型
        UnknownProperty
}PropertyNodeType;

//属性条目对应的数据结构
typedef struct _PropertyItemData
{
	QString						nam;
	QVariant					val;
    PropertyNodeType            type;
	string						indexName;				//用来修改属性时的属性查找名称
    bool						editFlag;				//编辑标志
	bool						atonceFlg;				//修改即刻提交标志 [wxj]2020/2/9
	_PropertyItemData(): editFlag(true), atonceFlg(true){
                
	}
}PropertyItemData, *PTRPropertyItemData;

//combox控件的条目数据
typedef struct _ComboBoxItem {
	QString text;
	bool enabled;
	_ComboBoxItem() : enabled(true) {
	}
}ComboBoxItem;

#define PROPERTY_GBKStr	 QString::fromLocal8Bit




#endif	//H_FHPROPERTYBASEDEF_H__
