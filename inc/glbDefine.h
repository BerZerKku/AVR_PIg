/*
 * glb_def.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef GLBDEF_H_
#define GLBDEF_H_

#include <stdint.h>
#include <avr/pgmspace.h>

/// ������ ������� ��������
#define VERS 0x0100

/// �������������� ���� CHAR � INT
#define TO_INT16(high, low) (((uint16_t) high << 8) + low)

/// ����������� ��������� ���-�� ��������� ���������
#define MAX_NUM_DEVICE_STATE 11

/// ����������� ��������� ���-�� ������� ���������
#define MAX_NUM_REGIME 6


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


/// ��� ������ �����
enum eGB_TYPE_LINE
{
	GB_TYPE_LINE_UM = 1,
	GB_TYPE_LINE_OPTIC = 2,
	GB_TYPE_LINE_E1 = 3
};


/// ���-�� ��������� � �����
enum eGB_NUM_DEVICES
{
	GB_NUM_DEVICES_2 = 1,
	GB_NUM_DEVICES_3 = 2
};

/// �������������
enum eGB_COMPATIBILITY
{
	GB_COMPATIBILITY_AVANT = 0,
	GB_COMPATIBILITY_PVZ90 = 1,
	GB_COMPATIBILITY_AVZK = 2,
	GB_COMPATIBILITY_PVZUE = 3
};


/// �������
enum eGB_COM
{
	GB_COM_GET_SOST		= 0x30,
	GB_COM_GET_FAULT 	= 0x31,
	GB_COM_GET_TIME 	= 0x32,
	GB_COM_GET_MEAS		= 0x34,
	GB_COM_GET_VERS		= 0x3F,
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
		enable_ = false;

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
	uint16_t getFaults() 	const { return faults_; }
	uint8_t getNumFaults()	const { return numFaults_; }


	// �������������
	bool setWarning(uint16_t warnings)
	{
		warning_ 	= getFirstSetBit(warnings);
		warnings_ 	= warnings;
		numWarnings_= getNumSetBits(warnings);
		return true;
	}
	uint8_t getWarning() 	const { return warning_; }
	uint16_t getWarnings() 	const { return warnings_; }
	uint8_t getNumWarnings()const { return numWarnings_; }

	// ����� ������
	bool setRegime(uint8_t regime)
	{
		bool status = false;
		if (regime < MAX_NUM_REGIME)
		{
			regime_ = regime;
			status = true;
		}
		else
			regime = MAX_NUM_REGIME;


		return true;
	}
	uint8_t getRegime() 	const { return regime_; }

	//���������
	bool setState(uint8_t state)
	{
		bool status = false;
		if (state < MAX_NUM_DEVICE_STATE)
		{
			state_ = state;
			status = true;
		}
		else
			state = MAX_NUM_DEVICE_STATE;

		return status;
	}
	uint8_t getState()		const { return state_; }

	uint8_t getDopByte() 	const { return dopByte_; }
	bool setDopByte(uint8_t byte) { dopByte_ = byte; return true; }

	// ������ � ������ ������� ����������
	// ���������� true ���� ����� �������� ���������� �� ��������
	bool setEnable(bool enable)
	{
		bool status = false;
		if (enable_ != enable)
		{
			enable_ = enable;
			status = true;
		}
		return status;
	}
	bool isEnable()	const { return enable_; }

	// ������� ����������� ������ � ��������������
	PGM_P faultText[16];
	PGM_P warningText[8];
	PGM_P stateText[MAX_NUM_DEVICE_STATE];
	PGM_P name;

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

	// True - �������� ������� ������� ���������� � ������� ���������� ������
	bool enable_;

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

	// ���������� ����� �������� �������������� ����
	uint8_t getFirstSetBit(uint16_t val)
	{
		uint8_t cnt = 0;
		for(; val > 0; val >>= 1)
		{
			if (val & 0x0001)
				break;
			cnt++;
		}
		return cnt;
	}
};


/// ����� ��� ����� ���������� � ��������
class TDeviceGlb
{
public:
	TDeviceGlb()
	{
		numDevices_ = GB_NUM_DEVICES_2;
		typeLine_ = GB_TYPE_LINE_UM;
		compatibility_ = GB_COMPATIBILITY_AVANT;
		versBsp_ = 0;
		versDsp_ = 0;
	}

	TDeviceStatus status;

	// ��� ��������� ���������� true ���� ����� �������� ���������� �� ��������
	eGB_NUM_DEVICES getNumDevices() const { return numDevices_; }
	bool setNumDevices(eGB_NUM_DEVICES numDevices)
	{
		bool status = false;
		if ( (numDevices == GB_NUM_DEVICES_2) ||
				(numDevices == GB_NUM_DEVICES_3) )
		{
			numDevices_ = numDevices;
			status = true;
		}
		return status;
	}

	// ��� ��������� ���������� true ���� ����� ������� ���������� �� ��������
	eGB_TYPE_LINE getTypeLine() const { return typeLine_; }
	bool setTypeLine(eGB_TYPE_LINE typeLine)
	{
		bool status = false;

		if ( (typeLine == GB_TYPE_LINE_UM) || (typeLine == GB_TYPE_LINE_E1) ||
				(typeLine == GB_TYPE_LINE_OPTIC) )
		{
			typeLine_ = typeLine;
			status = true;
		}
		return status;
	}

	// ������ �������� AtMega BSP
	uint16_t getVersBsp() const { return versBsp_; }
	void setVersBsp(uint16_t versBsp) { versBsp_ = versBsp; }

	//  ������ �������� DSP
	uint16_t getVersDsp() const { return versDsp_; }
	void setVersDsp(uint16_t versDsp) {  versDsp_ = versDsp; }

	// ��� ��������� ���������� true ���� ����� �������� ���������� �� ��������
	eGB_COMPATIBILITY getCompatibility() const { return compatibility_; }
	bool setCompatibility(eGB_COMPATIBILITY compatibility)
	{
		bool status = false;
		if ( (compatibility == GB_COMPATIBILITY_AVANT) ||
				(compatibility == GB_COMPATIBILITY_AVZK) ||
				(compatibility == GB_COMPATIBILITY_PVZ90) ||
				(compatibility == GB_COMPATIBILITY_PVZUE) )
		{
			compatibility_ = compatibility;
			status = true;
		}
		return status;
	}

private:
	// ���-�� ��������� � ����� 2 ��� 3
	eGB_NUM_DEVICES numDevices_;

	// ��� �����
	eGB_TYPE_LINE typeLine_;

	// ������ �������� ���
	uint16_t versBsp_;

	// ������ �������� DSP
	uint16_t versDsp_;

	// �������������
	eGB_COMPATIBILITY compatibility_;
};


/// ����� ��� ���������� � �������� ������
class TDeviceDef
{
public:
	TDeviceStatus status;

private:
};


/// ����� ��� ���������� � �������� ���������
class TDevicePrm
{
public:
	TDeviceStatus status;

	// ��������� ���-�� ������ � ���, ���� ��� ����� 0 ��� ������ 32
	// ���������� true ���� ����� �������� ���������� �� �����������, � �����
	// ������������� ���� enable
	bool setNumCom(uint8_t numCom)
	{
		bool status = false;
		if ( (numCom <= 32) && (numCom_ != numCom) )
		{
			numCom_ = numCom;
			this->status.setEnable(numCom != 0);
			status = true;
		}
		return status;
	}
	uint8_t getNumCom() const { return numCom_; }

private:
	// ���-�� ������
	uint8_t numCom_;
};


/// ����� ��� ���������� � �������� �����������
class TDevicePrd
{
public:
	TDeviceStatus status;

	// ��������� ���-�� ������ � ���, ���� ��� ����� 0 ��� ������ 32
	// ���������� true ���� ����� �������� ���������� �� �����������, � �����
	// ������������� ���� enable
	bool setNumCom(uint8_t numCom)
	{
		bool status = false;
		if ( (numCom <= 32) && (numCom_ != numCom) )
		{
			numCom_ = numCom;
			this->status.setEnable(numCom != 0);
			status = true;
		}
		return status;
	}
	uint8_t getNumCom() const { return numCom_; }
private:
	uint8_t numCom_;
};


/// ����� ��� ���������� ����������
class TMeasuredParameters
{
public:
	TMeasuredParameters()
	{
		voltDef_ = 0;
		voltCf_ = 0;
		voltNoise_ = 0;
		voltOut_ = 0;
		curOut_ = 0;
		resistOut_ = 0;
	}

	// ����� �� ������
	int8_t getVoltageDef() const { return voltDef_; }
	bool setVoltageDef(int8_t voltDef)
	{
		bool state = false;
		if ( (voltDef > - 100) && (voltDef < 100) )
		{
			voltDef_ = voltDef;
			state = true;
		}
		return state;

	}

	// ����� �� ��
	int8_t getVoltageCf() const { return voltCf_; }
	bool setVoltageCf(int8_t voltCf)
	{
		bool state = false;
		if ( (voltCf > -100) && (voltCf < 100) )
		{
			voltCf_ = voltCf;
			state = true;
		}
		return state;

	}

	// ���������� ������
	uint8_t getVoltageOutInt() const { return (voltOut_ / 10); }
	uint8_t getVoltageOutFract() const { return (voltOut_ % 10); }
	bool setVoltageOut(uint8_t voltOutInt, uint8_t voltOutFract)
	{
		bool state = false;
		if ( (voltOutInt < 100) && (voltOutFract < 10) )
		{
			voltOut_ = (((uint16_t) voltOutInt) * 10) + voltOutFract;
			state = true;
		}
		return state;
	}

	// ��� ������
	uint16_t getCurrentOut() const { return curOut_; }
	bool setCurrentOut(uint16_t curOut)
	{
		bool state = false;
		if (curOut < 999)
		{
			curOut_ = curOut;
			state = true;
		}
		return state;
	}

	// ������������� ������
	uint16_t getResistOut() const { return resistOut_; }
	bool setResistOut(uint16_t resistOut)
	{
		bool state = false;
		if (resistOut < 999)
		{
			resistOut_ = resistOut;
			state = true;
		}
		return state;
	}

	// ������� ����
	int8_t getVoltageNoise() const { return voltNoise_; }
	bool setVoltageNoise(int8_t voltNoise)
	{
		bool state = false;
		if ( (voltNoise > -100) && (voltNoise < 100) )
		{
			voltNoise_ = voltNoise;
			state = true;
		}
		return state;
	}
private:
	// ����� �� ������ (-99 .. 99)��
	int8_t voltDef_;
	// ����� �� �� (-99 .. 99)��
	int8_t voltCf_;
	// ������� ����� (-99 .. 99)��
	int8_t voltNoise_;
	// �������� ���������� ���������� �� 10 (0 .. 999)�
	uint16_t voltOut_;
	// �������� ��� (0 .. 999)��
	uint16_t curOut_;
	// �������� ������������� (0 .. 999)��
	uint16_t resistOut_;
};


/// ��������� ���������� ���
struct stGBparam
{

	// false - �������� ��� ���� ��������� ���� ��� ������� ��� ��������
	bool device;
	eGB_TYPE_DEVICE typeDevice;

	// ������
	TPassword password;

	// ����/�����
	TDataTime dataTime;

	// ���������� ���������
	TMeasuredParameters measParam;

	// ������� ��������� ���������
	TDeviceDef def;
	TDevicePrm prm;
	TDevicePrd prd;
	TDeviceGlb glb;
};

#endif /* GLBDEF_H_ */
