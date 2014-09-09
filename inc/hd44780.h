//������������ ������ � ��-�����������
//��� ���� ����� ������� ������ ���� ������ ��������� ��������:
//LCDprint(Line, AddressInLine, LCDstatus, ���_�������, convers)
//, ��� Line - ����� ������, AddressInLine -��������� �������,
//LCDstatus - 1(�������),  2(������)
//���_������� - ��������� ������� ������ �������� ����� ��������� �� ��������� (signed char)
//��������� ��������� ������� ������ ���� 0x00 (�.�. ��������� ������ � ��).
//������� ��-����������:
//0x01 - ������� ������, ������� ������� � ������� �������
//0�20 - ������ � ����, ������ DDRAM � ����
//���� ���� �������������� ������� ����� Windows � ������� ����� LCD ���� convers=1, ����� 0
#include <avr/io.h>
#include <stdint.h>

#define SIZE_BUF_STRING 80

#define SetE PORTB |= 1<<5

#define ClrE PORTB &= ~(1<<5)
#define SetRS PORTB |= 0x40
#define ClrRS PORTB &= 0xBF


/// ���� ����������� ������� E.
#define PORT_ES PORTB
/// ��� ����������� ������� �.
#define PIN_ES 5
/// ���� ����������� ������� RS
#define PORT_RS PORTB
/// ��� ����������� ������� RS.
#define PIN_RS 6
/// ���� ����������� ���� ������.
#define PORT_DB PORTA

/**	������������� ���.
 *
 * 	������������ ��������� ���.
 */
void vLCDinit(void);

/** ������� ����������� ������.
 *
 * 	��� ������� � ������ ���������� �� ' '.
 */
//void vLCDclear(void);

/**	�������� ������� ���������� ����������� ���.
 *
 * 	���������� �������� � �������� ������� 50-100���.
 */
void vLCDmain(void);

/**	������ ���������� ����������� ���.
 *
 * 	�������������� ���� ������������� �������� ���������� ���.
 */
void vLCDrefresh(void);

/**	�������������� ������� ��������, � ������ ��� ������ �� ���.
 *
 * 	������� ������� ����������� ������ �������� � ������� ������ ���.
 */
void vLCDputchar(char *b);
