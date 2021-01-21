# Deque - Dise�o y documentaci�n externa

El *deque* es un acr�nimo irregular de ***d**ouble-**e**nded **que**ue* (Cola de doble final). 
La cola de doble final es un contenedor secuencial con tama�o din�mico que puede expandirse o
contraerse en ambos finales (ya sea adelante o atr�s).

(Se pronuncia como *"deck"* en ingl�s, o sea que en espa�ol ser�a *"dec"* y no "*diqui�*").

![Abueno nosabia peroya C](https://user-images.githubusercontent.com/64336377/105109974-3d43c800-5a83-11eb-8477-60c664b000e9.jpg "Keanu Reeves chiquitillo")
Mi reacci�n al enterarme de lo que es un contenedor *deque* xd.

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

## Idea de implementaci�n

### Tama�o de los fragmentos de memoria.

Tenemos que los elementos del *deque* estar�n dispersados en diferentes fragmentos de memoria. Estos fragmentos son
de tama�o fijo, por lo tanto hay que pensar en qu� tama�o tendr�n. En la implementaci�n de `libstdc++` se puede observar
que para calcular el *n�mero de elementos* que tendr� un fragmento o nodo, se utiliza un n�mero fijo de 512 y se divide
entre el *tama�o en bytes* del tipo de los elementos del *deque*. Si el tama�o en bytes del tipo de elementos es mayor a 512,
cada fragmento/nodo tendr� capacidad para solo un elemento.

### Iteradores y acceso a los fragmentos de memoria.

El contenedor debe ser capaz de proporcionar el acceso a cualquiera de sus elementos en tiempo constante y mediante iteradores.
Se debe entonces implementar un iterador especial que conecte los fragmentos de memoria y permita ese acceso correctamente.
El contenedor principal ser� un puntero a punteros, cada puntero apuntar� a un fragmento de memoria y los iteradores ser�n
capaces de "saltar" entre esos fragmentos para tener acceso a todos los elementos.

Cada iterador va a contener tres punteros: Uno apuntar� a un elemento espec�fico en un fragmento de memoria (el elemento al
que se est� apuntando actualmente), y los otros apuntar�n al inicio y al final del fragmento. Tambi�n tendr� un puntero a punteros
que apunte al puntero del contenedor que apunta directamente a un fragmento de memoria espec�fico.

![Ejemplo de dise�o del iterador para el *deque* y los atributos que contiene.](https://user-images.githubusercontent.com/64336377/105266781-ec050880-5b56-11eb-8127-3225299d80f1.png "Dise�o del deque_iterator")
Dise�o que muestra c�mo ser�a un iterador del contenedor *deque* y hacia d�nde apuntar�an sus atributos.

El ejemplo anterior presenta un puntero a punteros llamado *map*, este es el contenedor principal que apunta hacia
los fragmentos de memoria que contienen los elementos. En la imagen, cada puntero est� apuntando a un fragmento, aunque
podr�a presentarse el caso donde algunos punteros cercanos a los extremos no apunten a ning�n fragmento de memoria hasta que
sea necesario, por la cantidad de elementos. Si cada puntero apunta a un fragmento lleno y se necesita m�s espacio, se crea
un contenedor con m�s espacio y se reasignan los punteros.

El *deque* va a contener dos iteradores en sus atributos: El primero, con su puntero `first` apunta al inicio del primer
fragmento de memoria donde hay elementos y con su puntero `current` apunta directamente al primer elemento de ese fragmento.
El segundo iterador apunta con su puntero `first` al inicio del �ltimo fragmento de memoria donde hay elementos y con
su puntero `current` apunta directamente al elemento siguiente al �ltimo de ese fragmento.

![Representaci�n de los dos iteradores que va a tener internamente el *deque*.](https://user-images.githubusercontent.com/64336377/105269247-2b345900-5b59-11eb-8931-374882561b61.png "Atributos privados del deque")
Representaci�n de los dos iteradores que va a tener internamente el contenedor y los elementos a los que apuntan los atributos de cada iterador (ft. despiche de flechas xd).




