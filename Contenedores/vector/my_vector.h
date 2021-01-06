#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include "../algorithm/my_algorithm.h"

#include <iterator>
#include <cmath>
#include <utility>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <iostream>

namespace mySTL
{
    // Intento de que el programa no explote al querer usar InputIterator
    // template <typename InputIterator>
    // using require_input_iterator = std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>;

    /**
     * @brief Contenedor secuencial que representa un arreglo que puede cambiar su tama�o.
     *
     * @tparam ValueType El tipo de los elementos que contiene.
     */
    template <typename ValueType>
    class vector
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
        typedef value_type* iterator;
        // Iterador constante.
        typedef const value_type* const_iterator;
        // Iterador inverso.
        typedef std::reverse_iterator<iterator> reverse_iterator;
        // Iterador inverso constante.
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // Atributos privados    
    private:
        size_type count;            // El n�mero de elementos almacenados en el contenedor.
        size_type storage_capacity; // La capacidad de almacenamiento del contenedor.
        value_type* elements;       // Contenedor interno para guardar elementos de tipo ValueType.

    // M�todos p�blicos
    public:
        // Constructores y destructor.

        /**
         * Constructor por defecto. 
         *
         * Construye un contendor vac�o, sin elementos.
         */
        vector()
            : count(0) // El contenedor no tiene elementos.
            , storage_capacity(1) // Capacidad inicializada en 1.
            , elements( new value_type[this->storage_capacity] ) // Inicializar el contenedor con capacidad para un elemento.
        {
        }

        /**
         * Constructor de relleno. 
         *
         * Construye el contenedor con @a count elementos. No se hacen copias.
         *
         * @param count El tama�o inicial del contenedor.
         */
        explicit vector(size_type count)
            : count(count) // El contenedor tiene count elementos.
            , storage_capacity(this->count) // Inicializar la capacidad seg�n la cantidad de elementos.
            , elements(new value_type[this->storage_capacity]) // Inicializar el contenedor con capacidad para count elementos.
        {
        }

        /**
         * Constructor de relleno. 
         *
         * Construye el contenedor con @a count elementos. Cada elemento es una copia de @a value.
         *
         * @param count El tama�o inicial del contenedor.
         * @param value Valor para inicializar los elementos del contenedor.
         */
        vector(size_type count, const value_type& value)
            : count(count)
            , storage_capacity(this->count)
            , elements(new value_type[this->storage_capacity])
        {
            for ( size_type index = 0; index < count; ++index )
                this->construct_element(index, value);
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
        vector(InputIterator first, InputIterator last)
        {
            // El n�mero de elementos es la distancia entre los iteradores.
            this->count = std::distance(first, last);
            this->storage_capacity = this->count;
            this->elements = new value_type[this->storage_capacity];

            size_type index = 0; // Posici�n en el contenedor.
            // Recorrer a trav�s de los iteradores y asignar cada elemento a la posici�n correspondiente del contenedor.
            for (InputIterator current = first; current != last; ++current)
                this->construct_element(index++, *current);
        }

        /**
         * Constructor de copia. 
         *
         * Construye el contenedor con una copia de cada uno de los elementos de @a other,
         * en el mismo orden.
         *
         * @param other Otro objeto vector del mismo tipo, para inicializar el contenedor.
         */
        vector(const vector& other)
            : count(other.count) // Inicializar el n�mero de elementos seg�n el n�mero del otro contenedor.
            , storage_capacity(other.storage_capacity) // Lo mismo con la capacidad.
            , elements(new value_type[this->storage_capacity])
        {
            // Recorrer el vector other y asignar cada elemento a la posici�n correspondiente de este contenedor.
            for (size_type index = 0; index < this->count; ++index)
                this->construct_element(index, *(other.elements + index));
        }

        /**
         * Constructor de movimiento. 
         *
         * Construye un contenedor que adquiere los elementos de @a temp, con sem�nticas de movimiento.
         *
         * @param temp  Otro objeto vector del mismo tipo, para inicializar el contenedor.
         */
        vector(vector&& temp)
            : count(temp.count)
            , storage_capacity(temp.storage_capacity)
            , elements(temp.elements) // Inicializar el contenedor interno directamente con el contenedor del vector recibido.
        {
            // Dejar vac�o el contenedor temp.
            temp.count = temp.storage_capacity = 0;
            temp.elements = nullptr;
        }

        /**
         * Constructor con lista de inicializaci�n. 
         *
         * Construye el contenedor con una copia de cada uno de los elementos de @a init_list.
         *
         * @param init_list Objeto initializer_list para inicializar los elementos del contenedor.
         */
        vector(std::initializer_list<value_type> init_list)
            : count(init_list.size()) // Inicializar el n�mero de elementos seg�n el tama�o de init_list.
            , storage_capacity(this->count)
            , elements(new ValueType[this->storage_capacity])
        {
            size_type index = 0; // Posici�n en el contenedor.
            // Para cada elemento en init_list, asignarlo a la posici�n correspondiente del contenedor.
            for (const value_type& element : init_list)
                this->construct_element(index++, element);
        }

        /**
         * Destructor.
         *
         * Destruye el objeto contenedor.
         */
        ~vector()
        {
            this->destroy_all();
        }

        // Operador de asignaci�n.

        /**
         * Asignaci�n por copia. 
         *
         * Reemplaza el contenido del contenedor por una copia del contenido de @a other.
         *
         * @param other Otro objeto vector del mismo tipo, para reemplazar el contenido del contenedor.
         * @return *this.
         */
        vector& operator=(const vector& other)
        {
            // Construir una copia temporal del vector recibido.
            // De esta manera no se modifica el vector recibido al intercambiar los elementos.
            auto temp_copy = vector(other);
            this->swap(temp_copy); // Intercambiar elementos con esa copia creada.
            return *this;
        }

        /**
         * Asignaci�n por movimiento. 
         *
         * Reemplaza el contenido del contenedor al mover dentro de este el contenido de @a temp.
         *
         * @param temp  Otro objeto vector del mismo tipo, para reemplazar el contenido del contenedor.
         * @return *this.
         */
        vector& operator=(vector&& temp)
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
        vector& operator=(std::initializer_list<value_type> init_list)
        {
            // Construir un vector con la lista de inicializaci�n recibida.
            auto temp_il_vector = vector(init_list);
            this->swap(temp_il_vector); // Intercambiar elementos con el vector creado.
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
            // Crear un vector con los par�metros recibidos.
            auto temp_fill_vector = vector(count, value);
            this->swap(temp_fill_vector); // Intercambiar elementos con el vector creado.
        }

        /**
         * Reemplaza el contenido del objeto con elementos en el rango [@a first, @a last).
         *
         * @param first, last   Iteradores a las posiciones inicial y final en un rango.
         */
        template <typename InputIterator,
            typename = typename std::enable_if_t<std::is_base_of_v<std::input_iterator_tag,
                typename std::iterator_traits<InputIterator>::iterator_category>>>
        void assign(InputIterator first, InputIterator last)
        {
            auto temp_it_vector = vector(first, last);
            this->swap(temp_it_vector);
        }

        /**
         * Reemplaza el contenido del contenedor por una copia de los elementos de @a init_list.
         *
         * @param init_list Objeto initializer_list para reemplazar el contenido del contenendor.
         */
        void assign(std::initializer_list<value_type> init_list)
        {
            auto temp_il_vector = vector(init_list);
            this->swap(temp_il_vector);
        }

        // Iteradores.

        /// Retorna un iterador al prinicipio.
        inline iterator begin() noexcept { return this->elements; } // Retornar un puntero al primer elemento del contenedor.
        /// Retorna un iterador al prinicipio.
        inline const_iterator begin() const noexcept { return this->elements; }
        /// Retorna un iterador constante al prinicipio.
        inline const_iterator cbegin() const noexcept { return begin(); }

        /// Retorna un iterador al final.
        inline iterator end() noexcept { return (this->elements + size()); } // Retornar un puntero a la posici�n siguiente de la �ltima del contenedor.
        /// Retorna un iterador al final.
        inline const_iterator end() const noexcept { return (this->elements + size()); }
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
        inline size_type size() const noexcept { return this->count; }

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
            // Si count es menor al tama�o actual del contenedor
            if (count > size())
            {
                // Si count es mayor a la capacidad actual del contenedor
                if (count > capacity())
                {
                    // Reasignar el almacenamiento.
                    this->reallocate(count);
                }
            }
            this->count = count; // count es la nueva cantidad de elementos.
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
            // Si count es mayor al tama�o actual del contenedor
            if (count > size())
            {
                // Si count es mayor a la capacidad actual del contenedor
                if (count > capacity())
                    this->reallocate(count); // Reasignar el almacenamiento.

                // Asignar los nuevos elementos con el valor value.
                for (size_type index = size(); index < count; ++index)
                    this->construct_element(index, value);
            }
            this->count = count; // count es la nueva cantidad de elementos.
        }

        /// Retorna el tama�o del espacio de almacenamiento asignado actualmente para el vector.
        inline size_type capacity() const noexcept { return this->storage_capacity; }
        /// Verifica si el contenedor est� vac�o.
        inline bool empty() const noexcept { return (size() == 0); }

        /**
         * Solicita un cambio en la capacidad del vector para que sea lo suficiente
         * como para contener @a new_capacity elementos.
         *
         * @param new_capacity  La nueva capacidad del vector.
         * @throw std::length_error Si new_capacity sobrepasa la capacidad m�xima.
         */
        void reserve(size_type new_capacity)
        {
            // Si la nueva capacidad es mayor al tama�o m�ximo del vector
            if (new_capacity > max_size())
                throw std::length_error("mySTL::vector::reserve"); // Lanzar excepci�n.

            // Si la nueva capacidad es mayor a la capacidad actual del vector
            if (new_capacity > capacity())
                this->reallocate(new_capacity); // Reasignar almacenamiento.
        }

        /// Solicita reducir la capacidad del contenedor para encajar con el n�mero de elementos.
        inline void shrink_to_fit() { this->reallocate(this->size()); }

        // Acceso a elementos.

        /**
         * Retorna una referencia al elemento en la posici�n @a index del contenedor.
         *
         * @param index Posici�n de un elemento en el contenedor.
         * @return El elemento en la posici�n especificada.
         */
        inline reference operator[](size_type index) { return *(this->elements + index); }

        /**
         * Retorna una referencia al elemento en la posici�n @a index del contenedor.
         *
         * @param index Posici�n de un elemento en el contenedor.
         * @return El elemento en la posici�n especificada.
         */
        inline const_reference operator[](size_type index) const { return *(this->elements + index); }

        /**
         * Retorna una referencia al elemento en la posici�n @a index del contenedor. 
         *
         * Este m�todo verifica si @a index est� dentro del l�mite de elementos v�lidos
         * en el contenedor. Si no lo est�, lanza una excepci�n.
         *
         * @param index Posici�n de un elemento en el contenedor.
         * @throw std::out_of_range
         * @return El elemento en la posici�n especificada.
         */
        inline reference at(size_type index)
        {
            if (index >= size())
                throw std::out_of_range("mySTL::vector::at");

            return *(this->elements + index);
        }

        /**
         * Retorna una referencia al elemento en la posici�n @a index del contenedor.
         *
         * Este m�todo verifica si @a index est� dentro del l�mite de elementos v�lidos
         * en el contenedor. Si no lo est�, lanza una excepci�n.
         *
         * @param index Posici�n de un elemento en el contenedor.
         * @throw std::out_of_range
         * @return El elemento en la posici�n especificada.
         */
        inline const_reference at(size_type index) const
        {
            if (index >= size())
                throw std::out_of_range("mySTL::vector::at");

            return *(this->elements + index);
        }

        /// Retorna una referencia al primer elemento en el contenedor.
        inline reference front() { return *(begin()); } // elements[0]
        /// Retorna una referencia al primer elemento en el contenedor.
        inline const_reference front() const { return *(begin()); }

        /// Retorna una referencia al �ltimo elemento en el contenedor.
        inline reference back() { return *(end() - 1); } // elements[count - 1]
        /// Retorna una referencia al �ltimo elemento en el contenedor.
        inline const_reference back() const { return *(end() - 1); }

        /// Retorna un puntero al primer elemento del contenedor.
        inline value_type* data() noexcept { return this->elements; }
        /// Retorna un puntero al primer elemento del contenedor.
        inline const value_type* data() const noexcept { return this->elements; }

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
            // Si el n�mero de elementos almacenados es igual a la capacidad del contenedor
            if (size() == capacity())
                this->reallocate(std::max(1.0, this->storage_capacity * 2.0)); // Doblar la capacidad y reasignar el almacenamiento.

            // Construir el nuevo elemento con los argumentos recibidos en la �ltima posici�n, y aumentar el n�mero de elementos.
            this->construct_element(this->count++, std::forward<Args>(args)...);
        }

        /**
         * Elimina el �ltimo elemento del contenedor y reduce su tama�o.
         */
        void pop_back()
        {
            --this->count; // Disminuir el n�mero de elementos.
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
            // El indice donde se inserta el nuevo elemento es la distancia entre
            // begin() (el iterador que apunta al inicio) y position (el iterador que apunta a la posici�n).
            size_type index = position - begin();
            // Iterador que apunta a la posici�n de inserci�n. (No const)
            iterator result = begin() + index;

            // Si se quiere insertar al final
            if (position == end())
            {
                // Llamar al m�todo correspondiente. Este adem�s incrementa el tama�o y reasigna si es necesario.
                this->emplace_back(std::forward<Args>(args)...);
            }
            else // De lo contrario
            {
                // Si el n�mero de elementos almacenados es igual a la capacidad del contenedor
                if (this->size() == this->capacity())
                {
                    // Doblar la capacidad y reasignar el almacenamiento.
                    this->reallocate(std::max(1.0, this->storage_capacity * 2.0));
                    result = begin() + index; // Volver a apuntar a la posici�n luego de reasignar.
                }

                // Asignar en la posici�n siguiente a la �ltima, el �ltimo elemento del contenedor.
                this->construct_element(this->count, back());
                // Copiar de forma inversa los elementos desde el elemento en position hasta el pen�ltimo, a su posici�n siguiente.
                mySTL::copy_backward(result, end() - 1, end());
                // Construir el nuevo elemento con los argumentos recibidos, en la posici�n de inserci�n.
                *result = value_type(std::forward<Args>(args)...);

                ++this->count; // Incrementar el n�mero de elementos.
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
            size_type index = position - begin();
            iterator result = begin() + index;

            if ( count == 0 )
                return result; // Nada que insertar xd.

            // Si el nuevo tama�o va a ser mayor a la capacidad actual 
            if ((size() + count) > capacity())
            {
                // Aumentar la capacidad seg�n count y reasignar el almacenamiento.
                this->reallocate(std::max((size_type) 1, this->storage_capacity + count));
                result = begin() + index; // Volver a apuntar a la posici�n luego de reasignar.
            }

            // Si el n�mero de elementos entre la posici�n de inserci�n y la �ltima posici�n es mayor
            // al n�mero de elementos por insertar
            if ((end() - result) > count)
            {
                // Copiar los �ltimos count elementos del contenedor al final del contenedor.
                mySTL::copy_n(end() - count, count, end());
                // Copiar de forma inversa los elementos restantes desde el elemento en position a su posici�n siguiente.
                mySTL::copy_backward(result, end() - count, end());

                // Recorrer entre las posiciones con iteradores y asignar el valor value a cada posici�n.
                for (iterator current = result; current != (result + count); ++current)
                    *current = value;
            }
            else // De lo contrario
            {
                // La distancia entre index y el tama�o del contenedor es el n�mero de elementos que se deben mover.
                size_type copy_count = size() - index;

                // Iterador que apunta a la posici�n de inserci�n.
                iterator current = result;
                // Copiar copy_count elementos a partir de la posici�n de inserci�n, a la posici�n correspondiente.
                mySTL::copy_n(current, copy_count, current + count);

                // Recorrer entre las posiciones con iteradores y asignar el valor value a cada posici�n.
                for (current = result; current != end(); ++current)
                    *current = value;

                // Asignar el valor value a las nuevas posiciones sobrantes del contenedor.
                for (size_type index2 = size(); index2 < size() + (count - (end() - result)); ++index2)
                    this->construct_element(index2, value);
            }
            this->count += count; // Asignar el nuevo tama�o del contendor.

            return result;
        }

        /**
         * Inserta elementos en el rango [@a first, @a last) en la posici�n @a position.
         *
         * @param position      La posici�n del contenedor donde se inserta el primero de los nuevos elementos.
         * @param first, last   El rango de copia de los elementos a insertar.
         * @return iterador que apunta al primero de los nuevos elementos insertados.
         */
        template <typename InputIterator, typename = typename std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
        iterator insert(const_iterator position, InputIterator first, InputIterator last)
        {
            size_type index = position - begin();
            iterator result = begin() + index;

            if ( first == last )
                return result;

            // El n�mero de elementos a insertar (count) es la distancia entre los iteradores.
            size_type count = std::distance(first, last);

            if ((size() + count) > capacity())
            {
                this->reallocate(std::max((size_type) 1, this->storage_capacity + count));
                result = begin() + index; // Volver a apuntar a la posici�n luego de reasignar.
            }

            // Si el n�mero de elementos entre la posici�n de inserci�n y la �ltima posici�n es mayor
            // al n�mero de elementos por insertar
            if ((end() - result) > count)
            {
                // Copiar los �ltimos count elementos del contenedor al final del contenedor.
                mySTL::copy_n(end() - count, count, end());
                // Copiar de forma inversa los elementos desde el elemento en position a su posici�n siguiente.
                mySTL::copy_backward(result, end() - count, end());

                // Iterador que apunta a la posici�n de inserci�n.
                iterator current = result;
                // Copiar los elementos del rango [first, last) al contenedor desde la posici�n de inserci�n.
                mySTL::copy(first, last, current);
            }
            else // De lo contrario
            {
                size_type copy_count = size() - index;

                // Iterador que apunta a la posici�n de inserci�n.
                iterator current = result;
                // Copiar copy_count elementos a partir de la posici�n de inserci�n, a la posici�n correspondiente.
                mySTL::copy_n(current, copy_count, current + count);

                // Copiar el n�mero de elementos del rango que quepan entre la posici�n de inserci�n y el final del contenedor.
                mySTL::copy(first, first + (end() - result), current);
                // Luego, copiar los elementos del rango restantes a las nuevas posiciones sobrantes del contenedor.
                mySTL::copy_n((first + (end() - result)), count - (end() - result), end());
            }
            this->count += count; // Asignar el nuevo tama�o del contendor.

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
            size_type index = position - begin();
            iterator result = begin() + index;

            // El n�mero de elementos por insertar es el tama�o de init_list.
            size_type count = init_list.size();

            if ( count == 0 )
                return result;

            if ((size() + count) > capacity())
            {
                this->reallocate(std::max((size_type) 1, this->storage_capacity + count));
                result = begin() + index; // Volver a apuntar a la posici�n luego de reasignar.
            }

            // Si el n�mero de elementos entre la posici�n de inserci�n y la �ltima posici�n es mayor
            // al n�mero de elementos por insertar
            if ((end() - result) > count)
            {
                // Copiar los �ltimos count elementos del contenedor al final del contenedor.
                mySTL::copy_n(end() - count, count, end());
                // Copiar de forma inversa los elementos desde el elemento en position a su posici�n siguiente.
                mySTL::copy_backward(result, end() - count, end());

                // Iterador que apunta a la posici�n de inserci�n.
                iterator current = result;
                // Copiar todos los elementos de init_list al contenedor, desde la posici�n de inserci�n.
                for (const value_type& element : init_list)
                    *(current++) = element;
            }
            else // De lo contrario
            {
                // Iterador que apunta a la posici�n de inserci�n.
                iterator current = result;
                mySTL::copy_n(current, size() - index, current + count);

                // Iterador al inicio de init_list.
                iterator il_current = init_list.begin();
                // Copiar los elementos de init_list que quepan entre la posici�n de inserci�n y el final del contenedor.
                for (current = result; current != end(); ++current)
                    *current = *(il_current++);

                // Copiar los elementos restantes a las nuevas posiciones sobrantes del contenedor.
                for (size_type index2 = size(); index2 < size() + (count - (end() - result)); ++index2)
                    this->construct_element(index2, *(il_current++));
            }
            this->count += count; // Asignar el nuevo tama�o del contendor.

            return result;
        }

        /**
         * Elimina el elemento en la posici�n @a position del contenedor.
         *
         * @param position  Iterador que apunta al elemento por eliminar.
         * @return iterador que apunta a la nueva ubicaci�n del elemento siguiente al eliminado.
         */
        iterator erase(const_iterator position)
        {
            // El indice donde se elimina el elemento es la distancia entre
            // begin() (el iterador que apunta al inicio) y position (el iterador que apunta a la posici�n).
            size_type index = position - begin();
            // Iterador que apunta a la posici�n de eliminado. (No const)
            iterator result = begin() + index;

            // Si el elemento que se quiere eliminar no es el �ltimo del contenedor
            if ((position + 1) != end())
            {
                // Iterador a la posici�n siguiente a la del elemento eliminado.
                iterator current = result + 1;
                // Iterador a la posici�n de eliminado.
                iterator destiny = result;

                // Copiar los elementos siguientes al eliminado a su posici�n anterior.
                mySTL::copy(current, end(), destiny);
            }
            this->pop_back(); // Eliminar el �ltimo elemento.

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
            // El indice donde se elimina el primer elemento es la distancia entre
            // begin() (el iterador que apunta al inicio) y first
            // (el iterador que apunta a la posici�n del primer elemento a eliminar).
            size_type first_index = first - begin();
            // El n�mero de elementos por eliminar (count) es la distancia entre los iteradores.
            size_type count = std::distance(first, last);
            // Iterador que apunta a la posici�n de eliminado. (No const)
            iterator result = begin() + first_index;

            // Iterador a la posici�n siguiente a la de los elementos eliminados.
            iterator current = result + count;
            // Iterador a la posici�n de eliminado.
            iterator destiny = result;

            // Copiar los elementos siguientes a los eliminados a sus posiciones correspondientes.
            mySTL::copy(current, end(), destiny);

            // Asignar el nuevo tama�o del contenedor. Al hacer esto, se "eliminan" los elementos sobrantes.
            this->count -= count;

            return result;
        }

        /**
         * Intercambia el contenido de este objeto por el contenido de @a other.
         *
         * @param other Otro objeto vector del mismo tipo, para intercambiar sus elementos.
         */
        void swap(vector& other)
        {
            using std::swap; // Funci�n swap de la biblioteca est�ndar.
            swap(this->count, other.count);
            swap(this->storage_capacity, other.storage_capacity);
            swap(this->elements, other.elements);
        }

        /**
         * Elimina todos los elementos del contenedor.
         */
        void clear() noexcept
        {
            // Destruir el contenedor.
            this->destroy_all();
            this->count = 0; // Actualizar el tama�o a 0.
        }

    // M�todos privados    
    private:

        /**
         * Construye un elemento con el valor @a value en la posici�n @a index del contenedor.
         * 
         * @param index La posici�n donde se construye el nuevo elemento.
         * @param value El valor del elemento por construir.
         */
        void construct_element(size_type index, const value_type& value)
        {
            *(this->elements + index) = value;
        }

        /**
         * Destruye el contenedor interno de elementos.
         */
        void destroy_all()
        {
            delete [] this->elements;
        }

        /**
         * Reasigna el almacenamiento de elementos. 
         * 
         * Crea un nuevo contenedor de elementos con capacidad para @a new_capacity elementos,
         * copia los elementos del contenedor actual y reemplaza el contenedor actual por el nuevo.
         * 
         * @param new_capacity  La nueva capacidad por asignar al contenedor.
         */
        void reallocate(size_type new_capacity)
        {
            // Nuevo contenedor con la nueva capacidad.
            value_type* new_elements = new value_type[new_capacity];

            // Copiar los elementos al nuevo contenedor.
            mySTL::copy_n(this->elements, this->count, new_elements);
            // Destruir el contenedor actual.
            this->destroy_all();

            // Asignar new_capacity como la nueva capacidad de almacenamiento.
            this->storage_capacity = new_capacity;
            // El nuevo contenedor es ahora el contenedor interno.
            this->elements = new_elements;
        }

    };

    // Funciones no miembros.

    // Operadores relacionales

    template <typename ValueType>
    inline bool operator==(const vector<ValueType>& lhs, const vector<ValueType>& rhs)
    {
        // Comparar tama�o, despu�s comparar cada elemento.
        return ( ( lhs.size() == rhs.size() ) && ( mySTL::equal(lhs.begin(), lhs.end(), rhs.begin()) ) );
    }

    template <typename ValueType>
    inline bool operator!=(const vector<ValueType>& lhs, const vector<ValueType>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename ValueType>
    inline bool operator<(const vector<ValueType>& lhs, const vector<ValueType>& rhs)
    {
        return mySTL::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <typename ValueType>
    inline bool operator<=(const vector<ValueType>& lhs, const vector<ValueType>& rhs)
    {
        return !(rhs < lhs);
    }

    template <typename ValueType>
    inline bool operator>(const vector<ValueType>& lhs, const vector<ValueType>& rhs)
    {
        return (rhs < lhs);
    }

    template <typename ValueType>
    inline bool operator>=(const vector<ValueType>& lhs, const vector<ValueType>& rhs)
    {
        return !(lhs < rhs);
    }

    // Funci�n swap

    /**
    * Intercambia el contenido de @a vector_one por el de @a vector_two.
    * 
    * @param vector_one, vector_two Contenedores vector del mismo tipo y tama�o.
    */ 
    template <typename ValueType>
    inline void swap(const vector<ValueType>& vector_one, const vector<ValueType>& vector_two)
    {
        vector_one.swap(vector_two);
    }

}

#endif /* MY_VECTOR_H */
