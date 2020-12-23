#ifndef MY_ARRAY_H
#define MY_ARRAY_H

#include "../algorithm/my_algorithm.h"

#include <cstdlib>
#include <iterator>
#include <utility>
#include <stdexcept>

namespace mySTL
{
    /// Accede al tamaño de un objeto array como si fuera un tuple.
    template <typename TupleType>
    class tuple_size;

    /// Accede al tipo estático de los miembros de un objeto array como si fuera un tuple.
    template <std::size_t Index, typename TupleType>
    class tuple_element;

    /**
     * @brief Contenedor de tamaño fijo que almacena una secuencia de elementos.
     *
     * @tparam ValueType    El tipo de los elementos que contiene.
     * @tparam Size         El tamaño del contenedor en términos de cantidad de elementos.
     */
    template <typename ValueType, std::size_t Size>
    struct array
    {
        // Tipos miembro

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
        typedef value_type* iterator;
        // Iterador constante.
        typedef const value_type* const_iterator;
        // Iterador inverso.
        typedef std::reverse_iterator<iterator> reverse_iterator;
        // Iterador inverso constante.
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        // Contenedor de tamaño Size para guardar elementos de tipo ValueType.
        ValueType elements[Size];

        // Iteradores.

        /// Retorna un iterador al principio.
        inline iterator begin() noexcept { return elements; } // Retornar un puntero al primer elemento del contenedor.
        /// Retorna un iterador al principio.
        inline const_iterator begin() const noexcept { return elements; }
        /// Retorna un iterador constante al principio.
        inline const_iterator cbegin() const noexcept { return begin(); }

        /// Retorna un iterador al final.
        inline iterator end() noexcept { return (elements + Size); } // Retornar un puntero a la posición siguiente de la última del contenedor.
        /// Retorna un iterador al final.
        inline const_iterator end() const noexcept { return (elements + Size); }
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

        // Capacidad

        /// Retorna el número de elementos del contenedor.
        inline constexpr size_type size() const noexcept { return Size; }
        /// Retorna la cantidad máxima de elementos que puede contener el contenedor.
        inline constexpr size_type max_size() const noexcept { return Size; }
        /// Verifica si el contenedor está vacío.
        inline constexpr bool empty() const noexcept { return (size() == 0); }

        // Acceso a elementos

        /**
         * Retorna una referencia al elemento en la posición @a index del contenedor.
         *
         * @param index Posición de un elemento en este objeto.
         * @return El elemento en la posición especificada.
         */
        inline reference operator[](size_type index) { return *(elements + index); }

        /**
         * Retorna una referencia al elemento en la posición @a index del contenedor.
         *
         * @param index Posición de un elemento en este objeto.
         * @return El elemento en la posición especificada.
         */
        inline const_reference operator[](size_type index) const { return *(elements + index); }

        /**
         * Retorna una referencia al elemento en la posición @a index del contenedor.
         *
         * Este método verifica si @a index está dentro del límite de elementos válidos
         * en el contenedor. Si no lo está, lanza una excepción.
         *
         * @param index Posición de un elemento en este objeto.
         * @throw std::out_of_range
         * @return El elemento en la posición especificada.
         */
        inline reference at(size_type index)
        {
            if ( index >= size() )
                throw std::out_of_range("mySTL::array::at");

            return *(elements + index);
        }

        /**
         * Retorna una referencia constante al elemento en la posición @a index del contenedor.
         *
         * Este método verifica si @a index está dentro del límite de elementos válidos
         * en el contenedor. Si no lo está, lanza una excepción.
         *
         * @param index Posición de un elemento en este objeto.
         * @throw std::out_of_range
         * @return El elemento en la posición especificada.
         */
        inline const_reference at(size_type index) const
        {
            if ( index >= size() )
                throw std::out_of_range("mySTL::array::at");

            return *(elements + index);
        }

        /// Retorna una referencia al primer elemento en el contenedor.
        inline reference front() { return *(elements); } // elements[0]
        /// Retorna una referencia al primer elemento en el contenedor.
        inline const_reference front() const { return *(elements); }

        /// Retorna una referencia al último elemento en el contenedor.
        inline reference back() { return *(elements + (Size - 1)); } // elements[Size - 1]
        /// Retorna una referencia al último elemento en el contenedor.
        inline const_reference back() const { return *(elements + (Size - 1)); }

        /// Retorna un puntero al primer elemento del contenedor.
        inline value_type* data() noexcept { return elements; }
        /// Retorna un puntero al primer elemento del contenedor.
        inline const value_type* data() const noexcept { return elements; }

        // Modificadores.

        /**
         * Asigna @a value como el valor para todos los elementos del objeto.
         *
         * @param value Valor para rellenar el array.
         */
        inline void fill(const value_type& value)
        {
            // Para cada elemento de tipo value_type en el contenedor de elementos
            for ( value_type& element : elements )
                element = value; // Asignarle el valor recibido como parámetro.
        }

        /**
         * Intercambia el contenido de este objeto por el de @a other.
         *
         * @param other Otro objeto array del mismo tipo y que incluye el mismo tamaño, para intercambiar sus elementos con este objeto.
         */
        void swap(array& other) noexcept(noexcept(std::swap(std::declval<value_type&>(), std::declval<value_type&>())))
        {
            mySTL::swap_ranges(this->begin(), this->end(), other.begin());
        }

    };

    // Funciones no-miembro

    // Operadores relacionales

    template <typename ValueType, std::size_t Size>
    inline bool operator==(const array<ValueType, Size>& lhs, const array<ValueType, Size>& rhs)
    {
        return mySTL::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <typename ValueType, std::size_t Size>
    inline bool operator!=(const array<ValueType, Size>& lhs, const array<ValueType, Size>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename ValueType, std::size_t Size>
    inline bool operator<(const array<ValueType, Size>& lhs, const array<ValueType, Size>& rhs)
    {
        return mySTL::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <typename ValueType, std::size_t Size>
    inline bool operator<=(const array<ValueType, Size>& lhs, const array<ValueType, Size>& rhs)
    {
        return !(rhs < lhs);
    }

    template <typename ValueType, std::size_t Size>
    inline bool operator>(const array<ValueType, Size>& lhs, const array<ValueType, Size>& rhs)
    {
        return (rhs < lhs);
    }

    template <typename ValueType, std::size_t Size>
    inline bool operator>=(const array<ValueType, Size>& lhs, const array<ValueType, Size>& rhs)
    {
        return !(lhs < rhs);
    }

    // get (array)

    /// Retorna una referencia al Index-ésimo elemento del array.
    template <std::size_t Index, typename ValueType, std::size_t Size>
    ValueType& get(array<ValueType, Size>& arr) noexcept
    {
        static_assert(Index < Size, "Index is out of bounds");
        return arr[Index];
    }

    /// Retorna una referencia al Index-ésimo elemento del array.
    template <std::size_t Index, typename ValueType, std::size_t Size>
    ValueType&& get(array<ValueType, Size>&& arr) noexcept
    {
        static_assert(Index < Size, "Index is out of bounds");
        return arr[Index];
    }

    /// Retorna una referencia al Index-ésimo elemento del array.
    template <std::size_t Index, typename ValueType, std::size_t Size>
    const ValueType& get(const array<ValueType, Size>& arr) noexcept
    {
        static_assert(Index < Size, "Index is out of bounds");
        return arr[Index];
    }

    // swap (array)

    /**
     * Intercambia el contenido de @a array_one por el de @a array_two.
     * 
     * @param array_one, array_two  Contenedores array del mismo tipo y tamaño.
     */ 
    template <typename ValueType, std::size_t Size>
    void swap(array<ValueType, Size>& array_one, array<ValueType, Size>& array_two)
    {
        array_one.swap(array_two);
    }

    // Funciones tuple.

    template <typename ValueType, std::size_t Size>
    struct tuple_size<array<ValueType, Size>>
        : std::integral_constant<std::size_t, Size>
    {
    };

    template <std::size_t Index, typename ValueType, std::size_t Size>
    struct tuple_element<Index, array<ValueType, Size>>
    {
        typedef ValueType type;
    };

}

#endif /* MY_ARRAY_H */