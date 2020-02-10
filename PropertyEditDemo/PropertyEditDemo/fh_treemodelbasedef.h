//树节点信息对应到属性窗口上的数据项索引, 用对应数据库的字段名修改数据库信息时比较方便
#ifndef FH_TREEMODELBASEDEF_H
#define FH_TREEMODELBASEDEF_H
static const char * tr_area_name	         = "NAME";
static const char * tr_area_type	         = "TYPE";
static const char * tr_area_id               = "AREA_AID";
static const char * tr_area_parent           = "PARENT";

static const char * tr_station_name	         = "NAME";
static const char * tr_station_type	         = "STATION_TYPE";
static const char * tr_station_id            = "STATION_SID";
static const char * tr_station_parent        = "PARENT";

static const char * tr_device_name	         = "NAME";
static const char * tr_device_type	         = "ADEVICE_TYPE";
static const char * tr_device_id             = "ADEVICE_LID";
static const char * tr_device_parent         = "PARENT";
static const char * tr_Breaker_no			 = "BREAKER_NO";


static const char * tr_leavas                = "LEAVAS";
static const char * tr_voltage               = "VOLTAGEGrade";
static const char * tr_enabled               = "ENABLED";

static const char * tr_equip_id              = "EQUIP_EID";
static const char * tr_equip_name	         = "EQUIP_NAME";
static const char * tr_equip_parent          = "PARENT";
static const char * tr_equip_group	         = "EQUIPGroup_ID";
static const char * tr_equip_subtype         = "SUBTYPE";
static const char * tr_equip_runstatus       = "RUN_STATUS";
static const char * tr_equip_commcard        = "COMMCARD_ID";
static const char * tr_equip_leadcomm	     = "LEADComm_ID";
static const char * tr_equip_commtype        = "COMMTYPE_ID";
static const char * tr_equip_commprotocal	 = "COMProtocalID";
static const char * tr_equip_manufact        = "MANUFACTURER_ID";
static const char * tr_equip_curchannal      = "CURRENT_CHANNEL";
static const char * tr_equip_check           = "CHECKInterval";
static const char * tr_equip_setting	     = "SETTINGInterval";
static const char * tr_equip_sample          = "SAMPLEInterval";
static const char * tr_equip_switch	         = "SWITCHInterval";
static const char * tr_equip_curtable        = "CurveTableInterval";
static const char * tr_equip_gpspair         = "GPSPairInterval";
static const char * tr_equip_sendfilter      = "SENDFilterLevel";
static const char * tr_equip_maketime	     = "SoftMakeTime";
static const char * tr_equip_version         = "EQUIPVersion";
static const char * tr_equip_verify          = "EQUIPVerify";
static const char * tr_equip_settingverify   = "SETTINGverify_Interval";


#endif // FH_TREEMODELBASEDEF_H
