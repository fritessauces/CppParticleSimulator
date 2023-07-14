#pragma once

/**
 * @file Vector2d.h
 *
 * @brief Vecteur 2D
 *
 *: Frederic Labelle
 * Code permanent : LABF11109803
 * Email : frederic.labelle.3@ens.etsmtl.ca
 *
 */

#include "Math3D.h"

namespace gti320
{
    typedef Vector<double, 2> Vector2d;

  /**
   * Spécialisation de template pour le les vecteurs 2D
   */
  template<typename _Scalar>
    class Vector<_Scalar, 2> : public MatrixBase<_Scalar, 2, 1>
      {
    public:

      /**
       * Constructeur par défaut
       */
      Vector() : MatrixBase<_Scalar, 2, 1>() { }

      /**
       * Constructeur à partir de la dimension. Afin d'assurer la compatibilité
       * avec la version non-spécialisée de la classe.
       *
       * @param _size doit être 2
       */
      Vector(int _size) : MatrixBase<_Scalar, 2, 1>() 
        { 
          assert(_size == 2);
        }

      /**
       * Constructeur à partir des coordonnées x et y
       */
      Vector(_Scalar x, _Scalar y) : MatrixBase<_Scalar, 2, 1>()
        {
          (*this)(0) = x;
          (*this)(1) = y;
        }

      /**
       * Constructeur de copie
       */
      Vector(const Vector& other) : MatrixBase<_Scalar, 2, 1>(other) { }

      /**
       * Destructeur
       */
      ~Vector() { }

      /**
       * Opérateur de copie
       */
      Vector& operator=(const Vector& other)
        {
          this->m_storage = other.m_storage;
          return *this;
        }

      /**
       * Accesseur à une entrée du vecteur (lecture seule)
       */
      _Scalar operator()(int i) const
        {
          assert(0 <= i && i < 2);
          return *(this->m_storage.data() + i);
        }

      /**
       * Accesseur à une entrée du vecteur (lecture et écriture)
       */
      _Scalar& operator()(int i)
        {
          assert(0 <= i && i < 2);
          return *(this->m_storage.data() + i);
        }


      /**
       * Produit scalaire de *this et other
       */
      inline _Scalar dot(const Vector& other) const
        {
          return (*this)(0)*other(0) + (*this)(1)*other(1);
        }

      /**
       * Norme eulidienne du vecteur
       */
      inline _Scalar norm() const
        {
          return sqrt(dot(*this));
        }
     

    Matrix<double, 2, 2, ColumnStorage> dyadicProduct()
    {
        Matrix<double, 2, 2, ColumnStorage> dyadic(2, 2);
		double sqrdNorm = (*this).norm() * (*this).norm();
        dyadic(0, 0) = (*this)(0) * (*this)(0);
        dyadic(0, 1) = (*this)(0) * (*this)(1);
        dyadic(1, 0) = (*this)(1) * (*this)(0);
        dyadic(1, 1) = (*this)(1) * (*this)(1);
        return (1 / sqrdNorm) * dyadic;
    }

	};


}
