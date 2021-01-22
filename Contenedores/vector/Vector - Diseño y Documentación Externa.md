# Vector - Diseño y documentación externa

El *vector* es un contenedor secuencial que representa un arreglo que puede cambiar su tamaño.

**Notas**:

* Hace falta documentar los métodos privados.
* Por el uso de algunas bibliotecas del `std`, este contenedor debe ser compilado usando C++ 17 (Revisar constructor de rango, método assign con rango, método insert con rango y la tercera referencia del documento). Probablemente los demás contenedores tengan que ser compilados de esta manera.

## Propiedades del contenedor

* **Secuencia**: Los elementos en contenedores secuenciales son ordenados en una secuencia estrictamente lineal. Los elementos individuales se acceden según su posición en esta secuencia.
* **Arreglo dinámico**: Permite el acceso directo a cualquier elemento en la secuencia, incluso a través de aritmética de punteros, y proporciona relativamente rápida agregación y eliminación de elementos al final de la secuencia.

## Parámetros de plantilla

* **ValueType**: El tipo de los elementos contenidos. Identificado como el tipo miembro `vector::value_type`.

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

## Atributos privados

* *count*: El número de elementos almacenados actualmente en el contenedor.
* *storage_capacity*: La capacidad de almacenamiento asignada actualmente para el contenedor, expresada en términos del número de elementos.
* *elements*: Contenedor interno para guardar elementos de tipo *ValueType*.

## Métodos públicos

### Constructores

1. #### Constructor por defecto

Construye un contendor vacío, sin elementos.

* **Parámetros**: Ninguno.
* **Complejidad**: Constante.
* **Declaración**:

```C++
vector();
```

2. #### Constructor de relleno

Construye el contenedor con *count* elementos. Si se recibe *value* como parámetro, cada elemento es una copia de *value*.

* **Parámetros**:
    * *count*: El tamaño inicial del contenedor.
    * *value*: Valor para inicializar los elementos del contenedor.
* **Complejidad**: Lineal en el tamaño del contenedor.
* **Declaración**:

```C++
explicit vector(size_type count);
vector(size_type count, const value_type& value);
```

3. #### Constructor de rango

Construye el contenedor con tantos elementos como el rango [*first*, *last*).

* **Parámetros**:
    * *first*, *last*: Iteradores a las posiciones inicial y final en un rango.
* **Complejidad**: Lineal en el tamaño del contenedor.
* **Declaración**:

```C++
template <typename InputIterator,
    typename = typename std::enable_if_t<std::is_base_of_v<std::input_iterator_tag,
        typename std::iterator_traits<InputIterator>::iterator_category>>>
    vector(InputIterator first, InputIterator last);
```
El uso de `std::enable_if_t` etc es para poder identificar correctamente si el parámetro recibido es un iterador, de lo contrario el programa se vuelve loco xd.

4. #### Constructor de copia

Construye el contenedor con una copia de cada uno de los elementos de *other*, en el mismo orden.

* **Parámetros**:
    * *other*: Otro objeto vector del mismo tipo, para inicializar el contenedor.
* **Complejidad**: Lineal en el tamaño del contenedor.
* **Declaración**:

```C++
vector(const vector& other);
```

5. #### Constructor de movimiento

Construye un contenedor que adquiere los elementos de *temp*, con semánticas de movimiento.

* **Parámetros**:
    * *temp*: Otro objeto vector del mismo tipo, para inicializar el contenedor.
* **Complejidad**: Constante.
* **Declaración**:

```C++
vector(vector&& temp);
```

6. #### Constructor con lista de inicialización

Construye el contenedor con una copia de cada uno de los elementos de *init_list*.

* **Parámetros**:
    * *init_list*: Objeto `std::initializer_list` para inicializar los elementos del contenedor.
* **Complejidad**: Lineal en el tamaño del contenedor.
* **Declaración**:

```C++
vector(std::initializer_list<value_type> init_list);
```

### Destructor

Destruye el objeto contenedor.

* **Declaración**:

```C++
~vector();
```

### operator=

**Asigna contenido.**

Asigna nuevo contenido al contenedor. Reemplaza su contenido actual y modifica su tamaño.

1. #### Asignación por copia

Reemplaza el contenido del contenedor por una copia del contenido de *other*.

* **Parámetros**:
    * *other*: Otro objeto vector del mismo tipo, para reemplazar el contenido del contenedor.
* **Retorna**: `*this`
* **Complejidad**: Lineal según el tamaño del contenedor.
* **Declaración**:

```C++
vector& operator=(const vector& other);
```

2. #### Asignación por movimiento

Reemplaza el contenido del contenedor al mover dentro de este el contenido de *temp*.

* **Parámetros**:
    * *temp*: Otro objeto vector del mismo tipo, para reemplazar el contenido del contenedor.
* **Retorna**: `*this`
* **Complejidad**: Lineal según el tamaño del contenedor.
* **Declaración**:

```C++
vector& operator=(vector&& temp);
```

3. #### Asignación por lista de inicialización

Reemplaza el contenido del contenedor por una copia de los elementos de *init_list*.

* **Parámetros**:
    * *init_list*: Objeto `std::initializer_list` para reemplazar el contenido del contenendor.
* **Retorna**: `*this`
* **Complejidad**: Lineal según el tamaño del contenedor.
* **Declaración**:

```C++
vector& operator=(std::initializer_list<value_type> init_list);
```

### assign

**Asigna contenido.**

Asigna nuevo contenido al contenedor. Reemplaza su contenido actual y modifica su tamaño.

1. #### Asignación con relleno

Reemplaza el contenido del contenedor con *count* elementos, donde cada elemento es una copia de *value*.

* **Parámetros**:
    * *count*: El nuevo tamaño del contenedor.
    * *value*: Valor para asignar los elementos del contenedor.
* **Retorna**: Nada.
* **Complejidad**: Lineal según el tamaño del contenedor.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
void assign(size_type count, const value_type& value);
```

2. #### Asignación con rango

Reemplaza el contenido del objeto con elementos en el rango [*first*, *last*).

* **Parámetros**:
    * *first*, *last*: Iteradores a las posiciones inicial y final en un rango.
* **Complejidad**: Lineal según el tamaño del contenedor.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
template <typename InputIterator,
    typename = typename std::enable_if_t<std::is_base_of_v<std::input_iterator_tag,
                typename std::iterator_traits<InputIterator>::iterator_category>>>
    void assign(InputIterator first, InputIterator last);
```

3. #### Asignación por lista de inicialización

Reemplaza el contenido del contenedor por una copia de los elementos de *init_list*.

* **Parámetros**:
    * *init_list*: Objeto `std::initializer_list` para reemplazar el contenido del contenendor.
* **Retorna**: `*this`
* **Complejidad**: Lineal según el tamaño del contenedor.
* **Declaración**:

```C++
void assign(std::initializer_list<value_type> init_list);
```

### Iteradores

1. #### begin

**Retorna un iterador al principio.**

Retorna un iterador que apunta al primer elemento del contenedor.

En contenedores vacíos, este método retorna lo mismo que `vector::end`. Este iterador retornado no debe ser desreferenciado.

* **Parámetros**: Ninguno.
* **Retorna**: Un iterador al principio de la secuencia. Si el objeto *vector* es const, el método retorna un `const_iterator`. De lo contrario, retorna un `iterator`.
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

Un `const_iterator` es un iterador que apunta a contenido constante. Este iterador se puede aumentar o disminuir (a menos de que sea también constante), igual que el iterador que retorna `vector::begin`, pero no puede usarse para modificar el contenido al que apunta, incluso si el objeto *vector* no es constante.

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
* **Retorna**: Un iterador al elemento siguiente al último de la secuencia. Si el objeto *vector* es const, el método retorna un `const_iterator`. De lo contrario, retorna un `iterator`.
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

![Representación de los iteradores begin y end del contenedor *vector*.](https://user-images.githubusercontent.com/64336377/103161183-9f225200-47a3-11eb-8d6a-8dd9643bd11d.png "Representación de los iteradores begin y end del contenedor vector")
Representación de los iteradores begin y end del contenedor.

5. #### rbegin

**Retorna un iterador inverso al principio del contenedor invertido.**

Retorna un iterador inverso que apunta al último elemento del contenedor no invertido.

Este método apunta al elemento anterior al que sería apuntado por el método `vector::end`.

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

Un `const_reverse_iterator` es un iterador que apunta a contenido constante e itera en orden inverso. Este iterador se puede aumentar o disminuir (a menos de que sea también constante), igual que el iterador que retorna `vector::rbegin`, pero no puede usarse para modificar el contenido al que apunta.

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

![Representación de los iteradores normales e inversos del contenedor *vector*.](https://user-images.githubusercontent.com/64336377/103161235-4acba200-47a4-11eb-8a1b-bed86acf283a.png "Representación de los iteradores normales e inversos del contenedor vector")
Representación de los iteradores normales e inversos del contenedor. El iterador inverso almacena un iterador al siguiente elemento del elemento al que realmente apunta.

### Capacidad

1. #### size

**Retorna el tamaño del contenedor.**

Retorna el número de elementos del contenedor.

Este número no necesariamente es igual a la capacidad de almacenamiento del contenedor.

* **Parámetros**: Ninguno.
* **Retorna**: La cantidad de elementos del contenedor.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
size_type size() const noexcept;
```

2. #### max_size

**Retorna el tamaño máximo del contenedor.**

Retorna la cantidad máxima de elementos que puede contener el contenedor.

Este es el tamaño máximo potencial que puede alcanzar el contenedor por limitaciones del sistema, pero no está garantizado que el contenedor sea capaz de alcanzar ese tamaño. Asignar almacenamiento en cualquier punto antes de ese tamaño podría fallar de igual forma.

Fórmula, basada en el valor que retorna el método en mi computadora: (2^63)/(Tamaño en bytes de *ValueType*) - 1

* **Parámetros**: Ninguno.
* **Retorna**: El número máximo de elementos que puede tener el contenedor como contenido.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
size_type max_size() const noexcept;
```

3. #### resize

**Cambia el tamaño del contenedor.**

Cambia el tamaño del contenedor para *count* elementos.

Si *count* es menor al tamaño actual del contenedor, el contenido se reduce a sus primeros *count* elementos, de forma que se eliminan y destruyen los elementos sobrantes.

Si *count* es mayor al tamaño actual del contenedor, el contenido se expande y se insertan al final los elementos necesarios para alcanzar un tamaño de *count* elementos. Si se especifica *value*, los nuevos elementos se inicializan como copias de *value*.

Si *count* es también mayor a la capacidad actual del contenedor, se hace una reasignación automática del almacenamiento asignado.

* **Parámetros**:
    * *count*: El nuevo tamaño del contenedor.
    * *value*: El valor para inicializar los elementos añadidos en caso de que *count* sea mayor al tamaño actual del contenedor.
* **Retorna**: Nada.
* **Complejidad**: Lineal en el número de elementos insertados o eliminados.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
void resize(size_type count);
void resize(size_type count, value_type value);
```

![Ejemplo de uso del método `resize` del *vector*.](https://user-images.githubusercontent.com/64336377/103180315-46fa5700-485a-11eb-997a-3a0295b0e2a3.png "Ejemplo de uso del método resize del vector")
Ejemplo de uso del método `resize` en un vector de números enteros. Primero se cambia el tamaño a tres elementos y automáticamente se eliminan los sobrantes. Luego se cambia a seis elementos, y a los nuevos se les asigna el número 20. El tamaño en estos casos no supera a la capacidad, por lo tanto esta no cambia y no hay necesidad de reasignar el almacenamiento.

4. #### capacity

**Retorna la capacidad asignada de almacenamiento.**

Retorna el tamaño del espacio de almacenamiento asignado actualmente para el *vector*, expresado en términos del número de elementos.

Esta capacidad no necesariamente es igual al tamaño del *vector*. Puede ser igual o mayor, con espacio extra que permite acomodar el crecimiento sin necesidad de reasignar en cada inserción.

Esta capacidad no representa un límite en el tamaño del *vector*. Cuando se agota y se necesita más, es expandida automáticamente por el contenedor (reasigna el espacio de almacenamiento). El límite teórico del tamaño del *vector* es dado por `vector::max_size`.

* **Parámetros**: Ninguno.
* **Retorna**: El tamaño de la capacidad de almacenamiento asignada en el *vector*, en términos del número de elementos que puede contener.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
size_type capacity() const noexcept;
```

5. #### empty

**Verifica si el contenedor está vacío.**

Retorna un valor `bool` que indica si el contenedor está vacío, es decir, cuando su tamaño es igual a 0.

* **Parámetros**: Ninguno.
* **Retorna**: `true` si el tamaño del *array* es 0; de lo contrario, `false`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
bool empty() const noexcept;
```

6. #### reserve

**Solicita un cambio en la capacidad del contenedor.**

Solicita un cambio en la capacidad del vector para que sea lo suficiente como para contener *new_capacity* elementos.

Si *new_capacity* es mayor a la capacidad actual del *vector*, se reasigna el almacenamiento del contenedor de forma que se incremente su capacidad hasta *new_capacity* o más.

En cualquier otro caso, el llamado a este método no hace una reasignación y la capacidad del *vector* no se ve afectada.

* **Parámetros**:
    * *new_capacity*: La nueva capacidad del *vector*.
* **Retorna**: Nada.
* **Complejidad**: Si se hace una reasignación, la complejidad es lineal en el tamaño del *vector* a lo sumo.
* **Excepciones**: `std::length_error` si *new_capacity* es mayor que `vector::max_size`.
* **Declaración**:

```C++
void reserve(size_type new_capacity);
```

![Ejemplo de uso de los métodos `reserve` y `empty` del *vector*.](https://user-images.githubusercontent.com/64336377/103180550-19161200-485c-11eb-8c5d-2b7a5ddc23e8.png "Ejemplo de uso de los métodos reserve y empty del vector")
Ejemplo de uso de los métodos `reserve` y `empty` del *vector*. El método `reserve` en este caso se utiliza para aumentar la capacidad del contenedor para que quepan siete elementos. Como no se ha agregado ningún elemento, el método `empty` retorna `true`.

7. #### shrink_to_fit

**Encogimiento para encajar con el tamaño del contenedor.**

Solicita reducir la capacidad del contenedor para encajar con el número de elementos.

* **Parámetros**: Ninguno.
* **Retorna**: Nada.
* **Complejidad**: Lineal en el tamaño del contenedor a lo sumo.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
void shrink_to_fit();
```

![Ejemplo del método `shrink_to_fit` del *vector*](https://user-images.githubusercontent.com/64336377/103181210-79a94d00-4864-11eb-9b68-3d6ecab81866.png "Ejemplo del método shrink_to_fit del vector")
Ejemplo del método `shrink_to_fit` del *vector*. En la imagen, el contenedor superior tiene capacidad para ocho elementos, pero solo posee cinco elementos. Después del llamado al método, el almacenamiento es reasignado a un contenedor con capacidad para cinco elementos.

![Mismo ejemplo, se detalla lo que sucede por dentro al llamar a los métodos que reasignan el almacenamiento y copian los elementos.](https://user-images.githubusercontent.com/64336377/103468337-b621f080-4d1d-11eb-978c-4bbfd215f0f0.png "Mismo ejemplo, se detalla lo que sucede por dentro al llamar a los métodos que reasignan el almacenamiento y copian los elementos")
Detalle de lo que realiza el método `shrink_to_fit` al llamar al método que se encarga de reasignar el almacenamiento a un contenedor que posea *count* elementos.

### Acceso a elementos

1. #### operator[]

**Accede a un elemento.**

Retorna una referencia al elemento en la posición *index* del contenedor.

A diferencia de `vector::at`, este método no hace comprobación de límites.

* **Parámetros**:
    * *index*: Posición de un elemento en el contenedor.
* **Retorna**: El elemento en la posición especificada del *vector*. Si el objeto *vector* es const, el método retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
reference operator[](size_type index);
const_reference operator[](size_type index) const;
```

![Ejemplos de valores de retorno según la posición usando el operador []](https://user-images.githubusercontent.com/64336377/103181427-69926d00-4866-11eb-9f47-3a6980421727.png "Ejemplos de valores de retorno según la posición usando el operador []")
Ejemplo de uso del operador **[]** en dos posiciones de un objeto *vector* llamado *my_vector*.

2. #### at

**Accede a un elemento.**

Retorna una referencia al elemento en la posición *index* del contenedor.

Este método verifica automáticamente si *index* se encuentra dentro del límite de elementos válidos en el contenedor. Si no lo está, lanza una excepción `std::out_of_range`.

* **Parámetros**:
  * *index*: Posición de un elemento en el contenedor. Si es mayor o igual al tamaño del contenedor, se lanza una excepción `std::out_of_range`.
* **Retorna**: El elemento en la posición especificada del *vector*. Si el objeto *vector* es const, el método retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: `std::out_of_range` si *index* está fuera de los límites del contenedor.
* **Declaración**:

```C++
reference at(size_type index);
const_reference at(size_type index) const;
```

![Ejemplos de valores de retorno según la posición usando el método at.](https://user-images.githubusercontent.com/64336377/103181468-dd347a00-4866-11eb-81b3-2c830a01300b.png "Ejemplos de valores de retorno según la posición usando el método at")
Ejemplo de uso del método *at* en dos posiciones de un objeto *vector* llamado *my_vector*.

3. #### front

**Accede al primer elemento.**

Retorna una referencia al primer elemento en el contenedor.

El llamado a este método en un contenedor vacío tiene comportamiento indefinido.

* **Parámetros**: Ninguno.
* **Retorna**: Una referencia al primer elemento del contenedor. Si el objeto *vector* es const, el método retorna una `const_reference`. De lo contrario, retrona una `reference`.
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
* **Retorna**: Una referencia al último elemento del contenedor. Si el objeto *vector* es const, el método retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
reference back();
const_reference back() const;
```

5. #### data

**Obtiene un puntero a los datos.**

Retorna un puntero directo al arreglo de memoria usado internamente por el *vector* para guardar sus elementos.

Como los elementos en el *vector* están almacenados en ubicaciones contiguas de memoria, el puntero que se retorna puede ser desplazado para acceder a cualquier elemento del contenedor.

* **Parámetros**: Ninguno.
* **Retorna**: Un puntero al primer elemento en el arreglo usado internamente por el *vector*. Si el objeto *vector* es const, el método retorna un puntero a un const `value_type`. De lo contrario, retrona un puntero a `value_type`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
value_type* data() noexcept;
const value_type* data() const noexcept;
```

![Representación de los métodos `front`, `back` y `data`.](https://user-images.githubusercontent.com/64336377/103181578-21744a00-4868-11eb-809a-b25f016d3b59.png "Representación de los métodos front, back y data")
Representación de los métodos `front`, `back` y `data` en el contenedor *vector*.

### Modificadores

1. #### push_back

**Agrega un elemento al final.**

Agrega un nuevo elemento al final del *vector*, después de su último elemento actual. El contenido  de *value* se copia o se mueve al nuevo elemento.

Este método incrementa eficazmente en uno el tamaño del contenedor, lo que causa una reasignación automática si y solo si el nuevo tamaño del *vector* sobrepasa la capacidad actual.

* **Parámetros**:
    * *value*: El valor del elemento por agregar al contenedor.
* **Retorna**: Nada.
* **Complejidad**: Constante amortizada.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
void push_back(const value_type& value);
void push_back(value_type&& value);
```

2. #### pop_back

**Elimina el último elemento.**

Elimina el último elemento del contenedor y reduce eficazmente su tamaño en uno.

* **Parámetros**: Ninguno.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
void pop_back();
```

3. #### insert

**Inserta elementos.**

Inserta nuevos elementos en el *vector* en la posición anterior al elemento en *position* e incrementa eficazmente el tamaño del contenedor en el número de elementos insertados.

Este método causa una reasignación automática si y solo si el nuevo tamaño del *vector* sobrepasa la capacidad actual.

Como el *vector* utiliza un arreglo como almacenamiento interno, insertar elementos en posiciones distintas al final del contenedor provoca que el contenedor reasigne todos los elementos que estaban luego de esa posición a sus nuevas posiciones. Esto generalmente es una operación ineficiente comparada con métodos que realizan la misma operación en otros tipos de contenedores secuenciales (Como `list` o `forward_list`).

#### a) Único elemento

Inserta el valor *value* en la posición *position*.

* **Parámetros**:
    * *position*: Posición del contenedor donde se inserta el nuevo elemento.
    * *value*: El valor del elemento por insertar.
* **Declaración**:

```C++
iterator insert(const_iterator position, const value_type& value);
iterator insert(const_iterator position, value_type&& value);
```

#### b) Relleno

Inserta *count* copias de *value* en la posición @a position.

* **Parámetros**:
    * *position*: Posición del contenedor donde se inserta el primero de los nuevos elementos.
    * *count*: El número de elementos a insertar.
    * *value*: El valor del elemento por insertar
* **Declaración**:

```C++
iterator insert(const_iterator position, size_type count, const value_type& value);
```

#### c) Rango

Inserta elementos en el rango [*first*, *last*) en la posición *position*.

* **Parámetros**:
    * *position*: Posición del contenedor donde se inserta el primero de los nuevos elementos. 
    * *first*, *last*: Iteradores a las posiciones inicial y final en un rango.
* **Declaración**:

```C++
template <typename InputIterator,
    typename = typename std::enable_if_t<std::is_base_of_v<std::input_iterator_tag,
                typename std::iterator_traits<InputIterator>::iterator_category>>>>
    iterator insert(const_iterator position, InputIterator first, InputIterator last);
```

#### d) Lista de inicialización

Inserta cada uno de los elementos de *init_list* en la posición *position*.

* **Parámetros**:
    * *position*: Posición del contenedor donde se inserta el primero de los nuevos elementos. 
    * *init_list*: Objeto initializer_list desde donde se insertan los nuevos elementos.
* **Declaración**:

```C++
iterator insert(const_iterator position, std::initializer_list<value_type> init_list);
```


* **Retorna**: Un iterador que apunta al primero de los nuevos elementos insertados.
* **Complejidad**: Lineal en el número de elementos que se insertan (construcción por copia o movimiento) más el número de elementos que estaban después de *position* (elementos movidos).
* **Excepciones**: No se lanzan excepciones.

![Ejemplo de uso del método `insert` con los parámetros de su versión de relleno.](https://user-images.githubusercontent.com/64336377/103586830-87d21b80-4eab-11eb-9695-542c8c83652e.png "Ejemplo de uso del método insert con los parámetros de su versión de relleno")
Ejemplo de uso del método `insert` con los parámetros de su versión de relleno. Se muestran algunas operaciones que realiza el método por dentro para mover e insertar los elementos correspondientes.

![Ejemplo del llamado al método privado `reallocate` desde el método `insert`.](https://user-images.githubusercontent.com/64336377/103587318-9f5dd400-4eac-11eb-95e9-54bc2418f20b.png "Ejemplo del llamado al método privado reallocate desde el método insert")
Detalle de la reasignación realizada en el método `insert`, cuando este llama al método privado `reallocate`.

4. #### erase

**Elimina elementos.**

Elimina del *vector* ya sea un elemento o un rango de elementos.

Este método reduce eficazmente el tamaño del contenedor en el número de elementos eliminados.

* **Parámetros**:
    * *position*: Iterador que apunta al único elemento por eliminar del *vector*.
    * *first*, *last*: Iteradores que especifican el rango de elementos a eliminar.
* **Retorna**: Un iterador que apunta a la nueva ubicación del elemento siguiente al último eliminado.
* **Complejidad**: Lineal en el número de elementos que se eliminados más el número de elementos que estaban después del último eliminado (elementos movidos).
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
iterator erase(const_iterator position);
iterator erase(const_iterator first, const_iterator last);
```

5. #### swap

**Intercambia contenido.**

Intercambia el contenido de el objeto *vector* por el de *other*, que es otro objeto *vector* del mismo tipo. Los tamaños pueden ser distintos.

Después del llamado a este método, los elementos en el contenedor son los que estaban anteriormente en *other*, y los elementos en *other* son los que estaban en el contenedor que llama al método.

Este método llama a `std::swap` para intercambiar los atributos privados del *vector*.

* **Parámetros**:
  * *other*: Otro objeto *vector* del mismo tipo, para intercambiar sus elementos.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
void swap(vector& other);
```

6. #### clear

**Vacía el contenido.**

Elimina todos los elementos del contenedor y lo deja con tamaño 0.

No está garantizado que se haga una reasignación, y la capacidad del *vector* no cambia al llamar a este método.

* **Parámetros**: Ninguno.
* **Retorna**: Nada.
* **Complejidad**: Lineal en el número de destrucciones.
* **Declaración**:

```C++
void clear() noexcept;
```

7. #### emplace

**Construye e inserta un elemento.**

Inserta un nuevo elemento en *position*. Este elemento utiliza *args* como los argumentos para su construcción.

Este método incrementa eficazmente el tamaño del contenedor en uno.

Este método causa una reasignación automática si y solo si el nuevo tamaño del *vector* sobrepasa la capacidad actual.

* **Parámetros**:
    * *position*: Posición del contenedor donde se inserta el nuevo elemento.
    * *args*: Argumentos que se "reenvían" (`std::forward`) para construir el nuevo elemento.
* **Retorna**: Un iterador que apunta al nuevo elemento.
* **Complejidad**: Lineal en el número de elementos después de *position*.
* **Declaración**:

```C++
template <typename... Args>
    iterator emplace(const_iterator position, Args&&... args);
```

8. #### emplace_back

**Construye e inserta un elemento al final.**

Agrega un nuevo elemento al final del *vector*, después de su último elemento actual. Este elemento utiliza *args* como los argumentos para su construcción.

Este método incrementa eficazmente en uno el tamaño del contenedor, lo que causa una reasignación automática si y solo si el nuevo tamaño del *vector* sobrepasa la capacidad actual.

* **Parámetros**:
    * *args*: Argumentos que se "reenvían" (`std::forward`) para construir el nuevo elemento.
* **Retorna**: Nada.
* **Complejidad**: Constante amortizada.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
template <typename... Args>
    void emplace_back(Args&&... args);
```

## Sobrecarga de funciones no-miembro

### Operadores relacionales

Realiza la operación de comparación apropiada entre los contenedores *vector* *lhs* y *rhs*.

La **comparación de igualdad** es realizada primero comparando el tamaño de ambos contenedores, y si coinciden, se comparan los elementos secuencialmente usando el operador **==**, deteniéndose en la primera discordancia.

La **comparación "menor que"** compara los elementos secuencialmente usando el operador **<**, deteniéndose en la primera discordancia.

Las otras operaciones utilizan los operadores sobrecargados **==** y **<**. Se comportan de la siguiente manera:

| Operación | Operación equivalente |
|-----------|-----------------------|
| a != b    | !( a == b )           |
| a > b     | b < a                 |
| a <= b    | !( b < a )            |
| a >= b    | !( a < b )            |

* **Parámetros**:
  * *lhs*, *rhs*: Contenedores *vector* (*left-hand side* y *right-hand side*) cuyos parámetros de plantilla son iguales.
* **Retorna**: `true` si se cumple la condición; de lo contrario `false`.
* **Complejidad**: Lineal según el tamaño de los contenedores.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
template <typename ValueType>
    bool operator==(const vector<ValueType>& lhs, const vector<ValueType>& rhs);
template <typename ValueType>
    bool operator!=(const vector<ValueType>& lhs, const vector<ValueType>& rhs);
template <typename ValueType>
    bool operator<(const vector<ValueType>& lhs, const vector<ValueType>& rhs);
template <typename ValueType>
    bool operator<=(const vector<ValueType>& lhs, const vector<ValueType>& rhs);
template <typename ValueType>
    bool operator>(const vector<ValueType>& lhs, const vector<ValueType>& rhs);
template <typename ValueType>
    bool operator>=(const vector<ValueType>& lhs, const vector<ValueType>& rhs);
```

### swap

**Intercambia contenido de *vectores*.**

Intercambia el contenido de *vector_one* por el de *vector_two*. Ambos objetos *vector* deben ser del mismo tipo, aunque los tamaños pueden ser distintos.

Después del llamado a este método, los elementos en *vector_one* son los que estaban anteriormente en *vector_two*, y los elementos en *vector_two* son los que estaban en *vector_one*.

* **Parámetros**:
  * *vector_one*, *vector_two*: Contenedores *vector* del mismo tipo y tamaño.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
void swap(const vector<ValueType>& vector_one, const vector<ValueType>& vector_two);
```

## Referencias

https://www.cplusplus.com/reference/vector/vector/

https://en.cppreference.com/w/cpp/container/vector

https://www.cplusplus.com/forum/beginner/270488/