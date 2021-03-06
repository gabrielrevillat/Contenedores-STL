#ifndef MY_DEQUE_H
#define MY_DEQUE_H

#include "../algorithm/my_algorithm.h"

#include <cmath>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace mySTL
{

#ifndef MY_DEQUE_CHUNK_SIZE
#define MY_DEQUE_CHUNK_SIZE 512
#endif

    /**
     * @brief Controla el tama�o de los fragmentos de memoria.
     * 
     * @param size	El tama�o en bytes de un elemento.
     * @return el n�mero de elementos que puede contener cada nodo.
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
        // Primer par�metro de plantilla.
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

        pointer current;	// Puntero a un elemento espec�fico en el fragmento de memoria actual.
        pointer first;		// Puntero al primer elemento del fragmento de memoria actual.
        pointer last;		// Puntero al elemento siguiente al �ltimo del fragmento de memoria actual.
        map_pointer node;	// Puntero al nodo del contenedor que apunta al fragmento de memoria.

        /**
         * Retorna la cantidad de elementos que puede almacenar un fragmento de memoria.
         * 
         * @return El n�mero de elementos que almacena cada fragmentos de memoria.
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
         * Operador de disminuci�n prefijo.
         */
        self& operator--() noexcept
        {
            // Si current est� en el inicio del fragmento
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
         * Operador de disminuci�n postfijo.
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
         * Operador de suma-asignaci�n.
         */
        self& operator+=(difference_type count) noexcept
        {
            // Nueva posici�n a la que va a apuntar el iterador.
            const difference_type new_position = count + (current - first);

            // Si la nueva posici�n se encuentra dentro de los l�mites del fragmento
            if ( (new_position >= 0) && (new_position < difference_type( buffer_size() )) )
            {
                // Desplazar el puntero current a la nueva posici�n.
                current += count;
            }
            else // De lo contrario
            {
                // Crear variable para calcular el desplazamiento del puntero a nodo.
                difference_type node_offset = 0;
                // Si la nueva posici�n es mayor a 0
                if (new_position > 0)
                {
                    // El puntero a nodo se desplaza hacia la derecha.
                    // Calcular cu�ntos fragmentos hacia la derecha debe moverse.
                    node_offset = new_position / difference_type( buffer_size() );
                }
                else // De lo contrario
                {
                    // El puntero a nodo se desplaza hacia la izquierda.
                    // Calcular cu�ntos fragmentos hacia la izquierda debe moverse.
                    node_offset = -( (-new_position - 1) / difference_type( buffer_size() ) ) - 1;
                }

                // Desplazar node al fragmento correspondiente.
                set_node(node + node_offset);
                // current ahora debe apuntar a la posici�n espec�fica del fragmento.
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
         * Operador de resta-asignaci�n.
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
         * Retorna el elemento al que apunta la posici�n current + @a position.
         * 
         * @param position	La posici�n del elemento que se quiere retornar, relativa a current.
         */
        reference operator[](difference_type position) const noexcept
        {
            return *(*this + position);
        }

        // Asignaci�n de nodo.

        /**
         * Asigna los punteros del iterador de manera que apunten correctamente a @a new_node.
         * El puntero current debe ser asignado correctamente despu�s del llamado a este m�todo.
         * 
         * @param new_node	Puntero a un nodo del contenedor que apunta a un fragmento de memoria espec�fico.
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
            // Verificar cu�l de los dos apunta a un elemento en alguna posici�n anterior.
            ? (iterator1.current < iterator2.current)
            // De lo contrario, verificar cu�l de los dos apunta a un nodo anterior.
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
    // Revisar explicaci�n de la f�rmula en la documentaci�n externa.

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
     * @brief Contenedor secuencial con tama�o din�mico que puede
     * expandirse o contraerse en ambos finales.
     * 
     * @tparam ValueType El tipo de los elementos que contiene.
     */
    template <typename ValueType>
    class deque
    {
    // Tipos miembro p�blicos
    public:
        // Primer par�metro de plantilla.
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
        size_type map_size;	// El n�mero de punteros a los que apunta el mapa. Esta cantidad es de al menos 8.
        iterator start;		// Iterador que apunta al primer elemento del contenedor.
        iterator finish;	// Iterador que apunta al elemento siguiente al �ltimo del contenedor.

    // M�todos p�blicos
    public:
        // Constructores y destructor.

        /**
         * Constructor por defecto. 
         *
         * Construye un contendor vac�o, sin elementos.
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
         * @param count El tama�o inicial del contenedor.
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
         * @param count El tama�o inicial del contenedor.
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

            // Llenar el �ltimo fragmento hasta alcanzar la posici�n
            // del �ltimo elemento.
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
            // El n�mero de elementos es la distancia entre los iteradores.
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
            // Inicializar arreglo de nodos con el n�mero de elementos de other.
            this->create_map_and_nodes( other.size() );
            // Copiar los elementos de other al contenedor.
            mySTL::copy(other.begin(), other.end(), this->start);
        }

        /**
         * Constructor de movimiento. 
         * 
         * Construye un contenedor que adquiere los elementos de @a temp,
         * con sem�nticas de movimiento.
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
            // Dejar vac�o el contenedor temp.
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
            // El n�mero de elementos es el tama�o de init_list.
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

        // Operador de asignaci�n.

        /**
         * Asignaci�n por copia. 
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
         * Asignaci�n por movimiento. 
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

        /**
         * Asignaci�n por lista de inicializaci�n. 
         * 
         * Reemplaza el contenido del contenedor por una copia de los elementos de @a init_list.
         * 
         * @param init_list Objeto initializer_list para reemplazar el contenido del contenendor.
         * @return *this.
         */
        deque& operator=(std::initializer_list<value_type> init_list)
        {
            // Construir un deque con la lista de inicializaci�n recibida.
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
         * @param count El nuevo tama�o del contenedor.
         * @param value Valor para asignar los elementos del contenedor.
         */
        void assign(size_type count, const value_type& value)
        {
            // Crear un deque con los par�metros recibidos.
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

        /// Retorna la cantidad m�xima de elementos que puede contener el contenedor.
        inline size_type max_size() const noexcept
        {
            // F�rmula basada en el resultado obtenido del max_size del std en mi computadora.
            return ( ( pow(2, 63) / sizeof(value_type) ) - 1 );
        }

        /**
         * Cambia el tama�o del contenedor para @a count elementos.
         * 
         * @param count El nuevo tama�o del contenedor.
         */
        void resize(size_type count)
        {
            this->resize(count, value_type());
        }

        /**
         * Cambia el tama�o del contenedor para @a count elementos. 
         * Los nuevos elementos se inicializan con copias de @a value. 
         * 
         * @param count El nuevo tama�o del contenedor.
         * @param value El valor para inicializar los elementos a�adidos.
         */
        void resize(size_type count, value_type value)
        {
            // Si count es menor al tama�o actual del contenedor
            if (count < size())
                // Eliminar los elementos sobrantes.
                this->erase(begin() + count, end());
            // De lo contrario
            else
                // Insertar el valor value al final del contenedor
                // hasta alcanzar el tama�o count.
                this->insert(end(), count - size(), value);
        }

        /// Verifica si el contenedor est� vac�o.
        inline bool empty() const noexcept { return (size() == 0); }

        /**
         * Solicita reducir la capacidad del contenedor para encajar
         * con el n�mero de elementos.
         */
        void shrink_to_fit()
        {
            // Calcular el espacio libre para nuevos elementos
            // al inicio del contenedor.
            difference_type front_capacity = (this->start.current
                                            - this->start.first);
            // Calcular el espacio libre para nuevos elementos
            // al final del contenedor.
            difference_type back_capacity = (this->finish.last
                                            - this->finish.current);

            // Si hay mucho m�s espacio libre de lo necesario
            if ( (front_capacity > 0)
                && (front_capacity + back_capacity >= buffer_size()) )
            {
                // Al crear un nuevo contenedor con el mismo contenido, 
                // se crea con la capacidad adicional necesaria.
                auto new_deque = deque(this->begin(), this->end());
                // Intercambiar con el contenedor actual.
                this->swap(new_deque);
            }
        }

        // Acceso a elementos.

        /**
         * Retorna una referencia al elemento en la posici�n @a index del contenedor.
         * 
         * @param index Posici�n de un elemento en el contenedor.
         * @return El elemento en la posici�n especificada.
         */
        inline reference operator[](size_type index) { return *(begin() + index); }

        /**
        * Retorna una referencia al elemento en la posici�n @a index del contenedor.
        * 
        * @param index Posici�n de un elemento en el contenedor.
        * @return El elemento en la posici�n especificada.
        */
        inline const_reference operator[](size_type index) const { return *(begin() + index); }

        /**
         * Retorna una referencia al elemento en la posici�n @a index del contenedor. 
         * 
         * Este m�todo verifica si @a index est� dentro del l�mite de elementos
         * v�lidos en el contenedor. Si no lo est�, lanza una excepci�n.
         * 
         * @param index Posici�n de un elemento en el contenedor.
         * @throw std::out_of_range
         * @return El elemento en la posici�n especificada.
         */
        inline reference at(size_type index)
        {
            if (index >= size())
                throw std::out_of_range("mySTL::deque::at");

            return *(begin() + index);
        }

        /**
         * Retorna una referencia al elemento en la posici�n @a index del contenedor. 
         * 
         * Este m�todo verifica si @a index est� dentro del l�mite de elementos
         * v�lidos en el contenedor. Si no lo est�, lanza una excepci�n.
         * 
         * @param index Posici�n de un elemento en el contenedor.
         * @throw std::out_of_range
         * @return El elemento en la posici�n especificada.
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

        /// Retorna una referencia al �ltimo elemento en el contenedor.
        inline reference back() { return *(end() - 1); }
        /// Retorna una referencia al �ltimo elemento en el contenedor.
        inline const_reference back() const { return *(end() - 1); }

        // Modificadores.

        /**
         * Agrega un nuevo elemento al final del contenedor y aumenta su tama�o.
         * 
         * @param value El valor del elemento por agregar al contenedor.
         */
        void push_back(const value_type& value)
        {
            this->emplace_back(value);
        }

        /**
         * Agrega un nuevo elemento al final del contenedor y aumenta su tama�o.
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
            // Si queda espacio en el �ltimo fragmento de memoria
            // (Para al menos dos elementos)
            if (this->finish.current != this->finish.last - 1)
            {
                // Construir el nuevo elemento en la posici�n siguiente a la
                // del �ltimo elemento. 
                *this->finish.current = value_type(std::forward<Args>(args)...);
                // Incrementar el iterador finish para que siga apuntando a
                // la posici�n siguiente a la del �ltimo elemento.
                ++this->finish.current;
            }
            else // De lo contrario
            {
                // Reservar espacio al final del mapa
                this->reserve_map_at_back();
                // Crear un nuevo nodo despu�s del �ltimo
                // e inicializarlo con un nuevo fragmento/buffer.
                *(this->finish.node + 1) = new value_type[ buffer_size() ];
                // Construir el nuevo elemento en la �ltima posici�n
                // del �ltimo fragmento viejo.
                *this->finish.current = value_type(std::forward<Args>(args)...);
                // Reajustar el iterador finish para que apunte al nuevo �ltimo nodo.
                this->finish.set_node(this->finish.node + 1);
                this->finish.current = this->finish.first;
            }
        }

        /**
         * Agrega un nuevo elemento al inicio del contenedor y aumenta su tama�o.
         * 
         * @param value El valor del elemento por agregar al contenedor.
         */
        void push_front(const value_type& value)
        {
            this->emplace_front(value);
        }

        /**
         * Agrega un nuevo elemento al inicio del contenedor y aumenta su tama�o.
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
                // Construir el nuevo elemento en la posici�n anterior a la
                // del primer elemento.
                *(this->start.current - 1) = value_type(std::forward<Args>(args)...);
                // Disminuir el iterador start para que siga apuntando a
                // la posici�n del primer elemento.
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
                // Construir el nuevo elemento en la �ltima posici�n
                // del nuevo primer fragmento.
                *this->start.current = value_type(std::forward<Args>(args)...);
            }
        }

        /**
         * Elimina el �ltimo elemento del contenedor y reduce su tama�o.
         */
        void pop_back()
        {
            // Si el �litmo fragmento de memoria contiene al menos un elemento
            if (this->finish.current != this->finish.first)
            {
                // Disminuir el iterador finish para que apunte
                // al elemento que se quiere eliminar.
                // (Ya que finish siempre apunta al elemento 
                // *siguiente al �ltimo* del contenedor).
                --this->finish.current;
                // Destruir el elemento.
                delete this->finish.current;
            }
            else // De lo contrario
            {
                // Destruir el �ltimo fragmento de memoria.
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
         * Elimina el primer elemento del contenedor y reduce su tama�o.
         */
        void pop_front()
        {
            // Si el primer fragmento de memoria contiene al menos dos elementos
            if (this->start.current != this->start.last - 1)
            {
                // Destruir el primer elemento.
                delete this->start.current;
                // Incrementar el iterador start para que apunte
                // al elemento siguiente al eliminado.
                ++this->start.current;
            }
            else // De lo contrario
            {
                // Destruir el primer y �nico elemento
                // del primer fragmento de memoria.
                delete this->start.current;
                // Destruir el fragmento de memoria.
                delete [] this->start.first;
                // Reajustar el iterador first para que apunte al
                // nodo siguiente.
                this->start.set_node(this->start.node + 1);
                this->start.current = this->start.first;
            }
        }

        /**
         * Inserta el valor @a value en la posici�n @a position. 
         * 
         * @param position   Posici�n del contenedor donde se inserta el nuevo elemento.
         * @param value      El valor del elemento por insertar.
         * @return iterador que apunta al elemento insertado.
         */
        iterator insert(const_iterator position, const value_type& value)
        {
            return this->emplace(position, value);
        }

        /**
         * Inserta el valor @a value en la posici�n @a position. 
         * 
         * @param position   Posici�n del contenedor donde se inserta el nuevo elemento.
         * @param value      El valor del elemento por insertar.
         * @return iterador que apunta al elemento insertado.
         */
        iterator insert(const_iterator position, value_type&& value)
        {
            return this->emplace(position, std::forward<value_type>(value));
        }

        /**
         * Inserta un nuevo elemento en la posici�n @a position. Este elemento es 
         * construido usando @a args como argumento para su construcci�n.
         * 
         * @param position   La posici�n del contenedor donde se inserta el nuevo elemento.
         * @param args       Argumentos para construir el nuevo elemento.
         * @return iterador que apunta al nuevo elemento.
         */
        template <typename... Args>
        iterator emplace(const_iterator position, Args&&... args)
        {
            // Iterador que apunta a la posici�n de inserci�n. (No const)
            iterator result;

            // Si se quiere insertar al inicio
            if (position.current == this->start.current)
            {
                // Llamar al m�todo correspondiente.
                this->emplace_front(std::forward<Args>(args)...);
                result = begin();
            }
            // De lo contrario, si se quiere insertar al final
            else if (position.current == this->finish.current)
            {
                this->emplace_back(std::forward<Args>(args)...);
                result = end() - 1;
            }
            // De lo contrario
            else
            {
                // El �ndice donde se inserta el nuevo elemento es la distancia entre
                // begin() (el iterador que apunta al inicio) y
                // position (el iterador que apunta a la posici�n).
                difference_type index = position - begin();

                // Si la posici�n de inserci�n es m�s cercana al inicio del contenedor
                if (index < (size() / 2))
                {
                    // Hacer una copia del primer elemento e insertarlo
                    // al inicio del contenedor.
                    this->push_front(front());

                    // Ahora el valor del primer elemento se encuentra tanto en la 
                    // primera como en la segunda posici�n del contenedor.

                    // Copiar los elementos que est�n entre la tercera posici�n
                    // y la posici�n de inserci�n, a su posici�n anterior.
                    result = begin() + index;
                    mySTL::copy(begin() + 2, result + 1, begin() + 1);
                }
                else // De lo contrario
                {
                    // Hacer una copia del �ltimo elemento e insertarlo
                    // al final del contenedor.
                    this->push_back(back());

                    // Ahora el valor del �ltimo elemento se encuentra 
                    // en las �ltimas dos posiciones del contenedor.

                    // Copiar los elementos que est�n entre la posici�n de 
                    // inserci�n y la �ltima posici�n - 2, a su posici�n siguiente.
                    result = begin() + index;
                    mySTL::copy_backward(result, end() - 2, end() - 1);
                }
                // Construir el nuevo elemento con los argumentos recibidos,
                // en la posici�n de inserci�n.
                *result = value_type(std::forward<Args>(args)...);
            }

            return result;
        }

        /**
         * Inserta @a count copias de @a value en la posici�n @a position. 
         * 
         * @param position   La posici�n del contenedor donde se inserta el primero de los nuevos elementos.
         * @param count      El n�mero de elementos a insertar.
         * @param value      El valor del elemento por insertar.
         * @return iterador que apunta al primero de los nuevos elementos insertados.
         */
        iterator insert(const_iterator position, size_type count, const value_type& value)
        {
            iterator result;

            if (count == 0)
            {
                result = position;
            }
            else
            {
                // Si los elementos se quieren insertar al inicio
                if (position.current == this->start.current)
                {
                    // Reservar espacio para count elementos al inicio del contenedor
                    // y guardar el nuevo inicio de la secuencia.
                    iterator new_start = this->reserve_elements_at_front(count);
                    // Llenar con el valor value los espacios entre el nuevo inicio
                    // y el inicio viejo.
                    mySTL::fill(new_start, this->start, value);
                    // Actualizar el inicio de la secuencia.
                    result = this->start = new_start;
                }
                // De lo contrario, si se quieren insertar al final
                else if (position.current == this->finish.current)
                {
                    // Reservar espacio para count elementos al final del contenedor
                    // y guardar el nuevo final de la secuencia.
                    iterator new_finish = this->reserve_elements_at_back(count);
                    // Llenar con el valor value los espacios entre el final viejo
                    // y el final nuevo.
                    mySTL::fill(this->finish, new_finish, value);
                    result = this->finish;
                    // Actualizar el final de la secuencia.
                    this->finish = new_finish;
                }
                // De lo contrario
                else
                {
                    // N�mero de elementos que se encuentran entre
                    // el inicio del contenedor y la posici�n de inserci�n.
                    difference_type elements_before_pos = position - begin();

                    // Si la posici�n de inserci�n es m�s cercana al inicio del contenedor
                    if (elements_before_pos < (size() / 2))
                    {
                        // Reservar espacio para count elementos al inicio del contenedor
                        // y guardar el nuevo inicio de la secuencia.
                        iterator new_start = this->reserve_elements_at_front(count);
                        // Guardar el inicio viejo de la secuencia.
                        iterator old_start = this->start;
                        result = this->start + elements_before_pos;

                        // Si entre el inicio y la posici�n de inserci�n hay espacio 
                        // suficiente para count elementos
                        if (elements_before_pos >= count)
                        {
                            // Copiar los primeros count elementos del contenedor al
                            // nuevo inicio.
                            iterator last_of_first_elements = this->start + count;
                            mySTL::copy(this->start, last_of_first_elements, new_start);
                            // Actualizar el inicio de la secuencia.
                            this->start = new_start;
                            // Copiar los elementos restantes al viejo inicio.
                            mySTL::copy(last_of_first_elements, result, old_start);
                            // Llenar con value los espacios restantes correspondientes.
                            mySTL::fill(result - count, result, value);
                        }
                        else
                        {
                            // Copiar los elementos entre el viejo inicio y la posici�n
                            // de inserci�n, al nuevo inicio.
                            iterator end_of_range = mySTL::copy(this->start, result, new_start);
                            // Llenar con value los espacios restantes hasta el viejo inicio.
                            mySTL::fill(end_of_range, this->start, value);
                            // Actualizar el inicio de la secuencia.
                            this->start = new_start;
                            // Llenar con value los espacios restantes desde el viejo inicio
                            // hasta la posici�n de inserci�n.
                            mySTL::fill(old_start, result, value);
                        }
                        // Actualizar el iterador a posici�n de inserci�n.
                        result = this->start + elements_before_pos;
                    }
                    else // De lo contrario
                    {
                        // Reservar espacio para count elementos al final del contenedor
                        // y guardar el nuevo final de la secuencia.
                        iterator new_finish = this->reserve_elements_at_back(count);
                        // Guardar el final viejo de la secuencia.
                        iterator old_finish = this->finish;
                        // N�mero de elementos entre la posici�n de inserci�n
                        // y el final del contenedor.
                        difference_type elements_after_pos = size() - elements_before_pos;
                        result = this->finish - elements_after_pos;

                        // Si entre la posici�n de inserci�n y el final hay espacio 
                        // suficiente para count elementos
                        if (elements_after_pos > count)
                        {
                            // Copiar los ultimos count elementos del contenedor a los
                            // nuevos espacios despu�s del viejo final.
                            iterator first_of_last_elements = this->finish - count;
                            mySTL::copy(first_of_last_elements, this->finish, this->finish);
                            // Actualizar el final de la secuencia.
                            this->finish = new_finish;
                            // Copiar los elementos restantes a las posiciones anteriores
                            // al viejo final.
                            mySTL::copy_backward(result, first_of_last_elements, old_finish);
                            // Llenar con value los espacios correspondientes desde 
                            // la posici�n de inserci�n.
                            mySTL::fill(result, result + count, value);
                        }
                        else
                        {
                            // Llenar con value los espacios correspondientes desde
                            // el viejo final.
                            mySTL::fill(this->finish, result + count, value);
                            // Copiar los elementos entre la posici�n de inserci�n
                            // y el viejo final, a la posici�n despu�s del �ltimo
                            // nuevo elemento insertado.
                            mySTL::copy(result, this->finish, result + count);
                            // Actualizar el final de la secuencia.
                            this->finish = new_finish;
                            // Llenar con value los espacios entre la posici�n
                            // de inserci�n y el viejo final.
                            mySTL::fill(result, old_finish, value);
                        }
                    }
                }
            }

            return result;
        }

        /**
         * Inserta elementos en el rango [@a first, @a last) en la posici�n @a position. 
         * 
         * @param position      La posici�n del contenedor donde se inserta el primero de los nuevos elementos.
         * @param first, last   El rango de copia de los elementos a insertar.
         * @return iterador que apunta al primero de los nuevos elementos insertados.
         */
        template <typename InputIterator,
            typename = typename std::enable_if_t<std::is_base_of_v<std::input_iterator_tag,
                typename std::iterator_traits<InputIterator>::iterator_category>>>
        iterator insert(const_iterator position, InputIterator first, InputIterator last)
        {
            iterator result;

            if (first == last)
            {
                result = position;
            }
            else
            {
                size_type count = std::distance(first, last);

                if (position.current == this->start.current)
                {
                    // Reservar espacio para count elementos al inicio del contenedor
                    // y guardar el nuevo inicio de la secuencia.
                    iterator new_start = this->reserve_elements_at_front(count);
                    // Copiar los elementos del rango [first, last) al contenedor
                    // desde su nuevo inicio.
                    mySTL::copy(first, last, new_start);
                    // Actualizar el inicio de la secuencia.
                    result = this->start = new_start;
                }
                else if (position.current == this->finish.current)
                {
                    // Reservar espacio para count elementos al final del contenedor
                    // y guardar el nuevo final de la secuencia.
                    iterator new_finish = this->reserve_elements_at_back(count);
                    // Copiar los elementos del rango [first, last) al contenedor
                    // despu�s del �ltimo elemento.
                    mySTL::copy(first, last, this->finish);
                    result = this->finish;
                    // Actualizar el final de la secuencia.
                    this->finish = new_finish;
                }
                else
                {
                    difference_type elements_before_pos = position - begin();

                    if (elements_before_pos < (size() / 2))
                    {
                        // Reservar espacio para count elementos al inicio del contenedor
                        // y guardar el nuevo inicio de la secuencia.
                        iterator new_start = this->reserve_elements_at_front(count);
                        // Guardar el inicio viejo de la secuencia.
                        iterator old_start = this->start;
                        result = this->start + elements_before_pos;

                        // Si entre el inicio y la posici�n de inserci�n hay espacio 
                        // suficiente para count elementos
                        if (elements_before_pos >= count)
                        {
                            // Copiar los primeros count elementos del contenedor al
                            // nuevo inicio.
                            iterator last_of_first_elements = this->start + count;
                            mySTL::copy(this->start, last_of_first_elements, new_start);
                            // Actualizar el inicio de la secuencia.
                            this->start = new_start;
                            // Copiar los elementos restantes al viejo inicio.
                            mySTL::copy(last_of_first_elements, result, old_start);
                            // Copiar los elementos del rango [first, last) al contenedor
                            // desde su posici�n correspondiente.
                            mySTL::copy(first, last, result - count);
                        }
                        else
                        {
                            // Copiar los elementos entre el viejo inicio y la posici�n
                            // de inserci�n, al nuevo inicio.
                            iterator end_of_range = mySTL::copy(this->start, result, new_start);
                            // Copiar los elementos del rango [first, last) al contenedor
                            // desde su posici�n correspondiente.
                            InputIterator range_mid = first + (count - elements_before_pos);
                            mySTL::copy(first, range_mid, end_of_range);
                            // Actualizar el inicio de la secuencia.
                            this->start = new_start;
                            mySTL::copy(range_mid, last, old_start);
                        }
                        // Actualizar el iterador a posici�n de inserci�n.
                        result = this->start + elements_before_pos;
                    }
                    else
                    {
                        // Reservar espacio para count elementos al final del contenedor
                        // y guardar el nuevo final de la secuencia.
                        iterator new_finish = this->reserve_elements_at_back(count);
                        // Guardar el final viejo de la secuencia.
                        iterator old_finish = this->finish;
                        // N�mero de elementos entre la posici�n de inserci�n
                        // y el final del contenedor.
                        difference_type elements_after_pos = size() - elements_before_pos;
                        result = this->finish - elements_after_pos;

                        // Si entre la posici�n de inserci�n y el final hay espacio 
                        // suficiente para count elementos
                        if (elements_after_pos > count)
                        {
                            // Copiar los ultimos count elementos del contenedor a los
                            // nuevos espacios despu�s del viejo final.
                            iterator first_of_last_elements = this->finish - count;
                            mySTL::copy(first_of_last_elements, this->finish, this->finish);
                            // Actualizar el final de la secuencia.
                            this->finish = new_finish;
                            // Copiar los elementos restantes a las posiciones anteriores
                            // al viejo final.
                            mySTL::copy_backward(result, first_of_last_elements, old_finish);
                            // Copiar los elementos del rango [first, last) al contenedor
                            // desde su posici�n correspondiente.
                            mySTL::copy(first, last, result);
                        }
                        else
                        {
                            // Copiar los elementos de [first, last) que no caben 
                            // entre la posici�n de  inserci�n y el final,
                            // al final del contenedor.
                            InputIterator range_mid = first + elements_after_pos;
                            iterator end_of_range = mySTL::copy(range_mid, last, this->finish);
                            // Copiar los elementos que est�n entre la posici�n de
                            // inserci�n y el final, a la posici�n despu�s de los nuevos
                            // elementos insertados.
                            mySTL::copy(result, this->finish, end_of_range);
                            // Actualizar el final de la secuencia.
                            this->finish = new_finish;
                            // Copiar los elementos restantes de [first, last)
                            // al contenedor desde la posici�n de inserci�n.
                            mySTL::copy(first, range_mid, result);
                        }
                    }
                }
            }

            return result;
        }

        /**
         * Inserta cada uno de los elementos de @a init_list en la posici�n @a position. 
         * 
         * @param position  La posici�n del contenedor donde se inserta el primero de los nuevos elementos.
         * @param init_list Objeto initializer_list desde donde se insertan los nuevos elementos.
         * @return iterador que apunta al primero de los nuevos elementos insertados.
         */
        iterator insert(const_iterator position, std::initializer_list<value_type> init_list)
        {
            return this->insert(position, init_list.begin(), init_list.end());
        }

        /**
         * Elimina el elemento en la posici�n @a position del contenedor. 
         * 
         * @param position  Iterador que apunta al elemento por eliminar.
         * @return iterador que apunta a la nueva ubicaci�n del elemento siguiente al eliminado.
         */
        iterator erase(const_iterator position)
        {
            // El �ndice donde se elimina el elemento es la distancia entre
            // begin() (el iterador que apunta al inicio) y
            // position (el iterador que apunta a la posici�n).
            difference_type index = position - begin();
            // Iterador que apunta a la posici�n de eliminado. (No const)
            iterator result = begin() + index;

            // Si la posici�n de eliminado se encuentra
            // en la primera mitad del contenedor
            if (index < (size() / 2))
            {
                // Copiar todos los elementos desde el primero hasta el
                // elemento en la posici�n de eliminado, a su posici�n siguiente.
                mySTL::copy_backward(begin(), result, result + 1);
                // Eliminar el primer elemento.
                this->pop_front();
            }
            else
            {
                // Copiar todos los elementos desde el elemento en la
                // posici�n de eliminado hasta el �ltimo, a su posici�n anterior.
                mySTL::copy_backward(result + 1, end(), result);
                // Eliminar el �ltimo elemento.
                this->pop_back();
            }

            return result;
        }

        /**
         * Elimina los elementos en el rango [@ first, @ last) del contenedor. 
         * 
         * @param first, last   Rango de elementos a eliminar.
         * @return iterador que apunta a la nueva ubicaci�n del elemento siguiente al �ltimo eliminado.
         */
        iterator erase(const_iterator first, const_iterator last)
        {
            iterator result;

            // Si el rango [first, last) coincide con el
            // rango del contenedor completo
            if (this->start == first && this->finish == last)
            {
                // Vaciar el contenedor.
                this->clear();
                result = this->finish;
            }
            else
            {
                // El indice donde se elimina el primer elemento es la distancia entre
                // begin() (el iterador que apunta al inicio) y first
                // (el iterador que apunta a la posici�n del primer elemento a eliminar).
                difference_type first_index = first - begin();
                // El n�mero de elementos por eliminar 
                // es la distancia entre los iteradores.
                difference_type count = std::distance(first, last);

                // Si la posici�n del primer elemento a eliminar 
                // es m�s cercana al inicio del contenedor
                if (first_index < ((size() - count) / 2))
                {
                    // Copiar los elementos que se encuentran entre
                    // el inicio del contenedor y first, a las posiciones
                    // anteriores al final del rango de eliminado.
                    mySTL::copy_backward(this->start, first, last);
                    // Desplazar el inicio de la secuencia
                    // seg�n el n�mero de elementos por eliminar.
                    iterator new_start = this->start + count;
                    
                    // Destruir los elementos entre el viejo inicio
                    // y el nuevo inicio.
                    /* for (pointer element = this->start.current;
                         element < new_start.current; ++element)
                        delete element; */

                    // Si hab�a m�s de un nodo entre el viejo inicio
                    // y el nuevo inicio, destruirlo.
                    for (map_pointer current = this->start.node;
                         current < new_start.node; ++current)
                        delete [] *current;

                    // Actualizar el inicio de la secuencia.
                    this->start = new_start;
                }
                else
                {
                    // Copiar los elementos que se encuentran entre
                    // last y el final del contenedor, a las posiciones
                    // a partir del inicio del rango de eliminado.
                    mySTL::copy(last, this->finish, first);
                    // Desplazar el final de la secuencia
                    // seg�n el n�mero de elementos por eliminar.
                    iterator new_finish = this->finish - count;

                    // Destruir los elementos entre el nuevo final
                    // y el viejo final.
                    /* for (pointer element = new_finish.current;
                         element < this->finish.current; ++element)
                        delete element; */

                    for (map_pointer current = new_finish.node + 1;
                         current <= this->finish.node; ++current)
                        delete [] *current;

                    // Actualizar el final de la secuencia.
                    this->finish = new_finish;
                }

                result = begin() + first_index;
            }

            return result;
        }

        /**
         * Intercambia el contenido de este objeto por el contenido de @a other.
         * 
         * @param other Otro objeto deque del mismo tipo, para intercambiar sus elementos.
         */
        void swap(deque& other)
        {
            using std::swap; // Funci�n swap de la biblioteca est�ndar.
            swap(this->map, other.map);
            swap(this->map_size, other.map_size);
            swap(this->start, other.start);
            swap(this->finish, other.finish);
        }

        /**
         * Elimina todos los elementos del contenedor.
         */
        void clear() noexcept
        {
            // Destruir cada fragmento de memoria excepto
            // los apuntados por los nodos de los extremos.
            for (map_pointer current_node = this->start.node + 1;
                 current_node < this->finish.node; ++current_node)
            {
                // Destruir todos los elementos del fragmento
                // de memoria apuntado por el nodo actual.
                for (pointer element = *current_node;
                     element < *current_node + buffer_size(); ++element)
                    delete element;

                // Destruir el fragmento.
                delete [] *current_node;
            }
                

            // Si hay m�s de un nodo en el mapa
            if (this->start.node != this->finish.node)
            {
                // Destruir todos los elementos del primer
                // fragmento de memoria.
                for (pointer element = this->start.current;
                     element < this->start.last; ++element)
                    delete element;

                // Destruir todos los elementos del �ltimo
                // fragmento de memoria.
                for (pointer element = this->finish.first;
                     element < this->finish.current; ++element)
                    delete element;

                // Destruir el �ltimo fragmento de memoria.
                delete [] this->finish.first;
            }
            else // De lo contrario
            {
                // Destruir los elementos del �nico fragmento de memoria.
                for (pointer element = this->start.current;
                     element < this->finish.current; ++element)
                    delete element;
            }

            // Actualizar iteradores.
            this->finish = this->start;
        }

    // M�todos privados
    private:

        /**
         * Retorna la cantidad de elementos que puede almacenar un fragmento de memoria.
         * 
         * @return El n�mero de elementos que almacena cada fragmentos de memoria.
         */
        static size_type buffer_size() noexcept
        {
            return my_deque_chunk_size( sizeof(value_type) );
        }

        /**
         * Inicializa el arreglo de punteros seg�n @a elements_count 
         * y asigna un fragmento a cada nodo.
         * 
         * @param elements_count	El n�mero de elementos que va a contener el contenedor.
         */
        void create_map_and_nodes(size_type elements_count)
        {
            // Calcular el n�mero de nodos necesarios para el contenedor. 
            // (N�mero de elementos entre el tama�o de los fragmentos de memoria).
            size_type nodes_count = elements_count / buffer_size() + 1;

            // Asignar el tama�o del arreglo de nodos.
            // Si el n�mero de nodos es menor a 8, asignar 8.
            // De lo contrario, asignar el n�mero de nodos m�s 2.
            this->map_size = mySTL::max(size_type(8), nodes_count + 2);
            // Inicializar el arreglo de punteros con el tama�o calculado.
            this->map = new pointer[map_size];

            // Asignar punteros a nodos de inicio y final de manera que en 
            // ambos extremos del contenedor quede al menos un nodo vac�o.
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
            // finish.current debe apuntar al elemento siguiente al �ltimo
            // del �ltimo fragmento.
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
                 current <= this->finish.node; ++current)
                delete [] *current;

            delete [] this->map; // Destruir arreglo de nodos.
        }

        /**
         * Realiza una reasignaci�n de los nodos del mapa o del mapa en s�,
         * seg�n el espacio sobrante en los extremos y seg�n por cu�l extremo
         * se est�n agregando los nuevos elementos.
         * 
         * @param num_of_nodes_to_add	El n�mero de nodos que se agregan a map.
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

            // Si el tama�o del arreglo de punteros es mayor al doble 
            // de la nueva cantidad de nodos, hay suficiente espacio 
            // en el otro extremo del contenedor. Por lo tanto, solo hace falta
            // desplazar los nodos.
            if ( this->map_size > (new_num_of_nodes * 2) )
            {
                // Calcular cu�ntos espacios deben desplazarse los nodos
                // seg�n el tama�o del mapa y la nueva cantidad de nodos.
                new_start_node = map + (this->map_size - new_num_of_nodes) / 2;
                // Verificar hacia d�nde deben desplazarse los nodos.
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
                // Calcular el nuevo tama�o del arreglo de nodos.
                size_type new_map_size = this->map_size
                    + mySTL::max(this->map_size, num_of_nodes_to_add) + 2;

                // Crear el nuevo mapa con el nuevo tama�o calculado.
                map_pointer new_map = new pointer[new_map_size];

                // Calcular el nuevo inicio en el nuevo mapa seg�n el nuevo tama�o
                // y la nueva cantidad de nodos.
                new_start_node = new_map + (new_map_size - new_num_of_nodes) / 2;
                if (adding_at_front)
                    new_start_node += num_of_nodes_to_add;

                // Copiar los elementos al inicio del nuevo mapa.
                mySTL::copy(this->start.node, this->finish.node + 1, new_start_node);

                delete [] this->map; // Destruir mapa actual.

                // Asignar a los atributos el nuevo mapa y su nuevo tama�o.
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
         * @param num_of_nodes_to_add	El n�mero de nodos que se agregan a map.
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
        * @param num_of_nodes_to_add	El n�mero de nodos que se agregan a map.
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
         * @param new_elements_count	El n�mero de elementos que se agregan
         * a los nuevos nodos.
         */
        void create_new_nodes_at_back(size_type new_elements_count)
        {
            // Calcular la cantidad de nodos que se deben crear,
            // seg�n la cantidad de elementos por agregar y el 
            // tama�o de los fragmentos de memoria.
            size_type new_nodes_count = (new_elements_count + buffer_size() - 1)
                / buffer_size();

            // Reservar el espacio necesario.
            this->reserve_map_at_back(new_nodes_count);

            // Crear los nuevos nodos despu�s del �ltimo nodo del mapa.
            for (size_type current = 1; current <= new_nodes_count; ++current)
                // Inicializar el nuevo fragmento de memoria actual.
                *(this->finish.node + current) = new value_type[ buffer_size() ];
        }

        /**
        * Crea nodos al inicio del contenedor para poder insertar 
        * @a new_elements_count elementos.
        * 
        * @param new_elements_count	El n�mero de elementos que se agregan
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
         * @param count	El n�mero de elementos que se quieren agregar.
         * @return Un iterador al nuevo final de la secuencia.
         */
        iterator reserve_elements_at_back(size_type count)
        {
            // Calcular los espacios disponibles para los nuevos
            // elementos en el �ltimo fragmento de memoria.
            size_type available_spaces = (this->finish.last - this->finish.current) - 1;

            // Si el n�mero de elementos por agregar es mayor
            // al n�mero de espacios disponibles en el fragmento
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
         * @param count	El n�mero de elementos que se quieren agregar.
         * @return Un iterador al nuevo inicio de la secuencia.
         */
        iterator reserve_elements_at_front(size_type count)
        {
            // Calcular los espacios disponibles para los nuevos
            // elementos en el primer fragmento de memoria.
            size_type available_spaces = this->start.current - this->start.first;

            // Si el n�mero de elementos por agregar es mayor
            // al n�mero de espacios disponibles en el fragmento
            if (count > available_spaces)
                // Crear nuevos nodos para poder agregar los elementos restantes.
                this->create_new_nodes_at_front(count - available_spaces);

            // Retornar el nuevo inicio del contenedor.
            return (this->start - difference_type(count));
        }

    };

    // Funciones no miembros.

    // Operadores relacionales

    template <typename ValueType>
    inline bool operator==(const deque<ValueType>& lhs, const deque<ValueType>& rhs)
    {
        return ( ( lhs.size() == rhs.size() ) && ( mySTL::equal(lhs.begin(), lhs.end(), rhs.begin()) ) );
    }

    template <typename ValueType>
    inline bool operator!=(const deque<ValueType>& lhs, const deque<ValueType>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename ValueType>
    inline bool operator<(const deque<ValueType>& lhs, const deque<ValueType>& rhs)
    {
        return mySTL::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <typename ValueType>
    inline bool operator<=(const deque<ValueType>& lhs, const deque<ValueType>& rhs)
    {
        return !(rhs < lhs);
    }

    template <typename ValueType>
    inline bool operator>(const deque<ValueType>& lhs, const deque<ValueType>& rhs)
    {
        return (rhs < lhs);
    }

    template <typename ValueType>
    inline bool operator>=(const deque<ValueType>& lhs, const deque<ValueType>& rhs)
    {
        return !(lhs < rhs);
    }

    // Funci�n swap

    /**
    * Intercambia el contenido de @a deque_one por el de @a deque_two.
    * 
    * @param deque_one, deque_two   Contenedores deque del mismo tipo y tama�o.
    */ 
    template <typename ValueType>
    inline void swap(const deque<ValueType>& deque_one, const deque<ValueType>& deque_two)
    {
        deque_one.swap(deque_two);
    }

}

#endif /* MY_DEQUE_H */
