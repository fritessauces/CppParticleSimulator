#pragma once

/**
 * @file Math3D.h
 *
 * @brief Fonctions pour l'intinialisation et la manipulation de matrices de
 * rotation, de matrices de transformations en coordonnées homogènes et de
 * vecteurs 3D.
 *
 * Nom: Frederic Labelle
 * Code permanent : LABF11109803
 * Email : frederic.labelle.3@ens.etsmtl.ca:
 *
 */

#include "Matrix.h"
#include "Vector.h"
#include "Operators.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>

namespace gti320
{

	// Deux types de vecteurs 3D considérés ici
	typedef Vector<double, 3> Vector3d;
	typedef Vector<float, 3> Vector3f;

	// Dans le cadre de ce projet, nous considérons seulement deux
	// cas :
	//
	//  - les rotations
	//  - les translations
	//
	// Deux types de matrices en coordonnées homogèes :
	typedef Matrix<double, 4, 4, ColumnStorage> Matrix4d;
	typedef Matrix<float, 4, 4, ColumnStorage> Matrix4f;
	//
	// Deux types de matrices pour les rotations
	typedef Matrix<double, 3, 3, ColumnStorage> Matrix3d;
	typedef Matrix<float, 3, 3, ColumnStorage> Matrix3f;

	/**
	* NOUVEAU
	* rend le code plus concis lors de la création d'un vector3d (pas besoin de caller les 3 operator(), le tout se fait ici)
	*/
	template <typename _Scalar>
	Vector3d createVector3d(_Scalar firstElem, _Scalar secondElem, _Scalar thirdElem)
	{
		Vector3d v;
		v(0) = firstElem;
		v(1) = secondElem;
		v(2) = thirdElem;
		return v;
	}

	/** Initialise et retourne la matrice identité */
	template <>
	inline void Matrix4d::setIdentity()
	{
		/** DONE: Redéfinir la fonction membre de façon optimisée pour les Matrix4d */
		// 4x4, diagonal: (0,0), (1,1), (2,2), (3,3)
		(*this)(0, 0) = 1.0;
		(*this)(0, 1) = 0.0;
		(*this)(0, 2) = 0.0;
		(*this)(0, 3) = 0.0;

		(*this)(1, 0) = 0.0;
		(*this)(1, 1) = 1.0;
		(*this)(1, 2) = 0.0;
		(*this)(1, 3) = 0.0;

		(*this)(2, 2) = 1.0;
		(*this)(2, 0) = 0.0;
		(*this)(2, 1) = 0.0;
		(*this)(2, 3) = 0.0;

		(*this)(3, 0) = 0.0;
		(*this)(3, 1) = 0.0;
		(*this)(3, 2) = 0.0;
		(*this)(3, 3) = 1.0;
	}

	/**
	 * Calcul de la matrice inverse SPÉCIALISÉ pour le cas
	 * d'une matrice de transformation en coordonnées homogènes.
	 */
	template <>
	inline Matrix4d Matrix4d::inverse() const
	{
		/** DONE: Implémenter. */
		Matrix4d inversed = Matrix4d();

		// Transpose the upper left 3x3 part
		Matrix3d rotation;
		rotation = (*this).block(0, 0, 3, 3);
		rotation = rotation.transpose<double, 3, 3, ColumnStorage>();

		// Transpose the last column and multiply by factor of -1*Rotation
		Vector<double, 3> lastCol(3);
		lastCol(0) = (*this)(0, 3);
		lastCol(1) = (*this)(1, 3);
		lastCol(2) = (*this)(2, 3);
		lastCol = -1.0 * rotation * lastCol;

		inversed.block(0, 0, 3, 3) = rotation; // put rotation matrix in the upper left 3x3
		inversed(0, 3) = lastCol(0);
		inversed(1, 3) = lastCol(1);
		inversed(2, 3) = lastCol(2);

		inversed(3, 0) = 0.0;
		inversed(3, 1) = 0.0;
		inversed(3, 2) = 0.0;
		inversed(3, 3) = 1.0;

		return inversed;
	}

	/**
	 * Calcul de la matrice inverse SPÉCIALISÉ pour le cas
	 * d'une matrice de rotation.
	 */
	template <>
	inline Matrix3d Matrix3d::inverse() const
	{
		/** DONE: Implémenter. */
		// Inverse of rotation:  R^-1 = R^T.
		// We just need to get the transpose

		return (*this).transpose<double, 3, 3, ColumnStorage>();
	}

	/**
	 * Multiplication d'une matrice 4x4 avec un vecteur 3D où la matrice
	 * représente une transformation en coordonnées homogène.
	 */
	template <typename _Scalar>
	Vector<_Scalar, 3> operator*(const Matrix<_Scalar, 4, 4, ColumnStorage>& A, const Vector<_Scalar, 3>& v)
	{
		/** DONE: Implémenter. */
		Vector<_Scalar, 3> mult(3);

		// Get the 3x3 part of the matrix
		Matrix3d matrix_3by3;
		matrix_3by3 = A.block(0, 0, 3, 3);

		mult = matrix_3by3 * v; // Consider only the upper left 3x3 part of the matrix
								// We don't want the last row (we do want the last column
								// so we are going to add it up by hand after)

	   // Add the missing last column
		mult(0) += A(0, 3);
		mult(1) += A(1, 3);
		mult(2) += A(2, 3);

		return mult;
	}

	/**
	 * Créer et retourne une matrice de rotation définie par les angles
	 * d'Euler XYZ exprimés en radians.
	 *
	 * La matrice doit correspondre au produit : Rz * Ry * Rx.
	 */
	template <typename _Scalar>
	static Matrix<_Scalar, 3, 3> makeRotation(_Scalar x, _Scalar y, _Scalar z)
	{
		/** DONE: Implémenter. */
		Matrix<_Scalar, 3, 3> rotationMat(3, 3);

		// theta = x
		// phi = y
		// psi = z

		_Scalar cosPhi = cos(y);
		_Scalar cosTheta = cos(x);
		_Scalar cosPsi = cos(z);
		_Scalar sinPsi = sin(z);
		_Scalar sinPhi = sin(y);
		_Scalar sinTheta = sin(x);

		rotationMat(0, 0) = cosPhi * cosPsi;
		rotationMat(0, 1) = -cosTheta * sinPsi + sinTheta * sinPhi * cosPsi;
		rotationMat(0, 2) = sinTheta * sinPsi + cosTheta * sinPhi * cosPsi;
		rotationMat(1, 0) = cosPhi * sinPsi;
		rotationMat(1, 1) = cosTheta * cosPsi + sinTheta * sinPhi * sinPsi;
		rotationMat(1, 2) = -sinTheta * cosPsi + cosTheta * sinPhi * sinPsi;
		rotationMat(2, 0) = -sinPhi;
		rotationMat(2, 1) = sinTheta * cosPhi;
		rotationMat(2, 2) = cosTheta * cosPhi;

		return rotationMat;
	}

}
