# Array - Diseño y documentación externa

El *Array* es un contenedor de tamaño fijo que almacena una secuencia de elementos.

## Propiedades del contenedor

* **Secuencia**: Los elementos en contenedores secuenciales son ordenados en una secuencia estrictamente lineal. Los elementos individuales se acceden según su posición en esta secuencia.
* **Almacenamiento contiguo**: Los elementos son almacenados en ubicaciones de memoria contiguas, permitiendo el acceso aleatorio en tiempo constante a elementos. Los punteros a un elemento pueden desplazarse para acceder a otros elementos.
* **Agregado de tamaño fijo**: El contenedor utiliza constructores y destructores implícitos para asignar el espacio requerido de manera estática. Su tamaño es consante en tiempo de compilación. No hay sobrecarga (*overhead*) de memoria o de tiempo.

## Parámetros de plantilla

* **ValueType**: El tipo de los elementos contenidos. Identificado como el tipo miembro `array::value_type`.
* **Size**: Tamaño del *array*, en términos de cantidad de elementos.

## Tipos miembro

| Tipo miembro | Definición |
| -------- | -------- |
| `value_type`     | Primer parámetro de plantilla    |
| `size_type`     | `std::size_t` |
| `difference_type`     | `std::ptrdiff_t` |
| `reference`    | `value_type&` |
| `const_reference`     | `const value_type&` |
| `pointer`     | `value_type*` |
| `const_pointer`     | `const value_type*` |
| `iterator`     | `value_type*`  |
| `const_iterator`     | `const value_type*` |
| `reverse_iterator`     | `std::reverse_iterator<iterator>`    |
| `const_reverse_iterator` | `std::reverse_iterator<const_iterator>` |

## Métodos

### Funciones implícitamente definidas

* Constructor
* Destructor
* `operator=`

### Iteradores

1. #### begin

**Retorna un iterador al principio.**

Retorna un iterador que apunta al primer elemento del contenedor.

En contenedores vacíos, este método retorna lo mismo que `array::end`. Este iterador retornado no debe ser desreferenciado.

* **Parámetros**: Ninguno.
* **Retorna**: Un iterador al principio de la secuencia. Si el objeto *array* es const, el método retorna un `const_iterator`. De lo contrario, retorna un `iterator`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
iterator begin() noexcept;
const_iterator begin() const noexcept;
```

2. #### cbegin

**Retorna un *const_iterator* al principio.**

Retorna un `const_iterator` que apunta al primer elemento del contenedor.

Un `const_iterator` es un iterador que apunta a contenido constante. Este iterador se puede aumentar o disminuir (a menos de que sea también constante), igual que el iterador que retorna `array::begin`, pero no puede usarse para modificar el contenido al que apunta, incluso si el objeto *array* no es constante.

* **Parámetros**: Ninguno.
* **Retorna**: Un `const_iterator` al principio de la secuencia.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
const_iterator cbegin() const noexcept;
```

3. #### end

**Retorna un iterador al final.**

Retorna un iterador que apunta al elemento siguiente al último del contenedor.

Este elemento sirve como un marcador de posición. No apunta a ningún elemento y por lo tanto no debe ser desreferenciado.

* **Parámetros**: Ninguno.
* **Retorna**: Un iterador al elemento siguiente al último de la secuencia. Si el objeto *array* es const, el método retorna un `const_iterator`. De lo contrario, retorna un `iterator`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
iterator end() noexcept;
const_iterator end() const noexcept;
```

4. #### cend

**Retorna un *const_iterator* al final.**

Retorna un `const_iterator` que apunta al elemento siguiente al último del contenedor.

* **Parámetros**: Ninguno.
* **Retorna**: Un `const_iterator` a la ubicación siguiente a la última de la secuencia.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
const_iterator cend() const noexcept;
```

![Representación de los iteradores begin y end del contenedor *array*.](https://user-images.githubusercontent.com/64336377/102536845-c7a5a000-406f-11eb-8b4b-f2dd481bed03.png "Representación de los iteradores begin y end del contenedor array")
Representación de los iteradores begin y end del contenedor.

5. #### rbegin

**Retorna un iterador inverso al principio del contenedor invertido.**

Retorna un iterador inverso que apunta al último elemento del contenedor no invertido.

Este método apunta al elemento anterior al que sería apuntado por el método `array::end`.

* **Parámetros**: Ninguno.
* **Retorna**: Un iterador inverso al principio de la secuencia invertida.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
reverse_iterator rbegin() noexcept;
const_reverse_iterator rbegin() const noexcept;
```

6. #### crbegin

**Retorna un *const_reverse_iterator* al principio del contenedor invertido.**

Retorna un `const_reverse_iterator` que apunta al último elemento del contenedor no invertido.

Un `const_reverse_iterator` es un iterador que apunta a contenido constante e itera en orden inverso. Este iterador se puede aumentar o disminuir (a menos de que sea también constante), igual que el iterador que retorna `array::rbegin`, pero no puede usarse para modificar el contenido al que apunta.

* **Parámetros**: Ninguno.
* **Retorna**: Un `const_reverse_iterator` al principio de la secuencia invertida.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
const_reverse_iterator crbegin() const noexcept;
```

7. #### rend

**Retorna un iterador inverso al final del contenedor invertido.**

Retorna un iterador inverso que apunta al elemento anterior al primer elemento del contenedor no invertido.

* **Parámetros**: Ninguno.
* **Retorna**: Un iterador inverso al final de la secuencia invertida.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
reverse_iterator rend() noexcept;
const_reverse_iterator rend() const noexcept;
```

8. #### crend

**Retorna un *const_reverse_iterator* al final del contenedor invertido.**

Retorna un `const_reverse_iterator` que apunta al elemento anterior al primer elemento del contenedor no invertido.

* **Parámetros**: Ninguno.
* **Retorna**: Un `const_reverse_iterator` al final de la secuencia invertida.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
const_reverse_iterator crend() const noexcept;
```

![Representación de los iteradores normales e inversos del contenedor *array*.](https://user-images.githubusercontent.com/64336377/102555526-925b7b00-408c-11eb-900d-bd27dd3621dd.png "Representación de los iteradores normales e inversos del contenedor array")
Representación de los iteradores normales e inversos del contenedor. El iterador inverso almacena un iterador al siguiente elemento del elemento al que realmente apunta.

### Capacidad

1. #### size

**Retorna el tamaño del contenedor.**

Retorna el número de elementos del contenedor.

El tamaño de un objeto *array* es siempre igual al segundo parámetro de plantilla que se utiliza para instanciar la clase de plantilla *array* (Size).

* **Parámetros**: Ninguno.
* **Retorna**: La cantidad de elementos contenidos en el objeto *array*. Esta es una expresión constante de tiempo de compilación (`constexpr`).
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
constexpr size_type size() noexcept;
```

2. #### max_size

**Retorna el tamaño máximo del contenedor.**

Retorna la cantidad máxima de elementos que puede contener el contenedor.

El tamaño de un objeto *array*, al igual que su tamaño, es siempre igual al segundo parámetro de plantilla que se utiliza para instanciar la clase de plantilla *array* (Size).

* **Parámetros**: Ninguno.
* **Retorna**: El número máximo de elementos que puede tener el objeto como contenido. Esta es una expresión constante de tiempo de compilación (`constexpr`).
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
constexpr size_type max_size() noexcept;
```

3. #### empty

**Verifica si el contenedor está vacío.**

Retorna un valor `bool` que indica si el contenedor está vacío, es decir, cuando su tamaño es igual a 0.

* **Parámetros**: Ninguno.
* **Retorna**: `true` si el tamaño del *array* es 0; de lo contrario, `false`. Esta es una expresión constante de tiempo de compilación (`constexpr`).
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
constexpr bool empty() noexcept;
```

![Ejemplo de valores de retorno de los métodos de capacidad del contenedor *array*.](https://user-images.githubusercontent.com/64336377/102669134-5f2fef00-4153-11eb-82e8-db3ba558fd7b.png "Ejemplo de valores de retorno de los métodos de capacidad del contenedor array")
Ejemplo de valores de retorno de los métodos de capacidad del contenedor *array*, con un objeto *array* que contiene siete elementos.

### Acceso a elementos

1. #### operator[]

**Accede a un elemento.**

Retorna una referencia al elemento en la posición *index* del contenedor.

A diferencia de `array::at`, este método no hace comprobación de límites.

* **Parámetros**:
  * *index*: Posición de un elemento en el *array*. 
* **Retorna**: El elemento en la posición especificada del *array*. Si el objeto *array* es const, el método retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
reference operator[](size_type index);
const_reference operator[](size_type index) const;
```

![Ejemplos de valores de retorno según la posición usando el operador []](https://user-images.githubusercontent.com/64336377/102681062-f319af00-4183-11eb-932d-95ce40fb0e2c.png "Ejemplos de valores de retorno según la posición usando el operador []")
Ejemplo de uso del operador **[]** en dos posiciones de un objeto *array* llamado *my_array*.

2. #### at

**Accede a un elemento.**

Retorna una referencia al elemento en la posición *index* del contenedor.

Este método verifica automáticamente si *index* se encuentra dentro del límite de elementos válidos en el contenedor. Si no lo está, lanza una excepción `std::out_of_range`.

* **Parámetros**:
  * *index*: Posición de un elemento en el *array*. Si es mayor o igual al tamaño del contenedor, se lanza una excepción `std::out_of_range`.
* **Retorna**: El elemento en la posición especificada del *array*. Si el objeto *array* es const, el método retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: `std::out_of_range` si *index* está fuera de los límites del contenedor.
* **Declaración**:

```C++
reference at(size_type index);
const_reference at(size_type index) const;
```

![Ejemplos de valores de retorno según la posición usando el método at.](https://user-images.githubusercontent.com/64336377/102701491-24928900-421d-11eb-92b2-f39fab768e23.png "Ejemplos de valores de retorno según la posición usando el método at")
Ejemplo de uso del método *at* en dos posiciones de un objeto *array* llamado *my_array*.

3. #### front

**Accede al primer elemento.**

Retorna una referencia al primer elemento en el contenedor.

El llamado a este método en un contenedor vacío tiene comportamiento indefinido.

* **Parámetros**: Ninguno.
* **Retorna**: Una referencia al primer elemento del contenedor. Si el objeto *array* es const, el método retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
reference front();
const_reference front() const;
```

4. #### back

**Accede al último elemento.**

Retorna una referencia al último elemento en el contenedor.

El llamado a este método en un contenedor vacío tiene comportamiento indefinido.

* **Parámetros**: Ninguno.
* **Retorna**: Una referencia al último elemento del contenedor. Si el objeto *array* es const, el método retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
reference back();
const_reference back() const;
```

5. #### data

**Obtiene un puntero a los datos.**

Retorna un puntero al primer elemento del contenedor.

Como los elementos en el *array* están almacenados en ubicaciones contiguas de memoria, el puntero que se retorna puede ser desplazado para acceder a cualquier elemento del contenedor.

* **Parámetros**: Ninguno.
* **Retorna**: Un puntero a los datos contenidos por el objeto. Si el objeto *array* es const, el método retorna un puntero a un const `value_type`. De lo contrario, retrona un puntero a `value_type`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
value_type* data() noexcept;
const value_type* data() const noexcept;
```

![Representación de los métodos `front`, `back` y `data`.](https://user-images.githubusercontent.com/64336377/102703032-544a8c80-422f-11eb-8631-6416d8cf797c.png "Representación de los métodos front, back y data")
Representación de los métodos `front`, `back` y `data` en el contenedor *array*.

### Modificadores

1. #### fill

**Rellena el *array* con un valor.**

Asigna *value* como el valor para todos los elementos del objeto *array*.

* **Parámetros**:
  * *value*: Valor para rellenar el *array*. 
* **Retorna**: Nada.
* **Complejidad**: Lineal. Realiza tantas operaciones de asignación como el tamaño del objeto *array*.
* **Excepciones**: *Basic exception safety*. Se lanza una excepción si así lo hace la asignación de algún elemento.
* **Declaración**:

```C++
void fill(const value_type& value);
```

![Ejemplo del método fill.](https://user-images.githubusercontent.com/64336377/102823046-c215c600-439f-11eb-9f2f-a18775864584.png "Ejemplo del método fill")
Ejemplo del método `fill` con el valor 7 en un objeto *array* llamado *my_array*. 

2. #### swap

**Intercambia contenido.**

Intercambia el contenido de el objeto *array* por el de *other*, que es otro objeto *array* del mismo tipo y el mismo tamaño.

Después del llamado a este método, los elementos en el contenedor son los que estaban anteriormente en *other*, y los elementos en *other* son los que estaban en el contenedor que llama al método.

A diferencia de los métodos `swap` de los otros contenedores, este método opera en tiempo lineal, realiza tantas operaciones individuales de intercambio como el tamaño del contenedor.

* **Parámetros**:
  * *other*: Otro objeto *array* del mismo tipo y que incluye el mismo tamaño, para intercambiar sus elementos con los del *array*.
* **Retorna**: Nada.
* **Complejidad**: Lineal. Realiza tantas operaciones de intercambio como el tamaño del objeto *array*.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
void swap(array& other) noexcept(noexcept(std::swap(std::declval<value_type&>(), std::declval<value_type&>())));
```

![Representación del método `swap` del contenedor *array*.](https://user-images.githubusercontent.com/64336377/102830084-2e97c180-43ae-11eb-8f82-4a75976f398e.png "Representación del método swap del contenedor array")
Representación del método `swap` entre dos contenedores *array*.

## Sobrecarga de funciones no-miembro

### Operadores relacionales

Realiza la operación de comparación apropiada entre los contenedores *array* *lhs* y *rhs*.

La **comparación de igualdad** es realizada comparando los elementos secuencialmente usando el operador **==**, deteniéndose en la primera discordancia.

La **comparación "menor que"** compara los elementos secuencialmente usando el operador **<**, deteniéndose en la primera discordancia.

Las otras operaciones utilizan los operadores sobrecargados **==** y **<**. Se comportan de la siguiente manera:

| Operación | Operación equivalente |
|-----------|-----------------------|
| a != b    | !( a == b )           |
| a > b     | b < a                 |
| a <= b    | !( b < a )            |
| a >= b    | !( a < b )            |

* **Parámetros**:
  * *lhs*, *rhs*: Contenedores *array* (*left-hand side* y *right-hand side*) cuyos parámetros de plantilla son iguales.
* **Retorna**: `true` si se cumple la condición; de lo contrario `false`.
* **Complejidad**: Lineal según el tamaño de los contenedores.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
template <typename ValueType, std::size_t Size>
    bool operator==(const array<ValueType, Size>& lhs, const array<ValueType, Size>& rhs);
template <typename ValueType, std::size_t Size>
    bool operator!=(const array<ValueType, Size>& lhs, const array<ValueType, Size>& rhs);
template <typename ValueType, std::size_t Size>
    bool operator<(const array<ValueType, Size>& lhs, const array<ValueType, Size>& rhs);
template <typename ValueType, std::size_t Size>
    bool operator<=(const array<ValueType, Size>& lhs, const array<ValueType, Size>& rhs);
template <typename ValueType, std::size_t Size>
    bool operator>(const array<ValueType, Size>& lhs, const array<ValueType, Size>& rhs);
template <typename ValueType, std::size_t Size>
    bool operator>=(const array<ValueType, Size>& lhs, const array<ValueType, Size>& rhs);
```

### get

**Obtiene elemento (interfaz *tuple*).**

Retorna una referencia al *Index*-ésimo elemento del *array*.

Esta sobrecarga de la función homónima *get* de *tuple* es proporcionada para que objetos *array* puedan ser tratados como *tuples*. Para ese propósito, el encabezado también sobrecarga los tipos `tuple_size` y `tuple_element` con los miembros apropiados definidos.

* **Parámetros de plantilla**:
  * *Index*: Posición de un elemento en el *array*, con 0 como la posición del primer elemento.
  * *ValueType*: El tipo de los elementos contenidos en el *array* (generalmente obtenido implícitamente desde el parámetro *arr*).
  * *Size*: Tamaño del *array*, en términos del número de elementos (generalmente obtenido implícitamente desde el parámetro *arr*).
* **Parámetros**:
  * *arr*: Un contenedor *array*.
* **Retorna**: Una referencia a la posición especificada del array.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
template <std::size_t Index, typename ValueType, std::size_t Size>
    ValueType& get(array<ValueType, Size>& arr) noexcept;
template <std::size_t Index, typename ValueType, std::size_t Size>
    ValueType&& get(array<ValueType, Size>&& arr) noexcept;
template <std::size_t Index, typename ValueType, std::size_t Size>
    const ValueType& get(const array<ValueType, Size>& arr) noexcept;
```

### swap

**Intercambia contenido de *arrays*.**

Intercambia el contenido de *array_one* por el de *array_two*. Ambos objetos *array* deben ser del mismo tipo y el mismo tamaño.

Después del llamado a este método, los elementos en *array_one* son los que estaban anteriormente en *array_two*, y los elementos en *array_two* son los que estaban en *array_one*.

* **Parámetros**:
  * *array_one*, *array_two*: Contenedores *array* del mismo tipo y tamaño.
* **Retorna**: Nada.
* **Complejidad**: Lineal. Realiza tantas operaciones de intercambio como el tamaño de los objetos *array*.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
template <typename ValueType, std::size_t Size>
    void swap(array<ValueType, Size>& array_one, array<ValueType, Size>& array_two);
```

## Especializaciones de clases no-miembro

### tuple_size

**Rasgos de *tuple_size* para *array*.**

Accede al tamaño de un objeto *array* como si fuera un *tuple*.

Esta clase proporciona el tamaño del *array* como el miembro *constexpr* `value` de `std::size_t`, heredando de `std::integral_constant`.

* **Parámetros de plantilla**:
  * *TupleType*: El tipo del que se obtiene el tamaño.
  * *ValueType*: El tipo de elementos contenidos.
  * *Size*: El tamaño del *array*, en términos del número de elementos.
* **Declaración**:

```C++
template <typename TupleType>
    class tuple_size;
template <typename ValueType, std::size_t Size>
    struct tuple_size<array<ValueType, Size>>
        : std::integral_constant<std::size_t, Size>;
{
};
```

### tuple_element

**Tipo *tuple_element* para *array*.**

Accede al tipo estático de los elementos de un objeto *array* como si fuera un *tuple*.

* **Parámetros de plantilla**:
  * *Index*: El número del elemento dentro del *array*.
  * *TupleType*: El tipo *tuple*.
  * *ValueType*: El tipo de elementos contenidos.
  * *Size*: El tamaño del *array*, en términos del número de elementos.
* **Declaración**:

```C++
template <std::size_t Index, typename TupleType>
    class tuple_element;
template <std::size_t Index, typename ValueType, std::size_t Size>
    struct tuple_element<Index, array<ValueType, Size>>
{
    typedef ValueType type;
};
```