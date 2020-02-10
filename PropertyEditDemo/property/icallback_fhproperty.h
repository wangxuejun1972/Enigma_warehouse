#ifndef H_ICALLBACK_FHPROPERTY_H__
#define H_ICALLBACK_FHPROPERTY_H__


/**
	*@class ICallBack_FHProperty   回调接口类  -- 属性编辑动态库需要导出
	*@file  icallback_fhproperty.h
	*@brief 用于属性编辑框内针对指定属性条目项进行修改操作反射到外部的回调接口类，用来处理动态库内部属性值变化操作反应到外部进行处理
	*date   20191028
	*author wxj

*/

class FHPropertyItem; 
class  ICallBack_PropertyVary
{
public:
	ICallBack_PropertyVary(){};
	~ICallBack_PropertyVary(){};
public:
	virtual void operatorProperty(FHPropertyItem * itemData) = 0;		//外部用户回调重载接口,		
};

#endif	//H_ICALLBACK_FHPROPERTY_H__