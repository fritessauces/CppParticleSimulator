/**
 * @file ParticleSimGLCanvas.cpp
 *
 * @brief GTI320 Labo 3 - Simulation d'un système masse-ressort
 *
 * Nom: Frederic Labelle
 * Code permanent : LABF11109803
 * Email : frederic.labelle.3@ens.etsmtl.ca
 *
 */
#include "ParticleSimGLCanvas.h"
#include "ParticleSystem.h"

using namespace nanogui;

namespace
{
  static const double r = 6.0;

  static inline gti320::Particle* pickParticle(std::vector<gti320::Particle>& particles, const gti320::Vector2d& mousePos)
    {
      for (gti320::Particle& particle : particles)
        {
          const double dist = (particle.x - mousePos).norm();
          if (dist <= r)
            {
              return &particle;
            }
        }

      return nullptr;
    }
}

ParticleSimGLCanvas::ParticleSimGLCanvas(ParticleSimApplication* _app) 
     : nanogui::GLCanvas(_app->getWindow()), m_app(_app), m_selectedParticle(nullptr) 
{

    // Un shader minimaliste pour afficher les particules
    m_particleShader.init(
       // Nom du shader
       "particle_shader",

       // Shader pour particules
       "#version 410\n"
       "uniform mat4 modelViewProj;\n"
       "uniform vec4 color;\n"
       "in vec2 position;\n"
       "void main() {\n"
       "    gl_Position = modelViewProj * vec4(position.xy, -1, 1);\n"
       "}",

       // Shader pour fragments
       "#version 410\n"
       "uniform vec4 color;\n"
       "out vec4 frag_color;\n"
       "void main() {\n"
       "    frag_color = color;\n"
       "}"
    );

    // Initialise la géométrie pour un cercle
    static const int numPoints = 8;
    m_circle.resize(2 * (numPoints+1));
    m_circle.setZero();
    for (int i = 0; i <= numPoints; ++i)
      {
        const double angle = 2.0 * M_PI * ((double)i / numPoints);
        m_circle(2 * i) = r*cos(angle);
        m_circle(2 * i + 1) = r*sin(angle);
      }

    m_particleShader.bind();
    const Matrix4f mvp = Matrix4f::Identity();
    m_particleShader.setUniform("modelViewProj", mvp);
    m_particleShader.setUniform("color", Eigen::Vector4f(0.0f, 0.0, 1.0f, 10.0f));
    m_particleShader.uploadAttrib("position", (uint32_t)m_circle.rows(), (int)2, sizeof(double),
                                  GL_DOUBLE, false, m_circle.storage().data());
}

ParticleSimGLCanvas::~ParticleSimGLCanvas() {
    m_particleShader.free();
}

void ParticleSimGLCanvas::drawGL()
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_DEPTH_TEST);

  m_particleShader.bind();
  m_particleShader.setUniform("color", Eigen::Vector4f(1.0f, 0.0, 0.0f, 1.0f));

  // Matrice de projection orthographique
  const Matrix4f projMat = nanogui::ortho(0, width()-1, 0, height()-1, 0.1f, 1.0f);
  const gti320::ParticleSystem& particleSystem = m_app->getParticleSystem();
  const auto particles = particleSystem.getParticles();
  const int numParticles = particles.size();

  // Affichage des ressorts
  const auto springs = particleSystem.getSprings();
  const int numSprings = springs.size();
  gti320::Vector<double, gti320::Dynamic> points(4 * numSprings);
  for (int i = 0; i < numSprings; ++i)
    {
      int index0 = springs[i].index0;
      int index1 = springs[i].index1;

      const gti320::Vector2d& pos0 = particles[index0].x;
      const gti320::Vector2d& pos1 = particles[index1].x;

      points(4 * i) = pos0(0);
      points(4 * i + 1) = pos0(1);
      points(4 * i + 2) = pos1(0);
      points(4 * i + 3) = pos1(1);
    }
  m_particleShader.setUniform("modelViewProj", projMat);
  m_particleShader.setUniform("color", Eigen::Vector4f(0.0f, 0.0, 1.0f, 1.0f));
  m_particleShader.uploadAttrib("position", (uint32_t)points.rows(), (int)2, 
                                sizeof(double), GL_DOUBLE, false, points.storage().data());
  m_particleShader.drawArray(GL_LINES, 0, points.rows() / 2);

  // Affichage des particules
  m_particleShader.setUniform("color", Eigen::Vector4f(1.0f, 0.0, 0.0f, 1.0f));
  m_particleShader.uploadAttrib("position", (uint32_t)m_circle.rows(), (int)2,
      sizeof(double), GL_DOUBLE, false, m_circle.storage().data());
  for (int i = 0; i < numParticles; ++i)
  {
      Matrix4f modelMat = Matrix4f::Identity();
      modelMat(0, 3) = particles[i].x(0);
      modelMat(1, 3) = particles[i].x(1);
      const Matrix4f mvp = projMat * modelMat;
      m_particleShader.setUniform("modelViewProj", mvp);

      if (particles[i].fixed)
          m_particleShader.setUniform("color", Eigen::Vector4f(0.6f, 0.6f, 1.0f, 1.0f));
      else
          m_particleShader.setUniform("color", Eigen::Vector4f(1.0f, 0.0, 0.0f, 1.0f));

      m_particleShader.drawArray(GL_TRIANGLE_FAN, 0, m_circle.rows() / 2);
  }

  // Affichage du ressort déféni par la souris
  if (m_selectedParticle)
    {
      const gti320::Vector2d p = m_selectedParticle->x;
      const double coords[4] = { m_mousePos(0), m_mousePos(1), p(0), p(1) };
      m_particleShader.setUniform("modelViewProj", projMat);
      m_particleShader.setUniform("color", Eigen::Vector4f(0.0f, 1.0, 0.0f, 1.0f));
      m_particleShader.uploadAttrib("position", (uint32_t)4, (int)2, sizeof(double), 
                                    GL_DOUBLE, false, coords);
      m_particleShader.drawArray(GL_LINES, 0, 2);
    }

}

bool ParticleSimGLCanvas::mouseButtonEvent(const Vector2i& p, int button, bool down, int modifiers)
{
  if ( modifiers == GLFW_MOD_SHIFT )
    {
      if (button == GLFW_MOUSE_BUTTON_1 && down)
        {
          convertAndStoreMousePos(p);
          m_selectedParticle = pickParticle(m_app->getParticleSystem().getParticles(), m_mousePos);
          if (m_selectedParticle != nullptr)
            {
              m_selectedParticle->fixed = !(m_selectedParticle->fixed);
            }
          return true;
        }
    }
  else
    {
      if (button == GLFW_MOUSE_BUTTON_1 && down)
        {
          convertAndStoreMousePos(p);
          m_selectedParticle = pickParticle(m_app->getParticleSystem().getParticles(), m_mousePos);
          return true;
        }
      else if (button == 0)
        {
          m_selectedParticle = nullptr;
          return true;
        }
    }

  return false;
}

bool ParticleSimGLCanvas::mouseDragEvent(const Vector2i& p, const Vector2i & rel, int button, int modifiers)
{
  if (button == GLFW_MOUSE_BUTTON_2 && modifiers == 0 && m_selectedParticle != nullptr )
    {
      convertAndStoreMousePos(p);
      return true;
    }
  return false;
}

void ParticleSimGLCanvas::convertAndStoreMousePos(const Vector2i & mousePos)
{
  const Vector2i& pos = position();
  const int y = height() - (mousePos.y() - pos.y()) - 1;
  m_mousePos(0) = (double)(mousePos.x() - pos.x());
  m_mousePos(1) = (double)y;
}

void ParticleSimGLCanvas::applyMouseSpring()
{
  if( m_selectedParticle != nullptr )
    {
      const double k = 20.0 * m_selectedParticle->m;
      const gti320::Vector2d f = k * (m_mousePos - m_selectedParticle->x);
      m_selectedParticle->f = m_selectedParticle->f + f;
    }
}
