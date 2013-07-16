/*
 * glb_def.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef GLBDEF_H_
#define GLBDEF_H_

/// ��� ��������
enum eGB_TYPE_DEVICE
{
	AVANT_NO = 0,		// ��������� ��������
	AVANT_R400 = 1,
	AVANT_R400_MSK,
	AVANT_RZSK,
	AVANT_RZSK_OPTIC,
	AVANT_K400,
	AVANT_K400_OPTIC
};

/// ��� �����
/// � ��� ���� �� ��� ��� ������� "��� ��������"?!
enum eGB_TYPE_LINE
{
	UM,
	OPTIC
};

/// �������
enum eGB_COM
{
	GB_COM_GET_SOST		= 0x30,
	GB_COM_GET_FAULT 	= 0x31,
	GB_COM_GET_TIME 	= 0x32,
	GB_COM_SET_TIME 	= 0xB2,

	// ������ � ��
	GB_COM_SET_PASSWORD = 0x73,
	GB_COM_GET_PASSWORD = 0x74
};

/// ����� ������
enum eGB_COM_MASK
{
	// ��� ����������
	GB_COM_MASK_DEVICE = 0x30,
	GB_COM_MASK_DEVICE_DEF = 0x00,
	GB_COM_MASK_DEVICE_PRM = 0x10,
	GB_COM_MASK_DEVICE_PRD = 0x20,
	GB_COM_MASK_DEVICE_GLB = 0x30,

	// ������ �������
	GB_COM_MASK_GROUP = 0xC0,
	GB_COM_MASK_GROUP_READ_PARAM = 0x00,
	GB_COM_MASK_GROUP_WRITE_REGIME = 0x40,
	GB_COM_MASK_GROUP_WRITE_PARAM = 0x80,
	GB_COM_MASK_GROUP_READ_JOURNAL = 0xC0
};

/// ���������� ���������
enum eGB_MEAS_PARAM
{
	GB_MEAS_PARAM_NO,
	GB_MEAS_PARAM_DATE,
	GB_MEAS_PARAM_TIME,
	GB_MEAS_PARAM_UZ,
	GB_MEAS_PARAM_UC,
	GB_MEAS_PARAM_UN,
	GB_MEAS_PARAM_UOUT,
	GB_MEAS_PARAM_IOUT,
	GB_MEAS_PARAM_ROUT
};

/// ����� ��� ���� � �������
class TDataTime
{
public:
	// ���������� � ��������� ������
	uint8_t getSecond() const { return second_; }
	bool setSecondFromBCD(uint8_t val) { return setSecond(bcdToBin(val)); }
	bool setSecond(uint8_t val)
	{
		bool stat = false;
		if (val < 60)
		{
			second_ = val;
			stat = true;
		}
		return stat;
	}

	// ���������� � ��������� �����
	uint8_t getMinute() const { return minute_; }
	bool setMinuteFromBCD(uint8_t val) { return setMinute(bcdToBin(val)); }
	bool setMinute(uint8_t val)
	{
		bool stat = false;
		if (val < 59)
		{
			minute_ = val;
			stat = true;
		}
		return stat;
	}

	// ���������� � ��������� �����
	uint8_t getHour() const { return hour_; }
	bool setHourFromBCD(uint8_t val) { return setHour(bcdToBin(val)); }
	bool setHour(uint8_t val)
	{
		bool stat = false;
		if (val < 24)
		{
			hour_ = val;
			stat = true;
		}
		return stat;
	}

	// ���������� � ��������� ���
	uint8_t getDay() const { return day_; }
	bool setDayFromBCD(uint8_t val) { return setDay(bcdToBin(val)); }
	bool setDay(uint8_t val)
	{
		bool stat = false;

		if (val > 0)
		{
			if ( (month_ == 4) || (month_ == 6) || (month_ == 9) ||
					 	 	 	 	 	 	 	 	 	 	 (month_ == 11) )
			{
				if (val < 31)
				{
					day_ = val;
					stat = true;
				}
			}
			else if (month_ == 2)
			{
				if (val <= 28)
				{

				}
				else if ( ((val <= 29)) && ((year_ % 4) == 0) )
				{
					day_ = val;
					stat = true;
				}
			}
			else
			{
				if (val <= 31)
				{
					day_ = val;
					stat = true;
				}
			}
		}
		return stat;
	}

	// ���������� � ��������� ������
	uint8_t getMonth() const { return month_; }
	bool setMonthFromBCD(uint8_t val) { return setMonth(bcdToBin(val)); }
	bool setMonth(uint8_t val)
	{
		bool stat = false;
		if ( (val > 0) && ( val <= 12) )
		{
			month_ = val;
			stat = true;
		}
		return stat;
	}

	// ���������� � ��������� ����
	uint8_t getYear() 	const { return year_; }
	bool setYearFromBCD(uint8_t val) { return setYear(bcdToBin(val)); }
	bool setYear(uint8_t val)
	{
		bool stat = false;
		if (val < 100)
		{
			year_ = val;
			stat = true;
		}
		return stat;
	}

private:
	uint8_t second_;
	uint8_t minute_;
	uint8_t hour_;
	uint8_t day_;
	uint8_t month_;
	uint8_t year_;

	uint8_t bcdToBin(uint8_t val) { return (val >> 4) * 10 + (val & 0x0F); }
};

/// ����� ��� ������
class TPassword
{
public:
	uint16_t get() const { return password_; }
	bool set(uint16_t pas)
	{
		bool stat = false;
		if (pas < 10000)
		{
			password_ = pas;
			stat = true;
		}
		return stat;
	}

private:
	uint16_t password_;
};

/// ����� ��� �������� ��������� ��������
class TDeviceStatus
{
public:
	TDeviceStatus()
	{
		fault_ = 0;
		faults_ = 0;
		numFaults_ = 0;

		warning_ = 0;
		warnings_ = 0;
		numWarnings_ = 0;

		regime_ = 0;
		state_ = 0;
		dopByte_ = 0;
	}

	// ������
	bool setFault(uint16_t faults)
	{
		fault_ 		= getFirstSetBit(faults);
		faults_ 	= faults;
		numFaults_ 	= getNumSetBits(faults);
		return true;
	}
	uint8_t getFault() 		const { return fault_; }
	uint8_t getFaults() 	const { return faults_; }
	uint8_t getNumFaults()	const { return numFaults_; }


	// �������������
	bool setWarning(uint16_t warnings)
	{
		warning_ 	= getFirstSetBit(warnings);
		warnings_ 	= warnings;
		numWarnings_= getNumSetBits(warnings);
		return true;
	}
	uint8_t getWarning() 	const { return fault_; }
	uint8_t getWarnings() 	const { return faults_; }
	uint8_t getNumWarnings()const { return numWarnings_; }

	// ����� ������
	bool setRegime(uint8_t regime){ regime_ = regime; return true; }
	uint8_t getRegime() 	const { return regime_; }

	//���������
	bool setState(uint8_t state)  { state_ = state; return true; }
	uint8_t getState()		const { return state_; }

	uint8_t getDopByte() 	const { return dopByte_; }
	bool setDopByte(uint8_t byte) { dopByte_ = byte; return true; }

private:
	// ������� ������������ �������������, ������������� � ���-�� ��������������
	uint8_t fault_;
	uint16_t faults_;
	uint8_t numFaults_;

	// ������� ������������ ��������������, �������������� � ���-�� ����������.
	uint8_t warning_;
	uint16_t warnings_;
	uint8_t numWarnings_;

	uint8_t regime_;
	uint8_t state_;
	uint8_t dopByte_;

	// ���������� ���-�� ������������� � 1 ���
	uint8_t getNumSetBits(uint16_t val)
	{
		uint8_t cnt = 0;
		for(; val > 0; val >>= 1)
		{
			if (val & 0x0001)
				cnt++;
		}
		return cnt;
	}

	// ���������� ����� �������� �������������� ����, 0 - ������ ��� ��� �����
	uint8_t getFirstSetBit(uint16_t val)
	{
		uint8_t cnt = 0;
		for(; val > 0; val >>= 1)
		{
			cnt++;
			if (val & 0x0001)
				break;
		}
		return cnt;
	}
};

class TDeviceGlb
{
public:
	TDeviceStatus status;

private:
};

class TDeviceDef
{
public:
	TDeviceStatus status;

private:
};

class TDevicePrm
{
public:
	TDeviceStatus status;

private:
};

class TDevicePrd
{
public:
	TDeviceStatus status;

private:
};

class TMeasuredParameters
{
private:
	int8_t voltDef_;
	int8_t voltCF_;
	int8_t voltNoise_;
	uint8_t voltOutInt_;
	uint8_t voltOutFract_;
	uint16_t curOut_;
	uint16_t resistOut_;
};

/// ��������� ���������� ���
struct stGBparam
{
	// �������
	eGB_TYPE_DEVICE typeDevice;
	eGB_TYPE_LINE typeLine;

	// ������
	TPassword password;

	// ����/�����
	TDataTime dataTime;

	// ���������� ���������
	TMeasuredParameters measurement;

	// ������� ��������� ���������
	TDeviceDef def;
	TDevicePrm prm;
	TDevicePrd prd;
	TDeviceGlb glb;
};

#endif /* GLBDEF_H_ */