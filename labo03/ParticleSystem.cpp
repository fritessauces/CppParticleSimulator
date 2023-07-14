/**
 * @file ParticleSystem.cpp
 *
 * @brief GTI320 Labo 3 - Simulation d'un système masse-ressort
 *
 * Nom: Frederic Labelle
 * Code permanent : LABF11109803
 * Email : frederic.labelle.3@ens.etsmtl.ca
 *
 */

#include "ParticleSystem.h"

using namespace gti320;

/**
 * Calcule des forces qui affectent chacune des particules.
 *
 * Étant donné une particule p, la force est stockée dans le vecteur p.f
 * Les forces prisent en compte sont : la gravité et la force des ressorts.
 */
void ParticleSystem::computeForces()
{

	// DONE 
	//
	// Calcul de la force gravitationnelle sur chacune des particules
	for (Particle& p : m_particles)
	{
		if (!p.fixed)
			p.f(1) = p.m * gti320::g;

		p.f(0) = 0.0;
	}

	// DONE
	//
	// Pour chaque ressort, ajouter la force exercée à chacune des exptrémités sur
	// les particules appropriées. Pour un ressort s, les deux particules
	// auxquelles le ressort est attaché sont m_particles[s.index0] et
	// m_particles[s.index1]. On rappelle que les deux forces sont de même
	// magnitude mais dans des directions opposées.

	double l;
	Vector2d f1;
	for (const Spring& s : m_springs)
	{
		l = (m_particles[s.index1].x - m_particles[s.index0].x).norm();
		f1 = s.k * (1 - (s.l0 / l)) * (m_particles[s.index1].x - m_particles[s.index0].x);

		m_particles[s.index0].f = m_particles[s.index0].f + f1;
		m_particles[s.index1].f = m_particles[s.index1].f - f1;
	}
}

/**
 * Assemble les données du système dans les vecteurs trois vecteurs d'état _pos,
 * _vel et _force.
 */
void ParticleSystem::pack(Vector<double, Dynamic>& _pos,
	Vector<double, Dynamic>& _vel,
	Vector<double, Dynamic>& _force)
{
	// DONE 
	//
	// Copier les données des particules dans les vecteurs. Attention, si on a
	// changé de modèle, il est possible que les vecteurs n'aient pas la bonne
	// taille. Rappel : la taille de ces vecteurs doit être 2 fois le nombre de
	// particules.

	_pos.resize(2 * m_particles.size());
	_vel.resize(2 * m_particles.size());
	_force.resize(2 * m_particles.size());
	for (int i = 0, j = 0; i < m_particles.size(); ++i, j += 2)
	{
		_pos(j) = m_particles.at(i).x(0);
		_pos(j + 1) = m_particles.at(i).x(1);

		_vel(j) = m_particles.at(i).v(0);
		_vel(j + 1) = m_particles.at(i).v(1);

		_force(j) = m_particles.at(i).f(0);
		_force(j + 1) = m_particles.at(i).f(1);
	}


}

/**
 * Copie les données des vecteurs d'états dans le particules du système.
 */
void ParticleSystem::unpack(const Vector<double, Dynamic>& _pos,
	const Vector<double, Dynamic>& _vel)
{
	// DONE 
	//
	// Mise à jour des propriétés de chacune des particules à partir des valeurs
	// contenues dans le vecteur d'état.

	for (int i = 0, j = 0; i < m_particles.size(); ++i, j += 2)
	{
		m_particles.at(i).x(0) = _pos(j);
		m_particles.at(i).x(1) = _pos(j + 1);

		m_particles.at(i).v(0) = _vel(j);
		m_particles.at(i).v(1) = _vel(j + 1);
	}

}



/**
 * Construction de la matirce de masses.
 */
void ParticleSystem::buildMassMatrix(Matrix<double, Dynamic, Dynamic>& M)
{
	const int numParticles = m_particles.size();
	const int dim = 2 * numParticles;
	M.resize(dim, dim);
	M.setZero();

	// DONE 
	//
	// Inscrire la masse de chacune des particules dans la matrice de masses M
	// "n, pour un système à n particules, la matrice est de dimension 2n*2n"
	// Si fixed-> masse infinie
	double masse;
	for (int i = 0, j = 0; i < numParticles; ++i, j += 2)
	{
		masse = m_particles.at(i).fixed ? DBL_MAX : m_particles.at(i).m;

		M(j, j) = masse;
		M(j + 1, j + 1) = masse;
	}
}


/**
 * Construction de la matrice de rigidité.
 */
void ParticleSystem::buildDfDx(Matrix<double, Dynamic, Dynamic>& dfdx)
{

	const int numParticles = m_particles.size();
	const int numSprings = m_springs.size();
	const int dim = 2 * numParticles;
	dfdx.resize(dim, dim);
	dfdx.setZero();

	// Pour chaque ressort...
	double alpha;
	double l;
	for (const Spring& spring : m_springs)
	{
		// DONE
		//
		// Calculer le coefficients alpha et le produit dyadique tel que décrit au cours.
		// Utiliser les indices spring.index0 et spring.index1 pour calculer les coordonnées des endroits affectés.
		//
		// Astuce: créer une matrice de taille fixe 2 par 2 puis utiliser la classe SubMatrix pour accumuler 
		// les modifications sur la diagonale (2 endroits) et pour mettre à jour les blocs non diagonale (2 endroits).
		double l = (m_particles[spring.index1].x - m_particles[spring.index0].x).norm();
		double alpha = spring.k * (1 - (spring.l0 / l));

		auto xi = m_particles[spring.index0].x;
		auto xj = m_particles[spring.index1].x;
		auto xjMinusxi = xj - xi;

		const Matrix<double, 2, 2, ColumnStorage> prodDyadique = xjMinusxi.dyadicProduct();

		// Alpha Matrix
		Matrix<double, 2, 2, ColumnStorage> alphaM(2, 2);
		alphaM.setIdentity();
		Matrix<double, 2, 2, ColumnStorage> n_alphaM(2, 2);
		alphaM.setIdentity();
		n_alphaM.setIdentity();

		n_alphaM(0, 0) = -alpha;
		n_alphaM(1, 1) = -alpha;

		alphaM(0, 0) = alpha;
		alphaM(1, 1) = alpha;


		// little dfdx diago vs not diago
		Matrix<double, 2, 2, ColumnStorage> littleDfdx(2, 2);
		Matrix<double, 2, 2, ColumnStorage> n_littleDfdx(2, 2);

		n_littleDfdx = n_alphaM + (-spring.k * (spring.l0 / xjMinusxi.norm()) * prodDyadique);
		littleDfdx = alphaM + (spring.k * (spring.l0 / xjMinusxi.norm()) * prodDyadique);

		dfdx.block(2 * spring.index0, 2 * spring.index0, 2, 2) = n_alphaM;
		dfdx.block(2 * spring.index0, 2 * spring.index1, 2, 2) = alphaM;
		dfdx.block(2 * spring.index1, 2 * spring.index0, 2, 2) = alphaM;
		dfdx.block(2 * spring.index1, 2 * spring.index1, 2, 2) = n_alphaM;
	}
}