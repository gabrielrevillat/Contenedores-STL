#include "TestDeque.h"

template <typename ValueType>
void print_deque(mySTL::deque<ValueType>& my_deque)
{
	unsigned int count = 0;

	std::cout << "[ ";
	for (typename mySTL::deque<ValueType>::iterator iteration = my_deque.begin();
		iteration != my_deque.end(); ++iteration)
	{
		std::cout << *iteration;

		if ( my_deque.begin() + (count + 1) < my_deque.end() )
			std::cout << ", ";

		++count;
	}
	std::cout << " ]\n";
}

int testDeque()
{
	std::cout << "Prueba de constructores:\n";

	mySTL::deque<int> deque1; // Constructor por defecto.
	mySTL::deque<int> deque2(4, 100); // 4 enteros con valor 100.

	std::cout << "deque1: ";
	print_deque(deque1); // [  ]
	std::cout << "deque2: ";
	print_deque(deque2); // [ 100, 100, 100, 100 ]

	std::cout << '\n';

	std::cout << "Funciona chingadaaaaa\n";

	return 0;
}