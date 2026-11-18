#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <optional>
#include <stdexcept>
#include <iostream>

//Nodo de la tabla hash
template<typename K, typename V>
struct NodoHash {
    K clave;
    V valor;
    std::unique_ptr<NodoHash<K,V>> sig;

    NodoHash(K k, V v)
        : clave(k), valor(v), sig(nullptr) {}
};

//Tabla Hash con encadenamiento separado
template<typename K, typename V>
class TablaHash {
    std::vector<std::unique_ptr<NodoHash<K,V>>> _tabla;
    size_t _m, _n;
    std::function<size_t(const K&)> _hashFn;

    size_t _idx(const K& k) const {
        return _hashFn(k) % _m;
    }

public:
    explicit TablaHash(
        size_t m = 31,
        std::function<size_t(const K&)> fn = std::hash<K>{}
    ) : _tabla(m), _m(m), _n(0), _hashFn(fn) {
        if (m == 0)
            throw std::invalid_argument("Tamano invalido");
    }

    //TODO 1: Insertar o actualizar
    void insertar(const K& clave, const V& valor) {
        size_t idx = _idx(clave);

        for (auto* p = _tabla[idx].get();
             p != nullptr; p = p->sig.get()) {
            if (p->clave == clave) {
                p->valor = valor;
                return;
            }
        }

        auto nuevo = std::make_unique<NodoHash<K,V>>(
            clave, valor
        );

        nuevo->sig = std::move(_tabla[idx]);
        _tabla[idx] = std::move(nuevo);
        ++_n;

        if (factor_carga() > 0.75)
            rehash();
    }

    //TODO 2: Buscar
    std::optional<V> buscar(const K& clave) const {
        size_t idx = _idx(clave);

        for (auto* p = _tabla[idx].get();
             p != nullptr; p = p->sig.get()) {
            if (p->clave == clave)
                return p->valor;
        }

        return std::nullopt;
    }

    //TODO 3: Eliminar
    bool eliminar(const K& clave) {
        size_t idx = _idx(clave);
        auto* enlace = &_tabla[idx];

        while (*enlace) {
            if ((*enlace)->clave == clave) {
                auto eliminado = std::move(*enlace);
                *enlace = std::move(eliminado->sig);
                --_n;
                return true;
            }

            enlace = &((*enlace)->sig);
        }

        return false;
    }

    //TODO 4: Rehashing
    void rehash() {
        size_t nuevo_m = siguiente_primo(2 * _m);

        std::vector<std::unique_ptr<NodoHash<K,V>>>
            nueva_tabla(nuevo_m);

        for (auto& cabeza : _tabla) {
            while (cabeza) {
                auto nodo = std::move(cabeza);
                cabeza = std::move(nodo->sig);

                size_t idx = _hashFn(nodo->clave) % nuevo_m;

                nodo->sig = std::move(nueva_tabla[idx]);
                nueva_tabla[idx] = std::move(nodo);
            }
        }

        _tabla = std::move(nueva_tabla);
        _m = nuevo_m;
    }

    double factor_carga() const {
        return static_cast<double>(_n) / _m;
    }

    size_t tamanio() const { return _n; }
    size_t cubos() const { return _m; }

    static size_t siguiente_primo(size_t n) {
        auto es_primo = [](size_t x) {
            if (x < 2) return false;

            for (size_t i = 2; i <= x / i; ++i)
                if (x % i == 0)
                    return false;

            return true;
        };

        size_t p = n + 1;

        while (!es_primo(p))
            ++p;

        return p;
    }
};
