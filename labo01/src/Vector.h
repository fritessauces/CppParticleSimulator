#pragma once

/**
 * @file Vector.h
 *
 * @brief Implémentation de vecteurs simples
 *
 * Nom: Frederic Labelle
 * Code permanent : LABF11109803
 * Email : frederic.labelle.3@ens.etsmtl.ca
 *
 */

#include "MatrixBase.h"

namespace gti320
{

	/**
	 * Classe vecteur générique.
	 *
	 * Cette classe réutilise la classe `MatrixBase` et ses spécialisations de
	 * templates pour les manipulation bas niveau.
	 */
	template <typename _Scalar = double, int _Rows = Dynamic>
	class Vector : public MatrixBase<_Scalar, _Rows, 1>
	{
	public:
		/** Constructeur par défaut */
		Vector() : MatrixBase<_Scalar, _Rows, 1>() {}

		/** Contructeur à partir d'un taille (rows). */
		explicit Vector(int rows) : MatrixBase<_Scalar, _Rows, 1>(rows, 1) {}

		/** Constructeur de copie */
		Vector(const Vector& other) : MatrixBase<_Scalar, _Rows, 1>(other) {}

		/** Destructeur */
		~Vector() {}

		/** Opérateur de copie */
		Vector& operator=(const Vector& other)
		{
			/** DONE: Implémenter. */
			if (this != &other)
			{
				resize(other.size());
				m_storage = other.m_storage;
			}
			return *this;
		}

		/** Accesseur à une entrée du vecteur (lecture seule) */
		_Scalar operator()(int i) const
		{
			/** DONE: Implémenter. */
			assert(i >= 0 && i < this->size());
			return this->data()[i];
		}

		/** Accesseur à une entrée du vecteur (lecture et écriture) */
		_Scalar& operator()(int i)
		{
			/** DONE: Implémenter. */
			assert(i >= 0 && i < this->size());
			return this->m_storage.data()[i];
		}

		/** Modifie le nombre de lignes du vecteur */
		void resize(int _rows)
		{
			MatrixBase<_Scalar, _Rows, 1>::resize(_rows, 1);
		}

		/** Produit scalaire de *this et other */
		inline _Scalar dot(const Vector& other) const
		{
			/** DONE: Implémenter. */
			assert(this->size() == other.size()); // both vectors sizes have to be the same

			_Scalar dot = 0.0;
			for (int i = 0; i < this->size(); ++i)
				dot += (*this)(i) * other(i);

			return dot;
		}

		/** Retourne la norme euclidienne du vecteur */
		inline _Scalar norm() const
		{
			/** DONE: Implémenter. */
			_Scalar norm = 0.0;

			for (int i = 0; i < this->size(); ++i)
				norm += (*this)(i) * (*this)(i);

			return sqrt(norm);
		}
	};
}
