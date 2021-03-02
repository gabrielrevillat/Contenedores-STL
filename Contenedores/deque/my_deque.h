#ifndef MY_DEQUE_H
#define MY_DEQUE_H

#include "../algorithm/my_algorithm.h"

#include <iterator>
#include <cmath>
#include <cstddef>
#include <utility>
#include <stdexcept>
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
			const difference_type new_position = count + (current - first);

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

		/**
		 * Asignación por copia. 
		 * 
		 * Reemplaza el contenido del contenedor por una copia del contenido
		 * de @a other.
		 * 
		 * @param other Otro objeto deque del mismo tipo, para reemplazar
		 * el contenido del contenedor.
		 * @return *this.
		 */
		deque& operator=(const deque& other)
		{
			// Construir una copia temporal del deque recibido.
			// De esta manera no se modifica el deque recibido al intercambiar los elementos.
			auto temp_copy = deque(other);
			// Intercambiar atributos con esa copia creada.
			this->swap(temp_copy);
			return *this;
		}

		/**
		 * Asignación por movimiento. 
		 * 
		 * Reemplaza el contenido del contenedor al mover dentro de este el contenido de @a temp.
		 * 
		 * @param temp  Otro objeto deque del mismo tipo, para reemplazar
		 * el contenido del contenedor.
		 * @return *this.
		 */
		deque& operator=(deque&& temp)
		{
			this->swap(temp); // Intercambiar elementos, no importa si temp se modifica.
			return *this;
		}

		deque& operator=(std::initializer_list<value_type> init_list)
		{
			// Construir un deque con la lista de inicialización recibida.
			auto temp_il_deque = deque(init_list);
			// Intercambiar elementos con el deque creado.
			this->swap(temp_il_deque);
			return *this;
		}

		// Modificador assign.

		/**
		 * Reemplaza el contenido del contenedor con @a count elementos, 
		 * donde cada elemento es una copia de @a value.
		 * 
		 * @param count El nuevo tamaño del contenedor.
		 * @param value Valor para asignar los elementos del contenedor.
		 */
		void assign(size_type count, const value_type& value)
		{
			// Crear un deque con los parámetros recibidos.
			auto temp_fill_deque = deque(count, value);
			// Intercambiar elementos con el deque creado.
			this->swap(temp_fill_deque);
		}

		/**
		 * Reemplaza el contenido del objeto con elementos
		 * en el rango [@a first, @a last).
		 * 
		 * @param first, last	Iteradores a las posiciones inicial y final
		 * en un rango.
		 */
		template <typename InputIterator,
			typename = typename std::enable_if_t<std::is_base_of_v<std::input_iterator_tag,
				typename std::iterator_traits<InputIterator>::iterator_category>>>
		void assign(InputIterator first, InputIterator last)
		{
			auto temp_it_deque = deque(first, last);
			this->swap(temp_it_deque);
		}

		/**
		 * Reemplaza el contenido del contenedor por una copia de
		 * los elementos de @a init_list.
		 * 
		 * @param init_list Objeto initializer_list para reemplazar
		 * el contenido del contenendor.
		 */
		void assign(std::initializer_list<value_type> init_list)
		{
			auto temp_il_deque = deque(init_list);
			this->swap(temp_il_deque);
		}

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

		/// Retorna un iterador inverso al principio del contenedor invertido.
		inline reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
		/// Retorna un iterador inverso al principio del contenedor invertido.
		inline const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
		/// Retorna un iterador inverso constante al principio del contenedor invertido.
		inline const_reverse_iterator crbegin() const noexcept { return rbegin(); }

		/// Retorna un iterador inverso al final del contenedor invertido.
		inline reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
		/// Retorna un iterador inverso al final del contenedor invertido.
		inline const_reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }
		/// Retorna un iterador inverso constante al final del contenedor invertido.
		inline const_reverse_iterator crend() const noexcept { return rend(); }

		// Capacidad.

		/// Retorna la cantidad de elementos del contenedor.
		inline size_type size() const noexcept { return std::distance(begin(), end()); }

		/// Retorna la cantidad máxima de elementos que puede contener el contenedor.
		inline size_type max_size() const noexcept
		{
			// Fórmula basada en el resultado obtenido del max_size del std en mi computadora.
			return ( ( pow(2, 63) / sizeof(value_type) ) - 1 );
		}



		/// Verifica si el contenedor está vacío.
		inline bool empty() const noexcept { return (size() == 0); }

		// Acceso a elementos.

		/**
		 * Retorna una referencia al elemento en la posición @a index del contenedor.
		 * 
		 * @param index Posición de un elemento en el contenedor.
		 * @return El elemento en la posición especificada.
		 */
		inline reference operator[](size_type index) { return *(begin() + index); }

		/**
		* Retorna una referencia al elemento en la posición @a index del contenedor.
		* 
		* @param index Posición de un elemento en el contenedor.
		* @return El elemento en la posición especificada.
		*/
		inline const_reference operator[](size_type index) const { return *(begin() + index); }

		/**
		 * Retorna una referencia al elemento en la posición @a index del contenedor. 
		 * 
		 * Este método verifica si @a index está dentro del límite de elementos
		 * válidos en el contenedor. Si no lo está, lanza una excepción.
		 * 
		 * @param index Posición de un elemento en el contenedor.
		 * @throw std::out_of_range
		 * @return El elemento en la posición especificada.
		 */
		inline reference at(size_type index)
		{
			if (index >= size())
				throw std::out_of_range("mySTL::deque::at");

			return *(begin() + index);
		}

		/**
		* Retorna una referencia al elemento en la posición @a index del contenedor. 
		* 
		* Este método verifica si @a index está dentro del límite de elementos
		* válidos en el contenedor. Si no lo está, lanza una excepción.
		* 
		* @param index Posición de un elemento en el contenedor.
		* @throw std::out_of_range
		* @return El elemento en la posición especificada.
		*/
		inline const_reference at(size_type index) const
		{
			if (index >= size())
				throw std::out_of_range("mySTL::deque::at");

			return *(begin() + index);
		}

		/// Retorna una referencia al primer elemento en el contenedor.
		inline reference front() { return *(begin()); }
		/// Retorna una referencia al primer elemento en el contenedor.
		inline const_reference front() const { return *(begin()); }

		/// Retorna una referencia al último elemento en el contenedor.
		inline reference back() { return *(end() - 1); }
		/// Retorna una referencia al último elemento en el contenedor.
		inline const_reference back() const { return *(end() - 1); }

		// Modificadores.

		/**
		 * Agrega un nuevo elemento al final del contenedor y aumenta su tamaño.
		 * 
		 * @param value El valor del elemento por agregar al contenedor.
		 */
		void push_back(const value_type& value)
		{
			this->emplace_back(value);
		}

		/**
		* Agrega un nuevo elemento al final del contenedor y aumenta su tamaño.
		* 
		* @param value El valor del elemento por agregar al contenedor.
		*/
		void push_back(value_type&& value)
		{
			this->emplace_back(std::forward<value_type>(value));
		}

		/**
		 * Construye e inserta un elemento al final del contenedor.
		 * 
		 * @param args  Argumentos para construir el nuevo elemento.
		 */
		template <typename... Args>
		void emplace_back(Args&&... args)
		{
			// Si queda espacio en el último fragmento de memoria
			// (Para al menos dos elementos)
			if (this->finish.current != this->finish.last - 1)
			{
				// Construir el nuevo elemento en la posición siguiente a la
				// del último elemento. 
				this->construct_element(this->finish.current, std::forward<Args>(args)...);
				// Incrementar el iterador finish para que siga apuntando a
				// la posición siguiente a la del último elemento.
				++this->finish.current;
			}
			else // De lo contrario
			{
				// Reservar espacio al final del mapa
				this->reserve_map_at_back();
				// Crear un nuevo nodo después del último
				// e inicializarlo con un nuevo fragmento/buffer.
				*(this->finish.node + 1) = new value_type[ buffer_size() ];
				// Construir el nuevo elemento en la última posición
				// del último fragmento viejo.
				this->construct_element(this->finish.current, std::forward<Args>(args)...);
				// Reajustar el iterador finish para que apunte al nuevo último nodo.
				this->finish.set_node(this->finish.node + 1);
				this->finish.current = this->finish.first;
			}
		}

		/**
		* Agrega un nuevo elemento al inicio del contenedor y aumenta su tamaño.
		* 
		* @param value El valor del elemento por agregar al contenedor.
		*/
		void push_front(const value_type& value)
		{
			this->emplace_front(value);
		}

		/**
		* Agrega un nuevo elemento al inicio del contenedor y aumenta su tamaño.
		* 
		* @param value El valor del elemento por agregar al contenedor.
		*/
		void push_front(value_type&& value)
		{
			this->emplace_front(std::forward<value_type>(value));
		}

		/**
		* Construye e inserta un elemento al principio del contenedor.
		* 
		* @param args  Argumentos para construir el nuevo elemento.
		*/
		template <typename... Args>
		void emplace_front(Args&&... args)
		{
			// Si queda espacio al inicio del primer fragmento de memoria
			// (Para al menos un elemento)
			if (this->start.current != this->start.first)
			{
				// Construir el nuevo elemento en la posición anterior a la
				// del primer elemento.
				this->construct_element(this->start.current - 1,
					std::forward<Args>(args)...);
				// Disminuir el iterador start para que siga apuntando a
				// la posición del primer elemento.
				--this->start.current;
			}
			else // De lo contrario
			{
				// Reservar espacio al inicio del mapa
				this->reserve_map_at_front();
				// Crear un nuevo nodo antes del primero
				// e inicializarlo con un nuevo fragmento/buffer.
				*(this->start.node - 1) = new value_type[ buffer_size() ];
				// Reajustar el iterador start para que apunte al nuevo primer nodo.
				this->start.set_node(this->start.node - 1);
				this->start.current = this->start.last - 1;
				// Construir el nuevo elemento en la última posición
				// del nuevo primer fragmento.
				this->construct_element(this->start.current, std::forward<Args>(args)...);
			}
		}

		/**
		 * Elimina el último elemento del contenedor y reduce su tamaño.
		 */
		void pop_back()
		{
			// Si el úlitmo fragmento de memoria contiene al menos un elemento
			if (this->finish.current != this->finish.first)
			{
				// Disminuir el iterador finish para que apunte
				// al elemento que se quiere eliminar.
				// (Ya que finish siempre apunta al elemento 
				// *siguiente al último* del contenedor).
				--this->finish.current;
				// Destruir el elemento.
				delete this->finish.current;
			}
			else // De lo contrario
			{
				// Destruir el último fragmento de memoria.
				delete [] this->finish.first;
				// Reajustar el iterador finish para que apunte al
				// nodo anterior.
				this->finish.set_node(this->finish.node - 1);
				this->finish.current = this->finish.last - 1;
				// Destruir el elemento.
				delete this->finish.current;
			}
		}

		/**
		 * Intercambia el contenido de este objeto por el contenido de @a other.
		 * 
		 * @param other Otro objeto deque del mismo tipo, para intercambiar sus elementos.
		 */
		void swap(deque& other)
		{
			using std::swap; // Función swap de la biblioteca estándar.
			swap(this->map, other.map);
			swap(this->map_size, other.map_size);
			swap(this->start, other.start);
			swap(this->finish, other.finish);
		}


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
		 * Construye un elemento con el valor @a value en la posición @a position.
		 * 
		 * @param position	La posición donde se construye el nuevo elemento.
		 * @param value		El valor del elemento por construir.
		 */
		void construct_element(pointer position, const value_type& value)
		{
			*position = value;
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

			delete [] this->map; // Destruir arreglo de nodos.
		}

		/**
		 * Realiza una reasignación de los nodos del mapa o del mapa en sí,
		 * según el espacio sobrante en los extremos y según por cuál extremo
		 * se están agregando los nuevos elementos.
		 * 
		 * @param num_of_nodes_to_add	El número de nodos que se agregan a map.
		 * @param adding_at_front		Indica el extremo por el cual se agregan
		 * los nuevos nodos.
		 */
		void reallocate_map(size_type num_of_nodes_to_add, bool adding_at_front)
		{
			// Guardar la cantidad actual/vieja de nodos del arreglo de nodos.
			size_type old_num_of_nodes = this->finish.node - this->start.node + 1;
			// Calcular la cantidad nueva de nodos que va a tener el arreglo de nodos.
			size_type new_num_of_nodes = old_num_of_nodes + num_of_nodes_to_add;

			// Puntero a nodo que va a apuntar al nuevo inicio del contenedor.
			map_pointer new_start_node;

			// Si el tamaño del arreglo de punteros es mayor al doble 
			// de la nueva cantidad de nodos, hay suficiente espacio 
			// en el otro extremo del contenedor. Por lo tanto, solo hace falta
			// desplazar los nodos.
			if ( this->map_size > (new_num_of_nodes * 2) )
			{
				// Calcular cuántos espacios deben desplazarse los nodos
				// según el tamaño del mapa y la nueva cantidad de nodos.
				new_start_node = map + (this->map_size - new_num_of_nodes) / 2;
				// Verificar hacia dónde deben desplazarse los nodos.
				if (adding_at_front)
					new_start_node += num_of_nodes_to_add;

				// Si el nuevo inicio se encuentra a la izquierda del inicio actual
				if (new_start_node < this->start.node)
					// Desplazar nodos hacia la izquierda.
					mySTL::copy(this->start.node, this->finish.node + 1, new_start_node);
				else // De lo contrario
					// Desplazar nodos hacia la derecha.
					mySTL::copy_backward(this->start.node, this->finish.node + 1,
						new_start_node + old_num_of_nodes);
			}
			// De lo contrario, no hay suficiente espacio en los extremos
			// y el mapa debe ser reasignado.
			else
			{
				// Calcular el nuevo tamaño del arreglo de nodos.
				size_type new_map_size = this->map_size
					+ mySTL::max(this->map_size, num_of_nodes_to_add) + 2;

				// Crear el nuevo mapa con el nuevo tamaño calculado.
				map_pointer new_map = new pointer[new_map_size];

				// Calcular el nuevo inicio en el nuevo mapa según el nuevo tamaño
				// y la nueva cantidad de nodos.
				new_start_node = new_map + (new_map_size - new_num_of_nodes) / 2;
				if (adding_at_front)
					new_start_node += num_of_nodes_to_add;

				// Copiar los elementos al inicio del nuevo mapa.
				mySTL::copy(this->start.node, this->finish.node + 1, new_start_node);

				delete [] this->map; // Destruir mapa actual.

				// Asignar a los atributos el nuevo mapa y su nuevo tamaño.
				this->map = new_map;
				this->map_size = new_map_size;
			}

			// Asignar iteradores.
			this->start.set_node(new_start_node);
			this->finish.set_node(new_start_node + old_num_of_nodes - 1);
		}

		/**
		 * Verifica si hay suficiente espacio al final del mapa para poder
		 * agregar @a num_of_nodes_to_add nodos. Si no hay suficiente espacio,
		 * se reasigna el almacenamiento.
		 * 
		 * @param num_of_nodes_to_add	El número de nodos que se agregan a map.
		 */
		void reserve_map_at_back(size_type num_of_nodes_to_add = 1)
		{
			// Si la cantidad de nodos que se quiere agregar
			// es mayor que el espacio libre al final del mapa
			if ( num_of_nodes_to_add + 1
				> (this->map_size - (this->finish.node - this->map)) )
				// Reasignar el almacenamiento del arreglo de nodos.
				// false indica que los nodos se quieren agregar al final.
				this->reallocate_map(num_of_nodes_to_add, false);
		}

		/**
		* Verifica si hay suficiente espacio al inicio del mapa para poder 
		* agregar @a num_of_nodes_to_add nodos. Si no hay suficiente espacio, 
		* se reasigna el almacenamiento.
		* 
		* @param num_of_nodes_to_add	El número de nodos que se agregan a map.
		*/
		void reserve_map_at_front(size_type num_of_nodes_to_add = 1)
		{
			// Si la cantidad de nodos que se quiere agregar
			// es mayor que el espacio libre al inicio del mapa
			if ( num_of_nodes_to_add > (this->start.node - this->map) )
				// Reasignar el almacenamiento del arreglo de nodos.
				// true indica que los nodos se quieren agregar al inicio.
				this->reallocate_map(num_of_nodes_to_add, true);
		}

		/**
		 * Crea nodos al final del contenedor para poder insertar 
		 * @a new_elements_count elementos.
		 * 
		 * @param new_elements_count	El número de elementos que se agregan
		 * a los nuevos nodos.
		 */
		void create_new_nodes_at_back(size_type new_elements_count)
		{
			// Calcular la cantidad de nodos que se deben crear,
			// según la cantidad de elementos por agregar y el 
			// tamaño de los fragmentos de memoria.
			size_type new_nodes_count = (new_elements_count + buffer_size() - 1)
				/ buffer_size();

			// Reservar el espacio necesario.
			this->reserve_map_at_back(new_nodes_count);

			// Crear los nuevos nodos después del último nodo del mapa.
			for (size_type current = 1; current <= new_nodes_count; ++current)
				// Inicializar el nuevo fragmento de memoria actual.
				*(this->finish.node + current) = new value_type[ buffer_size() ];
		}

		/**
		* Crea nodos al inicio del contenedor para poder insertar 
		* @a new_elements_count elementos.
		* 
		* @param new_elements_count	El número de elementos que se agregan
		* a los nuevos nodos.
		*/
		void create_new_nodes_at_front(size_type new_elements_count)
		{
			size_type new_nodes_count = (new_elements_count + buffer_size() - 1)
				/ buffer_size();

			this->reserve_map_at_back(new_nodes_count);

			// Crear los nuevos nodos antes del primer nodo del mapa.
			for (size_type current = 1; current <= new_nodes_count; ++current)
				// Inicializar el nuevo fragmento de memoria actual.
				*(this->start.node - current) = new value_type[ buffer_size() ];
		}

		/**
		 * Reserva espacio al final del contenedor para poder agregar 
		 * @a count elementos y retorna un iterador al nuevo final 
		 * del contenedor.
		 * 
		 * @param count	El número de elementos que se quieren agregar.
		 * @return Un iterador al nuevo final de la secuencia.
		 */
		iterator reserve_elements_at_back(size_type count)
		{
			// Calcular los espacios disponibles para los nuevos
			// elementos en el último fragmento de memoria.
			size_type available_spaces = (this->finish.last - this->finish.current) - 1;

			// Si el número de elementos por agregar es mayor
			// al número de espacios disponibles en el fragmento
			if (count > available_spaces)
				// Crear nuevos nodos para poder agregar los elementos restantes.
				this->create_new_nodes_at_back(count - available_spaces);

			// Retornar el nuevo final del contenedor.
			return (this->finish + difference_type(count));
		}

		/**
		 * Reserva espacio al inicio del contenedor para poder agregar 
		 * @a count elementos y retorna un iterador al nuevo inicio 
		 * del contenedor.
		 * 
		 * @param count	El número de elementos que se quieren agregar.
		 * @return Un iterador al nuevo inicio de la secuencia.
		 */
		iterator reserve_elements_at_front(size_type count)
		{
			// Calcular los espacios disponibles para los nuevos
			// elementos en el primer fragmento de memoria.
			size_type available_spaces = this->start.current - this->start.first;

			// Si el número de elementos por agregar es mayor
			// al número de espacios disponibles en el fragmento
			if (count > available_spaces)
				// Crear nuevos nodos para poder agregar los elementos restantes.
				this->create_new_nodes_at_front(count - available_spaces);

			// Retornar el nuevo inicio del contenedor.
			return (this->start + difference_type(count));
		}

	};

}

#endif /* MY_DEQUE_H */
