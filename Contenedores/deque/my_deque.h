#ifndef MY_DEQUE_H
#define MY_DEQUE_H

#include "../algorithm/my_algorithm.h"

#include <cstddef>

namespace mySTL
{

#ifndef MY_DEQUE_CHUNK_SIZE
#define MY_DEQUE_CHUNK_SIZE 512
#endif

	/**
	 * @brief Controla el tamaño de los fragmentos de memoria.
	 * 
	 * @param size	El tamaño en bytes de un elemento.
	 * @return el número de elementos que puede contener cada nodo.
	 */
	constexpr inline std::size_t my_deque_chunk_size(std::size_t size)
	{
		return ( (size < MY_DEQUE_CHUNK_SIZE) ? std::size_t(MY_DEQUE_CHUNK_SIZE / size) : std::size_t(1) );
	}

	/**
	 * @brief deque::iterator. Se encarga de manejar el acceso a los elementos del contenedor.
	 * 
	 * @tparam ValueType	El tipo de los elementos del contenedor.
	 */
	template <typename ValueType>
	struct my_deque_iterator
	{
		// Primer parámetro de plantilla.
		typedef ValueType value_type;
		// Tipo entero sin signo.
		typedef std::size_t     size_type;
		// Tipo entero con signo.
		typedef std::ptrdiff_t  difference_type;
		// Referencia.
		typedef value_type&		reference;
		// Puntero.
		typedef value_type*		pointer;
		// Puntero a punteros.
		typedef value_type**	map_pointer;
		// Iterador.
		typedef my_deque_iterator<value_type> iterator;
		// Nombre del struct.
		typedef my_deque_iterator self;

		pointer current;	// Puntero a un elemento específico en el fragmento de memoria actual.
		pointer first;		// Puntero al primer elemento del fragmento de memoria actual.
		pointer last;		// Puntero al elemento siguiente al último del fragmento de memoria actual.
		map_pointer node;	// Puntero al nodo del contenedor que apunta al fragmento de memoria.

		/**
		 * Retorna la cantidad de elementos que puede almacenar un fragmento de memoria.
		 * 
		 * @return El número de elementos que almacena cada fragmentos de memoria.
		 */
		static size_type buffer_size() noexcept
		{
			return my_deque_chunk_size( sizeof(value_type) );
		}

		// Sobrecarga de operadores.

		/**
		 * Retorna el contenido al que apunta current.
		 */
		reference operator*() const noexcept
		{
			return *current;
		}

		/**
		 * Retorna el puntero current.
		 */
		pointer operator->() const noexcept
		{
			return current;
		}

		/**
		 * Operador de incremento prefijo.
		 */
		self& operator++() noexcept
		{
			// Incrementar el puntero current.
			++current;
			// Si current alcanza el final del fragmento
			if (current == last)
			{
				// node debe apuntar al siguiente fragmento.
				set_node(node + 1);
				// current ahora debe apuntar al inicio de ese fragmento.
				current = first;
			}
			return *this;
		}

		/**
		 * Operador de incremento postfijo.
		 */
		self operator++(int) noexcept
		{
			// Guardar el estado actual del iterador.
			self temp = *this;
			// Utilizar el operador ++ sobrecargado para incrementar el iterador.
			++*this;
			// Retornar el estado del iterador antes de ser incrementado.
			return temp;
		}

		/**
		 * Operador de disminución prefijo.
		 */
		self& operator--() noexcept
		{
			// Si current está en el inicio del fragmento
			if (current == first)
			{
				// node debe apuntar al fragmento anterior.
				set_node(node - 1);
				// current ahora debe apuntar al final de ese fragmento.
				current = last;
			}
			// Disminuir el puntero current.
			--current;
			return *this;
		}

		/**
		 * Operador de disminución postfijo.
		 */
		self operator--(int) noexcept
		{
			// Guardar el estado actual del iterador.
			self temp = *this;
			// Utilizar el operador -- sobrecargado para disminuir el iterador.
			--*this;
			// Retornar el estado del iterador antes de ser disminuido.
			return temp;
		}

		/**
		 * Operador de suma-asignación.
		 */
		self& operator+=(difference_type count) noexcept
		{
			// Nueva posición a la que va a apuntar el iterador.
			const difference_type new_position = count + (first - current);

			// Si la nueva posición se encuentra dentro de los límites del fragmento
			if ( (new_position >= 0) && (new_position < difference_type( buffer_size() )) )
			{
				// Desplazar el puntero current a la nueva posición.
				current += count;
			}
			else // De lo contrario
			{
				// Crear variable para calcular el desplazamiento del nodo.
				const difference_type node_offset = 0;
				// Si la nueva posición es mayor a 0
				if (new_position > 0)
				{
					// El nodo se desplaza hacia la derecha.
					// Calcular cuántos fragmentos hacia la derecha debe moverse.
					node_offset = new_position / difference_type( buffer_size() );
				}
				else // De lo contrario
				{
					// El nodo se desplaza hacia la izquierda.
					// Calcular cuántos fragmentos hacia la izquierda debe moverse.
					node_offset = -( (-new_position - 1) / difference_type( buffer_size() ) ) - 1;
				}

				// Desplazar node al fragmento correspondiente.
				set_node(node + node_offset);
				// current ahora debe apuntar a la posición específica del fragmento.
				current = first + ( new_position - (node_offset * difference_type( buffer_size() )) );
			}
			return *this;
		}

		/**
		 * Operador de suma.
		 */
		self operator+(difference_type count) const noexcept
		{
			// Guardar el estado actual del iterador para no modificarlo.
			self temp = *this;
			// Retornar la copia del estado actual desplazada count veces.
			return (temp += count);
		}

		/**
		 * Operador de resta-asignación.
		 */
		self& operator-=(difference_type count) noexcept
		{
			return (*this += -count);
		}

		/**
		 * Operador de resta.
		 */
		self operator-(difference_type count) const noexcept
		{
			self temp = *this;
			return (temp -= count);
		}

		/**
		 * Retorna el elemento al que apunta la posición current + @a position.
		 * 
		 * @param position	La posición del elemento que se quiere retornar, relativa a current.
		 */
		reference operator[](difference_type position) const noexcept
		{
			return *(*this + position);
		}

		// Asignación de nodo.

		/**
		 * Asigna los punteros del iterador de manera que apunten correctamente a @a new_node.
		 * El puntero current debe ser asignado correctamente después del llamado a este método.
		 * 
		 * @param new_node	Puntero a un nodo del contenedor que apunta a un fragmento de memoria específico.
		 */
		void set_node(map_pointer new_node) noexcept
		{
			// El nodo del iterador es new_node.
			node = new_node;
			// first apunta al contenido de new_node, que es un puntero al primer elemento del fragmento de memoria.
			first = *new_node;
			// last apunta al final de ese fragmento de memoria.
			last = first + difference_type( buffer_size() );
		}

	};

	// Sobrecarga de operadores para deque::iterator

	template <typename ValueType>
	inline bool operator==(const my_deque_iterator<ValueType>& iterator1,
		const my_deque_iterator<ValueType>& iterator2) noexcept
	{
		// Si ambos apuntan al mismo elemento.
		return (iterator1.current == iterator2.current);
	}

	template <typename ValueType>
	inline bool operator!=(const my_deque_iterator<ValueType>& iterator1,
		const my_deque_iterator<ValueType>& iterator2) noexcept
	{
		return !(iterator1 == iterator2);
	}

	template <typename ValueType>
	inline bool operator<(const my_deque_iterator<ValueType>& iterator1,
		const my_deque_iterator<ValueType>& iterator2) noexcept
	{
		// Si ambos apuntan al mismo nodo
		return (iterator1.node == iterator2.node)
			// Verificar cuál de los dos apunta a un elemento en alguna posición anterior.
			? (iterator1.current < iterator2.current)
			// De lo contrario, verificar cuál de los dos apunta a un nodo anterior.
			: (iterator1.node < iterator2.node);
		
	}

	template <typename ValueType>
	inline bool operator<=(const my_deque_iterator<ValueType>& iterator1,
		const my_deque_iterator<ValueType>& iterator2) noexcept
	{
		return !(iterator2 < iterator1);
	}

	template <typename ValueType>
	inline bool operator>(const my_deque_iterator<ValueType>& iterator1,
		const my_deque_iterator<ValueType>& iterator2) noexcept
	{
		return (iterator2 < iterator1);
	}

	template <typename ValueType>
	inline bool operator>=(const my_deque_iterator<ValueType>& iterator1,
		const my_deque_iterator<ValueType>& iterator2) noexcept
	{
		return !(iterator1 < iterator2);
	}

	/**
	 * @brief Contenedor secuencial con tamaño dinámico que puede
	 * expandirse o contraerse en ambos finales.
	 * 
	 * @tparam ValueType El tipo de los elementos que contiene.
	 */
	template <typename ValueType>
	class deque
	{
	// Tipos miembro públicos
	public:
		// Primer parámetro de plantilla.
		typedef ValueType value_type;
		// Tipo entero sin signo.
		typedef std::size_t     size_type;
		// Tipo entero con signo.
		typedef std::ptrdiff_t  difference_type;
		// Referencia.
		typedef value_type& reference;
		// Referencia constante.
		typedef const value_type& const_reference;
		// Puntero.
		typedef value_type* pointer;
		// Puntero constante.
		typedef const value_type* const_pointer;
		// Iterador.
		typedef my_deque_iterator<value_type> iterator;
		// Iterador constante.
		typedef const my_deque_iterator<value_type> const_iterator;
		// Iterador inverso.
		typedef std::reverse_iterator<iterator> reverse_iterator;
		// Iterador inverso constante.
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	// Tipos miembro privados
	private:
		// Puntero a punteros.
		typedef pointer* map_pointer;

	// Atributos privados
	private:
		map_pointer map;	// Arreglo de punteros a fragmentos de memoria que almacenan elementos.
		size_type map_size;	// El número de punteros a los que apunta *map*. Esta cantidad es de al menos 8.
		iterator start;		// Iterador que apunta al primer elemento del contenedor.
		iterator finish;	// Iterador que apunta al elemento siguiente al último del contenedor.

	};
}

#endif /* MY_DEQUE_H */
