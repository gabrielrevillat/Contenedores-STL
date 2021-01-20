# Deque - Diseño y documentación externa

El *deque* es un acrónimo irregular de ***d**ouble-**e**nded **que**ue* (Cola de doble final). 
La cola de doble final es un contenedor secuencial con tamaño dinámico que puede expandirse o
contraerse en ambos finales (ya sea adelante o atrás).

(Se pronuncia como *"deck"* en inglés, o sea que en español sería *"dec"* y no "*diquiú*").

![Abueno nosabia peroya C](https://user-images.githubusercontent.com/64336377/105107696-3ebec180-5a7e-11eb-9ae4-cba5022cfc81.jpg "Keanu Reeves chiquitillo")
Mi reacción al enterarme de lo que es un contenedor *deque*.

## Descripción general (The *embolia* starts now)

Bibliotecas específicas pueden implementar un *deque* de diferentes maneras, generalmente como alguna forma
de arreglo dinámico. En cualquier caso permite el acceso a elementos individuales mediante iteradores,
con el almacenamiento siendo manejado automáticamente al expandir o contraer el contenedor según lo necesario.

Proporciona una funcionalidad similar al *vector*, pero con inserción y eliminado eficientes de elementos no solo
al final, sino también al inicio de la secuencia. Hasta ahora todo bien, **PERO** a diferencia del *vector*, el *deque*
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