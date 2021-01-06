#include "TestVector.h"

/// Imprime un vector con formato.
template <typename ValueType>
void print_vector(mySTL::vector<ValueType>& my_vector)
{
    unsigned int count = 0;

    std::cout << "{ ";
    for ( typename mySTL::vector<ValueType>::iterator iteration = my_vector.begin(); iteration != my_vector.end(); ++iteration, ++count )
        std::cout << *iteration << ((( my_vector.begin() + (count + 1) ) == my_vector.end()) ? " }\n" : ", ");

    if ( count == 0 )
        std::cout << " }\n";
}

int testVector()
{
    try
    {
        unsigned int count = 0;

        std::cout << "Prueba de constructores:\n";

        mySTL::vector<int> vector1; // Constructor por defecto.
        mySTL::vector<int> vector2(4, 100); // 4 enteros con valor 100.
        mySTL::vector<int> vector3(vector2.begin(), vector2.end()); // Iterando a través de vector2.
        mySTL::vector<int> vector4(vector3); // Una copia de vector3.

        print_vector(vector1); // {  }
        print_vector(vector2); // { 100, 100, 100, 100 }
        print_vector(vector3); // { 100, 100, 100, 100 }
        print_vector(vector4); // { 100, 100, 100, 100 }
        std::cout << '\n';

        // El constructor de rango también puede usarse para construir con arreglos.
        int my_ints[] = { 16, 2, 7, 29 };
        mySTL::vector<int> vector5(my_ints, (my_ints + sizeof(my_ints) / sizeof(int)));

        std::cout << "The contents of vector5 are: ";
        print_vector(vector5); // { 16, 2, 7, 29 }
        std::cout << '\n';

        std::cout << "Prueba del metodo size:\n";
        
        mySTL::vector<int> vector6(3, 0); // { 0, 0, 0 }
        mySTL::vector<int> vector7(5, 0); // { 0, 0, 0, 0, 0 }

        vector7 = vector6; // Ahora { 0, 0, 0 }
        vector6 = mySTL::vector<int>(); // y {  }

        std::cout << "Size of vector6: " << int(vector6.size()) << '\n';      // 0
        std::cout << "Size of vector7: " << int(vector7.size()) << "\n\n";    // 3

        std::cout << "Prueba de los metodos resize y push_back:\n";

        mySTL::vector<int> vector8;

        for ( int num = 1; num < 10; ++num )
            vector8.push_back(num); // { 1, 2, 3, 4, 5, 6, 7, 8, 9 }

        vector8.resize(5);      // { 1, 2, 3, 4, 5 }
        vector8.resize(8, 100); // { 1, 2, 3, 4, 5, 100, 100, 100 }
        vector8.resize(12);     // { 1, 2, 3, 4, 5, 100, 100, 100, 9, 0, 0, 0 }
        // El 9 cuenta como "basura" al asignar el nuevo tamaño. 

        std::cout << "vector8 contains: ";
        print_vector(vector8);
        std::cout << "\n\n";

        std::cout << "Prueba de los metodos de tamano:\n";

        mySTL::vector<int> vector9;

        for ( int num = 0; num < 100; ++num )
            vector9.push_back(num); // { 0, 1, ..., 98, 99 }

        std::cout << "vector9:\n";
        std::cout << "size: " << (int) vector9.size() << '\n';              // 100
        std::cout << "capacity: " << (int) vector9.capacity() << '\n';      // 128
        std::cout << "max_size: " << (long) vector9.max_size() << "\n\n";   // 2305843009213693952

        mySTL::vector<int> vector10;
        int sum(0);

        for ( int num = 1; num <= 10; ++num )
            vector10.push_back(num); // { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }

        // Suma el último elemento actual y lo saca del contenedor, uno por uno.
        while ( !(vector10.empty()) )
        {
            sum += vector10.back();
            vector10.pop_back();
        }

        std::cout << "sum of vector10 elements: " << sum << "\n\n"; // 56

        std::cout << "Prueba del metodo capacity:\n";

        mySTL::vector<int>::size_type vector_capacity;

        mySTL::vector<int> vector11;

        vector_capacity = vector11.capacity(); // 0

        std::cout << "making vector11 grow:\n";
        for ( int num = 0; num < 100; ++num )
        {
            vector11.push_back(num);

            if ( vector_capacity != vector11.capacity() )
            {
                vector_capacity = vector11.capacity();
                std::cout << "Capacity changed: " << vector_capacity << '\n'; // *2
            }
        }

        mySTL::vector<int> vector12;

        vector_capacity = vector12.capacity();
        vector12.reserve(100);

        std::cout << "making vector12 grow:\n";
        for ( int num = 0; num < 100; ++num )
        {
            vector12.push_back(num);

            if ( vector_capacity != vector12.capacity() )
            {
                vector_capacity = vector12.capacity();
                std::cout << "Capacity changed: " << vector_capacity << "\n\n"; // 100
            }
        }

        mySTL::vector<int> vector13(100);
        std::cout << "capacity of vector13: " << vector13.capacity() << '\n';   // 100

        vector13.resize(10);
        std::cout << "capacity of vector13: " << vector13.capacity() << '\n';   // 100

        vector13.shrink_to_fit();
        std::cout << "capacity of vector13: " << vector13.capacity() << "\n\n"; // 10

        std::cout << "Prueba del operador [] y el metodo at:\n";

        mySTL::vector<int> vector14(10);

        mySTL::vector<int>::size_type vector_size = vector14.size();

        for ( unsigned index = 0; index < vector_size; ++index )
            vector14[index] = index;

        // Invertir el vector14 usando el operador [].
        for ( unsigned index = 0; index < (vector_size / 2) ; ++index )
        {
            int temp;
            temp = vector14[vector_size - 1 - index];
            vector14[vector_size - 1 - index] = vector14[index];
            vector14[index] = temp;
        }

        std::cout << "vector14 contains: { ";
        for ( unsigned index = 0; index < vector_size; ++index )
            std::cout << vector14.at(index) << ((index < ( vector_size - 1 )) ? ", " : " }\n");
        std::cout << "\n\n";

        std::cout << "Prueba de front y back:\n";

        mySTL::vector<int> vector15;

        vector15.push_back(78);
        vector15.push_back(16);

        vector15.front() -= vector15.back(); // 78 - 16

        std::cout << "vector15 is now " << vector15.front() << "\n\n";

        std::cout << "Prueba del metodo assign:\n";

        mySTL::vector<int> vector16;
        mySTL::vector<int> vector17;
        mySTL::vector<int> vector18;

        vector16.assign(7, 100);

        vector17.assign(vector16.begin() + 1, vector16.end() - 1); // Los 5 valores centrales de vector16.

        int my_ints2[] = { 1776, 7, 4 };
        vector18.assign(my_ints2, my_ints2 + 3);

        std::cout << "Size of vector16: " << int(vector16.size()) << '\n';      // 7
        std::cout << "Size of vector17: " << int(vector17.size()) << '\n';      // 5
        std::cout << "Size of vector18: " << int(vector18.size()) << "\n\n";    // 3

        std::cout << "Prueba de insert y erase:\n";

        mySTL::vector<int> vector19(3, 100); // { 100, 100, 100 }

        mySTL::vector<int>::iterator vector19_itr;

        vector19_itr = vector19.begin();
        vector19_itr = vector19.insert(vector19_itr, 200); // { 200, 100, 100, 100 }

        vector19.insert(vector19_itr, 2, 300); // { 300, 300, 200, 100, 100, 100 }

        vector19_itr = vector19.begin();

        mySTL::vector<int> vector20(2, 400); // { 400, 400 }

        vector19.insert(vector19_itr + 2, vector20.begin(), vector20.end()); // { 300, 300, 400, 400, 200, 100, 100, 100 }

        int my_array3[] = { 501, 502, 503 };
        vector19.insert(vector19.begin(), my_array3, my_array3 + 3); // { 501, 502, 503, 300, 300, 400, 400, 200, 100, 100, 100 }

        std::cout << "vector19 contains: ";
        print_vector(vector19);
        std::cout << '\n';

        mySTL::vector<int> vector21;

        for ( int num = 1; num <= 10; ++num )
            vector21.push_back(num); // { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }

        vector21.erase(vector21.begin() + 5); // { 1, 2, 3, 4, 5, 7, 8, 9, 10 }

        std::cout << "vector21 contains: ";
        print_vector(vector21);

        vector21.erase(vector21.begin(), vector21.begin() + 3); // { 4, 5, 7, 8, 9, 10 }

        std::cout << "vector21 now contains: ";
        print_vector(vector21);
        std::cout << '\n';

        mySTL::vector<int> vector22;
        vector22.reserve(8);
        
        for ( unsigned num = 1; num <= 5; ++num )
            vector22.push_back(num); // { 1, 2, 3, 4, 5 }

        std::cout << "vector22 contains: ";
        print_vector(vector22);

        vector22.insert(vector22.begin() + 2, 4, 10); // { 1, 2, 10, 10, 10, 10, 3, 4, 5 }
        std::cout << "my_vector capacity: " << vector22.capacity() << '\n';

        std::cout << "my_vector now contains: ";
        print_vector(vector22);
        std::cout << '\n';
    }
    catch (std::bad_alloc& exception)
    {
        std::cerr << "Mamon\n";
        return 1;
    }
   
    std::cout << "Funciono crack xd\n";

	return 0;
}