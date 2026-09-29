#pragma once
#include <functional>
#include <vector>

class Ordenamientos {
private:
    // --- MÉTODOS PRIVADOS (Auxiliares internos para que los algoritmos funcionen) ---

    // Auxiliar HeapSort
    template <typename T>
    static void heapify(std::vector<T>& datos, int n, int i, std::function<bool(const T&, const T&)>& comparador) {
        int mayor = i, izq = 2 * i + 1, der = 2 * i + 2;
        if (izq < n && comparador(datos[mayor], datos[izq])) mayor = izq;
        if (der < n && comparador(datos[mayor], datos[der])) mayor = der;
        if (mayor != i) {
            std::swap(datos[i], datos[mayor]);
            heapify(datos, n, mayor, comparador);
        }
    }

    // Auxiliares QuickSort
    template <typename T>
    static void quickSortRec(std::vector<T>& datos, int bajo, int alto, std::function<bool(const T&, const T&)>& comparador) {
        if (bajo < alto) {
            int p = particionar(datos, bajo, alto, comparador);
            quickSortRec(datos, bajo, p - 1, comparador);
            quickSortRec(datos, p + 1, alto, comparador);
        }
    }

    template <typename T>
    static int particionar(std::vector<T>& datos, int bajo, int alto, std::function<bool(const T&, const T&)>& comparador) {
        T pivote = datos[alto];
        int i = bajo - 1;
        for (int j = bajo; j < alto; j++) {
            if (comparador(datos[j], pivote)) {
                i++;
                std::swap(datos[i], datos[j]);
            }
        }
        std::swap(datos[i + 1], datos[alto]);
        return i + 1;
    }

    // Auxiliares MergeSort
    template <typename T>
    static void mergeSortRec(std::vector<T>& datos, int izq, int der, std::function<bool(const T&, const T&)>& comparador) {
        if (izq >= der) return;
        int medio = izq + (der - izq) / 2;
        mergeSortRec(datos, izq, medio, comparador);
        mergeSortRec(datos, medio + 1, der, comparador);
        mezclar(datos, izq, medio, der, comparador);
    }

    template <typename T>
    static void mezclar(std::vector<T>& datos, int izq, int medio, int der, std::function<bool(const T&, const T&)>& comparador) {
        std::vector<T> izquierda(datos.begin() + izq, datos.begin() + medio + 1);
        std::vector<T> derecha(datos.begin() + medio + 1, datos.begin() + der + 1);

        size_t i = 0, j = 0;
        int k = izq;
        while (i < izquierda.size() && j < derecha.size()) {
            if (comparador(izquierda[i], derecha[j])) datos[k++] = izquierda[i++];
            else datos[k++] = derecha[j++];
        }
        while (i < izquierda.size()) datos[k++] = izquierda[i++];
        while (j < derecha.size()) datos[k++] = derecha[j++];
    }

public:
    // --- INTERFAZ PÚBLICA (Lo que el resto del programa utiliza) ---

    template <typename T>
    static void heapSort(std::vector<T>& datos, std::function<bool(const T&, const T&)> comparador) {
        int n = (int)datos.size();
        for (int i = n / 2 - 1; i >= 0; i--) heapify(datos, n, i, comparador);
        for (int i = n - 1; i > 0; i--) {
            std::swap(datos[0], datos[i]);
            heapify(datos, i, 0, comparador);
        }
    }

    template <typename T>
    static void quickSort(std::vector<T>& datos, std::function<bool(const T&, const T&)> comparador) {
        if (datos.empty()) return;
        quickSortRec(datos, 0, (int)datos.size() - 1, comparador);
    }

    template <typename T>
    static void mergeSort(std::vector<T>& datos, std::function<bool(const T&, const T&)> comparador) {
        if (datos.size() <= 1) return;
        mergeSortRec(datos, 0, (int)datos.size() - 1, comparador);
    }

    template <typename T, typename K>
    static int busquedaBinariaRecursiva(
        const std::vector<T>& datos, int bajo, int alto,
        const K& claveBuscada,
        std::function<K(const T&)> extractorClave) {

        if (bajo > alto) return -1;
        int medio = bajo + (alto - bajo) / 2;
        K claveMedio = extractorClave(datos[medio]);

        if (claveMedio == claveBuscada) return medio;

        if (claveBuscada < claveMedio) {
            return busquedaBinariaRecursiva(datos, bajo, medio - 1, claveBuscada, extractorClave);
        }
        else {
            return busquedaBinariaRecursiva(datos, medio + 1, alto, claveBuscada, extractorClave);
        }
    }
};