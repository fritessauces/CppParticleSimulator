#pragma once

/**
 * @file ParticleSystem.h
 *
 * @brief Système de particules de type masse-ressort
 *
 * Nom: Frederic Labelle
 * Code permanent : LABF11109803
 * Email : frederic.labelle.3@ens.etsmtl.ca
 *
 */

#include "Math3D.h"
#include "Vector2d.h"
#include <vector>

namespace gti320
{
    const static double g = -9.81;
  /**
   * Classe particule 2D.
   */
  class Particle
    {
  public:
    bool fixed;     // indique si la particule est stationnaire (impossible à bouger)
    double m;       // masse
    Vector2d x;     // position
    Vector2d v;     // vélocité
    Vector2d f;     // force

    Particle(const Vector2d& _x, const Vector2d& _v, const Vector2d& _f, double _m) 
      : fixed(false), m(_m), x(_x), v(_v), f(_f) 
      { }

    Particle() : fixed(false), m(1.0), x(0,0), v(0,0), f(0,0) 
      { }
    };

  /**
   * Classe ressort 2d.
   *
   * Un ressort relie toujours deux particules appelées particule 0 et particule
   * 1. Ces particules sont identifiées par leur indice dans le tableau de
   * particules du système de particule.
   */
  class Spring
    {
  public:
    int index0;     // indice de la particule 0
    int index1;     // indice de la particule 1
    double k;       // rigidité
    double l0;      // longueur au repos

    Spring(int _index0, int _index1, double _k, double _l0) 
      : index0(_index0), index1(_index1), k(_k), l0(_l0)  
      { }
    };

  /**
   * Classe représentant un système de particule masse-ressort 2D.
   */
  class ParticleSystem
    {

  private:
    std::vector<Particle> m_particles;      // les particules
    std::vector<Spring> m_springs;          // les ressorts

  public:
    ParticleSystem() : m_particles(), m_springs()  { }



    /**
     * Supprime toutes les particules et tous les ressorts
     */
    void clear()
      {
        m_particles.clear();
        m_springs.clear();
      }

    /**
     * Ajoute une particule au système. La particule est copiée dans le tableau
     * m_particles.
     */
    void addParticle(const Particle& _particle)
      {
        m_particles.push_back(_particle);
      }

    /**
     * Ajoute un ressort au système. Le ressort est copié dans le tableau
     * m_springs.
     */
    void addSpring(const Spring& _spring) { m_springs.push_back(_spring); }

    /**
     * Calcul des forces exercées sur chacune des particules.
     */
    void computeForces();

    /**
     * Accesseurs pour les particules et les ressorts
     */
    const std::vector<Particle>& getParticles() const
      {
        return m_particles;
      }

    std::vector<Particle>& getParticles()
      {
        return m_particles;
      }

    const std::vector<Spring>& getSprings() const
      {
        return m_springs;
      }

    std::vector<Spring>& getSprings()
      {
        return m_springs;
      }

    /**
     * Assemble les vecteurs d'états.
     */
    void pack(Vector<double, Dynamic>& _pos, 
              Vector<double, Dynamic>& _vel, 
              Vector<double, Dynamic>& _force);

    /**
     * Mise à jour de la position et de la vitesse de chacune des particules à
     * partir des vecteurs d'état.
     */
    void unpack(const Vector<double, Dynamic>& _pos, 
                const Vector<double, Dynamic>& _vel);

    /**
     * Contruit la matrice de masse.
     */
    void buildMassMatrix(Matrix<double, Dynamic, Dynamic>& M);

    /**
     * Construit la matrice df/dx
     */
    void buildDfDx(Matrix<double, Dynamic, Dynamic>& dfdx);

    };
}
