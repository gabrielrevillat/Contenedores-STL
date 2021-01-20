# Deque - Dise�o y documentaci�n externa

El *deque* es un acr�nimo irregular de ***d**ouble-**e**nded **que**ue* (Cola de doble final). 
La cola de doble final es un contenedor secuencial con tama�o din�mico que puede expandirse o
contraerse en ambos finales (ya sea adelante o atr�s).

(Se pronuncia como *"deck"* en ingl�s, o sea que en espa�ol ser�a *"dec"* y no "*diqui�*").

![Abueno nosabia peroya C](https://user-images.githubusercontent.com/64336377/105107696-3ebec180-5a7e-11eb-9ae4-cba5022cfc81.jpg "Keanu Reeves chiquitillo")
Mi reacci�n al enterarme de lo que es un contenedor *deque*.

## Descripci�n general (The *embolia* starts now)

Bibliotecas espec�ficas pueden implementar un *deque* de diferentes maneras, generalmente como alguna forma
de arreglo din�mico. En cualquier caso permite el acceso a elementos individuales mediante iteradores,
con el almacenamiento siendo manejado autom�ticamente al expandir o contraer el contenedor seg�n lo necesario.

Proporciona una funcionalidad similar al *vector*, pero con inserci�n y eliminado eficientes de elementos no solo
al final, sino tambi�n al inicio de la secuencia. Hasta ahora todo bien, **PERO** a diferencia del *vector*, el *deque*
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