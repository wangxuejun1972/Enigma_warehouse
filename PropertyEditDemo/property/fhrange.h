#ifndef H_FHRANGE_H__
#define H_FHRANGE_H__

typedef struct _FHRange_INT
{
	int			up;
	int			down;
}Range_INT;

typedef struct _FHRange_REAL
{
    float                   up;
    float                   down;
}Range_REAL;

typedef union _RangeDat
{
    Range_INT               intRange;
    Range_REAL              realRange;
}RangeDat;

typedef struct _RangeNode
{
    int						dataType;//数据类型---0==int 1==double -1==无效
    RangeDat                dat;	 //数据值的上下限范围
}RangeNode;


/**
	*@class FHRange
	*@brief 标识整形或者浮点型的数值的上限与下限，提供设置上限值下限值设置以及获取上限值下限值功能成员，设置范围对象的类型【整型或者浮点】
	*date   20191028
	*author wxj
*/

class FHRange
{
public:
	FHRange() {
		m_dat.dataType = 0;
		m_dat.dat.intRange.up = INT_MAX;
		m_dat.dat.intRange.down = INT_MIN;
	}
	~FHRange(){
	}
public:
	int type() { return m_dat.dataType; }
	void setType(int typ) { m_dat.dataType = typ; }

	void init_int() {
		m_dat.dat.intRange.up = INT_MAX;
		m_dat.dat.intRange.down = INT_MIN;
	}

	void init_qreal() {
		m_dat.dat.realRange.up = FLT_MAX_10_EXP;
		m_dat.dat.realRange.down = FLT_MIN_10_EXP;
	}

	int up_int() { return m_dat.dat.intRange.up; }
	int down_int() { return m_dat.dat.intRange.down; }
	void setUp_int(int up) { m_dat.dat.intRange.up = up; }
	void setDown_int(int down) { m_dat.dat.intRange.down = down; }

	float up_real() { return m_dat.dat.realRange.up; }
	float down_real() { return m_dat.dat.realRange.down; }
	void setUp_real(qreal up) { m_dat.dat.realRange.up = up; }
	void setDown_real(qreal down) { m_dat.dat.realRange.down = down; }
private:
        RangeNode  m_dat;
};



#endif	//H_FHRANGE_H__
