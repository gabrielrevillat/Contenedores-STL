#ifndef MY_ALGORITHM_H
#define MY_ALGORITHM_H

#include <utility>

namespace mySTL
{
	// Este encabezado contiene algoritmos que se utilizan en varios contenedores.
	// Estos algoritmos poseen los mismos nombres del namespace std y su implementación
	// está basada en el ejemplo de implementación mostrado en cplusplus.com

	/**
	 * Intercambia los valores de cada uno de los elementos en el rango [@a first1, @a last1)
	 * con los de sus respectivos elementos en el rango que empieza en @a first2.
	 * 
	 * @param first1	Iterador a la posición inicial de una de las secuencias por intercambiar.
	 * @param last1		Iterador a la posición final de una de las secuencias por intercambiar.
	 * @param first2	Iterador a la posición inicial de la otra secuencia por intercambiar.
	 */
	template <typename ForwardIterator1, typename ForwardIterator2>
	void swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
	{
		// Mientras no se haya alcanzado el final del rango
		while (first1 != last1)
		{
			// Intercambiar los elementos a los que apuntan los iteradores first1 y first2.
			std::swap(*first1, *first2);
			// Incrementar ambos iteradores.
			++first1;
			++first2;
		}
	}

	/**
	 * Copia los elementos en el rango [@a first, @a last) al rango que empieza en @a result.
	 * 
	 * @param first		Iterador a la posición inicial de la secuencia por copiar.
	 * @param last		Iterador a la posición final de la secuencia por copiar.
	 * @param result	Iterador a la posición inicial de la secuencia destino.
	 */
	template <typename InputIterator, typename OutputIterator>
	void copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		// Mientras no se haya alcanzado el final del rango
		while (first != last)
		{
			// Copiar el elemento al que apunta first, al elemento al que apunta result.
			*result = *first;
			// Incrementar ambos iteradores.
			++first;
			++result;
		}
	}

	/**
	 * Copia @a count elementos del rango que empieza en @a first al rango que empieza en @a result.
	 * 
	 * @param first		Iterador a la posición inicial de la secuencia por copiar.
	 * @param count		El número de elementos por copiar.
	 * @param result	Iterador a la posición inicial de la secuencia destino.
	 */ 
	template <typename InputIterator, typename Size, typename OutputIterator>
	void copy_n(InputIterator first, Size count, OutputIterator result)
	{
		// Desde index = 0 hasta index = (count - 1)
		for (Size index = 0; index < count; ++index)
		{
			// Copiar el elemento al que apunta first, al elemento al que apunta (result + index).
			*(result + index) = *first;
			// Incrementar el iterador first.
			++first;
		}
	}

	/**
	 * Copia los elementos en el rango [@a first, @a last) empezando desde el final,
	 * al rango que termina en @a result.
	 * 
	 * @param first		Iterador a la posición inicial de la secuencia por copiar.
	 * @param last		Iterador a la posición final de la secuencia por copiar.
	 * @param result	Iterador a la posición siguiente a la última de la secuencia destino.
	 */ 
	template <typename BidirectionalIterator1, typename BidirectionalIterator2>
	void copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result)
	{
		// Mientras no se haya alcanzado el inicio del rango
		while (last != first)
		{
			// Disminuir ambos iteradores.
			--last;
			--result;
			// Copiar el elemento al que apunta last, al elemento al que apunta result.
			*result = *last;
		}
	}

	/**
	 * Asigna el valor @a value a los elementos en el rango [@a first, @a last).
	 * 
	 * @param first	Iterador a la posición inicial de una secuencia de elementos.
	 * @param last	Iterador a la posición final de una secuencia de elementos.
	 * @param value	El valor por asignar a los elementos en el rango de relleno.
	 */
	template <typename ForwardIterator, typename ValueType>
	void fill(ForwardIterator first, ForwardIterator last, const ValueType& value)
	{
		// Mientras no se haya alcanzado el final del rango
		while (first != last)
		{
			// Asignar el valor value al elemento al que apunta first.
			*first = value;
			// Incrementar el iterador first.
			++first;
		}
	}

	/**
	 * Compara los elementos en el rango [@a first1, @a last1) con los del rango que empieza en @a first2
	 * y retorna TRUE si todos los elementos en ambos rangos coinciden.
	 * 
	 * @param first1	Iterador a la posición inicial de la primera secuencia por comparar.
	 * @param last1		Iterador a la posición final de la primera secuencia por comparar.
	 * @param first2	Iterador a la posición inicial de la segunda secuencia por comparar.
	 * @return TRUE si todos los elementos entre ambas secuencias coinciden; de lo contrario FALSE.
	 */
	template <typename InputIterator1, typename InputIterator2>
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		// Mientras no se haya alcanzado el final del rango
		while (first1 != last1)
		{
			// Si los elementos actuales a los que apuntan los iteradores no coinciden
			if (*first1 != *first2)
				return false; // Los rangos no son iguales. Retornar FALSE.

			// De lo contario, incrementar ambos iteradores.
			++first1;
			++first2;
		}
		// Si se llega al final, los rangos coinciden. Retornar TRUE.
		return true;
	}

	/**
	 * Retorna TRUE si el rango [@a first1, @a last1) es lexicográficamente menor que el rango [@a first2, @a last2).
	 * 
	 * @param first1	Iterador a la posición inicial de la primera secuencia por comparar.
	 * @param last1		Iterador a la posición final de la primera secuencia por comparar.
	 * @param first2	Iterador a la posición inicial de la segunda secuencia por comparar.
	 * @param last2		Iterador a la posición final de la segunda secuencia por comparar.
	 */
	template <typename InputIterator1, typename InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2)
	{
		// Mientras no se haya alcanzado el final del rango de la primera secuencia
		while (first1 != last1)
		{
			// Si ya se alcanzó el final del rango de la segunda secuencia
			// O si el elemento actual al que apunta el iterador de la segunda secuencia es menor al de la primera
			if ( (first2 == last2) || (*first2 < *first1) )
				return false; // La primera secuencia no es menor. Retornar FALSE.

			// De lo contrario, si el elemento actual al que apunta el iterador de la primera secuencia
			// es menor al de la segunda
			if (*first1 < *first2)
				return true; // La primera secuencia es menor. Retornar TRUE.

			// De lo contrario (los elementos actuales son iguales), incrementar ambos iteradores.
			++first1;
			++first2;
		}

		// Si se llega al final de la primera secuencia pero no se ha alcanzado el final de la segunda,
		// la primera secuencia es menor; de lo contrario, no lo es.
		if (first2 != last2)
			return true; // Es menor.

		return false; // No es menor.
	}

	/**
	 * Retorna el mayor entre @a value1 y @a value2. Si ambos son iguales, 
	 * retorna @a value1.
	 * 
	 * @param value1, value2	Valores por comparar.
	 * @return El mayor de los valores recibidos.
	 */
	template <typename ValueType>
	inline const ValueType& max(const ValueType& value1, const ValueType& value2)
	{
		// Si el primer valor es menor que el segundo, retornar el segundo valor.
		// De lo contrario, retornar el primer valor.
		return ( (value1 < value2) ? value2 : value1 );
	}
}

#endif /* MY_ALGORITHM_H */
