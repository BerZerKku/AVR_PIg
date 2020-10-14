#ifndef FIFO_H_
#define FIFO_H_

#include "stdint.h"
#include "stdio.h"

template <size_t size, typename type>
class TFifo {
    type fifo[size];

public:
	/// �����������.
	TFifo() {
		head = 0;
        len = 0;
	}

	/**	�������� �������� � �������.
	 *
	 * 	@param[in] var ��������.
	 * 	@return False - ���� ������� ���������, ����� true.
	 */
	bool push(type var) {
	  bool isovf = true;

        if (len < size) {
            fifo[(head + len++) % size] = var;
			isovf = false;
		}

		return !isovf;
	}

	/**	������� ���������� �� �������.
	 *
	 *	@param[out] ��������.
	 *	@return False - ���� ������� �����, ����� true.
	 */
	bool pop(type &var) {
	  bool isempty = true;

        if (len > 0) {
			var = fifo[head];
            head = (head + 1) % size;
            len--;
			isempty = false;
		}

		return !isempty;
	}

	///	������� �������.
	void flush() {
        len =0;
	}

    /// ��������� ���������� ������ � �������.
    bool isEmpty() const {
        return (len == 0);
    }

public:
	/// ������ �������.
	size_t head;

    /// ������ �������.
    size_t len;
};



#endif /* FIFO_H_ */
