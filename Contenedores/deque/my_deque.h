#ifndef MY_DEQUE_H
#define MY_DEQUE_H

#include "../algorithm/my_algorithm.h"

#include <iterator>
#include <cstddef>
#include <type_traits>

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
		// Identifica este struct como un iterador de acceso aleatorio.
		typedef std::random_access_iterator_tag iterator_category;
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
				difference_type node_offset = 0;
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

	// Sobrecarga de operador de resta para calcular
	// distancia entre dos elementos apuntados por iteradores.
	// Revisar explicación de la fórmula en la documentación externa.

	template <typename ValueType>
	inline typename my_deque_iterator<ValueType>::difference_type
		operator-(const my_deque_iterator<ValueType>& iterator1,
			const my_deque_iterator<ValueType>& iterator2) noexcept
	{
		return typename my_deque_iterator<ValueType>::difference_type
			(my_deque_iterator<ValueType>::buffer_size())
			* (iterator1.node - iterator2.node - 1)
			+ (iterator1.current - iterator1.first)
			+ (iterator2.last - iterator2.current);
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

	// Métodos públicos
	public:
		// Constructores y destructor.

		/**
		 * Constructor por defecto. 
		 *
		 * Construye un contendor vacío, sin elementos.
		 */
		explicit deque()
			: map(nullptr) // Inicializar atributos por defecto.
			, map_size(0)
			, start()
			, finish()
		{
			// Inicializar arreglo de nodos sin elementos.
			this->create_map_and_nodes(0);
		}

		/**
		 * Constructor de relleno. 
		 *
		 * Construye el contenedor con @a count elementos. No se hacen copias.
		 *
		 * @param count El tamaño inicial del contenedor.
		 */
		explicit deque(size_type count)
			: map(nullptr) // Inicializar atributos por defecto.
			, map_size(0)
			, start()
			, finish()
		{
			// Inicializar arreglo de nodos con count elementos.
			this->create_map_and_nodes(count);
		}

		/**
		 * Constructor de relleno. 
		 *
		 * Construye el contenedor con @a count elementos. Cada elemento es una copia de @a value.
		 *
		 * @param count El tamaño inicial del contenedor.
		 * @param value Valor para inicializar los elementos del contenedor.
		 */
		deque(size_type count, const value_type& value)
			: map(nullptr) // Inicializar atributos por defecto.
			, map_size(0)
			, start()
			, finish()
		{
			// Inicializar arreglo de nodos con count elementos.
			this->create_map_and_nodes(count);

			// Recorrer entre los nodos que apuntan a los fragmentos de memoria
			for (map_pointer current = this->start.node;
				current < this->finish.node; ++current)
				// Llenar el fragmento de memoria actual con el valor value.
				mySTL::fill(*current, *current + buffer_size(), value);

			// Llenar el último fragmento hasta alcanzar la posición
			// del último elemento.
			mySTL::fill(this->finish.first, this->finish.current, value);
		}

		/**
		 * Constructor de rango. 
		 *
		 * Construye el contenedor con tantos elementos como el rango [first, last).
		 *
		 * @param first, last   Iteradores a las posiciones inicial y final en un rango.
		 */
		template <typename InputIterator,
			typename = typename std::enable_if_t<std::is_base_of_v<std::input_iterator_tag,
				typename std::iterator_traits<InputIterator>::iterator_category>>>
		deque(InputIterator first, InputIterator last)
			: map(nullptr)
			, map_size(0)
			, start()
			, finish()
		{
			// El número de elementos es la distancia entre los iteradores.
			size_type count = std::distance(first, last);
			// Inicializar arreglo de nodos con count elementos.
			this->create_map_and_nodes(count);

			// Copiar los elementos del rango al contenedor.
			mySTL::copy(first, last, this->start);
		}

		/**
		 * Constructor de copia. 
		 * 
		 * Construye el contenedor con una copia de cada uno de los elementos de @a other,
		 * en el mismo orden.
		 *
		 * @param other Otro objeto vector del mismo tipo, para inicializar el contenedor.
		 */
		deque(const deque& other)
			: map(nullptr)
			, map_size(0)
			, start()
			, finish()
		{
			// Inicializar arreglo de nodos con el número de elementos de other.
			this->create_map_and_nodes( other.size() );
			// Copiar los elementos de other al contenedor.
			mySTL::copy(other.begin(), other.end(), this->start);
		}

		/**
		 * Constructor de movimiento. 
		 * 
		 * Construye un contenedor que adquiere los elementos de @a temp,
		 * con semánticas de movimiento.
		 * 
		 * @param temp  Otro objeto vector del mismo tipo,
		 * para inicializar el contenedor.
		 */
		deque(deque&& temp)
			// Inicializar atributos directamente con los del contenedor recibido.
			: map(temp.map)
			, map_size(temp.map_size)
			, start(temp.start)
			, finish(temp.finish)
		{
			// Dejar vacío el contenedor temp.
			temp.map = nullptr;
			temp.start = temp.finish = nullptr;
			temp.map_size = 0;
		}

		deque(std::initializer_list<value_type> init_list)
			: map(nullptr)
			, map_size(0)
			, start()
			, finish()
		{
			// El número de elementos es el tamaño de init_list.
			size_type count = init_list.size();
			// Inicializar arreglo de nodos con count elementos.
			this->create_map_and_nodes(count);

			// Copiar los elementos de init_list al contenedor.
			mySTL::copy(init_list.begin(), init_list.end(), this->start);
		}

		/**
		* Destructor.
		*
		* Destruye el objeto contenedor.
		*/
		~deque()
		{
			this->destroy_map_and_nodes();
		}

		// Operador de asignación.

		// Modificador assign.

		// Iteradores.

		/// Retorna un iterador al prinicipio.
		inline iterator begin() noexcept { return this->start; } // Retornar el iterador que apunta al inicio del contenedor.
		/// Retorna un iterador al prinicipio.
		inline const_iterator begin() const noexcept { return this->start; }
		/// Retorna un iterador constante al prinicipio.
		inline const_iterator cbegin() const noexcept { return begin(); }

		/// Retorna un iterador al final.
		inline iterator end() noexcept { return this->finish; } // Retornar el iterador que apunta al final del contenedor.
		/// Retorna un iterador al final.
        inline const_iterator end() const noexcept { return this->finish; }
		/// Retorna un iterador constante al final.
		inline const_iterator cend() const noexcept { return end(); }

		// Capacidad

		/// Retorna la cantidad de elementos del contenedor.
		inline size_type size() const noexcept { return std::distance(begin(), end()); }


	// Métodos privados
	private:

		/**
		 * Retorna la cantidad de elementos que puede almacenar un fragmento de memoria.
		 * 
		 * @return El número de elementos que almacena cada fragmentos de memoria.
		 */
		static size_type buffer_size() noexcept
		{
			return my_deque_chunk_size( sizeof(value_type) );
		}

		/**
		 * Inicializa el arreglo de punteros según @a elements_count 
		 * y asigna un fragmento a cada nodo.
		 * 
		 * @param elements_count	El número de elementos que va a contener el contenedor.
		 */
		void create_map_and_nodes(size_type elements_count)
		{
			// Calcular el número de nodos necesarios para el contenedor. 
			// (Número de elementos entre el tamaño de los fragmentos de memoria).
			size_type nodes_count = elements_count / buffer_size() + 1;

			// Asignar el tamaño del arreglo de nodos.
			// Si el número de nodos es menor a 8, asignar 8.
			// De lo contrario, asignar el número de nodos más 2.
			this->map_size = mySTL::max(size_type(8), nodes_count + 2);
			// Inicializar el arreglo de punteros con el tamaño calculado.
			this->map = new pointer[map_size];

			// Asignar punteros a nodos de inicio y final de manera que en 
			// ambos extremos del contenedor quede al menos un nodo vacío.
			map_pointer start_node = map + ( (this->map_size - nodes_count) / 2 );
			map_pointer finish_node = start_node + nodes_count - 1;

			// Inicializar cada fragmento de memoria desde
			// start_node hasta finish_node.
			for (map_pointer current = start_node; current <= finish_node; ++current)
				*current = new value_type[ buffer_size() ];

			// Asignar los atributos de los iteradores del contenedor.
			this->start.set_node(start_node);
			this->finish.set_node(finish_node);
			// start.current debe apuntar al primer elemento del primer fragmento.
			this->start.current = this->start.first;
			// finish.current debe apuntar al elemento siguiente al último
			// del último fragmento.
			this->finish.current = this->finish.first + elements_count % buffer_size();
		}

		/**
		 * Destruye el arreglo de punteros y su contenido.
		 */
		void destroy_map_and_nodes()
		{
			// Destruir cada fragmento de memoria apuntado
			// desde start hasta finish.
			for (map_pointer current = this->start.node;
				current < this->finish.node; ++current)
				delete [] *current;

			delete [] map; // Destruir arreglo de nodos.
		}


	};

}

#endif /* MY_DEQUE_H */
