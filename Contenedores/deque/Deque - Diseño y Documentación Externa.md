# Deque - Diseño y documentación externa

El *deque* es un acrónimo irregular de ***d**ouble-**e**nded **que**ue* (Cola de doble final). 
La cola de doble final es un contenedor secuencial con tamaño dinámico que puede expandirse o
contraerse en ambos finales (ya sea adelante o atrás).

(Se pronuncia como *"deck"* en inglés, o sea que en español sería *"dec"* y no "*diquiú*").

![Abueno nosabia peroya C](https://user-images.githubusercontent.com/64336377/105109974-3d43c800-5a83-11eb-8477-60c664b000e9.jpg "Keanu Reeves chiquitillo")
Mi reacción al enterarme de lo que es un contenedor *deque* xd.

## Descripción general (The *embolia* starts now)

Bibliotecas específicas pueden implementar un *deque* de diferentes maneras, generalmente como alguna forma
de arreglo dinámico. En cualquier caso permite el acceso a elementos individuales mediante iteradores,
con el almacenamiento siendo manejado automáticamente al expandir o contraer el contenedor según lo necesario.

Proporciona una funcionalidad similar al *vector*, pero con inserción y eliminado eficientes de elementos no solo
al final, sino también al inicio de la secuencia. Hasta aquí todo bien, **PERO** a diferencia del *vector*, el *deque*
no garantiza almacenar todos sus elementos en ubicaciones contiguas de almacenamiento: El acceso a elementos en un *deque*
desplazando el puntero hacia otro elemento causa comportamiento indefinido.

Tanto el *vector* como el *deque* proporcionan una interfaz similar y pueden usarse para propósitos similares,
pero internamente ambos funcionan de diferentes maneras: Mientras que el *vector* utiliza un solo arreglo
que necesita ser reasignado ocacionalmente, los elementos de un *deque* pueden estar dispersados en **diferentes fragmentos
de memoria** y el contenedor mantiene la información necesaria internamente para proporcionar acceso directo a cualquiera
de sus elementos en **tiempo constante** y **mediante iteradores**.

![mind_blown](https://user-images.githubusercontent.com/64336377/105108815-ba217280-5a80-11eb-8070-c15e86daead4.gif "Puaaaaaa, puaaaaaa")

Por lo tanto, el *deque* es **un poquito** más complejo internamente que el *vector*, pero esto permite un crecimiento más
eficiente bajo ciertas circunstancias, especialmente con secuencias muy largas, donde la reasignación es más costosa.

Para operaciones que implican inserción y eliminado frecuentes de elementos en posiciones diferentes al inicio o el final,
el *deque* actúa peor y posee iteradores y referencias menos consistentes que *list* y *forward list*.

## Propiedades del contenedor

* **Secuencia**: Los elementos en contenedores secuenciales son ordenados en una secuencia estrictamente lineal. Los elementos individuales se acceden según su posición en esta secuencia.
* **Arreglo dinámico**: Implementado generalmente como un arreglo dinámico, permite el acceso directo a cualquier elemento en la secuencia y proporciona relativamente rápida agregación y eliminación de elementos al inicio o al final de la secuencia.

## Introducción - Idea de implementación

### Tamaño de los fragmentos de memoria

Tenemos que los elementos del *deque* estarán dispersados en diferentes fragmentos de memoria. Estos fragmentos son
de tamaño fijo, por lo tanto hay que pensar en qué tamaño tendrán. En la implementación de `libstdc++` se puede observar
que para calcular el *número de elementos* que tendrá un fragmento, se utiliza un número fijo de 512 y se divide
entre el *tamaño en bytes* del tipo de los elementos del *deque*. Si el tamaño en bytes del tipo de elementos es mayor a 512,
cada fragmento tendrá capacidad para solo un elemento.

### Iteradores y acceso a los fragmentos de memoria

El contenedor debe ser capaz de proporcionar el acceso a cualquiera de sus elementos en tiempo constante y mediante iteradores.
Se debe entonces implementar un iterador especial que conecte los fragmentos de memoria y permita ese acceso correctamente.
El contenedor principal tendrá un arreglo de punteros, cada puntero apuntará a un fragmento de memoria y los iteradores serán
capaces de "saltar" entre esos fragmentos para tener acceso a todos los elementos. Los punteros a fragmentos de memoria
son llamados **nodos**.

Cada iterador va a contener tres punteros: Uno apuntará a un elemento específico en un fragmento de memoria (el elemento al
que se está apuntando actualmente), y los otros apuntarán al inicio y al final del fragmento. También tendrá un puntero a punteros
que apunte al nodo que apunta directamente a un fragmento de memoria específico.

![Ejemplo de diseño del iterador para el *deque* y los atributos que contiene.](https://user-images.githubusercontent.com/64336377/105266781-ec050880-5b56-11eb-8127-3225299d80f1.png "Diseño del deque_iterator")
Diseño que muestra cómo sería un iterador del contenedor *deque* y hacia dónde apuntarían sus atributos.

El ejemplo anterior presenta un arreglo de punteros llamado *map*, los punteros de este arreglo apuntan hacia
los fragmentos de memoria que contienen los elementos. En la imagen, cada puntero está apuntando a un fragmento, aunque
podría presentarse el caso donde algunos punteros cercanos a los extremos no apunten a ningún fragmento de memoria hasta que
sea necesario, por la cantidad de elementos. Si cada puntero apunta a un fragmento lleno y se necesita más espacio, se crea
un contenedor con más espacio y se reasignan los punteros.

El *deque* va a contener dos iteradores en sus atributos: El primero, con su puntero `first` apunta al inicio del primer
fragmento de memoria donde hay elementos y con su puntero `current` apunta directamente al primer elemento de ese fragmento.
El segundo iterador apunta con su puntero `first` al inicio del último fragmento de memoria donde hay elementos y con
su puntero `current` apunta directamente al elemento siguiente al último de ese fragmento.

![Representación de los dos iteradores que va a tener internamente el *deque*.](https://user-images.githubusercontent.com/64336377/105269247-2b345900-5b59-11eb-8931-374882561b61.png "Atributos privados del deque")
Representación de los dos iteradores que va a tener internamente el contenedor y los elementos a los que apuntan los atributos de cada iterador (ft. despiche de flechas xd).

La mayoría de métodos serán basados en una versión antigua de la implementación oficial del contenedor de `std`.
Cuando esos métodos sean analizados y entendidos correctamente, se agregará un diseño más detallado a este apartado.

## Documentación del `deque::iterator`

Antes de empezar a documentar e implementar el *deque*, es necesario diseñar correctamente un `struct` que sirva 
para el manejo de los elementos en los fragmentos de memoria.

### Tipos miembro

| Tipo miembro			| Definición								|
| --------------------- | ----------------------------------------- |
| `iterator_category`	| `std::random_access_iterator_tag`			|
| `value_type`			| Primer parámetro de plantilla de `deque`	|
| `size_type`			| `std::size_t`								|
| `difference_type`		| `std::ptrdiff_t`							|
| `reference`			| `value_type&`								|
| `pointer`				| `value_type*`								|
| `map_pointer`			| `value_type**`							|
| `iterator`			| `my_deque_iterator<value_type>`			|
| `self`				| `my_deque_iterator`						|

### Atributos

* *current*: Puntero a un elemento específico en el fragmento de memoria actual.
* *first*: Puntero al primer elemento del fragmento de memoria actual.
* *last*: Puntero al elemento siguiente al último del fragmento de memoria actual.
* *node*: Puntero al nodo del arreglo que apunta al fragmento de memoria.

### Funciones 

1. #### set_node

Asigna los punteros `node`, `first` y `last` del iterador de manera que apunten correctamente
a *new_node*. El puntero `current` debe ser asignado justo después por la función que invoca a
esta función, basándose en los punteros `first` y `last`.

* **Parámetros**:
	* *new_node*: Puntero a un nodo del contenedor que apunta a un fragmento de memoria específico.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Declaración**:

```C++
void set_node(map_pointer new_node) noexcept;
```

2. #### buffer_size

Retorna la cantidad de elementos que puede almacenar un fragmento de memoria según el tamaño
en bytes de `value_type`.

* **Parámetros**: Ninguno.
* **Retorna**: El número de elementos que almacena cada fragmentos de memoria.
* **Complejidad**: Constante.
* **Declaración**:

```C++
static size_type buffer_size() noexcept;
```

3. #### operator*

Retorna el contenido al que apunta `current`.

* **Parámetros**: Ninguno.
* **Retorna**: `*current`.
* **Complejidad**: Constante.
* **Declaración**:

```C++
reference operator*() const noexcept;
```

4. #### operator->

Retorna el puntero `current`.

* **Parámetros**: Ninguno.
* **Retorna**: `current`.
* **Complejidad**: Constante.
* **Declaración**:

```C++
pointer operator->() const noexcept;
```

5. #### operator++ (prefix)

Incrementa el puntero `current` para que apunte al elemento siguiente del fragmento
de almacenamiento. Si `current` alcanza el final del fragmento, el iterador
debe apuntar al siguiente fragmento de memoria y `current` apunta al inicio
de ese fragmento.

* **Parámetros**: Ninguno.
* **Retorna**: `*this`
* **Complejidad**: Constante.
* **Declaración**:

```C++
self& operator++() noexcept;
```

6. #### operator++ (postfix)

Incrementa el iterador de manera postfija. Para esto utiliza la versión prefija de `operator++`.

* **Parámetros**: Argumento de tipo `int` que distingue la sobrecarga del operador en su versión postfija.
* **Retorna**: `*this` antes de ser incrementado.
* **Complejidad**: Constante.
* **Declaración**:

```C++
self operator++(int) noexcept;
```

![Ejemplo del `operator++` que salta entre los fragmentos.](https://user-images.githubusercontent.com/64336377/105432062-7c595100-5c1c-11eb-986f-c93bbe799c62.png "Operador de incremento que salta entre los fragmentos de memoria")
Representación de cómo el operador de incremento hace un salto entre los fragmentos de memoria.

7. #### operator-- (prefix)

Decrementa el puntero `current` para que apunte al elemento anterior del fragmento
de almacenamiento. Si `current` alcanza el inicio del fragmento, el iterador
debe apuntar al fragmento anterior de memoria y `current` apunta al último elemento
de ese fragmento.

* **Parámetros**: Ninguno.
* **Retorna**: `*this`
* **Complejidad**: Constante.
* **Declaración**:

```C++
self& operator--() noexcept;
```

8. #### operator-- (postfix)

Decrementa el iterador de manera postfija. Para esto utiliza la versión prefija de `operator--`.

* **Parámetros**: Argumento de tipo `int` que distingue la sobrecarga del operador en su versión postfija.
* **Retorna**: `*this` antes de ser decrementado.
* **Complejidad**: Constante.
* **Declaración**:

```C++
self operator--(int) noexcept;
```

9. #### operator+=

Desplaza *count* veces el puntero `current`. Si la nueva posición del puntero se encuentra
fuera de los límites del fragmento de memoria, `node` debe desplazarse hacia el fragmento correspondiente
anterior o siguiente según el caso, y `current` debe apuntar al elemento correspondiente de ese fragmento.

* **Parámetros**:
	* *count*: El número de espacios de desplazamiento.
* **Retorna**: `*this`
* **Complejidad**: Constante.
* **Pseudocódigo**:

```
FUNCTION operator+=(count)
	new_position <- count + (current - first)

	IF new_position >= 0 AND new_position < buffer_size() THEN
		current <- current + count
	ELSE
		IF new_position > 0 THEN
			node_offset <- new_position / buffer_size()
		ELSE
			node_offset <- -( (-offset - 1) / buffer_size() ) - 1
		END-IF

		CALL set_node(node + node_offset)
		current <- first + (new_position - node_offset * buffer_size())
	END-IF

	RETURN *this
END operator+=
```

* **Declaración**:

```C++
self& operator+=(difference_type count) noexcept;
```

10. #### operator+

Retorna un iterador que apunta a la posición (`current` + *count*). Utiliza la sobrecarga de `operator+=`.

* **Parámetros**:
	* *count*: El número de espacios de desplazamiento.
* **Retorna**: `*this` + *count*.
* **Complejidad**: Constante.
* **Declaración**:

```C++
self operator+(difference_type count) noexcept;
```

11. #### operator-=

Desplaza *count* veces el puntero `current`. Utiliza la sobrecarga de `operator+=`.

* **Parámetros**:
	* *count*: El número de espacios de desplazamiento.
* **Retorna**: `*this`
* **Complejidad**: Constante.
* **Declaración**:

```C++
self& operator-=(difference_type count) noexcept;
```

12. #### operator-

Retorna un iterador que apunta a la posición (`current` - *count*). Utiliza la sobrecarga de `operator+=`.

* **Parámetros**:
	* *count*: El número de espacios de desplazamiento.
* **Retorna**: `*this` - *count*.
* **Complejidad**: Constante.
* **Declaración**:

```C++
self operator-(difference_type count) noexcept;
```

13. #### operator[]

Retorna el elemento al que apunta la posición (`current` + *position*). Utiliza la sobrecarga de `operator+`.

* **Parámetros**:
	* *position*: La posición del elemento que se quiere retornar, relativa a `current`.
* **Retorna**: *(`*this` + *position*).
* **Complejidad**: Constante.
* **Declaración**:

```C++
reference operator[](difference_type position) noexcept;
```

### Operadores relacionales

Se sobrecargan todos los operadores relacionales según el puntero `current`.

### Operador de resta

Retorna el número de elementos entre `iterator1` y `iterator2`.

* **Parámetros**:
	* *iterator1*, *iterator2*: Iteradores del contenedor *deque* a los cuales se les aplica
	la operación de resta.
* **Retorna**: El resultado de la resta, la distancia entre ambos iteradores.
* **Complejidad**: Constante.
* **Declaración**:

```C++
template <typename ValueType>
inline typename my_deque_iterator<ValueType>::difference_type
		operator-(const my_deque_iterator<ValueType>& iterator1,
			const my_deque_iterator<ValueType>& iterator2) noexcept;
```

![Representación y explicación de la resta entre iteradores.](https://user-images.githubusercontent.com/64336377/108461358-32c24d00-7240-11eb-8aa1-3a6c75249670.png "Resta entre iteradores")
Representación y explicación de cómo se realiza la resta entre iteradores para calcular cuántos
elementos hay entre cada uno.

## Parámetros de plantilla

* **ValueType**: El tipo de los elementos contenidos. Identificado como el tipo miembro `deque::value_type`.

## Tipos miembro públicos

| Tipo miembro				| Definición										|
| -----------------------	| ------------------------------------------------- |
| `value_type`				| Primer parámetro de plantilla						|
| `size_type`				| `std::size_t`										|
| `difference_type`			| `std::ptrdiff_t`									|
| `reference`				| `value_type&`										|
| `const_reference`			| `const value_type&`								|
| `pointer`					| `value_type*`										|
| `const_pointer`			| `const value_type*`								|
| `iterator`				| `mySTL::my_deque_iterator<value_type>`			|
| `const_iterator`			| `const mySTL::my_deque_iterator<value_type>`		|
| `reverse_iterator`		| `std::reverse_iterator<iterator>`					|
| `const_reverse_iterator`	| `std::reverse_iterator<const_iterator>`			|

## Tipos miembro privados

| Tipo miembro		| Definición							|
| ----------------- | ------------------------------------- |
| `map_pointer`		| `pointer*`							|

## Atributos privados

* *map*: Arreglo de punteros a fragmentos de memoria que almacenan elementos.
* *map_size*: El número de nodos a los que apunta *map*. Esta cantidad es de al menos 8.
* *start*: Iterador que apunta al primer elemento del contenedor.
* *finish*: Iterador que apunta al elemento siguiente al último del contenedor.

## Métodos privados

### buffer_size

Retorna la cantidad de elementos que puede almacenar un fragmento de memoria según el tamaño
en bytes de `value_type`.

* **Parámetros**: Ninguno.
* **Retorna**: El número de elementos que almacena cada fragmentos de memoria.
* **Complejidad**: Constante.
* **Declaración**:

```C++
static size_type buffer_size() noexcept;
```

### create_map_and_nodes

Inicializa el arreglo de punteros `map` según la cantidad de elementos
con la que se está inicializando el *deque* y asigna un fragmento a cada nodo.

El número de nodos necesarios para el contenedor se calcula al dividir el número
de elementos entre el tamaño de los fragmentos de memoria. Si la división es exacta,
se le suma un nodo a esta cantidad. De lo contrario, se le aplica "función techo" al
resultado.

Si el número de nodos necesarios es menor a 8, el tamaño de `map` se asigna en 8.
De lo contrario, se asigna según el número de nodos necesarios + 2, para
que se reserve un nodo al inicio y otro al final del arreglo, para facilitar la expansión
del contenedor. Los iteradores `start` y `finish` se asignan de manera que en ambos extremos
quede sobrando al menos un nodo vacío.

* **Parámetros**:
	* *elements_count*: El número de elementos que va a contener el contenedor.
* **Retorna**: Nada.
* **Complejidad**: Lineal en la cantidad de nodos.
* **Declaración**:

```C++
void create_map_and_nodes(size_type elements_count);
```

### destroy_map_and_nodes

Destruye el arreglo de punteros y su contenido.

* **Parámetros**: Ninguno.
* **Retorna**: Nada.
* **Complejidad**: Lineal en la cantidad de nodos del arreglo.
* **Declaración**:

```C++
void destroy_map_and_nodes();
```

## Métodos públicos

### Constructores

1. #### Constructor por defecto

Construye un contendor vacío, sin elementos.

* **Parámetros**: Ninguno.
* **Complejidad**: Constante.
* **Declaración**:

```C++
explicit deque();
```

2. #### Constructor de relleno

Construye el contenedor con *count* elementos. Si se recibe *value* como parámetro, cada elemento es una copia de *value*.

* **Parámetros**:
    * *count*: El tamaño inicial del contenedor.
    * *value*: Valor para inicializar los elementos del contenedor.
* **Complejidad**: Lineal en el tamaño del contenedor.
* **Declaración**:

```C++
explicit deque(size_type count);
deque(size_type count, const value_type& value);
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
    deque(InputIterator first, InputIterator last);
```

### Destructor

Destruye el objeto contenedor.

* **Declaración**:

```C++
~deque();
```

### Iteradores

1. #### begin

**Retorna un iterador al principio.**

Retorna un iterador que apunta al primer elemento del contenedor.

En contenedores vacíos, este método retorna lo mismo que `deque::end`. Este iterador retornado no debe ser desreferenciado.

* **Parámetros**: Ninguno.
* **Retorna**: Un iterador al principio de la secuencia. Si el objeto *deque* es const,
el método retorna un `const_iterator`. De lo contrario, retorna un `iterator`.
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

Un `const_iterator` es un iterador que apunta a contenido constante.
Este iterador se puede aumentar o disminuir (a menos de que sea también constante),
igual que el iterador que retorna `deque::begin`, pero no puede usarse para modificar
el contenido al que apunta, incluso si el objeto *deque* no es constante.

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
* **Retorna**: Un iterador al elemento siguiente al último de la secuencia.
Si el objeto *deque* es const, el método retorna un `const_iterator`. De lo contrario, retorna un `iterator`.
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