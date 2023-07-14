#pragma once

/**
 * @file Matrix.h
 *
 * @brief Implémentation de matrices simples.
 *
 * Nom: Frederic Labelle
 * Code permanent : LABF11109803
 * Email : frederic.labelle.3@ens.etsmtl.ca
 *
 */

#include "MatrixBase.h"
#include "Vector.h"
#include<string>

namespace gti320
{
    enum StorageType { ColumnStorage = 0, RowStorage = 1 };

    // Déclaration avancée
    template <typename _Scalar, int _RowsAtCompile, int _ColsAtCompile, int _StorageType>
    class SubMatrix;


#pragma region // columnMatrix
    /**
     * Classe Matrix spécialisé pour le cas générique. (defaut par colonne)
     *
     * Note :
     * Le cas d'un stockage par ligne fait l'objet d'une spécialisation de patron. (voir plus bas)
     */
    template <typename _Scalar = double, int _RowsAtCompile = Dynamic, int _ColsAtCompile = Dynamic, int _StorageType = ColumnStorage>
    class Matrix : public MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>
    {
    public:
        /** Constructeur par défaut */
        Matrix() : MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>() { }

        /** Constructeur de copie */
        Matrix(const Matrix& other) : MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>(other) {}

        /** Constructeur avec spécification du nombre de ligne et de colonnes */
        explicit Matrix(int _rows, int _cols) : MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>(_rows, _cols) {  }

        /** Destructeur */
        ~Matrix() {}

        /**
         * Opérateur de copie à partir d'une sous-matrice.
         * Exemple :
         * Matrix<...> A(...);
         * Matrix<...> B(...);
         * B = A.block(i,j,m,n);
         */
        template <typename _OtherScalar, int OtherRows, int _OtherCols, int _OtherStorage>
        Matrix& operator=(const SubMatrix<_OtherScalar, OtherRows, _OtherCols, _OtherStorage>& submatrix)
        {
            /**
             * DONE : Copier les données de la sous-matrice.
             *
             * Note :
             * Si les dimensions ne correspondent pas, la matrice doit être redimensionnée.
             * Vous pouvez présumer qu'il s'agit d'un stockage par colonnes.
             */
            int subRows = submatrix.rows();
            int subCols = submatrix.cols();

            if (rows() != subRows || cols() != subCols)
                resize(subRows, subCols);

            for (int j = 0; j < subCols; ++j)
            {
                for (int i = 0; i < subRows; ++i)
                {
                    (*this)(i, j) = submatrix(i, j);
                }
            }


            return *this;
        }

        /** Accesseur à une entrée de la matrice (lecture seule) */
        _Scalar operator()(int i, int j) const
        {
            /** DONE: Implémenter. */
            assert((rows() * j + i) < (*this).size());
            return m_storage.data()[rows() * j + i];
        }

        /** Accesseur à une entrée de la matrice (lecture ou écriture) */
        _Scalar& operator()(int i, int j)
        {
            /**
             * DONE: Implémenter.
             *
             * Indice :
             * L'implémentation devrait être identique à celle de la fonction précédente.
             */
            assert((rows() * j + i) < (*this).size());
            return m_storage.data()[rows() * j + i];
        }

        /** Créer une sous-matrice de taille (rows, cols) à partir de l'index (i,j) */
        SubMatrix<_Scalar, _RowsAtCompile, _ColsAtCompile, _StorageType> block(int i, int j, int rows, int cols) const
        {
            return SubMatrix<_Scalar, _RowsAtCompile, _ColsAtCompile, _StorageType>(*this, i, j, rows, cols);
        }

        /** Calcule l'inverse de la matrice */
        Matrix inverse() const
        {
            // Ne rien faire.
            return *this;
        }

        /** Retourne la transposée de la matrice */
        template <typename _OtherScalar, int _OtherRows, int _OtherCols, int _OtherStorage>
        Matrix<_OtherScalar, _OtherRows, _OtherCols, _OtherStorage> transpose() const
        {
            /** DONE: Calculer et retourner la transposée de la matrice. */
            int n_cols = cols();
            int n_rows = rows();

            Matrix<_OtherScalar, _OtherRows, _OtherCols, _OtherStorage> transposed(n_cols, n_rows);
            transposed.setZero();

            for (int i = 0; i < n_cols; ++i)
            {
                for (int j = 0; j < n_rows; ++j)
                {
                     transposed(i, j) = (*this)(j, i);
                }
            }

            return transposed;
        }

        /** Affecte l'identité à la matrice */
        inline void setIdentity()
        {
            /**
             * DONE: Affecter la valeur 0.0 partout, sauf sur la diagonale principale où c'est 1.0.
             *
             * Note :
             * Votre implémentation devrait aussi fonctionner pour des matrices qui ne sont pas carrées.
             */

            int n_cols = cols();
            int n_rows = rows();

            for (int i = 0; i < n_rows; ++i)
            {
                for (int j = 0; j < n_cols; ++j)
                {
                    if (i == j)
                        (*this)(i, j) = 1.0;
                    else
                        (*this)(i,j) = 0.0;
                }
            }
        }

    };
#pragma endregion // columnMatrix

#pragma region // RowMatrix

    /** Classe Matrix spécialisée pour un stockage par lignes */
    template <typename _Scalar, int _RowsAtCompile, int _ColsAtCompile>
    class Matrix<_Scalar, _RowsAtCompile, _ColsAtCompile, RowStorage> : public MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>
    {

    public:
        /** Constructeur par défaut */
        Matrix() : MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>() { }

        /** Constructeur de copie */
        Matrix(const Matrix& other) : MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>(other) { }

        /** Constructeur avec spécification du nombre de ligne et de colonnes */
        explicit Matrix(int rows, int cols) : MatrixBase<_Scalar, _RowsAtCompile, _ColsAtCompile>(rows, cols) { }

        /** Destructeur */
        ~Matrix() {}

        /**
         * Opérateur de copie à partir d'une sous-matrice.
         * Exemple :
         * Matrix<...> A(...);
         * Matrix<...> B(...);
         * B = A.block(i,j,m,n);
         */
        template <typename _OtherScalar, int OtherRows, int _OtherCols, int _OtherStorage>
        Matrix& operator=(const SubMatrix<_OtherScalar, OtherRows, _OtherCols, _OtherStorage>& submatrix)
        {
            /**
             * DONE: Copier les données de la sous-matrice.
             *
             * Note :
             * Si les dimensions ne correspondent pas, la matrice doit être redimensionnée.
             * Vous pouvez présumer qu'il s'agit d'un stockage par lignes.
             */

            int subRows = submatrix.rows();
            int subCols = submatrix.cols();

            if (rows() != subRows || cols() != subCols)
                resize(subRows, subCols);

            for (int i = 0; i < subRows; ++i)
            {
                for (int j = 0; j < subCols; ++j)
                {
                    (*this)(i, j) = submatrix(i, j);
                }
            }


            return *this;
        }

        /** Accesseur à une entrée de la matrice (lecture seule) */
        _Scalar operator()(int i, int j) const
        {
            /** DONE: Implémenter. */
            assert((cols() * i + j) < (*this).size());
            return m_storage.data()[cols() * i + j];
        }

        /** Accesseur à une entrée de la matrice (lecture ou écriture) */
        _Scalar& operator()(int i, int j)
        {
            /** DONE: Implémenter. */
            assert((cols() * i + j) < (*this).size());
            return m_storage.data()[cols() * i + j];
        }

        /** Créer une sous-matrice de taille (rows, cols) à partir de l'index (i,j) */
        SubMatrix<_Scalar, _RowsAtCompile, _ColsAtCompile, RowStorage> block(int i, int j, int rows, int cols) const
        {
            return SubMatrix<_Scalar, _RowsAtCompile, _ColsAtCompile, RowStorage>(*this, i, j, rows, cols);
        }

        /** Calcule l'inverse de la matrice */
        Matrix inverse() const
        {
            // Do nothing.
            return *this;
        }

        /** Retourne la transposée de la matrice */
        Matrix<_Scalar, _ColsAtCompile, _RowsAtCompile, ColumnStorage> transpose() const
        {
            /**
             * DONE:
             * - Calculer et retourner la transposée de la matrice.
             * - Optimisez cette fonction en tenant compte du type de stockage utilisé.
             */

            int n_cols = cols();
            int n_rows = rows();

            Matrix<_Scalar, _ColsAtCompile, _RowsAtCompile, ColumnStorage> transposed(n_cols, n_rows);
            transposed.setZero();
       
            for (int j = 0; j < n_rows; ++j)
            {
                for (int i = 0; i < n_cols; ++i)
                {
                    transposed(i, j) = (*this)(j, i);
                }
            }

            return transposed;
        }

        /** Affecte l'identité à la matrice */
        inline void setIdentity()
        {
            /**
             * DONE: Affecter la valeur 0.0 partout, sauf sur la diagonale principale où c'est 1.0.
             *
             * Note :
             * Votre implémentation devrait aussi fonctionner pour des matrices qui ne sont pas carrées.
             */
            int n_cols = cols();
            int n_rows = rows();

            for (int i = 0; i < n_rows; ++i)
            {
                for (int j = 0; j < n_cols; ++j)
                {
                    if (i == j)
                        (*this)(i, j) = 1.0;
                    else
                        (*this)(i, j) = 0.0;
                }
            }
        }
    };
#pragma endregion // rowMatrix

#pragma region // SubMatrix
    /**
     * Classe pour accéder à une sous-matrice.
     *
     * Note :
     * Un sous-matrice ne copie pas les données.
     * Elle conserve plutôt une référence à la matrice originale.
     */
    template <typename _Scalar, int _RowsAtCompile, int _ColsAtCompile, int _StorageType>
    class SubMatrix
    {
    private:
        // Référence à la matrice originale
        Matrix<_Scalar, _RowsAtCompile, _ColsAtCompile, _StorageType>& m_matrix;

        // Constructeur par défaut (privé)
        SubMatrix() {  }

        // (m_i, m_j) est le coin supérieur gauche de la sous-matrice dans la matrice originale.
        int m_i; // Décalage en ligne
        int m_j; // Décalage en colonne

        // La sous-matrice est de dimension : m_rows x m_cols
        int m_rows; // Hauteur de la sous-matrice (nombre de lignes)
        int m_cols; // Largeur de la sous-matrice (nombre de colonnes)

    public:
        /** Constructeur à partir d'une référence en lecture seule à une matrice. */
        SubMatrix(const Matrix<_Scalar, _RowsAtCompile, _ColsAtCompile, _StorageType>& _matrix, int _i, int _j, int _rows, int _cols) :
            m_matrix(const_cast<Matrix<_Scalar, _RowsAtCompile, _ColsAtCompile, _StorageType>&>(_matrix)), m_i(_i), m_j(_j), m_rows(_rows), m_cols(_cols)
        {
            assert(_i < _matrix.rows() && _i >= 0);
            assert(_j < _matrix.cols() && _j >= 0);

            int max_rows = _matrix.rows() - _i;
            int max_cols = _matrix.cols() - _j;

            assert(_rows <= max_rows && _cols <= max_cols);
        }

        /** Constructeur à partir d'une référence en lecture et écriture à une matrice. */
        explicit SubMatrix(Matrix<_Scalar, _RowsAtCompile, _ColsAtCompile, _StorageType>& _matrix, int _i, int _j, int _rows, int _cols) :
        m_matrix(_matrix), m_i(_i), m_j(_j), m_rows(_rows), m_cols(_cols)
        { 
            assert(_i < _matrix.rows() && _i >= 0);
            assert(_j < _matrix.cols() && _j >= 0);

            int max_rows = _matrix.rows() - _i;
            int max_cols = _matrix.cols() - _j;

            assert(_rows <= max_rows && _cols <= max_cols);
        }

        /** Constructeur de copie */
        SubMatrix(const SubMatrix& other) : m_matrix(other.m_matrix), m_i(other.m_i), m_j(other.m_j), m_rows(other.m_rows), m_cols(other.m_cols)
        {
        }

        /** Destructeur */
        ~SubMatrix() {}

        /** Opérateur de copie (à partir d'une matrice) */
        template <typename _OtherScalar, int _OtherRows, int _OtherCols, int _OtherStorage>
        SubMatrix& operator=(const Matrix<_OtherScalar, _OtherRows, _OtherCols, _OtherStorage>& matrix)
        {
            /**
             * DONE: Copier les valeurs de la matrice dans la sous-matrice.
             *
             * Note :
             * Les dimensions de la matrice doivent correspondre à celle de la sous-matrice.
             */
            assert(matrix.rows() == m_rows && matrix.cols() == m_cols);

            for (int i = 0; i < m_rows; ++i)
            {
                for (int j = 0; j < m_cols; ++j)
                {
                    (*this)(i, j) = matrix(i, j);
                }
            }

            return *this;
        }

        /**
        * NOUVEAU
        * rend le code plus concis lors l'affectation d'un vector à une colonne d'une matrice
        */
        template<typename _Scalar>
        SubMatrix& operator=(const Vector<_Scalar, 3> v)
        {
            assert(m_cols == 1 && m_rows == 3);
            (*this)(0, 0) = v(0);
            (*this)(1, 0) = v(1);
            (*this)(2, 0) = v(2);

            return *this;
        }

        /**
        * NOUVEAU
        * rend le code plus concis lors de l'initialisation d'un vector à partir d'une colonne d'une matrice
        */   
        template<typename _Scalar>
        Vector<_Scalar, 3> getVector()
        {
            assert(m_cols == 1 && m_rows == 3);
            Vector<_Scalar, 3> v;
            v(0) = (*this)(0, 0);
            v(1) = (*this)(1, 0);
            v(2) = (*this)(2, 0);

            return v;
        }

        /** Accesseur aux entrées de la sous-matrice (lecture seule) */
        _Scalar operator()(int i, int j) const
        {
            /**
             * DONE: Implémenter.
             * 
             * Note :
             * Il faut s'assurer que les indices respectent la taille de la sous-matrice.
             * */
            assert(i >= 0 && j >= 0 && i < m_rows&& j < m_cols);
            return m_matrix(i + m_i, j + m_j);
        }

        /** Accesseur aux entrées de la sous-matrice (lecture et écriture) */
        _Scalar& operator()(int i, int j)
        {
            /**
             * DONE: Implémenter.
             * 
             * Note :
             * Il faut s'assurer que les indices respectent la taille de la sous-matrice.
             * */
            assert(i >= 0 && j >= 0 && i < m_rows&& j < m_cols);
            return m_matrix(i + m_i, j + m_j);
        }

        /** Retourne la transposée de la sous-matrice sous la forme d'une matrice. */
        template <typename _OtherScalar, int _OtherRows, int _OtherCols, int _OtherStorage>
        Matrix<_OtherScalar, _OtherRows, _OtherCols, _OtherStorage> transpose() const
        {
            /** DONE: Implémenter. */
            Matrix<_OtherScalar, _OtherRows, _OtherCols, _OtherStorage> transposed(m_cols, m_rows);
            transposed.setZero();

            for (int j = 0; j < m_cols; ++j)
            {
                for (int i = 0; i < m_rows; ++i)
                {
                    transposed(j, i) = (*this)(i, j);
                }
            }

            return transposed;
        }

        inline int rows() const { return m_rows; }
        inline int cols() const { return m_cols; }
    };
#pragma endregion // SubMatrix
}
