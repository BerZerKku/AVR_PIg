#ifndef FIFO_H_
#define FIFO_H_

#include "stdint.h"
#include "stdio.h"

template <size_t size, typename type>
class TFifo {
    type fifo[size];

public:
	/// Конструктор.
	TFifo() {
		head = 0;
        len = 0;
	}

	/**	Добавить значение в очередь.
	 *
	 * 	@param[in] var Значение.
	 * 	@return False - если очередь заполнена, иначе true.
	 */
	bool push(type var) {
	  bool isovf = true;

        if (len < size) {
            fifo[(head + len++) % size] = var;
			isovf = false;
		}

		return !isovf;
	}

	/**	Достать переменную из очереди.
	 *
	 *	@param[out] Значение.
	 *	@return False - если очередь пуста, иначе true.
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

	///	Очистка очереди.
	void flush() {
        len =0;
	}

    /// Проверяет остутствие данных в очереди.
    bool isEmpty() const {
        return (len == 0);
    }

public:
	/// Начало очереди.
	size_t head;

    /// Размер очереди.
    size_t len;
};



#endif /* FIFO_H_ */
