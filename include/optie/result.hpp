#ifndef ENN_OPTIE_RESULT_HPP
#define ENN_OPTIE_RESULT_HPP

#include "helper.hpp"
#include "memory.hpp"
#include "status.hpp"

#include <utility> // std::move

namespace optie {

    // -------------------------------------------------------------------------
    //   RESULT
    // -------------------------------------------------------------------------

    template<typename T, typename E>
    class result {

    private: // data members ---------------------------------------------------

        using A = helper::biggest_t<T, E>;

        memory::storage<sizeof(A), alignof(A)> _storage;
        bool _ok;

    private: // helper methods -------------------------------------------------

        template<typename V>
        V* _storage_get() {
            return _storage.template get_as<V>();
        }

        template<typename V>
        void _storage_init(V&& value) {
            _storage.create(std::forward<V>(value));
        }

        template<typename V>
        void _storage_deinit() {
            _storage.template destroy<V>();
        }

        void _result_init_from_copy(result const& other) {
            _ok = other._ok;

            if (_ok)
                _storage_init(*other._storage_get<T>());
            else
                _storage_init(*other._storage_get<E>());
        }

        void _result_init_from_move(result&& other) {
            _ok = other._ok;

            if (_ok)
                _storage_init<T>(std::move(*other._storage_get<T>()));
            else
                _storage_init<E>(std::move(*other._storage_get<E>()));
        }

        void _result_deinit() {
            if (_ok)
                _storage_deinit<T>();
            else
                _storage_deinit<E>();
        }

    public: // special methods: ctor dtor --------------------------------------

        template<typename V, helper::enable_if_convertible<V, T> = 0>
        result(ok_t<V>&& ok) : _ok(true) {
            _storage_init<T>(std::move(ok.value)); // possible convertation
        };

        template<typename V, helper::enable_if_convertible<V, E> = 0>
        result(err_t<V>&& err) : _ok(false) {
            _storage_init<E>(std::move(err.value)); // possible convertation
        };

        ~result() {
            _result_deinit();
        };

        result(result const& other) {
            _result_init_from_copy(other);
        }

        result(result&& other) {
            _result_init_from_move(std::move(other));
        }

        result& operator=(result const& other) {
            _result_deinit();
            _result_init_from_copy(other);
            return *this;
        }

        result& operator=(result&& other) {
            _result_deinit();
            _result_init_from_move(std::move(other));
            return *this;
        }

    public: // if value methods ------------------------------------------------

        bool is_ok() const {
            return _ok;
        }

        bool is_err() const {
            return !is_ok();
        }

    public: // get value methods -----------------------------------------------

        T&& or_exit(char const* message = NULL) && {
            if (is_err())
                helper::exit_handler{message}();

            return std::move(*_storage_get<T>());
        }

        T& or_exit(char const* message = NULL) & {
            if (is_err())
                helper::exit_handler{message}();

            return *_storage_get<T>();
        }

        T/*&&*/ or_default(T/*&&*/ value) && {
            if (is_err())
                return std::move(value);

            return std::move(*_storage_get<T>());
        }

        T/*&*/ or_default(T/*&*/ value) & {
            if (is_err())
                return value;

            return *_storage_get<T>();
        }

        template<typename F>
        T or_call(F handler) && {
            if (is_err())
                return handler(*_storage_get<E>());

            return std::move(*_storage_get<T>());
        }

        template<typename F>
        T or_call(F handler) & {
            if (is_err())
                return handler(*_storage_get<E>());

            return *_storage_get<T>();
        }
    };

} // namespace optie

#endif // ENN_OPTIE_RESULT_HPP
