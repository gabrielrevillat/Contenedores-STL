# Vector - Dise�o y documentaci�n externa

El *vector* es un contenedor secuencial que representa un arreglo que puede cambiar su tama�o.

**Notas**:

* Hace falta documentar los m�todos privados.
* Por el uso de algunas bibliotecas del `std`, este contenedor debe ser compilado usando C++ 17 (Revisar constructor de rango, m�todo assign con rango, m�todo insert con rango y la tercera referencia del documento). Probablemente los dem�s contenedores tengan que ser compilados de esta manera.

## Propiedades del contenedor

* **Secuencia**: Los elementos en contenedores secuenciales son ordenados en una secuencia estrictamente lineal. Los elementos individuales se acceden seg�n su posici�n en esta secuencia.
* **Arreglo din�mico**: Permite el acceso directo a cualquier elemento en la secuencia, incluso a trav�s de aritm�tica de punteros, y proporciona relativamente r�pida agregaci�n y eliminaci�n de elementos al final de la secuencia.

## Par�metros de plantilla

* **ValueType**: El tipo de los elementos contenidos. Identificado como el tipo miembro `vector::value_type`.

## Tipos miembro

| Tipo miembro | Definici�n |
| -------- | -------- |
| `value_type`     | Primer par�metro de plantilla    |
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

* *count*: El n�mero de elementos almacenados actualmente en el contenedor.
* *storage_capacity*: La capacidad de almacenamiento asignada actualmente para el contenedor, expresada en t�rminos del n�mero de elementos.
* *elements*: Contenedor interno para guardar elementos de tipo *ValueType*.

## M�todos p�blicos

### Constructores

1. #### Constructor por defecto

Construye un contendor vac�o, sin elementos.

* **Par�metros**: Ninguno.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
vector();
```

2. #### Constructor de relleno

Construye el contenedor con *count* elementos. Si se recibe *value* como par�metro, cada elemento es una copia de *value*.

* **Par�metros**:
    * *count*: El tama�o inicial del contenedor.
    * *value*: Valor para inicializar los elementos del contenedor.
* **Complejidad**: Lineal en el tama�o del contenedor.
* **Declaraci�n**:

```C++
explicit vector(size_type count);
vector(size_type count, const value_type& value);
```

3. #### Constructor de rango

Construye el contenedor con tantos elementos como el rango [*first*, *last*).

* **Par�metros**:
    * *first*, *last*: Iteradores a las posiciones inicial y final en un rango.
* **Complejidad**: Lineal en el tama�o del contenedor.
* **Declaraci�n**:

```C++
template <typename InputIterator,
    typename = typename std::enable_if_t<std::is_base_of_v<std::input_iterator_tag,
        typename std::iterator_traits<InputIterator>::iterator_category>>>
    vector(InputIterator first, InputIterator last);
```
El uso de `std::enable_if_t` etc es para poder identificar correctamente si el par�metro recibido es un iterador, de lo contrario el programa se vuelve loco xd.

4. #### Constructor de copia

Construye el contenedor con una copia de cada uno de los elementos de *other*, en el mismo orden.

* **Par�metros**:
    * *other*: Otro objeto vector del mismo tipo, para inicializar el contenedor.
* **Complejidad**: Lineal en el tama�o del contenedor.
* **Declaraci�n**:

```C++
vector(const vector& other);
```

5. #### Constructor de movimiento

Construye un contenedor que adquiere los elementos de *temp*, con sem�nticas de movimiento.

* **Par�metros**:
    * *temp*: Otro objeto vector del mismo tipo, para inicializar el contenedor.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
vector(vector&& temp);
```

6. #### Constructor con lista de inicializaci�n

Construye el contenedor con una copia de cada uno de los elementos de *init_list*.

* **Par�metros**:
    * *init_list*: Objeto `std::initializer_list` para inicializar los elementos del contenedor.
* **Complejidad**: Lineal en el tama�o del contenedor.
* **Declaraci�n**:

```C++
vector(std::initializer_list<value_type> init_list);
```

### Destructor

Destruye el objeto contenedor.

* **Declaraci�n**:

```C++
~vector();
```

### operator=

**Asigna contenido.**

Asigna nuevo contenido al contenedor. Reemplaza su contenido actual y modifica su tama�o.

1. #### Asignaci�n por copia

Reemplaza el contenido del contenedor por una copia del contenido de *other*.

* **Par�metros**:
    * *other*: Otro objeto vector del mismo tipo, para reemplazar el contenido del contenedor.
* **Retorna**: `*this`
* **Complejidad**: Lineal seg�n el tama�o del contenedor.
* **Declaraci�n**:

```C++
vector& operator=(const vector& other);
```

2. #### Asignaci�n por movimiento

Reemplaza el contenido del contenedor al mover dentro de este el contenido de *temp*.

* **Par�metros**:
    * *temp*: Otro objeto vector del mismo tipo, para reemplazar el contenido del contenedor.
* **Retorna**: `*this`
* **Complejidad**: Lineal seg�n el tama�o del contenedor.
* **Declaraci�n**:

```C++
vector& operator=(vector&& temp);
```

3. #### Asignaci�n por lista de inicializaci�n

Reemplaza el contenido del contenedor por una copia de los elementos de *init_list*.

* **Par�metros**:
    * *init_list*: Objeto `std::initializer_list` para reemplazar el contenido del contenendor.
* **Retorna**: `*this`
* **Complejidad**: Lineal seg�n el tama�o del contenedor.
* **Declaraci�n**:

```C++
vector& operator=(std::initializer_list<value_type> init_list);
```

### assign

**Asigna contenido.**

Asigna nuevo contenido al contenedor. Reemplaza su contenido actual y modifica su tama�o.

1. #### Asignaci�n con relleno

Reemplaza el contenido del contenedor con *count* elementos, donde cada elemento es una copia de *value*.

* **Par�metros**:
    * *count*: El nuevo tama�o del contenedor.
    * *value*: Valor para asignar los elementos del contenedor.
* **Retorna**: Nada.
* **Complejidad**: Lineal seg�n el tama�o del contenedor.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
void assign(size_type count, const value_type& value);
```

2. #### Asignaci�n con rango

Reemplaza el contenido del objeto con elementos en el rango [*first*, *last*).

* **Par�metros**:
    * *first*, *last*: Iteradores a las posiciones inicial y final en un rango.
* **Complejidad**: Lineal seg�n el tama�o del contenedor.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
template <typename InputIterator,
    typename = typename std::enable_if_t<std::is_base_of_v<std::input_iterator_tag,
                typename std::iterator_traits<InputIterator>::iterator_category>>>
    void assign(InputIterator first, InputIterator last);
```

3. #### Asignaci�n por lista de inicializaci�n

Reemplaza el contenido del contenedor por una copia de los elementos de *init_list*.

* **Par�metros**:
    * *init_list*: Objeto `std::initializer_list` para reemplazar el contenido del contenendor.
* **Retorna**: `*this`
* **Complejidad**: Lineal seg�n el tama�o del contenedor.
* **Declaraci�n**:

```C++
void assign(std::initializer_list<value_type> init_list);
```

### Iteradores

1. #### begin

**Retorna un iterador al principio.**

Retorna un iterador que apunta al primer elemento del contenedor.

En contenedores vac�os, este m�todo retorna lo mismo que `vector::end`. Este iterador retornado no debe ser desreferenciado.

* **Par�metros**: Ninguno.
* **Retorna**: Un iterador al principio de la secuencia. Si el objeto *vector* es const, el m�todo retorna un `const_iterator`. De lo contrario, retorna un `iterator`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
iterator begin() noexcept;
const_iterator begin() const noexcept;
```

2. #### cbegin

**Retorna un *const_iterator* al principio.**

Retorna un `const_iterator` que apunta al primer elemento del contenedor.

Un `const_iterator` es un iterador que apunta a contenido constante. Este iterador se puede aumentar o disminuir (a menos de que sea tambi�n constante), igual que el iterador que retorna `vector::begin`, pero no puede usarse para modificar el contenido al que apunta, incluso si el objeto *vector* no es constante.

* **Par�metros**: Ninguno.
* **Retorna**: Un `const_iterator` al principio de la secuencia.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
const_iterator cbegin() const noexcept;
```

3. #### end

**Retorna un iterador al final.**

Retorna un iterador que apunta al elemento siguiente al �ltimo del contenedor.

Este elemento sirve como un marcador de posici�n. No apunta a ning�n elemento y por lo tanto no debe ser desreferenciado.

* **Par�metros**: Ninguno.
* **Retorna**: Un iterador al elemento siguiente al �ltimo de la secuencia. Si el objeto *vector* es const, el m�todo retorna un `const_iterator`. De lo contrario, retorna un `iterator`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
iterator end() noexcept;
const_iterator end() const noexcept;
```

4. #### cend

**Retorna un *const_iterator* al final.**

Retorna un `const_iterator` que apunta al elemento siguiente al �ltimo del contenedor.

* **Par�metros**: Ninguno.
* **Retorna**: Un `const_iterator` a la ubicaci�n siguiente a la �ltima de la secuencia.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
const_iterator cend() const noexcept;
```

![Representaci�n de los iteradores begin y end del contenedor *vector*.](https://user-images.githubusercontent.com/64336377/103161183-9f225200-47a3-11eb-8d6a-8dd9643bd11d.png "Representaci�n de los iteradores begin y end del contenedor vector")
Representaci�n de los iteradores begin y end del contenedor.

5. #### rbegin

**Retorna un iterador inverso al principio del contenedor invertido.**

Retorna un iterador inverso que apunta al �ltimo elemento del contenedor no invertido.

Este m�todo apunta al elemento anterior al que ser�a apuntado por el m�todo `vector::end`.

* **Par�metros**: Ninguno.
* **Retorna**: Un iterador inverso al principio de la secuencia invertida.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
reverse_iterator rbegin() noexcept;
const_reverse_iterator rbegin() const noexcept;
```

6. #### crbegin

**Retorna un *const_reverse_iterator* al principio del contenedor invertido.**

Retorna un `const_reverse_iterator` que apunta al �ltimo elemento del contenedor no invertido.

Un `const_reverse_iterator` es un iterador que apunta a contenido constante e itera en orden inverso. Este iterador se puede aumentar o disminuir (a menos de que sea tambi�n constante), igual que el iterador que retorna `vector::rbegin`, pero no puede usarse para modificar el contenido al que apunta.

* **Par�metros**: Ninguno.
* **Retorna**: Un `const_reverse_iterator` al principio de la secuencia invertida.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
const_reverse_iterator crbegin() const noexcept;
```

7. #### rend

**Retorna un iterador inverso al final del contenedor invertido.**

Retorna un iterador inverso que apunta al elemento anterior al primer elemento del contenedor no invertido.

* **Par�metros**: Ninguno.
* **Retorna**: Un iterador inverso al final de la secuencia invertida.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
reverse_iterator rend() noexcept;
const_reverse_iterator rend() const noexcept;
```

8. #### crend

**Retorna un *const_reverse_iterator* al final del contenedor invertido.**

Retorna un `const_reverse_iterator` que apunta al elemento anterior al primer elemento del contenedor no invertido.

* **Par�metros**: Ninguno.
* **Retorna**: Un `const_reverse_iterator` al final de la secuencia invertida.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
const_reverse_iterator crend() const noexcept;
```

![Representaci�n de los iteradores normales e inversos del contenedor *vector*.](https://user-images.githubusercontent.com/64336377/103161235-4acba200-47a4-11eb-8a1b-bed86acf283a.png "Representaci�n de los iteradores normales e inversos del contenedor vector")
Representaci�n de los iteradores normales e inversos del contenedor. El iterador inverso almacena un iterador al siguiente elemento del elemento al que realmente apunta.

### Capacidad

1. #### size

**Retorna el tama�o del contenedor.**

Retorna el n�mero de elementos del contenedor.

Este n�mero no necesariamente es igual a la capacidad de almacenamiento del contenedor.

* **Par�metros**: Ninguno.
* **Retorna**: La cantidad de elementos del contenedor.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
size_type size() const noexcept;
```

2. #### max_size

**Retorna el tama�o m�ximo del contenedor.**

Retorna la cantidad m�xima de elementos que puede contener el contenedor.

Este es el tama�o m�ximo potencial que puede alcanzar el contenedor por limitaciones del sistema, pero no est� garantizado que el contenedor sea capaz de alcanzar ese tama�o. Asignar almacenamiento en cualquier punto antes de ese tama�o podr�a fallar de igual forma.

F�rmula, basada en el valor que retorna el m�todo en mi computadora: (2^63)/(Tama�o en bytes de *ValueType*) - 1

* **Par�metros**: Ninguno.
* **Retorna**: El n�mero m�ximo de elementos que puede tener el contenedor como contenido.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
size_type max_size() const noexcept;
```

3. #### resize

**Cambia el tama�o del contenedor.**

Cambia el tama�o del contenedor para *count* elementos.

Si *count* es menor al tama�o actual del contenedor, el contenido se reduce a sus primeros *count* elementos, de forma que se eliminan y destruyen los elementos sobrantes.

Si *count* es mayor al tama�o actual del contenedor, el contenido se expande y se insertan al final los elementos necesarios para alcanzar un tama�o de *count* elementos. Si se especifica *value*, los nuevos elementos se inicializan como copias de *value*.

Si *count* es tambi�n mayor a la capacidad actual del contenedor, se hace una reasignaci�n autom�tica del almacenamiento asignado.

* **Par�metros**:
    * *count*: El nuevo tama�o del contenedor.
    * *value*: El valor para inicializar los elementos a�adidos en caso de que *count* sea mayor al tama�o actual del contenedor.
* **Retorna**: Nada.
* **Complejidad**: Lineal en el n�mero de elementos insertados o eliminados.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
void resize(size_type count);
void resize(size_type count, value_type value);
```

![Ejemplo de uso del m�todo `resize` del *vector*.](https://user-images.githubusercontent.com/64336377/103180315-46fa5700-485a-11eb-997a-3a0295b0e2a3.png "Ejemplo de uso del m�todo resize del vector")
Ejemplo de uso del m�todo `resize` en un vector de n�meros enteros. Primero se cambia el tama�o a tres elementos y autom�ticamente se eliminan los sobrantes. Luego se cambia a seis elementos, y a los nuevos se les asigna el n�mero 20. El tama�o en estos casos no supera a la capacidad, por lo tanto esta no cambia y no hay necesidad de reasignar el almacenamiento.

4. #### capacity

**Retorna la capacidad asignada de almacenamiento.**

Retorna el tama�o del espacio de almacenamiento asignado actualmente para el *vector*, expresado en t�rminos del n�mero de elementos.

Esta capacidad no necesariamente es igual al tama�o del *vector*. Puede ser igual o mayor, con espacio extra que permite acomodar el crecimiento sin necesidad de reasignar en cada inserci�n.

Esta capacidad no representa un l�mite en el tama�o del *vector*. Cuando se agota y se necesita m�s, es expandida autom�ticamente por el contenedor (reasigna el espacio de almacenamiento). El l�mite te�rico del tama�o del *vector* es dado por `vector::max_size`.

* **Par�metros**: Ninguno.
* **Retorna**: El tama�o de la capacidad de almacenamiento asignada en el *vector*, en t�rminos del n�mero de elementos que puede contener.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
size_type capacity() const noexcept;
```

5. #### empty

**Verifica si el contenedor est� vac�o.**

Retorna un valor `bool` que indica si el contenedor est� vac�o, es decir, cuando su tama�o es igual a 0.

* **Par�metros**: Ninguno.
* **Retorna**: `true` si el tama�o del *array* es 0; de lo contrario, `false`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
bool empty() const noexcept;
```

6. #### reserve

**Solicita un cambio en la capacidad del contenedor.**

Solicita un cambio en la capacidad del vector para que sea lo suficiente como para contener *new_capacity* elementos.

Si *new_capacity* es mayor a la capacidad actual del *vector*, se reasigna el almacenamiento del contenedor de forma que se incremente su capacidad hasta *new_capacity* o m�s.

En cualquier otro caso, el llamado a este m�todo no hace una reasignaci�n y la capacidad del *vector* no se ve afectada.

* **Par�metros**:
    * *new_capacity*: La nueva capacidad del *vector*.
* **Retorna**: Nada.
* **Complejidad**: Si se hace una reasignaci�n, la complejidad es lineal en el tama�o del *vector* a lo sumo.
* **Excepciones**: `std::length_error` si *new_capacity* es mayor que `vector::max_size`.
* **Declaraci�n**:

```C++
void reserve(size_type new_capacity);
```

![Ejemplo de uso de los m�todos `reserve` y `empty` del *vector*.](https://user-images.githubusercontent.com/64336377/103180550-19161200-485c-11eb-8c5d-2b7a5ddc23e8.png "Ejemplo de uso de los m�todos reserve y empty del vector")
Ejemplo de uso de los m�todos `reserve` y `empty` del *vector*. El m�todo `reserve` en este caso se utiliza para aumentar la capacidad del contenedor para que quepan siete elementos. Como no se ha agregado ning�n elemento, el m�todo `empty` retorna `true`.

7. #### shrink_to_fit

**Encogimiento para encajar con el tama�o del contenedor.**

Solicita reducir la capacidad del contenedor para encajar con el n�mero de elementos.

* **Par�metros**: Ninguno.
* **Retorna**: Nada.
* **Complejidad**: Lineal en el tama�o del contenedor a lo sumo.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
void shrink_to_fit();
```

![Ejemplo del m�todo `shrink_to_fit` del *vector*](https://user-images.githubusercontent.com/64336377/103181210-79a94d00-4864-11eb-9b68-3d6ecab81866.png "Ejemplo del m�todo shrink_to_fit del vector")
Ejemplo del m�todo `shrink_to_fit` del *vector*. En la imagen, el contenedor superior tiene capacidad para ocho elementos, pero solo posee cinco elementos. Despu�s del llamado al m�todo, el almacenamiento es reasignado a un contenedor con capacidad para cinco elementos.

![Mismo ejemplo, se detalla lo que sucede por dentro al llamar a los m�todos que reasignan el almacenamiento y copian los elementos.](https://user-images.githubusercontent.com/64336377/103468337-b621f080-4d1d-11eb-978c-4bbfd215f0f0.png "Mismo ejemplo, se detalla lo que sucede por dentro al llamar a los m�todos que reasignan el almacenamiento y copian los elementos")
Detalle de lo que realiza el m�todo `shrink_to_fit` al llamar al m�todo que se encarga de reasignar el almacenamiento a un contenedor que posea *count* elementos.

### Acceso a elementos

1. #### operator[]

**Accede a un elemento.**

Retorna una referencia al elemento en la posici�n *index* del contenedor.

A diferencia de `vector::at`, este m�todo no hace comprobaci�n de l�mites.

* **Par�metros**:
    * *index*: Posici�n de un elemento en el contenedor.
* **Retorna**: El elemento en la posici�n especificada del *vector*. Si el objeto *vector* es const, el m�todo retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
reference operator[](size_type index);
const_reference operator[](size_type index) const;
```

![Ejemplos de valores de retorno seg�n la posici�n usando el operador []](https://user-images.githubusercontent.com/64336377/103181427-69926d00-4866-11eb-9f47-3a6980421727.png "Ejemplos de valores de retorno seg�n la posici�n usando el operador []")
Ejemplo de uso del operador **[]** en dos posiciones de un objeto *vector* llamado *my_vector*.

2. #### at

**Accede a un elemento.**

Retorna una referencia al elemento en la posici�n *index* del contenedor.

Este m�todo verifica autom�ticamente si *index* se encuentra dentro del l�mite de elementos v�lidos en el contenedor. Si no lo est�, lanza una excepci�n `std::out_of_range`.

* **Par�metros**:
  * *index*: Posici�n de un elemento en el contenedor. Si es mayor o igual al tama�o del contenedor, se lanza una excepci�n `std::out_of_range`.
* **Retorna**: El elemento en la posici�n especificada del *vector*. Si el objeto *vector* es const, el m�todo retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: `std::out_of_range` si *index* est� fuera de los l�mites del contenedor.
* **Declaraci�n**:

```C++
reference at(size_type index);
const_reference at(size_type index) const;
```

![Ejemplos de valores de retorno seg�n la posici�n usando el m�todo at.](https://user-images.githubusercontent.com/64336377/103181468-dd347a00-4866-11eb-81b3-2c830a01300b.png "Ejemplos de valores de retorno seg�n la posici�n usando el m�todo at")
Ejemplo de uso del m�todo *at* en dos posiciones de un objeto *vector* llamado *my_vector*.

3. #### front

**Accede al primer elemento.**

Retorna una referencia al primer elemento en el contenedor.

El llamado a este m�todo en un contenedor vac�o tiene comportamiento indefinido.

* **Par�metros**: Ninguno.
* **Retorna**: Una referencia al primer elemento del contenedor. Si el objeto *vector* es const, el m�todo retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
reference front();
const_reference front() const;
```

4. #### back

**Accede al �ltimo elemento.**

Retorna una referencia al �ltimo elemento en el contenedor.

El llamado a este m�todo en un contenedor vac�o tiene comportamiento indefinido.

* **Par�metros**: Ninguno.
* **Retorna**: Una referencia al �ltimo elemento del contenedor. Si el objeto *vector* es const, el m�todo retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
reference back();
const_reference back() const;
```

5. #### data

**Obtiene un puntero a los datos.**

Retorna un puntero directo al arreglo de memoria usado internamente por el *vector* para guardar sus elementos.

Como los elementos en el *vector* est�n almacenados en ubicaciones contiguas de memoria, el puntero que se retorna puede ser desplazado para acceder a cualquier elemento del contenedor.

* **Par�metros**: Ninguno.
* **Retorna**: Un puntero al primer elemento en el arreglo usado internamente por el *vector*. Si el objeto *vector* es const, el m�todo retorna un puntero a un const `value_type`. De lo contrario, retrona un puntero a `value_type`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
value_type* data() noexcept;
const value_type* data() const noexcept;
```

![Representaci�n de los m�todos `front`, `back` y `data`.](https://user-images.githubusercontent.com/64336377/103181578-21744a00-4868-11eb-809a-b25f016d3b59.png "Representaci�n de los m�todos front, back y data")
Representaci�n de los m�todos `front`, `back` y `data` en el contenedor *vector*.

### Modificadores

1. #### push_back

**Agrega un elemento al final.**

Agrega un nuevo elemento al final del *vector*, despu�s de su �ltimo elemento actual. El contenido  de *value* se copia o se mueve al nuevo elemento.

Este m�todo incrementa eficazmente en uno el tama�o del contenedor, lo que causa una reasignaci�n autom�tica si y solo si el nuevo tama�o del *vector* sobrepasa la capacidad actual.

* **Par�metros**:
    * *value*: El valor del elemento por agregar al contenedor.
* **Retorna**: Nada.
* **Complejidad**: Constante amortizada.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
void push_back(const value_type& value);
void push_back(value_type&& value);
```

2. #### pop_back

**Elimina el �ltimo elemento.**

Elimina el �ltimo elemento del contenedor y reduce eficazmente su tama�o en uno.

* **Par�metros**: Ninguno.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
void pop_back();
```

3. #### insert

**Inserta elementos.**

Inserta nuevos elementos en el *vector* en la posici�n anterior al elemento en *position* e incrementa eficazmente el tama�o del contenedor en el n�mero de elementos insertados.

Este m�todo causa una reasignaci�n autom�tica si y solo si el nuevo tama�o del *vector* sobrepasa la capacidad actual.

Como el *vector* utiliza un arreglo como almacenamiento interno, insertar elementos en posiciones distintas al final del contenedor provoca que el contenedor reasigne todos los elementos que estaban luego de esa posici�n a sus nuevas posiciones. Esto generalmente es una operaci�n ineficiente comparada con m�todos que realizan la misma operaci�n en otros tipos de contenedores secuenciales (Como `list` o `forward_list`).

#### a) �nico elemento

Inserta el valor *value* en la posici�n *position*.

* **Par�metros**:
    * *position*: Posici�n del contenedor donde se inserta el nuevo elemento.
    * *value*: El valor del elemento por insertar.
* **Declaraci�n**:

```C++
iterator insert(const_iterator position, const value_type& value);
iterator insert(const_iterator position, value_type&& value);
```

#### b) Relleno

Inserta *count* copias de *value* en la posici�n @a position.

* **Par�metros**:
    * *position*: Posici�n del contenedor donde se inserta el primero de los nuevos elementos.
    * *count*: El n�mero de elementos a insertar.
    * *value*: El valor del elemento por insertar
* **Declaraci�n**:

```C++
iterator insert(const_iterator position, size_type count, const value_type& value);
```

#### c) Rango

Inserta elementos en el rango [*first*, *last*) en la posici�n *position*.

* **Par�metros**:
    * *position*: Posici�n del contenedor donde se inserta el primero de los nuevos elementos. 
    * *first*, *last*: Iteradores a las posiciones inicial y final en un rango.
* **Declaraci�n**:

```C++
template <typename InputIterator,
    typename = typename std::enable_if_t<std::is_base_of_v<std::input_iterator_tag,
                typename std::iterator_traits<InputIterator>::iterator_category>>>>
    iterator insert(const_iterator position, InputIterator first, InputIterator last);
```

#### d) Lista de inicializaci�n

Inserta cada uno de los elementos de *init_list* en la posici�n *position*.

* **Par�metros**:
    * *position*: Posici�n del contenedor donde se inserta el primero de los nuevos elementos. 
    * *init_list*: Objeto initializer_list desde donde se insertan los nuevos elementos.
* **Declaraci�n**:

```C++
iterator insert(const_iterator position, std::initializer_list<value_type> init_list);
```


* **Retorna**: Un iterador que apunta al primero de los nuevos elementos insertados.
* **Complejidad**: Lineal en el n�mero de elementos que se insertan (construcci�n por copia o movimiento) m�s el n�mero de elementos que estaban despu�s de *position* (elementos movidos).
* **Excepciones**: No se lanzan excepciones.

![Ejemplo de uso del m�todo `insert` con los par�metros de su versi�n de relleno.](https://user-images.githubusercontent.com/64336377/103586830-87d21b80-4eab-11eb-9695-542c8c83652e.png "Ejemplo de uso del m�todo insert con los par�metros de su versi�n de relleno")
Ejemplo de uso del m�todo `insert` con los par�metros de su versi�n de relleno. Se muestran algunas operaciones que realiza el m�todo por dentro para mover e insertar los elementos correspondientes.

![Ejemplo del llamado al m�todo privado `reallocate` desde el m�todo `insert`.](https://user-images.githubusercontent.com/64336377/103587318-9f5dd400-4eac-11eb-95e9-54bc2418f20b.png "Ejemplo del llamado al m�todo privado reallocate desde el m�todo insert")
Detalle de la reasignaci�n realizada en el m�todo `insert`, cuando este llama al m�todo privado `reallocate`.

4. #### erase

**Elimina elementos.**

Elimina del *vector* ya sea un elemento o un rango de elementos.

Este m�todo reduce eficazmente el tama�o del contenedor en el n�mero de elementos eliminados.

* **Par�metros**:
    * *position*: Iterador que apunta al �nico elemento por eliminar del *vector*.
    * *first*, *last*: Iteradores que especifican el rango de elementos a eliminar.
* **Retorna**: Un iterador que apunta a la nueva ubicaci�n del elemento siguiente al �ltimo eliminado.
* **Complejidad**: Lineal en el n�mero de elementos que se eliminados m�s el n�mero de elementos que estaban despu�s del �ltimo eliminado (elementos movidos).
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
iterator erase(const_iterator position);
iterator erase(const_iterator first, const_iterator last);
```

5. #### swap

**Intercambia contenido.**

Intercambia el contenido de el objeto *vector* por el de *other*, que es otro objeto *vector* del mismo tipo. Los tama�os pueden ser distintos.

Despu�s del llamado a este m�todo, los elementos en el contenedor son los que estaban anteriormente en *other*, y los elementos en *other* son los que estaban en el contenedor que llama al m�todo.

Este m�todo llama a `std::swap` para intercambiar los atributos privados del *vector*.

* **Par�metros**:
  * *other*: Otro objeto *vector* del mismo tipo, para intercambiar sus elementos.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
void swap(vector& other);
```

6. #### clear

**Vac�a el contenido.**

Elimina todos los elementos del contenedor y lo deja con tama�o 0.

No est� garantizado que se haga una reasignaci�n, y la capacidad del *vector* no cambia al llamar a este m�todo.

* **Par�metros**: Ninguno.
* **Retorna**: Nada.
* **Complejidad**: Lineal en el n�mero de destrucciones.
* **Declaraci�n**:

```C++
void clear() noexcept;
```

7. #### emplace

**Construye e inserta un elemento.**

Inserta un nuevo elemento en *position*. Este elemento utiliza *args* como los argumentos para su construcci�n.

Este m�todo incrementa eficazmente el tama�o del contenedor en uno.

Este m�todo causa una reasignaci�n autom�tica si y solo si el nuevo tama�o del *vector* sobrepasa la capacidad actual.

* **Par�metros**:
    * *position*: Posici�n del contenedor donde se inserta el nuevo elemento.
    * *args*: Argumentos que se "reenv�an" (`std::forward`) para construir el nuevo elemento.
* **Retorna**: Un iterador que apunta al nuevo elemento.
* **Complejidad**: Lineal en el n�mero de elementos despu�s de *position*.
* **Declaraci�n**:

```C++
template <typename... Args>
    iterator emplace(const_iterator position, Args&&... args);
```

8. #### emplace_back

**Construye e inserta un elemento al final.**

Agrega un nuevo elemento al final del *vector*, despu�s de su �ltimo elemento actual. Este elemento utiliza *args* como los argumentos para su construcci�n.

Este m�todo incrementa eficazmente en uno el tama�o del contenedor, lo que causa una reasignaci�n autom�tica si y solo si el nuevo tama�o del *vector* sobrepasa la capacidad actual.

* **Par�metros**:
    * *args*: Argumentos que se "reenv�an" (`std::forward`) para construir el nuevo elemento.
* **Retorna**: Nada.
* **Complejidad**: Constante amortizada.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
template <typename... Args>
    void emplace_back(Args&&... args);
```

## Sobrecarga de funciones no-miembro

### Operadores relacionales

Realiza la operaci�n de comparaci�n apropiada entre los contenedores *vector* *lhs* y *rhs*.

La **comparaci�n de igualdad** es realizada primero comparando el tama�o de ambos contenedores, y si coinciden, se comparan los elementos secuencialmente usando el operador **==**, deteni�ndose en la primera discordancia.

La **comparaci�n "menor que"** compara los elementos secuencialmente usando el operador **<**, deteni�ndose en la primera discordancia.

Las otras operaciones utilizan los operadores sobrecargados **==** y **<**. Se comportan de la siguiente manera:

| Operaci�n | Operaci�n equivalente |
|-----------|-----------------------|
| a != b    | !( a == b )           |
| a > b     | b < a                 |
| a <= b    | !( b < a )            |
| a >= b    | !( a < b )            |

* **Par�metros**:
  * *lhs*, *rhs*: Contenedores *vector* (*left-hand side* y *right-hand side*) cuyos par�metros de plantilla son iguales.
* **Retorna**: `true` si se cumple la condici�n; de lo contrario `false`.
* **Complejidad**: Lineal seg�n el tama�o de los contenedores.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

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

Intercambia el contenido de *vector_one* por el de *vector_two*. Ambos objetos *vector* deben ser del mismo tipo, aunque los tama�os pueden ser distintos.

Despu�s del llamado a este m�todo, los elementos en *vector_one* son los que estaban anteriormente en *vector_two*, y los elementos en *vector_two* son los que estaban en *vector_one*.

* **Par�metros**:
  * *vector_one*, *vector_two*: Contenedores *vector* del mismo tipo y tama�o.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
void swap(const vector<ValueType>& vector_one, const vector<ValueType>& vector_two);
```

## Referencias

https://www.cplusplus.com/reference/vector/vector/

https://en.cppreference.com/w/cpp/container/vector

https://www.cplusplus.com/forum/beginner/270488/