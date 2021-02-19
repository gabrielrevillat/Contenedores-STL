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

	// Constructor por defecto.
	mySTL::deque<int> deque1;
	// 4 enteros con valor 100.
	mySTL::deque<int> deque2(4, 100);
	// Iterando a través de deque2.
	mySTL::deque<int> deque3(deque2.begin(), deque2.end());
	// Una copia de deque3.
	mySTL::deque<int> deque4(deque3);
	// initializer_list
	mySTL::deque<int> deque5{ 16, 2, 77, 29 };

	// Iterando a través de un arreglo.
	double my_doubles[] = { 4.20, 3.14, 0.69 };
	mySTL::deque<double> deque6(my_doubles,
		my_doubles + ( sizeof(my_doubles) / sizeof(double) ));

	std::cout << "deque1: ";
	print_deque(deque1); // [  ]
	std::cout << "deque2: ";
	print_deque(deque2); // [ 100, 100, 100, 100 ]
	std::cout << "deque3: ";
	print_deque(deque3); // [ 100, 100, 100, 100 ]
	std::cout << "deque4: ";
	print_deque(deque4); // [ 100, 100, 100, 100 ]
	std::cout << "deque5: ";
	print_deque(deque5); // [ 16, 2, 77, 29 ]
	std::cout << "deque6: ";
	print_deque(deque6); // [ 4.20, 3.14, 0.69 ]

	std::cout << '\n';

	std::cout << "Prueba del operador = y el metodo size:\n";

	mySTL::deque<char> deque7(3);
	mySTL::deque<char> deque8(5);

	std::cout << "Size of deque7: " << int( deque7.size() ) << '\n'; // 3
	std::cout << "Size of deque8: " << int( deque8.size() ) << '\n'; // 5
	std::cout << '\n';

	deque8 = deque7;
	deque7 = mySTL::deque<char>();

	std::cout << "Size of deque7: " << int( deque7.size() ) << '\n'; // 0
	std::cout << "Size of deque8: " << int( deque8.size() ) << '\n'; // 3
	std::cout << '\n';

	mySTL::deque<char> deque9 = { 'c', 'h', 'a', 'r' };

	std::cout << "deque9: ";
	print_deque(deque9); // [ 5, 6, 7, 8 ]

	std::cout << "Size of deque9: " << int( deque9.size() ) << '\n'; // 4
	std::cout << '\n';



	std::cout << "Funciona chingadaaaaa\n";

	return 0;
}