#pragma once

/**
 * @file Operators.h
 *
 * @brief Implémentation de divers opérateurs arithmétiques pour les matrices et les vecteurs.
 *
 * Nom: Frederic Labelle
 * Code permanent : LABF11109803
 * Email : frederic.labelle.3@ens.etsmtl.ca
 *
 */

#include "Matrix.h"
#include "Vector.h"

namespace gti320
{
#pragma region // Multiplications
    /** Multiplication : Matrice * Matrice (générique) */
    template <typename _Scalar, int RowsA, int ColsA, int StorageA, int RowsB, int ColsB, int StorageB>
    Matrix<_Scalar, RowsA, ColsB> operator*(const Matrix<_Scalar, RowsA, ColsA, StorageA>& A, const Matrix<_Scalar, RowsB, ColsB, StorageB>& B)
    {
        /** DONE: optimiser. */
        // col*col  optimizer
        int n_rows_A = A.rows();
        int n_cols_A = A.cols();
        int n_rows_B = B.rows();
        int n_cols_B = B.cols();

        assert(n_cols_A == n_rows_B);

        Matrix<_Scalar, RowsA, ColsB> mMult(n_rows_A, n_cols_B);
        mMult.setZero();

        for (int rowB = 0; rowB < n_rows_B; ++rowB)
        {
            for (int colB = 0; colB < n_cols_B; ++colB)
            {
                for (int rowA = 0; rowA < n_rows_A; ++rowA)
                {
                    mMult(rowA, colB) += A(rowA, rowB) * B(rowB, colB);
                }
            }
        }

        return mMult;
    }

    /**
     * Multiplication : Matrice (colonne) * Matrice (ligne)
     *
     * Spécialisation de l'opérateur de multiplication pour le cas où les matrices
     * ont un stockage à taille dynamique et où la matrice de gauche utilise un
     * stockage par colonnes et celle de droite un stockage par lignes.
     */
    template <typename _Scalar>
    Matrix<_Scalar, Dynamic, Dynamic> operator*(const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, RowStorage>& B)
    {
        /** DONE: optimiser. */
        int n_rows_A = A.rows();
        int n_cols_A = A.cols();
        int n_rows_B = B.rows();
        int n_cols_B = B.cols();

        assert(n_cols_A == n_rows_B);

        Matrix<_Scalar, Dynamic, Dynamic> mMult(n_rows_A, n_cols_B);
        mMult.setZero();

        for (int rowB = 0; rowB < n_rows_B; ++rowB)
        {
            for (int colB = 0; colB < n_cols_B; ++colB)
            {
                for (int rowA = 0; rowA < n_rows_A; ++rowA)
                {
                    mMult(rowA, colB) += A(rowA, rowB) * B(rowB, colB);
                }
            }
        }

        return mMult;
    }

    /**
     * Multiplication : Matrice (ligne) * Matrice (colonne)
     *
     * Spécialisation de l'opérateur de multiplication pour le cas où les matrices
     * ont un stockage à taille dynamique et où la matrice de gauche utilise un
     * stockage par lignes et celle de droite un stockage par colonnes.
     */
    template <typename _Scalar>
    Matrix<_Scalar, Dynamic, Dynamic> operator*(const Matrix<_Scalar, Dynamic, Dynamic, RowStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& B)
    {
        /** DONE: optimiser. */
        int n_rows_A = A.rows();
        int n_cols_A = A.cols();
        int n_rows_B = B.rows();
        int n_cols_B = B.cols();

        assert(n_cols_A == n_rows_B);

        Matrix<_Scalar, Dynamic, Dynamic> mMult(n_rows_A, n_cols_B);
        mMult.setZero();

        for (int colB = 0; colB < n_cols_B; ++colB)
        {
            for (int rowA = 0; rowA < n_rows_A; ++rowA)
            {
                for (int rowB = 0; rowB < n_rows_B; ++rowB)
                {
                    mMult(rowA, colB) += A(rowA, rowB) * B(rowB, colB);
                }
            }
        }

        return mMult;
    }

    /**
    * Multiplication  : Scalaire * Matrice (colonne)
    *
    * Spécialisation de l'opérateur de multiplication par un scalaire pour le
    * cas d'une matrice stockée par colonnes.
    */
    template <typename _Scalar, int _Rows, int _Cols>
    Matrix<_Scalar, _Rows, _Cols, ColumnStorage> operator*(const _Scalar& a, const Matrix<_Scalar, _Rows, _Cols, ColumnStorage>& A)
    {
        /** DONE: Implémenter. */
        int n_rows = A.rows();
        int n_cols = A.cols();

        Matrix<_Scalar, _Rows, _Cols> mMult(n_rows, n_cols);

        for (int j = 0; j < n_cols; ++j)
        {
            for (int i = 0; i < n_rows; ++i)
            {
                mMult(i, j) = A(i, j) * a;
            }
        }

        return mMult;
    }

    /**
     * Multiplication  : Scalaire * Matrice (ligne)
     *
     * Spécialisation de l'opérateur de multiplication par un scalaire pour le
     * cas d'une matrice stockée par lignes.
     */
    template <typename _Scalar, int _Rows, int _Cols>
    Matrix<_Scalar, _Rows, _Cols, RowStorage> operator*(const _Scalar& a, const Matrix<_Scalar, _Rows, _Cols, RowStorage>& A)
    {
        /** DONE: Implémenter. */
        int n_rows = A.rows();
        int n_cols = A.cols();

        Matrix<_Scalar, _Rows, _Cols, RowStorage> mMult(n_rows, n_cols);

        for (int i = 0; i < n_rows; ++i)
        {
            for (int j = 0; j < n_cols; ++j)
            {
                mMult(i, j) = A(i, j) * a;
            }
        }

        return mMult;
    }

    /**
     * Multiplication : Matrice (ligne) * Vecteur
     *
     * Spécialisation de l'opérateur de multiplication matrice*vecteur pour le
     * cas où la matrice est représentée par lignes.
     */
    template <typename _Scalar, int _Rows, int _Cols>
    Vector<_Scalar, _Rows> operator*(const Matrix<_Scalar, _Rows, _Cols, RowStorage>& A, const Vector<_Scalar, _Cols>& v)
    {
        /** DONE : Implémenter. */
        assert(A.cols() == v.rows());
        Vector<_Scalar, _Rows> vMult(v.rows());
        vMult.setZero();
        for (int i = 0; i < A.rows(); ++i)
        {
            for (int j = 0; j < A.cols(); ++j)
            {
                vMult(i) += A(i, j) * v(j);
            }
        }

        return vMult;
    }

    /**
     * Multiplication : Matrice (colonne) * Vecteur
     *
     * Spécialisation de l'opérateur de multiplication matrice*vecteur pour le
     * cas où la matrice est représentée par colonnes.
     */
    template <typename _Scalar, int _Rows, int _Cols>
    Vector<_Scalar, _Rows> operator*(const Matrix<_Scalar, _Rows, _Cols, ColumnStorage>& A, const Vector<_Scalar, _Cols>& v)
    {
        assert(A.cols() == v.rows());
        Vector<_Scalar, _Rows> vMult(v.rows());
        vMult.setZero();

        for (int j = 0; j < A.cols(); ++j)
        {
            for (int i = 0; i < A.rows(); ++i)
            {
                vMult(i) += A(i, j) * v(j);
            }
        }

        return vMult;
    }

    /** Multiplication : Scalaire * Vecteur */
    template <typename _Scalar, int _Rows>
    Vector<_Scalar, _Rows> operator*(const _Scalar& a, const Vector<_Scalar, _Rows>& v)
    {
        /** DONE: Implémenter. */
        int n_rows = v.rows();
        Vector<_Scalar, _Rows> vMult(n_rows);

        for (int i = 0; i < n_rows; ++i)
        {
            vMult(i) = v(i) * a;
        }

        return vMult;
    }
#pragma endregion // Multiplications

#pragma region // Sums
    /** Addition : Matrice + Matrice (générique) */
    template <typename _Scalar, int Rows, int Cols, int StorageA, int StorageB>
    Matrix<_Scalar, Rows, Cols> operator+(const Matrix<_Scalar, Rows, Cols, StorageA>& A, const Matrix<_Scalar, Rows, Cols, StorageB>& B)
    {
        /** DONE: Implémenter. */
        assert(A.rows() == B.rows() && A.cols() == B.cols());

        int n_rows = A.rows();
        int n_cols = A.cols();

        Matrix<_Scalar, Rows, Cols> mSum(n_rows, n_cols);

        for (int i = 0; i < n_rows; ++i)
        {
            for (int j = 0; j < n_cols; ++j)
            {
                mSum(i, j) = A(i, j) + B(i, j);
            }
        }

        return mSum;
    }

    /**
     * Addition : Matrice (colonne) + Matrice (colonne)
     *
     * Spécialisation de l'opérateur d'addition pour le cas où les deux matrices
     * sont stockées par colonnes.
     */
    template <typename _Scalar>
    Matrix<_Scalar, Dynamic, Dynamic> operator+(const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& B)
    {
        /** DONE: Implémenter. */
        assert(A.rows() == B.rows() && A.cols() == B.cols());

        int n_rows = A.rows();
        int n_cols = A.cols();

        Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage> mSum(n_rows, n_cols);

        for (int j = 0; j < n_cols; ++j)
        {
            for (int i = 0; i < n_rows; ++i)
            {
                mSum(i, j) = A(i, j) + B(i,j);
            }
        }

        return mSum;
    }

    /**
     * Addition : Matrice (ligne) + Matrice (ligne)
     *
     * Spécialisation de l'opérateur d'addition pour le cas où les deux matrices
     * sont stockées par lignes.
     */
    template <typename _Scalar>
    Matrix<_Scalar, Dynamic, Dynamic, RowStorage> operator+(const Matrix<_Scalar, Dynamic, Dynamic, RowStorage>& A,
        const Matrix<_Scalar, Dynamic, Dynamic, RowStorage>& B)
    {
        /** DONE: Implémenter. */
        assert(A.rows() == B.rows() && A.cols() == B.cols());

        int n_rows = A.rows();
        int n_cols = A.cols();

        Matrix<_Scalar, Dynamic, Dynamic, RowStorage> mSum(n_rows, n_cols);

        for (int i = 0; i < n_rows; ++i)
        {
            for (int j = 0; j < n_cols; ++j)
            {
                mSum(i, j) = A(i, j) + B(i, j);
            }
        }

        return mSum;
    }

    /** Addition : Vecteur + Vecteur */
    template <typename _Scalar, int _RowsA, int _RowsB>
    Vector<_Scalar, _RowsA> operator+(const Vector<_Scalar, _RowsA>& a, const Vector<_Scalar, _RowsB>& b)
    {
        /** DONE: Implémenter. */
        assert(a.rows() == b.rows());

        int n_rows = a.rows();

        Vector<_Scalar, _RowsA> vSum(n_rows);

        for (int i = 0; i < n_rows; ++i)
        {
            vSum(i) = a(i) + b(i);
        }

        return vSum;
    }

    /** Soustraction : Vecteur - Vecteur */
    template <typename _Scalar, int _RowsA, int _RowsB>
    Vector<_Scalar, _RowsA> operator-(const Vector<_Scalar, _RowsA>& a, const Vector<_Scalar, _RowsB>& b)
    {
        /** DONE: Implémenter. */
        assert(a.rows() == b.rows());

        int n_rows = a.rows();

        Vector<_Scalar, _RowsA> vMinus(n_rows);

        for (int i = 0; i < n_rows; ++i)
        {
            vMinus(i) = a(i) - b(i);
        }

        return vMinus;
    }

#pragma endregion // Sums

}
