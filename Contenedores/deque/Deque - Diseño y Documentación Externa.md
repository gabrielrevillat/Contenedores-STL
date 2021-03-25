# Deque - Diseño y documentación externa

El *deque* es un acrónimo irregular de ***d**ouble-**e**nded **que**ue* (Cola de doble final). 
La cola de doble final es un contenedor secuencial con tamaño dinámico que puede expandirse o
contraerse en ambos finales (ya sea adelante o atrás).

(Se pronuncia como *"deck"* en inglés, o sea que en español sería *"dec"* y no "*diquiú*").

![Abueno nosabia peroya C](https://user-images.githubusercontent.com/64336377/105109974-3d43c800-5a83-11eb-8477-60c664b000e9.jpg "Keanu Reeves chiquitillo")
Mi reacción al enterarme de lo que es un contenedor *deque*. xd

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

**Nota: La mayor parte del diseño y la implementación de este contenedor está basado en la implementación de la biblioteca
estándar de C++. El propósito general es aprender cómo se realiza por dentro el manejo de almacenamiento de los elementos,
ya que este es un contenedor bastante complicado como para ser pensado por sí solo.**

### Tamaño de los fragmentos de memoria

Tenemos que los elementos del *deque* estarán dispersados en diferentes fragmentos de memoria. Estos fragmentos son
de tamaño fijo, por lo tanto hay que pensar en qué tamaño tendrán. En la implementación de `libstdc++` se puede observar
que para calcular el *número de elementos* que tendrá un fragmento, se utiliza un número fijo de 512 y se divide
entre el *tamaño en bytes* del tipo de los elementos del *deque*. Si el tamaño en bytes del tipo de elementos es mayor a 512,
cada fragmento tendrá capacidad para solo un elemento.

### Iteradores y acceso a los fragmentos de memoria

El contenedor debe ser capaz de proporcionar el acceso a cualquiera de sus elementos en tiempo constante y mediante iteradores.
Se debe entonces implementar un iterador especial que conecte los fragmentos de memoria y permita ese acceso correctamente.
El contenedor principal tendrá un arreglo de punteros llamado **mapa**, cada puntero apuntará a un fragmento de memoria y los iteradores serán
capaces de "saltar" entre esos fragmentos para tener acceso a todos los elementos. Los punteros a fragmentos de memoria
son llamados **nodos**.

Cada iterador va a contener tres punteros: Uno (`current`) apuntará a un elemento específico en un fragmento de memoria
(el elemento al que se está apuntando actualmente), y los otros (`first` y `last`) apuntarán al inicio y al final del
fragmento, respectivamente. También tendrá un puntero a punteros (`node`) que apunte al nodo que apunta directamente
a un fragmento de memoria específico.

![Ejemplo de diseño del iterador para el *deque* y los atributos que contiene.](https://user-images.githubusercontent.com/64336377/105266781-ec050880-5b56-11eb-8127-3225299d80f1.png "Diseño del deque_iterator")
Diseño que muestra cómo sería un iterador del contenedor *deque* y hacia dónde apuntarían sus atributos.

El ejemplo anterior presenta un arreglo de punteros llamado `map`, los punteros de este arreglo apuntan hacia
los fragmentos de memoria que contienen los elementos. En la imagen, cada nodo está apuntando a un fragmento, aunque
podría presentarse el caso donde algunos nodos cercanos a los extremos no apunten a ningún fragmento de memoria hasta que
sea necesario, por la cantidad de elementos. Si cada nodo apunta a un fragmento lleno y se necesita más espacio, se crea
un mapa con más espacio y se reasignan los punteros. Preferiblemente, el mapa debería tener un nodo vacío en cada extremo.

El *deque* va a contener dos iteradores en sus atributos: El primer iterador (`start`),
con su puntero `first` apunta al inicio del primer fragmento de memoria donde hay elementos, 
con su puntero `current` apunta directamente al primer elemento de ese fragmento y
con su puntero `last` apunta al final de ese fragmento (después de la última posición).
El segundo iterador (`finish`) apunta con su puntero `first` al inicio del último fragmento de memoria
donde hay elementos, con su puntero `current` apunta directamente al elemento siguiente al último de ese fragmento
y con su puntero `last` apunta al final de ese fragmento.

![Representación de los dos iteradores que va a tener internamente el *deque*.](https://user-images.githubusercontent.com/64336377/105269247-2b345900-5b59-11eb-8931-374882561b61.png "Atributos privados del deque")
Representación de los dos iteradores que va a tener internamente el contenedor y los elementos a los que apuntan los atributos de cada iterador (ft. despiche de flechas xd).

### Métodos privados del contenedor

Los métodos privados del contenedor son los que se encargan principalmente de:

* Crear el arreglo (llamado mapa) de punteros (llamados nodos) a fragmentos de memoria.
* Reasignar el arreglo de punteros cuando su almacenamiento se llena.
* Asignar nuevo espacio para elementos al inicio o al final del contenedor.
* Agregar elementos al inicio o al final del contenedor.

La implementación de estos métodos está basada casi en su totalidad en la implementación de `libstdc++`.
Una explicación detallada de cada método se puede encontrar en el apartado 
[Métodos privados](https://github.com/gabrielrevillat/Contenedores-STL/blob/main/Contenedores/deque/Deque%20-%20Dise%C3%B1o%20y%20Documentaci%C3%B3n%20Externa.md#m%C3%A9todos-privados "Métodos privados del contenedor").

## Documentación del `deque::iterator`

Antes de empezar a documentar e implementar el *deque*, es necesario diseñar correctamente un registro (`struct`) que sirva 
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

`std::random_access_iterator_tag` sirve para identificar al `struct` como un iterador de acceso aleatorio.

### Atributos

* *current*: Puntero a un elemento específico en el fragmento de memoria actual.
* *first*: Puntero al primer elemento del fragmento de memoria actual.
* *last*: Puntero al elemento siguiente al último del fragmento de memoria actual.
* *node*: Puntero al nodo del mapa que apunta al fragmento de memoria.

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
* **Algoritmo**:

```
Proceso operator+=(count)
	Calcular la nueva posición a la que va a apuntar el iterador como count + (current - first).

	Si la nueva posición se encuentra dentro de los límites del fragmento:
		Desplazar current hacia la nueva posición del fragmento: current <- current + count.
	De lo contrario:
		Si la nueva posición es mayor a 0 (nos queremos mover hacia la derecha):
			Calcular cuántos fragmentos hacia la derecha debe moverse el puntero a nodo:
			node_offset <- new_position / buffer_size()
		De lo contrario:
			Calcular cuántos fragmentos hacia la izquierda debe moverse el puntero a nodo:
			node_offset <- -( (-offset - 1) / buffer_size() ) - 1
		Fin-Si.

		Desplazar el puntero a nodo al fragmento correspondiente:
		set_node(node + node_offset)
		Hacer que current apunte a la posición específica de ese fragmento:
		current <- first + (new_position - node_offset * buffer_size())
	Fin-Si.

	Retornar *this
Fin-Proceso
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
typename my_deque_iterator<ValueType>::difference_type
	operator-(const my_deque_iterator<ValueType>& iterator1,
	          const my_deque_iterator<ValueType>& iterator2) noexcept;
```

![Representación y explicación de la resta entre iteradores.](https://user-images.githubusercontent.com/64336377/111384248-e61b3780-866e-11eb-8e4b-a31893febfee.png "Resta entre iteradores")
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

* *map*: Arreglo de punteros (nodos) a fragmentos de memoria que almacenan elementos.
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

### construct_element

Construye un elemento con el valor *value* en la posición *position*.

* **Parámetros**:
	* *position*: La posición donde se construye el nuevo elemento.
	* *value*: El valor del elemento por construir.
* **Complejidad**: Constante.
* **Declaración**:

```C++
void construct_element(pointer position, const value_type& value);
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

### reallocate_map

Realiza una re-ubicación de los nodos del arreglo de punteros `map` si no hay suficiente
espacio en alguno de los extremos. Si no hay espacio en ambos extremos, realiza
una reasignación completa del arreglo de punteros.

Este método calcula la cantidad nueva de nodos que va a tener el mapa.
Si el tamaño actual del mapa es mayor al doble de esta cantidad calculada,
hay suficiente espacio en el otro extremo del contenedor; por lo tanto, solo
hace falta desplazar los nodos un poco hacia el otro extremo. En caso contrario,
no hay suficiente espacio en el otro extremo y entonces el mapa debe ser reasignado.

Al reasignar el mapa, para calcular su nuevo tamaño: Si la cantidad de nodos por agregar supera
el tamaño actual del mapa, sumarle esa cantidad al tamaño. De lo contrario, doblar el tamaño
del mapa. En ambos casos, sumar 2 para los nodos extra en los extremos.

* **Parámetros**:
	* *num_of_nodes_to_add*: La cantidad de nodos a los que se debe hacer espacio en el mapa
	para poder ser agregados.
	* *adding_at_front*: Indica el extremo donde se quieren agregar los nodos. Su valor es `true` si
	se quieren agregar al inicio del contenedor, de lo contrario es `false`.
* **Retorna**: Nada.
* **Complejidad**: Lineal en la cantidad de nodos del arreglo.
* **Declaración**:

```C++
void reallocate_map(size_type num_of_nodes_to_add, bool adding_at_front);
```

### reserve_map_at_back

Verifica si hay suficiente espacio al final del mapa para poder agregar *num_of_nodes_to_add*
nodos. Si no hay suficiente espacio, se reasigna el almacenamiento.

* **Parámetros**:
	* *num_of_nodes_to_add*: La cantidad de nodos a los que se debe hacer espacio en el mapa
	para poder ser agregados. Esta cantidad es de 1 por defecto.
* **Retorna**: Nada.
* **Complejidad**: Constante amortizada. (?, batazo y medio
* **Declaración**:

```C++
void reserve_map_at_back(size_type num_of_nodes_to_add = 1);
```

### reserve_map_at_front

Verifica si hay suficiente espacio al inicio del mapa para poder agregar *num_of_nodes_to_add*
nodos. Si no hay suficiente espacio, se reasigna el almacenamiento.

* **Parámetros**:
	* *num_of_nodes_to_add*: La cantidad de nodos a los que se debe hacer espacio en el mapa
	para poder ser agregados. Esta cantidad es de 1 por defecto.
* **Retorna**: Nada.
* **Complejidad**: Constante amortizada. (? x2
* **Declaración**:

```C++
void reserve_map_at_front(size_type num_of_nodes_to_add = 1);
```

### create_new_nodes_at_back

Crea nodos al final del contenedor para poder insertar *new_elements_count* elementos.

Calcula la cantidad de nodos que se deben crear, reasigna el almacenamiento si es necesario
y crea la cantidad de nodos correspondiente.

* **Parámetros**:
	* *new_elements_count*: El número de elementos que se quieren agregar al contenedor
	a partir del primer nuevo nodo.
* **Retorna**: Nada.
* **Complejidad**: Lineal en el número de nodos.
* **Declaración**:

```C++
void create_new_nodes_at_back(size_type new_elements_count);
```

### create_new_nodes_at_front

Crea nodos al inicio del contenedor para poder insertar *new_elements_count* elementos.

Calcula la cantidad de nodos que se deben crear, reasigna el almacenamiento si es necesario
y crea la cantidad de nodos correspondiente.

* **Parámetros**:
	* *new_elements_count*: El número de elementos que se quieren agregar al contenedor
	a partir del primer nuevo nodo.
* **Retorna**: Nada.
* **Complejidad**: Lineal en el número de nodos.
* **Declaración**:

```C++
void create_new_nodes_at_front(size_type new_elements_count);
```

### reserve_elements_at_back

Reserva espacio al final del contenedor para poder agregar *count* elementos
y retorna un iterador al nuevo final del contenedor.

* **Parámetros**:
	* *count*: El número de elementos que se quieren agregar al contenedor.
* **Retorna**: Un iterador al nuevo final de la secuencia.
* **Complejidad**: Constante amortizada. (??
* **Declaración**:

```C++
iterator reserve_elements_at_back(size_type count);
```

### reserve_elements_at_front

Reserva espacio al inicio del contenedor para poder agregar *count* elementos
y retorna un iterador al nuevo inicio del contenedor.

* **Parámetros**:
	* *count*: El número de elementos que se quieren agregar al contenedor.
* **Retorna**: Un iterador al nuevo inicio de la secuencia.
* **Complejidad**: Constante amortizada. (??
* **Declaración**:

```C++
iterator reserve_elements_at_front(size_type count);
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

4. #### Constructor de copia

Construye el contenedor con una copia de cada uno de los elementos de *other*, en el mismo orden.

* **Parámetros**:
    * *other*: Otro objeto *deque* del mismo tipo, para inicializar el contenedor.
* **Complejidad**: Lineal en el tamaño del contenedor.
* **Declaración**:

```C++
deque(const deque& other);
```

5. #### Constructor de movimiento

Construye un contenedor que adquiere los elementos de *temp*, con semánticas de movimiento.

* **Parámetros**:
    * *temp*: Otro objeto *deque* del mismo tipo, para inicializar el contenedor.
* **Complejidad**: Constante.
* **Declaración**:

```C++
deque(deque&& temp);
```

6. #### Constructor con lista de inicialización

Construye el contenedor con una copia de cada uno de los elementos de *init_list*.

* **Parámetros**:
    * *init_list*: Objeto `std::initializer_list` para inicializar los elementos del contenedor.
* **Complejidad**: Lineal en el tamaño del contenedor.
* **Declaración**:

```C++
deque(std::initializer_list<value_type> init_list);
```

### Destructor

Destruye el objeto contenedor.

* **Declaración**:

```C++
~deque();
```

### operator=

**Asigna contenido.**

Asigna nuevo contenido al contenedor. Reemplaza su contenido actual y modifica su tamaño.

1. #### Asignación por copia

Reemplaza el contenido del contenedor por una copia del contenido de *other*.

* **Parámetros**:
    * *other*: Otro objeto *deque* del mismo tipo, para reemplazar el contenido del contenedor.
* **Retorna**: `*this`
* **Complejidad**: Lineal según el tamaño del contenedor.
* **Declaración**:

```C++
deque& operator=(const deque& other);
```

2. #### Asignación por movimiento

Reemplaza el contenido del contenedor al mover dentro de este el contenido de *temp*.

* **Parámetros**:
    * *temp*: Otro objeto *deque* del mismo tipo, para reemplazar el contenido del contenedor.
* **Retorna**: `*this`
* **Complejidad**: Lineal según el tamaño del contenedor.
* **Declaración**:

```C++
deque& operator=(deque&& temp);
```

3. #### Asignación por lista de inicialización

Reemplaza el contenido del contenedor por una copia de los elementos de *init_list*.

* **Parámetros**:
    * *init_list*: Objeto `std::initializer_list` para reemplazar el contenido del contenendor.
* **Retorna**: `*this`
* **Complejidad**: Lineal según el tamaño del contenedor.
* **Declaración**:

```C++
deque& operator=(std::initializer_list<value_type> init_list);
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

5. #### rbegin

**Retorna un iterador inverso al principio del contenedor invertido.**

Retorna un iterador inverso que apunta al último elemento del contenedor no invertido.

Este método apunta al elemento anterior al que sería apuntado por el método `deque::end`.

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

Un `const_reverse_iterator` es un iterador que apunta a contenido constante e itera en orden inverso.
Este iterador se puede aumentar o disminuir (a menos de que sea también constante),
igual que el iterador que retorna `deque::rbegin`, pero no puede usarse para modificar el contenido al que apunta.

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

### Capacidad

1. #### size

**Retorna el tamaño del contenedor.**

Retorna el número de elementos del contenedor.

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

Este es el tamaño máximo potencial que puede alcanzar el contenedor por limitaciones del sistema,
pero no está garantizado que el contenedor sea capaz de alcanzar ese tamaño.
Asignar almacenamiento en cualquier punto antes de ese tamaño podría fallar de igual forma.

Fórmula, basada en el valor que retorna el método en mi computadora: (2^63)/(Tamaño en bytes de *ValueType*) - 1

* **Parámetros**: Ninguno.
* **Retorna**: El número máximo de elementos que puede tener el contenedor como contenido.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
size_type max_size() const noexcept;
```

4. #### empty

**Verifica si el contenedor está vacío.**

Retorna un valor `bool` que indica si el contenedor está vacío, es decir, cuando su tamaño es igual a 0.

* **Parámetros**: Ninguno.
* **Retorna**: `true` si el tamaño del *deque* es 0; de lo contrario, `false`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
bool empty() const noexcept;
```

### Acceso a elementos

1. #### operator[]

**Accede a un elemento.**

Retorna una referencia al elemento en la posición *index* del contenedor.

A diferencia de `deque::at`, este método no hace comprobación de límites.

* **Parámetros**:
    * *index*: Posición de un elemento en el contenedor.
* **Retorna**: El elemento en la posición especificada del *deque*.
Si el objeto *deque* es const, el método retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
reference operator[](size_type index);
const_reference operator[](size_type index) const;
```

2. #### at

**Accede a un elemento.**

Retorna una referencia al elemento en la posición *index* del contenedor.

Este método verifica automáticamente si *index* se encuentra dentro del límite de elementos válidos en el contenedor.
Si no lo está, lanza una excepción `std::out_of_range`.

* **Parámetros**:
  * *index*: Posición de un elemento en el contenedor. Si es mayor o igual al tamaño del contenedor,
  se lanza una excepción `std::out_of_range`.
* **Retorna**: El elemento en la posición especificada del *deque*. Si el objeto *deque* es const,
el método retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: `std::out_of_range` si *index* está fuera de los límites del contenedor.
* **Declaración**:

```C++
reference at(size_type index);
const_reference at(size_type index) const;
```

3. #### front

**Accede al primer elemento.**

Retorna una referencia al primer elemento en el contenedor.

El llamado a este método en un contenedor vacío tiene comportamiento indefinido.

* **Parámetros**: Ninguno.
* **Retorna**: Una referencia al primer elemento del contenedor. Si el objeto *deque* es const,
el método retorna una `const_reference`. De lo contrario, retrona una `reference`.
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
* **Retorna**: Una referencia al último elemento del contenedor. Si el objeto *deque* es const,
el método retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
reference back();
const_reference back() const;
```

### Modificadores

1. #### push_back

**Agrega un elemento al final.**

Agrega un nuevo elemento al final del *deque*, después de su último elemento actual.
El contenido de *value* se copia o se mueve al nuevo elemento.

Este método incrementa eficazmente en uno el tamaño del contenedor.

* **Parámetros**:
    * *value*: El valor del elemento por agregar al contenedor.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
void push_back(const value_type& value);
void push_back(value_type&& value);
```

2. #### push_front

**Inserta un elemento al principio.**

Inserta un nuevo elemento al principio del *deque*, antes de su primer elemento actual.
El contenido de *value* se copia o se mueve al nuevo elemento.

Este método incrementa eficazmente en uno el tamaño del contenedor.

* **Parámetros**:
    * *value*: El valor del elemento por agregar al contenedor.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
void push_front(const value_type& value);
void push_front(value_type&& value);
```

3. #### pop_back

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

4. #### pop_front

**Elimina el primer elemento.**

Elimina el primer elemento del contenedor y reduce eficazmente su tamaño en uno.

* **Parámetros**: Ninguno.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
void pop_front();
```

5. #### insert

**Inserta elementos.**

Inserta nuevos elementos en el *deque* antes del elemento en *position* e incrementa eficazmente
el tamaño del contenedor en el número de elementos insertados.

Las colas de doble final están diseñadas para ser eficientes al insertar y eliminar elementos ya sea al final
o al inicio de la secuencia. Las inserciones en otras posiciones normalmente son menos eficientes que en los contenedores
*list* o *forward_list*.

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
		typename std::iterator_traits<InputIterator>::iterator_category>>>
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
* **Complejidad**: Lineal en el número de elementos que se insertan (construcción por copia o movimiento)
más el número de elementos que estaban antes o después de *position* (elementos movidos).
* **Excepciones**: No se lanzan excepciones.

6. #### erase

**Elimina elementos.**

Elimina del *deque* ya sea un elemento o un rango de elementos.

Este método reduce eficazmente el tamaño del contenedor en el número de elementos eliminados.

* **Parámetros**:
    * *position*: Iterador que apunta al único elemento por eliminar del *deque*.
    * *first*, *last*: Iteradores que especifican el rango de elementos a eliminar.
* **Retorna**: Un iterador que apunta a la nueva ubicación del elemento siguiente al último eliminado.
* **Complejidad**: Lineal en el número de elementos que se eliminados más el número de elementos
que estaban antes o después del último eliminado (elementos movidos).
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
iterator erase(const_iterator position);
iterator erase(const_iterator first, const_iterator last);
```

7. #### swap

**Intercambia contenido.**

Intercambia el contenido de el objeto *deque* por el de *other*, que es otro objeto *deque* del mismo tipo.
Los tamaños pueden ser distintos.

Después del llamado a este método, los elementos en el contenedor son los que estaban anteriormente en *other*
y los elementos en *other* son los que estaban en el contenedor que llama al método.

Este método llama a `std::swap` para intercambiar los atributos privados del *deque*.

* **Parámetros**:
  * *other*: Otro objeto *deque* del mismo tipo, para intercambiar sus elementos.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
void swap(deque& other);
```

8. #### clear

**Vacía el contenido.**

Elimina todos los elementos del contenedor y lo deja con tamaño 0.

* **Parámetros**: Ninguno.
* **Retorna**: Nada.
* **Complejidad**: Lineal en el número de destrucciones.
* **Declaración**:

```C++
void clear() noexcept;
```

9. #### emplace

**Construye e inserta un elemento.**

Inserta un nuevo elemento en *position*. Este elemento utiliza *args* como los argumentos para su construcción.

Este método incrementa eficazmente el tamaño del contenedor en uno.

Las colas de doble final están diseñadas para ser eficientes al insertar y eliminar elementos ya sea al final
o al inicio de la secuencia. Las inserciones en otras posiciones normalmente son menos eficientes que en los contenedores
*list* o *forward_list*.

* **Parámetros**:
	* *position*: Posición del contenedor donde se inserta el nuevo elemento.
    * *args*: Argumentos que se "reenvían" (`std::forward`) para construir el nuevo elemento.
* **Retorna**: Un iterador que apunta al nuevo elemento.
* **Complejidad**: Lineal en el número de elementos entre *position* y el inicio o el final del contenedor.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
template <typename... Args>
    iterator emplace(const_iterator position, Args&&... args);
```

10. #### emplace_front

**Construye e inserta un elemento al principio.**

Inserta un nuevo elemento al principio del *deque*, antes de su primer elemento actual.
El contenido de *value* se copia o se mueve al nuevo elemento.

Este método incrementa eficazmente en uno el tamaño del contenedor.

Para insertar un elemento, este método verifica que el primer elemento del contenedor
no se encuentra en la primera posición del primer fragmento de memoria
(Para esto, `start.current`, que apunta al primer elemento del contenedor, no debe
estar apuntando a `start.first`, que es la primera posición del primer fragmento de memoria).
De lo contrario, crea un nuevo nodo antes del primero y lo inicializa
con un nuevo fragmento de memoria, conecta ese fragmento con el primer fragmento de memoria
viejo e inserta el elemento en la última posición de ese nuevo fragmento.

* **Parámetros**:
    * *args*: Argumentos que se "reenvían" (`std::forward`) para construir el nuevo elemento.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
template <typename... Args>
    void emplace_front(Args&&... args);
```

11. #### emplace_back

**Construye e inserta un elemento al final.**

Agrega un nuevo elemento al final del *deque*, después de su último elemento actual.
Este elemento utiliza *args* como los argumentos para su construcción.

Este método incrementa eficazmente en uno el tamaño del contenedor.

Para insertar un elemento, este método verifica que el último elemento del contenedor
no se encuentra en las últimas dos posiciones del último fragmento de memoria
(Para esto, `finish.current`, que apunta al elemento siguiente al último del contenedor,
no debe estar apuntando a `finish.last - 1`, que es la última posición del´último fragmento de
memoria). De lo contrario, crea un nuevo nodo después del último y lo inicializa
con un nuevo fragmento de memoria, inserta el elemento en la última posición del último
fragmento de memoria y conecta ese fragmento con el nuevo.

* **Parámetros**:
    * *args*: Argumentos que se "reenvían" (`std::forward`) para construir el nuevo elemento.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaración**:

```C++
template <typename... Args>
    void emplace_back(Args&&... args);
```