/**
 * @file ParticleSimApplication.cpp
 *
 * @brief GTI320 Labo 3 - Simulation d'un système masse-ressort
 *
 * Nom: Frederic Labelle
 * Code permanent : LABF11109803
 * Email : frederic.labelle.3@ens.etsmtl.ca
 *
 */
#include "ParticleSimApplication.h"
#include "ParticleSimGLCanvas.h"

#include <nanogui/window.h>
#include <nanogui/formhelper.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/entypo.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <nanogui/opengl.h>
#include <nanogui/glutil.h>

#include <fstream>
#include <random>

#include "Solvers.hpp"

using namespace nanogui;
using namespace gti320;

namespace
{

	static const double DELTA_T = 0.01; // secondes

	/**
	 * Crée un système masse-ressort qui simule un tissu suspendu
	 */
	static inline void createHangingCloth(ParticleSystem& particleSystem, double k)
	{
		particleSystem.clear();

		const int N = 16;
		const int x_start = 240;
		const int y_start = 100;
		const int dx = 32;
		const int dy = 32;

		int index = 0;
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				const int x = x_start + j * dx;
				const int y = y_start + i * dy;

				Particle particle(Vector2d(x, y), Vector2d(0, 0), Vector2d(0, 0), 1.0);
				if (j == 0 && i == (N - 1)) particle.fixed = true;
				if (j == (N - 1) && i == (N - 1)) particle.fixed = true;
				particleSystem.addParticle(particle);

				if (i > 0)
				{
					Spring s(index - N, index, k, (double)dy);
					particleSystem.addSpring(s);
				}
				if (j > 0)
				{
					Spring s(index - 1, index, k, (double)dx);
					particleSystem.addSpring(s);
				}

				if (i > 0 && j > 0)
				{
					Spring s(index - N - 1, index, k, std::sqrt((double)dx * dx + (double)dy * dy));
					particleSystem.addSpring(s);
				}
				++index;
			}
		}

	}

	/**
	 * Crée un système masse-ressort qui simule une corde suspendu par ses
	 * extrémités.
	 */
	static inline void createHangingRope(ParticleSystem& particleSystem, double k)
	{
		particleSystem.clear();

		const int N = 20;
		const int x_start = 200;
		const int dx = 32;

		int index = 0;
		for (int j = 0; j < N; ++j)
		{
			const int x = x_start + j * dx;
			const int y = 480;

			Particle particle(Vector2d(x, y), Vector2d(0, 0), Vector2d(0, 0), 1.0);
			particle.fixed = (index == 0) || (index == N - 1);
			particleSystem.addParticle(particle);
			if (j > 0)
			{
				Spring s(index - 1, index, k, (double)dx);
				particleSystem.addSpring(s);
			}
			++index;
		}

	}

	/**
	 * Crée un système masse-ressort qui  simule une poutre flexible
	 */
	static inline void createBeam(ParticleSystem& particleSystem, double k)
	{
		particleSystem.clear();

		const int N = 20;
		const int x_start = 200;
		const int y_start = 400;
		const int dx = 32;
		const int dy = 32;

		int index = 0;
		for (int j = 0; j < N; ++j)
		{
			const int x = x_start + j * dx;

			// Bottom particle
			{
				Particle particle(Vector2d(x, y_start), Vector2d(0, 0), Vector2d(0, 0), 1.0);
				particle.fixed = (j == 0);
				particleSystem.addParticle(particle);
				if (j > 0)
				{
					Spring s(index - 1, index, k, (double)sqrt((double)dx * dx + (double)dy * dy));
					particleSystem.addSpring(s);
					Spring s2(index - 2, index, k, (double)dx);
					particleSystem.addSpring(s2);

				}
				++index;
			}


			// Top particle
			{
				Particle particle(Vector2d(x, y_start + dy), Vector2d(0, 0), Vector2d(0, 0), 1.0);
				particle.fixed = (j == 0);
				particleSystem.addParticle(particle);
				Spring s(index - 1, index, k, (double)dy);
				particleSystem.addSpring(s);
				if (j > 0)
				{
					Spring s2(index - 2, index, k, (double)dx);
					particleSystem.addSpring(s2);
					Spring s3(index - 3, index, k, (double)sqrt((double)dx * dx + (double)dy * dy));
					particleSystem.addSpring(s3);
				}
				++index;
			}
		}

	}


	/**
	 * DONE Créez votre propre exemple !
	 */
	static inline void createVotreExemple(ParticleSystem& particleSystem, double k)
	{
		particleSystem.clear();


		const int N = 20;
		const int x_start = 200;
		const int dx = 32;

		int index = 0;


		Particle particle(Vector2d(500, 480), Vector2d(0, 0), Vector2d(0, 0), 1.0);
		particleSystem.addParticle(particle);

		Particle particle2(Vector2d(550, 480), Vector2d(0, 0), Vector2d(0, 0), 1.0);
		particle2.fixed = true;
		particleSystem.addParticle(particle2);

		Particle particle3(Vector2d(550, 450), Vector2d(0, 0), Vector2d(0, 0), 1.0);
		particleSystem.addParticle(particle3);

		Particle particle4(Vector2d(500, 450), Vector2d(0, 0), Vector2d(0, 0), 1.0);
		particleSystem.addParticle(particle4);

		Spring s(0, 3, k, (double)30);
		particleSystem.addSpring(s);

		Spring s2(2, 3, k, (double)50);
		particleSystem.addSpring(s2);


		Spring s3(2, 3, k, (double)50);
		particleSystem.addSpring(s3);


		Spring s4(0, 1, k, (double)30);
		particleSystem.addSpring(s4);
	}


}

ParticleSimApplication::ParticleSimApplication() : nanogui::Screen(Eigen::Vector2i(1280, 820), "GTI320 Labo 03", true, false, 8, 8, 24, 8, 0, 4, 1)
, m_particleSystem(), m_stepping(false), m_stiffness(300), m_kmax(10), m_solverType(kGaussSeidel), m_fpsCounter(0), m_fpsTime(0.0)
{
	initGui();

	createBeam(m_particleSystem, m_stiffness); // le modèle "poutre" est sélectionné à l'initialisation
	m_particleSystem.pack(m_p0, m_v0, m_f0);

	performLayout();
	reset();
}

void ParticleSimApplication::initGui()
{
	// Initialisation de la fenêtre
	m_window = new Window(this, "Particle sim");
	m_window->setPosition(Vector2i(8, 8));
	m_window->setLayout(new GroupLayout());

	// initialisation du canvas où est affiché le système de particules
	m_canvas = new ParticleSimGLCanvas(this);
	m_canvas->setBackgroundColor({ 255, 255, 255, 255 });
	m_canvas->setSize({ 1000, 600 });
	m_canvas->setDrawBorder(false);

	// Initialisation de la fenêtre de contrôle
	Window* controls = new Window(this, "Controls");
	controls->setPosition(Vector2i(960, 10));
	controls->setLayout(new GroupLayout());

	Widget* tools = new Widget(controls);
	tools->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 0, 20));

	// Intervalles des curseur
	const auto stiffnessMinMax = std::make_pair<float, float>(0.0f, logf(5000.f));
	const auto iterMinMax = std::make_pair<float, float>(1.f, 100.f);

	// Affichage du FPS
	m_panelFPS = new Widget(tools);
	m_panelFPS->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 5));
	m_labelFPS = new Label(m_panelFPS, "FPS :");
	m_textboxFPS = new TextBox(m_panelFPS);
	m_textboxFPS->setFixedWidth(60);
	m_textboxFPS->setValue("0");

	// Affichage du numéro de frame
	m_panelFrames = new Widget(tools);
	m_panelFrames->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 5));
	m_labelFrames = new Label(m_panelFrames, "Frame :");
	m_textboxFrames = new TextBox(m_panelFrames);
	m_textboxFrames->setFixedWidth(60);
	m_textboxFrames->setValue("0");

	// Boutons pour le choix du solveur
	m_panelSolver = new Widget(tools);
	m_panelSolver->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 0, 5));
	new Label(m_panelSolver, "Solver : ");
	Button* b = new Button(m_panelSolver, "Gauss-Seidel");
	b->setFlags(Button::RadioButton);
	b->setPushed(true);
	b->setCallback([this] { m_solverType = kGaussSeidel; });
	b = new Button(m_panelSolver, "Jacobi");
	b->setFlags(Button::RadioButton);
	b->setCallback([this] { m_solverType = kJacobi; });
	b = new Button(m_panelSolver, "Cholesky");
	b->setCallback([this] { m_solverType = kCholesky; });
	b->setFlags(Button::RadioButton);
	b = new Button(m_panelSolver, "None");
	b->setCallback([this] { m_solverType = kNone; });
	b->setFlags(Button::RadioButton);

	// Curseur de rigidité 
	Widget* panelSimControl = new Widget(tools);
	panelSimControl->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 0, 5));
	m_panelStiffness = new Widget(panelSimControl);
	m_panelStiffness->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 5));
	m_labelStiffness = new Label(m_panelStiffness, "Stiffness : ");
	m_sliderStiffness = new Slider(m_panelStiffness);
	m_sliderStiffness->setRange(stiffnessMinMax);
	m_textboxStiffness = new TextBox(m_panelStiffness);
	m_sliderStiffness->setCallback([this](float value)
		{
			m_stiffness = exp(value);
			onStiffnessSliderChanged();
		});
	m_sliderStiffness->setValue(logf(300.f));

	// Curseur du nombre maximum d'itération pour Jacobi et Gauss-Seidel
	Widget* panelMaxIter = new Widget(panelSimControl);
	panelMaxIter->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 5));
	new Label(panelMaxIter, "Max iterations : ");
	Slider* sliderMaxIter = new Slider(panelMaxIter);
	sliderMaxIter->setRange(iterMinMax);
	TextBox* textboxMaxIter = new TextBox(panelMaxIter);
	textboxMaxIter->setValue(std::to_string(m_kmax));
	sliderMaxIter->setValue(m_kmax);
	sliderMaxIter->setCallback([this, textboxMaxIter](float value)
		{
			m_kmax = (int)value;
			textboxMaxIter->setValue(std::to_string(m_kmax));
		});

	// Bouton «Simulate»
	Button* startStopButton = new Button(panelSimControl, "Simulate");
	startStopButton->setFlags(Button::ToggleButton);
	startStopButton->setChangeCallback([this](bool val)
		{
			m_stepping = val;
			if (val)
			{
				m_prevTime = glfwGetTime();
				drawAll();
			}
		});

	// Bouton «Step»
	Button* stepButton = new Button(panelSimControl, "Step");
	stepButton->setCallback([this]
		{
			if (!m_stepping)
				step(DELTA_T);
		});

	// Bouton «Reset»
	Button* resetButton = new Button(panelSimControl, "Reset");
	resetButton->setCallback([this]
		{
			reset();
		});

	// Boutons pour le choix du modèle
	Widget* panelExamples = new Widget(tools);
	panelExamples->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 0, 5));
	new Label(panelExamples, "Examples : ");
	Button* loadClothButton = new Button(panelExamples, "Cloth");
	loadClothButton->setCallback([this]
		{
			createHangingCloth(m_particleSystem, m_stiffness);
			m_particleSystem.pack(m_p0, m_v0, m_f0);
			reset();
		});

	Button* loadBeamButton = new Button(panelExamples, "Beam");
	loadBeamButton->setCallback([this]
		{
			createBeam(m_particleSystem, m_stiffness);
			m_particleSystem.pack(m_p0, m_v0, m_f0);
			reset();
		});

	Button* loadRopeButton = new Button(panelExamples, "Rope");
	loadRopeButton->setCallback([this]
		{
			createHangingRope(m_particleSystem, m_stiffness);
			m_particleSystem.pack(m_p0, m_v0, m_f0);
			reset();
		});

	Button* loadVotreExemple = new Button(panelExamples, "Le vôtre");
	loadVotreExemple->setCallback([this]
		{
			createVotreExemple(m_particleSystem, m_stiffness);
			m_particleSystem.pack(m_p0, m_v0, m_f0);
			reset();
		});
}


/**
 * Réaction aux événements déclenchés par le clavier
 */
bool ParticleSimApplication::keyboardEvent(int key, int scancode, int action, int modifiers)
{
	if (Screen::keyboardEvent(key, scancode, action, modifiers))
		return true;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		setVisible(false);
		return true;
	}
	return false;
}

/**
 * Boucle principale
 *
 * Cette fonction est appelée périodiquement lorsque le programme est actif.
 * C'est ici que tout se passe. Si la simulation est en cours, la fonction
 * `step` est appelée pour faire avancer le système d'un intervalle de temps
 * DELTA_T. Ensuite, l'affichage est mis à jour.
 */
void ParticleSimApplication::drawContents()
{
	if (m_stepping)
	{
		auto now = glfwGetTime();
		double dt = now - m_prevTime;

		step(DELTA_T);

		// Update frames per second 
		//
		m_fpsTime += dt;
		++m_fpsCounter;
		if (m_fpsCounter > 30)
		{
			const double fps = (double)m_fpsCounter / m_fpsTime;
			char buf[64];
			snprintf(buf, sizeof(buf), "%3.1f", fps);
			m_fpsCounter = 0;
			m_fpsTime = 0.0;
			m_textboxFPS->setValue(buf);
		}
		m_prevTime = now;

		updateFrameCounter();
	}
}

/**
 * Appelée lorsque le curseur de rigidité est modifié. La nouvelle rigidité est
 * affectée à tous les ressorts
 */
void ParticleSimApplication::onStiffnessSliderChanged()
{
	// Update all springs with the slider value
	for (Spring& s : getParticleSystem().getSprings())
	{
		s.k = m_stiffness;
	}

	char buf[16];
	snprintf(buf, sizeof(buf), "%4.0f", m_stiffness);
	m_textboxStiffness->setValue(buf);
}

/**
 * Effectue un pas de simulation de taille dt.
 */
void ParticleSimApplication::step(double dt)
{
	// Construction des matrices de masse et de rigidité
	//
	m_particleSystem.buildMassMatrix(m_M);
	m_particleSystem.buildDfDx(m_dfdx);

	// Calcul des forces actuelles sur chacune de sparticules
	//
	m_particleSystem.computeForces();
	m_canvas->applyMouseSpring();

	// Assemblage des vecteurs d'états.
	// 
	m_particleSystem.pack(m_x, m_v, m_f);

	//////////////////////////////////////////////////////////////////////////////////
	// DONE Construire le système d'équation linéaire sous la forme `A*v_plus = b`.
	//      la construction de A et b est donnée dans les diapos du Cours 8.
	//
	//      Note : lors du calcul de b, NE PAS calculer `Mg + Kx` ce
	//      calcul est inutilement coûteux. Pour être plus efficace, on utilise
	//      directement le vecteur d'état m_f.
	//
	//////////////////////////////////////////////////////////////////////////////////
	// Remarque : A et b sont déclarés `const` et ce n'est pas une erreur. C'est
	// pour vous forcer à optimiser votre code.
	//
	// Considérez les exemples suivants :
	// 
	// # Version 1
	// Matrix A;    // constructeur par défaut
	// A = B + C;   // operator+ on construit une matrice et elle est suite copiée avec operator=
	//
	// # Version 2
	// Matrix A = B + C; // la matrice construite dans operator+ est la matrice A.
	//
	// Bilan : Version 1 utilise 2 constructeurs et 1 opérateur de copie
	//         Version 2 utilise un seul constructeur et aucune copie
	//////////////////////////////////////////////////////////////////////////////////
	//

	const Matrix<double, Dynamic, Dynamic> A = m_M + (-1 * dt * dt * m_dfdx);
	const Vector<double, Dynamic> b = dt * m_f + m_M * m_v;

	// Solve the linear system A*v_plus = b using the selected solver.
	// 
	// Résolution du système d'équations  `A*v_plus = b`.
	Vector<double, Dynamic> v_plus;
	Vector<double, Dynamic> acc; // vecteur d'accélérations
	switch (m_solverType)
	{
	case kGaussSeidel:
		gaussSeidel(A, b, v_plus, m_kmax);
		break;
	case kCholesky:
		cholesky(A, b, v_plus);
		break;
	default:
		jacobi(A, b, v_plus, m_kmax);
		break;
	case kNone:
		// N'utilise pas de solveur, il s'agit de l'implémentation naive de
		// l'intégration d'Euler.
		acc.resize(m_M.rows()); // vecteur d'accélérations
		for (int i = 0; i < m_M.rows(); ++i)
			acc(i) = (1.0 / m_M(i, i)) * m_f(i);
		v_plus = m_v + dt * acc;
		break;
	}

	// DONE Mise à jour du vecteur d'état de position via l'intégration d'Euler
	// implicite. Les nouvelles position sont calculées à partir des position
	// actuelles m_x et des nouvelles vitesses v_plus. Les nouvelles positions
	// sont stockées directement dans le vecteur m_x.
	m_x = m_x + dt * v_plus;

	// Affecte les valeurs calculées dans le vecteurs d'états aux particules du
	// système
	m_particleSystem.unpack(m_x, v_plus);
}

/**
 * Réinitialisation du système de particules
 */
void ParticleSimApplication::reset()
{
	m_frameCounter = 0;
	m_particleSystem.unpack(m_p0, m_v0);

	onStiffnessSliderChanged();
}

/**
 * Mise à jour du compteur de frames
 */
void ParticleSimApplication::updateFrameCounter()
{
	++m_frameCounter;
	char buf[16];
	snprintf(buf, sizeof(buf), "%d", m_frameCounter);
	m_textboxFrames->setValue(buf);
}
