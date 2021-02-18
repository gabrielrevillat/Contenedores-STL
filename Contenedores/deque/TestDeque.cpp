#include "TestDeque.h"

template <typename ValueType>
void print_deque(mySTL::deque<ValueType>& my_deque)
{
	unsigned int count = 0;


}

int testDeque()
{
	std::cout << "Prueba de constructores:\n";

	mySTL::deque<int> deque1; // Constructor por defecto.
	mySTL::deque<int> deque2(4, 100); // 4 enteros con valor 100.

	std::cout << "Construido y destruido\n";

	return 0;
}