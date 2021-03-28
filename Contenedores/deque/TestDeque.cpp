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

	std::cout << "deque6: [ ";
	for (unsigned index = 0; index < 3; ++index)
	{
		printf("%.2f", deque6[index]);
		if (index < 2) std::cout << ", ";
	}
	std::cout << " ]\n";

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

	std::cout << "Prueba del metodo assign:\n";

	mySTL::deque<int> deque10;
	// 7 enteros con valor 100.
	deque10.assign(7, 100);

	mySTL::deque<int> deque11;
	// Los 5 valores centrales de deque10.
	deque11.assign(deque10.begin() + 1, deque10.end() - 1);

	mySTL::deque<int> deque12;
	// Asignar desde un arreglo.
	int my_ints[] = { 3, 4, 5 };
	deque12.assign(my_ints, my_ints + 3);

	std::cout << "Size of deque10: " << int( deque10.size() ) << '\n'; // 7
	std::cout << "Size of deque11: " << int( deque11.size() ) << '\n'; // 5
	std::cout << "Size of deque12: " << int( deque12.size() ) << '\n'; // 3
	std::cout << '\n';

	std::cout << "Prueba del metodo resize:\n";

	mySTL::deque<int> deque13{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	deque13.resize(5);		// [ 1, 2, 3, 4, 5 ]
	deque13.resize(8, 100); // [ 1, 2, 3, 4, 5, 100, 100, 100 ]
	deque13.resize(12);		// [ 1, 2, 3, 4, 5, 100, 100, 100, 0, 0, 0, 0 ]

	print_deque(deque13);
	std::cout << '\n';

	std::cout << "Prueba del operador [] y el metodo at:\n";

	mySTL::deque<int> deque14(10);

	mySTL::deque<int>::size_type deque_size = deque14.size();

	for (unsigned index = 0; index < deque_size; ++index)
		deque14[index] = index;

	// Invertir el deque14 usando el operador [].
	for (unsigned index = 0; index < (deque_size / 2); ++index)
	{
		int temp;
		temp = deque14[deque_size - 1 - index];
		deque14[deque_size - 1 - index] = deque14[index];
		deque14[index] = temp;
	}

	std::cout << "deque14: [ ";
	for (unsigned index = 0; index < deque_size; ++index)
		std::cout	<< deque14.at(index)
					<< ( (index < (deque_size - 1)) ? ", " : " ]\n" );
	std::cout << '\n';

	std::cout << "Prueba de los metodos push_back y push_front:\n";

	mySTL::deque<int> deque15;

	for (unsigned num = 0; num <= 10; ++num)
		deque15.push_back(num);

	for (int num = -1; num >= -10; --num)
		deque15.push_front(num);

	std::cout << "deque15: ";
	print_deque(deque15);
	std::cout << '\n';

	std::cout << "Prueba de los metodos pop_back y pop_front:\n";

	deque15.pop_back();
	deque15.pop_front();

	std::cout << "deque15: ";
	print_deque(deque15);
	std::cout << '\n';

	std::cout << "-----Alerta de SPAM en la pantalla-----\n\n";

	std::cout << "Prueba de los metodos insert:\n";

	mySTL::deque<int> deque16 = { 1, 2, 3, 4, 5 };

	std::cout << "\nUn elemento:\n";
	deque16.insert(deque16.begin() + 1, 100); // [ 1, 100, 2, 3, 4, 5 ]

	std::cout << "deque16: ";
	print_deque(deque16);

	std::cout << "\nCon 500 elementos:\n";
	deque16.insert(deque16.begin() + 3, 500, 25); // [ 1, 100, 2, 25 (x500), 3, 4, 5 ]

	std::cout << "deque16: ";
	print_deque(deque16);

	std::cout << "\nOtros 100 elementos:\n";
	deque16.insert(deque16.begin() + 505, 100, 50); // [ 1, 100, 2, 25 (x500), 3, 4, 50 (x100), 5 ]

	std::cout << "deque16: ";
	print_deque(deque16);

	std::cout << "\nCon iteradores:\n";
	// [ 1, 100, 25 (x95?), 2, 25 (x500), 3, 4, 50 (x100), 5 ]
	deque16.insert(deque16.begin() + 2, deque16.begin() + 5, deque16.begin() + 100);

	std::cout << "deque16: ";
	print_deque(deque16);

	std::cout << "\nMas iteradores:\n";
	// [ 1, 100, 25 (x95?), 2, 25 (x500), 3, 4, 50 (x100), 25 (x85?), 5 ]
	deque16.insert(deque16.end() - 1, deque16.begin() + 5, deque16.begin() + 90);

	std::cout << "deque16: ";
	print_deque(deque16);

	std::cout << "\nCon initializer_list:\n";
	deque16.insert(deque16.begin() + (deque16.size() / 2), { 40, 70, 80 });

	std::cout << "deque16: ";
	print_deque(deque16);

	std::cout << '\n';
	std::cout << "Prueba de los metodos erase:\n";

	std::cout << "\nUn elemento:\n";
	deque16.erase(deque16.begin() + 1);

	std::cout << "deque16: ";
	print_deque(deque16);

	std::cout << "\nCon iteradores:\n";
	deque16.erase(deque16.begin() + 1, deque16.begin() + 96);

	std::cout << "deque16: ";
	print_deque(deque16);

	std::cout << '\n';
	std::cout << "-----Fin del SPAM, gracias por su atencion. xd-----\n\n";

	std::cout << '\n';

	return 0;
}