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
	GB_COM_GET_TIME = 0x32,
	GB_COM_SET_TIME = 0xB2,
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

/// ��������� ���������� ���
struct stGBparam
{
	// �������
	eGB_TYPE_DEVICE typeDevice;
	eGB_TYPE_LINE typeLine;

	// ������
	uint16_t password;
	bool setPassword(uint16_t pas)
	{
		bool stat = false;
		if (pas < 10000)
		{
			password = pas;
			stat = true;
		}
		return stat;
	}

	// ����/�����
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	bool setData(uint8_t day, uint8_t month, uint8_t year);
	bool setTime(uint8_t hour, uint8_t minute, uint8_t second);

	// ���������� ���������
	int8_t voltDef;
	int8_t voltCF;
	int8_t voltNoise;
	uint8_t voltOutInt;
	uint8_t voltOutFract;
	uint16_t curOut;
	uint16_t resistOut;

	// ��������� �����
	uint16_t glb_avar;
	uint16_t glb_warn;

	// ��������� ������
	uint16_t def_avar;
	uint16_t def_warn;
	uint8_t def_regime;
	uint8_t def_sost;
	uint8_t def_dop;

	// ��������� ���������
	uint16_t prm_avar;
	uint16_t prm_warn;
	uint8_t prm_regime;
	uint8_t prm_sost;
	uint8_t prm_dop;

	// ��������� �����������
	uint16_t prd_avar;
	uint16_t prd_warn;
	uint8_t prd_regime;
	uint8_t prd_sost;
	uint8_t prd_dop;
};

#endif /* GLBDEF_H_ */
