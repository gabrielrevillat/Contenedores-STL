# Deque - Dise�o y documentaci�n externa

El *deque* es un acr�nimo irregular de ***d**ouble-**e**nded **que**ue* (Cola de doble final). 
La cola de doble final es un contenedor secuencial con tama�o din�mico que puede expandirse o
contraerse en ambos finales (ya sea adelante o atr�s).

(Se pronuncia como *"deck"* en ingl�s, o sea que en espa�ol ser�a *"dec"* y no "*diqui�*").

![Abueno nosabia peroya C](https://user-images.githubusercontent.com/64336377/105109974-3d43c800-5a83-11eb-8477-60c664b000e9.jpg "Keanu Reeves chiquitillo")
Mi reacci�n al enterarme de lo que es un contenedor *deque*. xd

## Descripci�n general (The *embolia* starts now)

Bibliotecas espec�ficas pueden implementar un *deque* de diferentes maneras, generalmente como alguna forma
de arreglo din�mico. En cualquier caso permite el acceso a elementos individuales mediante iteradores,
con el almacenamiento siendo manejado autom�ticamente al expandir o contraer el contenedor seg�n lo necesario.

Proporciona una funcionalidad similar al *vector*, pero con inserci�n y eliminado eficientes de elementos no solo
al final, sino tambi�n al inicio de la secuencia. Hasta aqu� todo bien, **PERO** a diferencia del *vector*, el *deque*
no garantiza almacenar todos sus elementos en ubicaciones contiguas de almacenamiento: El acceso a elementos en un *deque*
desplazando el puntero hacia otro elemento causa comportamiento indefinido.

Tanto el *vector* como el *deque* proporcionan una interfaz similar y pueden usarse para prop�sitos similares,
pero internamente ambos funcionan de diferentes maneras: Mientras que el *vector* utiliza un solo arreglo
que necesita ser reasignado ocacionalmente, los elementos de un *deque* pueden estar dispersados en **diferentes fragmentos
de memoria** y el contenedor mantiene la informaci�n necesaria internamente para proporcionar acceso directo a cualquiera
de sus elementos en **tiempo constante** y **mediante iteradores**.

![mind_blown](https://user-images.githubusercontent.com/64336377/105108815-ba217280-5a80-11eb-8070-c15e86daead4.gif "Puaaaaaa, puaaaaaa")

Por lo tanto, el *deque* es **un poquito** m�s complejo internamente que el *vector*, pero esto permite un crecimiento m�s
eficiente bajo ciertas circunstancias, especialmente con secuencias muy largas, donde la reasignaci�n es m�s costosa.

Para operaciones que implican inserci�n y eliminado frecuentes de elementos en posiciones diferentes al inicio o el final,
el *deque* act�a peor y posee iteradores y referencias menos consistentes que *list* y *forward list*.

## Propiedades del contenedor

* **Secuencia**: Los elementos en contenedores secuenciales son ordenados en una secuencia estrictamente lineal. Los elementos individuales se acceden seg�n su posici�n en esta secuencia.
* **Arreglo din�mico**: Implementado generalmente como un arreglo din�mico, permite el acceso directo a cualquier elemento en la secuencia y proporciona relativamente r�pida agregaci�n y eliminaci�n de elementos al inicio o al final de la secuencia.

## Introducci�n - Idea de implementaci�n

**Nota: La mayor parte del dise�o y la implementaci�n de este contenedor est� basado en la implementaci�n de la biblioteca
est�ndar de C++. El prop�sito general es aprender c�mo se realiza por dentro el manejo de almacenamiento de los elementos,
ya que este es un contenedor bastante complicado como para ser pensado por s� solo.**

### Tama�o de los fragmentos de memoria

Tenemos que los elementos del *deque* estar�n dispersados en diferentes fragmentos de memoria. Estos fragmentos son
de tama�o fijo, por lo tanto hay que pensar en qu� tama�o tendr�n. En la implementaci�n de `libstdc++` se puede observar
que para calcular el *n�mero de elementos* que tendr� un fragmento, se utiliza un n�mero fijo de 512 y se divide
entre el *tama�o en bytes* del tipo de los elementos del *deque*. Si el tama�o en bytes del tipo de elementos es mayor a 512,
cada fragmento tendr� capacidad para solo un elemento.

### Iteradores y acceso a los fragmentos de memoria

El contenedor debe ser capaz de proporcionar el acceso a cualquiera de sus elementos en tiempo constante y mediante iteradores.
Se debe entonces implementar un iterador especial que conecte los fragmentos de memoria y permita ese acceso correctamente.
El contenedor principal tendr� un arreglo de punteros llamado **mapa**, cada puntero apuntar� a un fragmento de memoria y los iteradores ser�n
capaces de "saltar" entre esos fragmentos para tener acceso a todos los elementos. Los punteros a fragmentos de memoria
son llamados **nodos**.

Cada iterador va a contener tres punteros: Uno (`current`) apuntar� a un elemento espec�fico en un fragmento de memoria
(el elemento al que se est� apuntando actualmente), y los otros (`first` y `last`) apuntar�n al inicio y al final del
fragmento, respectivamente. Tambi�n tendr� un puntero a punteros (`node`) que apunte al nodo que apunta directamente
a un fragmento de memoria espec�fico.

![Ejemplo de dise�o del iterador para el *deque* y los atributos que contiene.](https://user-images.githubusercontent.com/64336377/105266781-ec050880-5b56-11eb-8127-3225299d80f1.png "Dise�o del deque_iterator")
Dise�o que muestra c�mo ser�a un iterador del contenedor *deque* y hacia d�nde apuntar�an sus atributos.

El ejemplo anterior presenta un arreglo de punteros llamado `map`, los punteros de este arreglo apuntan hacia
los fragmentos de memoria que contienen los elementos. En la imagen, cada nodo est� apuntando a un fragmento, aunque
podr�a presentarse el caso donde algunos nodos cercanos a los extremos no apunten a ning�n fragmento de memoria hasta que
sea necesario, por la cantidad de elementos. Si cada nodo apunta a un fragmento lleno y se necesita m�s espacio, se crea
un mapa con m�s espacio y se reasignan los punteros. Preferiblemente, el mapa deber�a tener un nodo vac�o en cada extremo.

El *deque* va a contener dos iteradores en sus atributos: El primer iterador (`start`),
con su puntero `first` apunta al inicio del primer fragmento de memoria donde hay elementos, 
con su puntero `current` apunta directamente al primer elemento de ese fragmento y
con su puntero `last` apunta al final de ese fragmento (despu�s de la �ltima posici�n).
El segundo iterador (`finish`) apunta con su puntero `first` al inicio del �ltimo fragmento de memoria
donde hay elementos, con su puntero `current` apunta directamente al elemento siguiente al �ltimo de ese fragmento
y con su puntero `last` apunta al final de ese fragmento.

![Representaci�n de los dos iteradores que va a tener internamente el *deque*.](https://user-images.githubusercontent.com/64336377/105269247-2b345900-5b59-11eb-8931-374882561b61.png "Atributos privados del deque")
Representaci�n de los dos iteradores que va a tener internamente el contenedor y los elementos a los que apuntan los atributos de cada iterador (ft. despiche de flechas xd).

### M�todos privados del contenedor

Los m�todos privados del contenedor son los que se encargan principalmente de:

* Crear el arreglo (llamado mapa) de punteros (llamados nodos) a fragmentos de memoria.
* Reasignar el arreglo de punteros cuando su almacenamiento se llena.
* Asignar nuevo espacio para elementos al inicio o al final del contenedor.
* Agregar elementos al inicio o al final del contenedor.

La implementaci�n de estos m�todos est� basada casi en su totalidad en la implementaci�n de `libstdc++`.
Una explicaci�n detallada de cada m�todo se puede encontrar en el apartado 
[M�todos privados](https://github.com/gabrielrevillat/Contenedores-STL/blob/main/Contenedores/deque/Deque%20-%20Dise%C3%B1o%20y%20Documentaci%C3%B3n%20Externa.md#m%C3%A9todos-privados "M�todos privados del contenedor").

## Documentaci�n del `deque::iterator`

Antes de empezar a documentar e implementar el *deque*, es necesario dise�ar correctamente un registro (`struct`) que sirva 
para el manejo de los elementos en los fragmentos de memoria.

### Tipos miembro

| Tipo miembro			| Definici�n								|
| --------------------- | ----------------------------------------- |
| `iterator_category`	| `std::random_access_iterator_tag`			|
| `value_type`			| Primer par�metro de plantilla de `deque`	|
| `size_type`			| `std::size_t`								|
| `difference_type`		| `std::ptrdiff_t`							|
| `reference`			| `value_type&`								|
| `pointer`				| `value_type*`								|
| `map_pointer`			| `value_type**`							|
| `iterator`			| `my_deque_iterator<value_type>`			|
| `self`				| `my_deque_iterator`						|

`std::random_access_iterator_tag` sirve para identificar al `struct` como un iterador de acceso aleatorio.

### Atributos

* *current*: Puntero a un elemento espec�fico en el fragmento de memoria actual.
* *first*: Puntero al primer elemento del fragmento de memoria actual.
* *last*: Puntero al elemento siguiente al �ltimo del fragmento de memoria actual.
* *node*: Puntero al nodo del mapa que apunta al fragmento de memoria.

### Funciones 

1. #### set_node

Asigna los punteros `node`, `first` y `last` del iterador de manera que apunten correctamente
a *new_node*. El puntero `current` debe ser asignado justo despu�s por la funci�n que invoca a
esta funci�n, bas�ndose en los punteros `first` y `last`.

* **Par�metros**:
	* *new_node*: Puntero a un nodo del contenedor que apunta a un fragmento de memoria espec�fico.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
void set_node(map_pointer new_node) noexcept;
```

2. #### buffer_size

Retorna la cantidad de elementos que puede almacenar un fragmento de memoria seg�n el tama�o
en bytes de `value_type`.

* **Par�metros**: Ninguno.
* **Retorna**: El n�mero de elementos que almacena cada fragmentos de memoria.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
static size_type buffer_size() noexcept;
```

3. #### operator*

Retorna el contenido al que apunta `current`.

* **Par�metros**: Ninguno.
* **Retorna**: `*current`.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
reference operator*() const noexcept;
```

4. #### operator->

Retorna el puntero `current`.

* **Par�metros**: Ninguno.
* **Retorna**: `current`.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
pointer operator->() const noexcept;
```

5. #### operator++ (prefix)

Incrementa el puntero `current` para que apunte al elemento siguiente del fragmento
de almacenamiento. Si `current` alcanza el final del fragmento, el iterador
debe apuntar al siguiente fragmento de memoria y `current` apunta al inicio
de ese fragmento.

* **Par�metros**: Ninguno.
* **Retorna**: `*this`
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
self& operator++() noexcept;
```

6. #### operator++ (postfix)

Incrementa el iterador de manera postfija. Para esto utiliza la versi�n prefija de `operator++`.

* **Par�metros**: Argumento de tipo `int` que distingue la sobrecarga del operador en su versi�n postfija.
* **Retorna**: `*this` antes de ser incrementado.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
self operator++(int) noexcept;
```

![Ejemplo del `operator++` que salta entre los fragmentos.](https://user-images.githubusercontent.com/64336377/105432062-7c595100-5c1c-11eb-986f-c93bbe799c62.png "Operador de incremento que salta entre los fragmentos de memoria")
Representaci�n de c�mo el operador de incremento hace un salto entre los fragmentos de memoria.

7. #### operator-- (prefix)

Decrementa el puntero `current` para que apunte al elemento anterior del fragmento
de almacenamiento. Si `current` alcanza el inicio del fragmento, el iterador
debe apuntar al fragmento anterior de memoria y `current` apunta al �ltimo elemento
de ese fragmento.

* **Par�metros**: Ninguno.
* **Retorna**: `*this`
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
self& operator--() noexcept;
```

8. #### operator-- (postfix)

Decrementa el iterador de manera postfija. Para esto utiliza la versi�n prefija de `operator--`.

* **Par�metros**: Argumento de tipo `int` que distingue la sobrecarga del operador en su versi�n postfija.
* **Retorna**: `*this` antes de ser decrementado.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
self operator--(int) noexcept;
```

9. #### operator+=

Desplaza *count* veces el puntero `current`. Si la nueva posici�n del puntero se encuentra
fuera de los l�mites del fragmento de memoria, `node` debe desplazarse hacia el fragmento correspondiente
anterior o siguiente seg�n el caso, y `current` debe apuntar al elemento correspondiente de ese fragmento.

* **Par�metros**:
	* *count*: El n�mero de espacios de desplazamiento.
* **Retorna**: `*this`
* **Complejidad**: Constante.
* **Algoritmo**:

```
Proceso operator+=(count)
	Calcular la nueva posici�n a la que va a apuntar el iterador como count + (current - first).

	Si la nueva posici�n se encuentra dentro de los l�mites del fragmento:
		Desplazar current hacia la nueva posici�n del fragmento: current <- current + count.
	De lo contrario:
		Si la nueva posici�n es mayor a 0 (nos queremos mover hacia la derecha):
			Calcular cu�ntos fragmentos hacia la derecha debe moverse el puntero a nodo:
			node_offset <- new_position / buffer_size()
		De lo contrario:
			Calcular cu�ntos fragmentos hacia la izquierda debe moverse el puntero a nodo:
			node_offset <- -( (-offset - 1) / buffer_size() ) - 1
		Fin-Si.

		Desplazar el puntero a nodo al fragmento correspondiente:
		set_node(node + node_offset)
		Hacer que current apunte a la posici�n espec�fica de ese fragmento:
		current <- first + (new_position - node_offset * buffer_size())
	Fin-Si.

	Retornar *this
Fin-Proceso
```

* **Declaraci�n**:

```C++
self& operator+=(difference_type count) noexcept;
```

10. #### operator+

Retorna un iterador que apunta a la posici�n (`current` + *count*). Utiliza la sobrecarga de `operator+=`.

* **Par�metros**:
	* *count*: El n�mero de espacios de desplazamiento.
* **Retorna**: `*this` + *count*.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
self operator+(difference_type count) noexcept;
```

11. #### operator-=

Desplaza *count* veces el puntero `current`. Utiliza la sobrecarga de `operator+=`.

* **Par�metros**:
	* *count*: El n�mero de espacios de desplazamiento.
* **Retorna**: `*this`
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
self& operator-=(difference_type count) noexcept;
```

12. #### operator-

Retorna un iterador que apunta a la posici�n (`current` - *count*). Utiliza la sobrecarga de `operator+=`.

* **Par�metros**:
	* *count*: El n�mero de espacios de desplazamiento.
* **Retorna**: `*this` - *count*.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
self operator-(difference_type count) noexcept;
```

13. #### operator[]

Retorna el elemento al que apunta la posici�n (`current` + *position*). Utiliza la sobrecarga de `operator+`.

* **Par�metros**:
	* *position*: La posici�n del elemento que se quiere retornar, relativa a `current`.
* **Retorna**: *(`*this` + *position*).
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
reference operator[](difference_type position) noexcept;
```

### Operadores relacionales

Se sobrecargan todos los operadores relacionales seg�n el puntero `current`.

### Operador de resta

Retorna el n�mero de elementos entre `iterator1` y `iterator2`.

* **Par�metros**:
	* *iterator1*, *iterator2*: Iteradores del contenedor *deque* a los cuales se les aplica
	la operaci�n de resta.
* **Retorna**: El resultado de la resta, la distancia entre ambos iteradores.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
template <typename ValueType>
typename my_deque_iterator<ValueType>::difference_type
	operator-(const my_deque_iterator<ValueType>& iterator1,
	          const my_deque_iterator<ValueType>& iterator2) noexcept;
```

![Representaci�n y explicaci�n de la resta entre iteradores.](https://user-images.githubusercontent.com/64336377/111384248-e61b3780-866e-11eb-8e4b-a31893febfee.png "Resta entre iteradores")
Representaci�n y explicaci�n de c�mo se realiza la resta entre iteradores para calcular cu�ntos
elementos hay entre cada uno.

## Par�metros de plantilla

* **ValueType**: El tipo de los elementos contenidos. Identificado como el tipo miembro `deque::value_type`.

## Tipos miembro p�blicos

| Tipo miembro				| Definici�n										|
| -----------------------	| ------------------------------------------------- |
| `value_type`				| Primer par�metro de plantilla						|
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

| Tipo miembro		| Definici�n							|
| ----------------- | ------------------------------------- |
| `map_pointer`		| `pointer*`							|

## Atributos privados

* *map*: Arreglo de punteros (nodos) a fragmentos de memoria que almacenan elementos.
* *map_size*: El n�mero de nodos a los que apunta *map*. Esta cantidad es de al menos 8.
* *start*: Iterador que apunta al primer elemento del contenedor.
* *finish*: Iterador que apunta al elemento siguiente al �ltimo del contenedor.

## M�todos privados

### buffer_size

Retorna la cantidad de elementos que puede almacenar un fragmento de memoria seg�n el tama�o
en bytes de `value_type`.

* **Par�metros**: Ninguno.
* **Retorna**: El n�mero de elementos que almacena cada fragmentos de memoria.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
static size_type buffer_size() noexcept;
```

### construct_element

Construye un elemento con el valor *value* en la posici�n *position*.

* **Par�metros**:
	* *position*: La posici�n donde se construye el nuevo elemento.
	* *value*: El valor del elemento por construir.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
void construct_element(pointer position, const value_type& value);
```

### create_map_and_nodes

Inicializa el arreglo de punteros `map` seg�n la cantidad de elementos
con la que se est� inicializando el *deque* y asigna un fragmento a cada nodo.

El n�mero de nodos necesarios para el contenedor se calcula al dividir el n�mero
de elementos entre el tama�o de los fragmentos de memoria. Si la divisi�n es exacta,
se le suma un nodo a esta cantidad. De lo contrario, se le aplica "funci�n techo" al
resultado.

Si el n�mero de nodos necesarios es menor a 8, el tama�o de `map` se asigna en 8.
De lo contrario, se asigna seg�n el n�mero de nodos necesarios + 2, para
que se reserve un nodo al inicio y otro al final del arreglo, para facilitar la expansi�n
del contenedor. Los iteradores `start` y `finish` se asignan de manera que en ambos extremos
quede sobrando al menos un nodo vac�o.

* **Par�metros**:
	* *elements_count*: El n�mero de elementos que va a contener el contenedor.
* **Retorna**: Nada.
* **Complejidad**: Lineal en la cantidad de nodos.
* **Declaraci�n**:

```C++
void create_map_and_nodes(size_type elements_count);
```

### destroy_map_and_nodes

Destruye el arreglo de punteros y su contenido.

* **Par�metros**: Ninguno.
* **Retorna**: Nada.
* **Complejidad**: Lineal en la cantidad de nodos del arreglo.
* **Declaraci�n**:

```C++
void destroy_map_and_nodes();
```

### reallocate_map

Realiza una re-ubicaci�n de los nodos del arreglo de punteros `map` si no hay suficiente
espacio en alguno de los extremos. Si no hay espacio en ambos extremos, realiza
una reasignaci�n completa del arreglo de punteros.

Este m�todo calcula la cantidad nueva de nodos que va a tener el mapa.
Si el tama�o actual del mapa es mayor al doble de esta cantidad calculada,
hay suficiente espacio en el otro extremo del contenedor; por lo tanto, solo
hace falta desplazar los nodos un poco hacia el otro extremo. En caso contrario,
no hay suficiente espacio en el otro extremo y entonces el mapa debe ser reasignado.

Al reasignar el mapa, para calcular su nuevo tama�o: Si la cantidad de nodos por agregar supera
el tama�o actual del mapa, sumarle esa cantidad al tama�o. De lo contrario, doblar el tama�o
del mapa. En ambos casos, sumar 2 para los nodos extra en los extremos.

* **Par�metros**:
	* *num_of_nodes_to_add*: La cantidad de nodos a los que se debe hacer espacio en el mapa
	para poder ser agregados.
	* *adding_at_front*: Indica el extremo donde se quieren agregar los nodos. Su valor es `true` si
	se quieren agregar al inicio del contenedor, de lo contrario es `false`.
* **Retorna**: Nada.
* **Complejidad**: Lineal en la cantidad de nodos del arreglo.
* **Declaraci�n**:

```C++
void reallocate_map(size_type num_of_nodes_to_add, bool adding_at_front);
```

### reserve_map_at_back

Verifica si hay suficiente espacio al final del mapa para poder agregar *num_of_nodes_to_add*
nodos. Si no hay suficiente espacio, se reasigna el almacenamiento.

* **Par�metros**:
	* *num_of_nodes_to_add*: La cantidad de nodos a los que se debe hacer espacio en el mapa
	para poder ser agregados. Esta cantidad es de 1 por defecto.
* **Retorna**: Nada.
* **Complejidad**: Constante amortizada. (?, batazo y medio
* **Declaraci�n**:

```C++
void reserve_map_at_back(size_type num_of_nodes_to_add = 1);
```

### reserve_map_at_front

Verifica si hay suficiente espacio al inicio del mapa para poder agregar *num_of_nodes_to_add*
nodos. Si no hay suficiente espacio, se reasigna el almacenamiento.

* **Par�metros**:
	* *num_of_nodes_to_add*: La cantidad de nodos a los que se debe hacer espacio en el mapa
	para poder ser agregados. Esta cantidad es de 1 por defecto.
* **Retorna**: Nada.
* **Complejidad**: Constante amortizada. (? x2
* **Declaraci�n**:

```C++
void reserve_map_at_front(size_type num_of_nodes_to_add = 1);
```

### create_new_nodes_at_back

Crea nodos al final del contenedor para poder insertar *new_elements_count* elementos.

Calcula la cantidad de nodos que se deben crear, reasigna el almacenamiento si es necesario
y crea la cantidad de nodos correspondiente.

* **Par�metros**:
	* *new_elements_count*: El n�mero de elementos que se quieren agregar al contenedor
	a partir del primer nuevo nodo.
* **Retorna**: Nada.
* **Complejidad**: Lineal en el n�mero de nodos.
* **Declaraci�n**:

```C++
void create_new_nodes_at_back(size_type new_elements_count);
```

### create_new_nodes_at_front

Crea nodos al inicio del contenedor para poder insertar *new_elements_count* elementos.

Calcula la cantidad de nodos que se deben crear, reasigna el almacenamiento si es necesario
y crea la cantidad de nodos correspondiente.

* **Par�metros**:
	* *new_elements_count*: El n�mero de elementos que se quieren agregar al contenedor
	a partir del primer nuevo nodo.
* **Retorna**: Nada.
* **Complejidad**: Lineal en el n�mero de nodos.
* **Declaraci�n**:

```C++
void create_new_nodes_at_front(size_type new_elements_count);
```

### reserve_elements_at_back

Reserva espacio al final del contenedor para poder agregar *count* elementos
y retorna un iterador al nuevo final del contenedor.

* **Par�metros**:
	* *count*: El n�mero de elementos que se quieren agregar al contenedor.
* **Retorna**: Un iterador al nuevo final de la secuencia.
* **Complejidad**: Constante amortizada. (??
* **Declaraci�n**:

```C++
iterator reserve_elements_at_back(size_type count);
```

### reserve_elements_at_front

Reserva espacio al inicio del contenedor para poder agregar *count* elementos
y retorna un iterador al nuevo inicio del contenedor.

* **Par�metros**:
	* *count*: El n�mero de elementos que se quieren agregar al contenedor.
* **Retorna**: Un iterador al nuevo inicio de la secuencia.
* **Complejidad**: Constante amortizada. (??
* **Declaraci�n**:

```C++
iterator reserve_elements_at_front(size_type count);
```

## M�todos p�blicos

### Constructores

1. #### Constructor por defecto

Construye un contendor vac�o, sin elementos.

* **Par�metros**: Ninguno.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
explicit deque();
```

2. #### Constructor de relleno

Construye el contenedor con *count* elementos. Si se recibe *value* como par�metro, cada elemento es una copia de *value*.

* **Par�metros**:
    * *count*: El tama�o inicial del contenedor.
    * *value*: Valor para inicializar los elementos del contenedor.
* **Complejidad**: Lineal en el tama�o del contenedor.
* **Declaraci�n**:

```C++
explicit deque(size_type count);
deque(size_type count, const value_type& value);
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
    deque(InputIterator first, InputIterator last);
```

4. #### Constructor de copia

Construye el contenedor con una copia de cada uno de los elementos de *other*, en el mismo orden.

* **Par�metros**:
    * *other*: Otro objeto *deque* del mismo tipo, para inicializar el contenedor.
* **Complejidad**: Lineal en el tama�o del contenedor.
* **Declaraci�n**:

```C++
deque(const deque& other);
```

5. #### Constructor de movimiento

Construye un contenedor que adquiere los elementos de *temp*, con sem�nticas de movimiento.

* **Par�metros**:
    * *temp*: Otro objeto *deque* del mismo tipo, para inicializar el contenedor.
* **Complejidad**: Constante.
* **Declaraci�n**:

```C++
deque(deque&& temp);
```

6. #### Constructor con lista de inicializaci�n

Construye el contenedor con una copia de cada uno de los elementos de *init_list*.

* **Par�metros**:
    * *init_list*: Objeto `std::initializer_list` para inicializar los elementos del contenedor.
* **Complejidad**: Lineal en el tama�o del contenedor.
* **Declaraci�n**:

```C++
deque(std::initializer_list<value_type> init_list);
```

### Destructor

Destruye el objeto contenedor.

* **Declaraci�n**:

```C++
~deque();
```

### operator=

**Asigna contenido.**

Asigna nuevo contenido al contenedor. Reemplaza su contenido actual y modifica su tama�o.

1. #### Asignaci�n por copia

Reemplaza el contenido del contenedor por una copia del contenido de *other*.

* **Par�metros**:
    * *other*: Otro objeto *deque* del mismo tipo, para reemplazar el contenido del contenedor.
* **Retorna**: `*this`
* **Complejidad**: Lineal seg�n el tama�o del contenedor.
* **Declaraci�n**:

```C++
deque& operator=(const deque& other);
```

2. #### Asignaci�n por movimiento

Reemplaza el contenido del contenedor al mover dentro de este el contenido de *temp*.

* **Par�metros**:
    * *temp*: Otro objeto *deque* del mismo tipo, para reemplazar el contenido del contenedor.
* **Retorna**: `*this`
* **Complejidad**: Lineal seg�n el tama�o del contenedor.
* **Declaraci�n**:

```C++
deque& operator=(deque&& temp);
```

3. #### Asignaci�n por lista de inicializaci�n

Reemplaza el contenido del contenedor por una copia de los elementos de *init_list*.

* **Par�metros**:
    * *init_list*: Objeto `std::initializer_list` para reemplazar el contenido del contenendor.
* **Retorna**: `*this`
* **Complejidad**: Lineal seg�n el tama�o del contenedor.
* **Declaraci�n**:

```C++
deque& operator=(std::initializer_list<value_type> init_list);
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

En contenedores vac�os, este m�todo retorna lo mismo que `deque::end`. Este iterador retornado no debe ser desreferenciado.

* **Par�metros**: Ninguno.
* **Retorna**: Un iterador al principio de la secuencia. Si el objeto *deque* es const,
el m�todo retorna un `const_iterator`. De lo contrario, retorna un `iterator`.
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

Un `const_iterator` es un iterador que apunta a contenido constante.
Este iterador se puede aumentar o disminuir (a menos de que sea tambi�n constante),
igual que el iterador que retorna `deque::begin`, pero no puede usarse para modificar
el contenido al que apunta, incluso si el objeto *deque* no es constante.

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
* **Retorna**: Un iterador al elemento siguiente al �ltimo de la secuencia.
Si el objeto *deque* es const, el m�todo retorna un `const_iterator`. De lo contrario, retorna un `iterator`.
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

5. #### rbegin

**Retorna un iterador inverso al principio del contenedor invertido.**

Retorna un iterador inverso que apunta al �ltimo elemento del contenedor no invertido.

Este m�todo apunta al elemento anterior al que ser�a apuntado por el m�todo `deque::end`.

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

Un `const_reverse_iterator` es un iterador que apunta a contenido constante e itera en orden inverso.
Este iterador se puede aumentar o disminuir (a menos de que sea tambi�n constante),
igual que el iterador que retorna `deque::rbegin`, pero no puede usarse para modificar el contenido al que apunta.

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

### Capacidad

1. #### size

**Retorna el tama�o del contenedor.**

Retorna el n�mero de elementos del contenedor.

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

Este es el tama�o m�ximo potencial que puede alcanzar el contenedor por limitaciones del sistema,
pero no est� garantizado que el contenedor sea capaz de alcanzar ese tama�o.
Asignar almacenamiento en cualquier punto antes de ese tama�o podr�a fallar de igual forma.

F�rmula, basada en el valor que retorna el m�todo en mi computadora: (2^63)/(Tama�o en bytes de *ValueType*) - 1

* **Par�metros**: Ninguno.
* **Retorna**: El n�mero m�ximo de elementos que puede tener el contenedor como contenido.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
size_type max_size() const noexcept;
```

4. #### empty

**Verifica si el contenedor est� vac�o.**

Retorna un valor `bool` que indica si el contenedor est� vac�o, es decir, cuando su tama�o es igual a 0.

* **Par�metros**: Ninguno.
* **Retorna**: `true` si el tama�o del *deque* es 0; de lo contrario, `false`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
bool empty() const noexcept;
```

### Acceso a elementos

1. #### operator[]

**Accede a un elemento.**

Retorna una referencia al elemento en la posici�n *index* del contenedor.

A diferencia de `deque::at`, este m�todo no hace comprobaci�n de l�mites.

* **Par�metros**:
    * *index*: Posici�n de un elemento en el contenedor.
* **Retorna**: El elemento en la posici�n especificada del *deque*.
Si el objeto *deque* es const, el m�todo retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
reference operator[](size_type index);
const_reference operator[](size_type index) const;
```

2. #### at

**Accede a un elemento.**

Retorna una referencia al elemento en la posici�n *index* del contenedor.

Este m�todo verifica autom�ticamente si *index* se encuentra dentro del l�mite de elementos v�lidos en el contenedor.
Si no lo est�, lanza una excepci�n `std::out_of_range`.

* **Par�metros**:
  * *index*: Posici�n de un elemento en el contenedor. Si es mayor o igual al tama�o del contenedor,
  se lanza una excepci�n `std::out_of_range`.
* **Retorna**: El elemento en la posici�n especificada del *deque*. Si el objeto *deque* es const,
el m�todo retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: `std::out_of_range` si *index* est� fuera de los l�mites del contenedor.
* **Declaraci�n**:

```C++
reference at(size_type index);
const_reference at(size_type index) const;
```

3. #### front

**Accede al primer elemento.**

Retorna una referencia al primer elemento en el contenedor.

El llamado a este m�todo en un contenedor vac�o tiene comportamiento indefinido.

* **Par�metros**: Ninguno.
* **Retorna**: Una referencia al primer elemento del contenedor. Si el objeto *deque* es const,
el m�todo retorna una `const_reference`. De lo contrario, retrona una `reference`.
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
* **Retorna**: Una referencia al �ltimo elemento del contenedor. Si el objeto *deque* es const,
el m�todo retorna una `const_reference`. De lo contrario, retrona una `reference`.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
reference back();
const_reference back() const;
```

### Modificadores

1. #### push_back

**Agrega un elemento al final.**

Agrega un nuevo elemento al final del *deque*, despu�s de su �ltimo elemento actual.
El contenido de *value* se copia o se mueve al nuevo elemento.

Este m�todo incrementa eficazmente en uno el tama�o del contenedor.

* **Par�metros**:
    * *value*: El valor del elemento por agregar al contenedor.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
void push_back(const value_type& value);
void push_back(value_type&& value);
```

2. #### push_front

**Inserta un elemento al principio.**

Inserta un nuevo elemento al principio del *deque*, antes de su primer elemento actual.
El contenido de *value* se copia o se mueve al nuevo elemento.

Este m�todo incrementa eficazmente en uno el tama�o del contenedor.

* **Par�metros**:
    * *value*: El valor del elemento por agregar al contenedor.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
void push_front(const value_type& value);
void push_front(value_type&& value);
```

3. #### pop_back

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

4. #### pop_front

**Elimina el primer elemento.**

Elimina el primer elemento del contenedor y reduce eficazmente su tama�o en uno.

* **Par�metros**: Ninguno.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
void pop_front();
```

5. #### insert

**Inserta elementos.**

Inserta nuevos elementos en el *deque* antes del elemento en *position* e incrementa eficazmente
el tama�o del contenedor en el n�mero de elementos insertados.

Las colas de doble final est�n dise�adas para ser eficientes al insertar y eliminar elementos ya sea al final
o al inicio de la secuencia. Las inserciones en otras posiciones normalmente son menos eficientes que en los contenedores
*list* o *forward_list*.

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
		typename std::iterator_traits<InputIterator>::iterator_category>>>
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
* **Complejidad**: Lineal en el n�mero de elementos que se insertan (construcci�n por copia o movimiento)
m�s el n�mero de elementos que estaban antes o despu�s de *position* (elementos movidos).
* **Excepciones**: No se lanzan excepciones.

6. #### erase

**Elimina elementos.**

Elimina del *deque* ya sea un elemento o un rango de elementos.

Este m�todo reduce eficazmente el tama�o del contenedor en el n�mero de elementos eliminados.

* **Par�metros**:
    * *position*: Iterador que apunta al �nico elemento por eliminar del *deque*.
    * *first*, *last*: Iteradores que especifican el rango de elementos a eliminar.
* **Retorna**: Un iterador que apunta a la nueva ubicaci�n del elemento siguiente al �ltimo eliminado.
* **Complejidad**: Lineal en el n�mero de elementos que se eliminados m�s el n�mero de elementos
que estaban antes o despu�s del �ltimo eliminado (elementos movidos).
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
iterator erase(const_iterator position);
iterator erase(const_iterator first, const_iterator last);
```

7. #### swap

**Intercambia contenido.**

Intercambia el contenido de el objeto *deque* por el de *other*, que es otro objeto *deque* del mismo tipo.
Los tama�os pueden ser distintos.

Despu�s del llamado a este m�todo, los elementos en el contenedor son los que estaban anteriormente en *other*
y los elementos en *other* son los que estaban en el contenedor que llama al m�todo.

Este m�todo llama a `std::swap` para intercambiar los atributos privados del *deque*.

* **Par�metros**:
  * *other*: Otro objeto *deque* del mismo tipo, para intercambiar sus elementos.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
void swap(deque& other);
```

8. #### clear

**Vac�a el contenido.**

Elimina todos los elementos del contenedor y lo deja con tama�o 0.

* **Par�metros**: Ninguno.
* **Retorna**: Nada.
* **Complejidad**: Lineal en el n�mero de destrucciones.
* **Declaraci�n**:

```C++
void clear() noexcept;
```

9. #### emplace

**Construye e inserta un elemento.**

Inserta un nuevo elemento en *position*. Este elemento utiliza *args* como los argumentos para su construcci�n.

Este m�todo incrementa eficazmente el tama�o del contenedor en uno.

Las colas de doble final est�n dise�adas para ser eficientes al insertar y eliminar elementos ya sea al final
o al inicio de la secuencia. Las inserciones en otras posiciones normalmente son menos eficientes que en los contenedores
*list* o *forward_list*.

* **Par�metros**:
	* *position*: Posici�n del contenedor donde se inserta el nuevo elemento.
    * *args*: Argumentos que se "reenv�an" (`std::forward`) para construir el nuevo elemento.
* **Retorna**: Un iterador que apunta al nuevo elemento.
* **Complejidad**: Lineal en el n�mero de elementos entre *position* y el inicio o el final del contenedor.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
template <typename... Args>
    iterator emplace(const_iterator position, Args&&... args);
```

10. #### emplace_front

**Construye e inserta un elemento al principio.**

Inserta un nuevo elemento al principio del *deque*, antes de su primer elemento actual.
El contenido de *value* se copia o se mueve al nuevo elemento.

Este m�todo incrementa eficazmente en uno el tama�o del contenedor.

Para insertar un elemento, este m�todo verifica que el primer elemento del contenedor
no se encuentra en la primera posici�n del primer fragmento de memoria
(Para esto, `start.current`, que apunta al primer elemento del contenedor, no debe
estar apuntando a `start.first`, que es la primera posici�n del primer fragmento de memoria).
De lo contrario, crea un nuevo nodo antes del primero y lo inicializa
con un nuevo fragmento de memoria, conecta ese fragmento con el primer fragmento de memoria
viejo e inserta el elemento en la �ltima posici�n de ese nuevo fragmento.

* **Par�metros**:
    * *args*: Argumentos que se "reenv�an" (`std::forward`) para construir el nuevo elemento.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
template <typename... Args>
    void emplace_front(Args&&... args);
```

11. #### emplace_back

**Construye e inserta un elemento al final.**

Agrega un nuevo elemento al final del *deque*, despu�s de su �ltimo elemento actual.
Este elemento utiliza *args* como los argumentos para su construcci�n.

Este m�todo incrementa eficazmente en uno el tama�o del contenedor.

Para insertar un elemento, este m�todo verifica que el �ltimo elemento del contenedor
no se encuentra en las �ltimas dos posiciones del �ltimo fragmento de memoria
(Para esto, `finish.current`, que apunta al elemento siguiente al �ltimo del contenedor,
no debe estar apuntando a `finish.last - 1`, que es la �ltima posici�n del��ltimo fragmento de
memoria). De lo contrario, crea un nuevo nodo despu�s del �ltimo y lo inicializa
con un nuevo fragmento de memoria, inserta el elemento en la �ltima posici�n del �ltimo
fragmento de memoria y conecta ese fragmento con el nuevo.

* **Par�metros**:
    * *args*: Argumentos que se "reenv�an" (`std::forward`) para construir el nuevo elemento.
* **Retorna**: Nada.
* **Complejidad**: Constante.
* **Excepciones**: No se lanzan excepciones.
* **Declaraci�n**:

```C++
template <typename... Args>
    void emplace_back(Args&&... args);
```