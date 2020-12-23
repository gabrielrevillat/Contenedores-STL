#include "TestArray.h"

int testArray()
{
    // Prueba de iteradores.
    mySTL::array<int, 5> my_array = { 2, 16, 77, 34, 50 };
    
    std::cout << "Probando iteradores: \n";

    std::cout << "my_array contains:";
    for ( auto iteration = my_array.begin(); iteration != my_array.end(); ++iteration )
        std::cout << ' ' << *iteration;
    std::cout << "\n\n";

    // Prueba de iteradores inversos.
    mySTL::array<int, 4> my_array2 = { 4, 26, 80, 14 };

    std::cout << "Probando iteradores inversos: \n";

    std::cout << "my_array2 contains:";
    for ( auto reverse_iteration = my_array2.rbegin(); reverse_iteration != my_array2.rend(); ++reverse_iteration )
        std::cout << ' ' << *reverse_iteration;
    std::cout << "\n\n";

    std::cout << "Probando metodos de capacidad: \n";

    // Prueba de los métodos size y max_size.
    mySTL::array<int, 5> my_ints;

    std::cout << "size of my_ints: " << my_ints.size() << '\n';
    std::cout << "max size of my_ints: " << my_ints.max_size() << "\n\n";

    // Prueba del método empty.
    mySTL::array<int, 0> first;
    mySTL::array<int, 5> second;
    
    std::cout << "first " << (first.empty() ? "is empty" : "is not empty") << '\n';
    std::cout << "second " << (second.empty() ? "is empty" : "is not empty") << "\n\n";

    // Prueba del operador sobrecargado [].
    mySTL::array<int, 10> my_array3;
    
    unsigned int index;

    // Asignar valores.
    for ( index = 0; index < 10; ++index )
        my_array3[index] = index;

    
    std::cout << "Probando operador []: \n";

    // Imprimir contenido.
    std::cout << "my_array3 contains: { ";
    for ( index = 0; index < 10; ++index )
        std::cout << my_array3[index] << (index < 9 ? ", " : " }\n\n");

    // Prueba del método at.
    mySTL::array<int, 10> my_array4;

    for ( index = 0; index < 10; ++index )
        my_array4.at(index) = index + 1;

    std::cout << "Probando metodo at: \n";

    std::cout << "my_array4 contains: { ";
    for ( index = 0; index < 10; ++index )
        std::cout << my_array4.at(index) << (index < 9 ? ", " : " }\n\n");

    // Prueba de los métodos front y back.
    mySTL::array<int, 3> my_array5 = { 2, 16, 77 };

    std::cout << "Probando metodos front y back: \n";

    std::cout << "front is: " << my_array5.front() << '\n';
    std::cout << "back is: " << my_array5.back() << '\n';

    my_array5.front() = 100;
    my_array5.back() = 50;

    std::cout << "my_array5 now contains:";
    for ( int& element : my_array5 )
        std::cout << ' ' << element;
    std::cout << "\n\n";

    // Prueba del método data.

    std::cout << "Probando metodo data: \n";

    const char* char_string = "Test string";
    mySTL::array<char, 12> char_array;

    std::memcpy(char_array.data(), char_string, 12); // Copiar char_string al arreglo de caracteres.

    std::cout << "char_array contains: " << char_array.data() << "\n\n";

    // Prueba del método fill.

    mySTL::array<int, 6> my_array6;

    std::cout << "Llenando el array con 5 usando el metodo fill: \n";
    my_array6.fill(5);

    std::cout << "myarray6 contains:";
    for ( int& element : my_array6 )
        std::cout << ' ' << element;
    std::cout << "\n\n";

    // Prueba del método swap.

    mySTL::array<int, 5> my_array7 = { 10, 20, 30, 40, 50 };
    mySTL::array<int, 5> my_array8 = { 11, 22, 33, 44, 55 };

    std::cout << "Intercambiando dos arrays usando el metodo swap: \n";
    my_array7.swap(my_array8);

    std::cout << "my_array7:";
    for ( int& element : my_array7 )
        std::cout << ' ' << element;
    std::cout << '\n';

    std::cout << "my_array8:";
    for ( int& element : my_array8 )
        std::cout << ' ' << element;
    std::cout << "\n\n";

    // Prueba de los operadores relacionales.

    mySTL::array<int, 5> a = { 10, 20, 30, 40, 50 };
    mySTL::array<int, 5> b = { 10, 20, 30, 40, 50 };
    mySTL::array<int, 5> c = { 50, 40, 30, 20, 10 };

    std::cout << "Probando operadores relacionales: \n";

    if ( a == b ) std::cout << "a and b are equal\n";
    if ( b != c ) std::cout << "b and c are not equal\n";
    if ( b < c )  std::cout << "b is less than c\n";
    if ( c > b )  std::cout << "c is greater than b\n";
    if ( a <= b ) std::cout << "a is less than or equal to b\n";
    if ( a >= b ) std::cout << "a is greater than or equal to b\n\n";

    // Prueba de la función get.

    mySTL::array<int, 3> my_array9 = { 10, 20, 30 };

    std::cout << "Probando la funcion get: \n";

    std::cout << "first element in my_array9: " << mySTL::get<0>(my_array9) << "\n\n";

    // Prueba de la función swap.

    std::cout << "Probando la funcion swap: \n";

    mySTL::swap(a, c);

    std::cout << "a contains: { ";
    for ( index = 0; index < 5; ++index )
        std::cout << a[index] << (index < 4 ? ", " : " }\n");

    std::cout << "c contains: { ";
    for ( index = 0; index < 5; ++index )
        std::cout << c[index] << (index < 4 ? ", " : " }\n\n");

    std::cout << "Funciono crack xd\n";

    return 0;
}
