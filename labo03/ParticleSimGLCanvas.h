#pragma once

/**
 * @file ParticleSimGLCanvas.h
 *
 * @brief Classe Canvas pour l'affichage via OpenGL.
 *
 * Nom: Frederic Labelle
 * Code permanent : LABF11109803
 * Email : frederic.labelle.3@ens.etsmtl.ca
 *
 */

#include <nanogui/window.h>
#include <nanogui/glcanvas.h>

#include "ParticleSimApplication.h"

/**
 * Classe canvas OpenGL pour afficher des points et des segments.
 */
class ParticleSimGLCanvas : public nanogui::GLCanvas
{
public:
  ParticleSimGLCanvas(ParticleSimApplication* _app);

  ~ParticleSimGLCanvas();

  virtual void drawGL() override;

  virtual bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers) override;

  virtual bool mouseDragEvent(const Eigen::Vector2i &p, const Eigen::Vector2i &rel, int button, int modifiers) override;

  void applyMouseSpring();

private:

  void convertAndStoreMousePos(const Eigen::Vector2i& mousePos);

  nanogui::GLShader m_particleShader;
  nanogui::GLShader m_springShader;

  ParticleSimApplication* m_app;

  gti320::Particle* m_selectedParticle;
  double m_mouseStiffness;
  gti320::Vector2d m_mousePos;
  gti320::Vector<double> m_circle;

};
