#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  DoingInteractive = NONE;
}
void MyGLWidget::canvicamara1(){
    makeCurrent();
    camara2=false;
    viewTransform();
    projectTransform();
    update();

}
void MyGLWidget::canvicamara2(){
    makeCurrent();
    camara2=true;
    viewTransform();
    projectTransform();
    update();

}
MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  glEnable(GL_DEPTH_TEST);
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersModels();
  creaBuffersTerra();
  iniEscena();
  iniCamera();
}

void MyGLWidget::iniEscena()
{
  radiEscena = sqrt(80);
  centreEscena = glm::vec3(0,0,0);
}

void MyGLWidget::iniCamera(){
  //posicion de la camara inicial
  float r= d/2;
  obs = glm::vec3(0, 1.5, d);
  vrp = glm::vec3(0, 1.5, 0);
  up = glm::vec3(0, 1, 0);
  fov = 2*glm::radians(30.f);
  znear =  d-r;
  zfar  = d+r;
  factorAngleX=0;
  factorAngleY=45;
  posMorty = glm::vec3(1,0,0);
  angleMorty=0;
  girRick=180;
  portx=-2.5;
  portz=-3;
  girPortal=180;
  morty2=false;
  turnport=true;
  viewTransform();
}

void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Rick
  glBindVertexArray (VAO_models[RICK]);
  RickTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);

  // Morty
  glBindVertexArray (VAO_models[MORTY]);
  MortyTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);

  if(MortyPortalTest(glm::radians(girPortal),posMorty)and turnport){
      //borrem el portal, y inicialitzem el morty2
      morty2=true;
      posMorty2=posMorty;
      angleMorty2=angleMorty;
      turnport=false;
  }

  if (morty2){
      //pintem el morty2 amb el seu color vermell
      glUniform1f(morty2Loc,true);
      glBindVertexArray (VAO_models[MORTY]);

      MortyTransform2(posMorty2,angleMorty2);
      glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);
      glUniform1f(morty2Loc,false);


  }
  
  // Portal
  if (turnport){
      //pintem el portal si turnport es true
      glBindVertexArray (VAO_models[PORTAL]);
      PortalTransform();
      glDrawArrays(GL_TRIANGLES, 0, models[PORTAL].faces().size()*3);
  }
  
  // Terra
  glBindVertexArray (VAO_Terra);
  identTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::RickTransform ()
{
  glm::mat4 TG(1.0f);  

  TG = glm::translate(TG, glm::vec3(-2.5, 0, 0));
  TG= glm::rotate(TG, glm::radians(girRick), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[RICK]));
  TG = glm::translate(TG, -centreBaseModels[RICK]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::MortyTransform2 (glm::vec3 posMorty2, float angleMorty2)
{
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG,glm::vec3(posMorty2));
    TG= glm::rotate(TG,glm::radians(angleMorty2),glm::vec3(0.f,1.f,0.f));
    TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
    TG = glm::translate(TG, -centreBaseModels[MORTY]);
    TGMorty2 = TG;

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);


}

void MyGLWidget::MortyTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, posMorty);
  TG= glm::rotate(TG,glm::radians(angleMorty),glm::vec3(0.f,1.f,0.f));
  TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
  TG = glm::translate(TG, -centreBaseModels[MORTY]);
  TGMorty = TG;
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::PortalTransform ()
{
  glm::mat4 TG(1.0f);

  TG = glm::translate(TG, glm::vec3(portx,0,portz));
  TG= glm::rotate(TG,glm::radians(girPortal),glm::vec3(0.f,1.f,0.f));
  TG = glm::scale(TG,glm::vec3(0.25,1,1));
  TG = glm::scale(TG, glm::vec3(escalaModels[PORTAL]));
  TG = glm::translate(TG, -centreBaseModels[PORTAL]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::identTransform ()
{
  glm::mat4 TG(1.0f);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  if (not camara2){
     //camara normal
     Proj = glm::perspective (fov, ra, znear, zfar);
  }
  else{
      //camara 3a persona
      Proj = glm::perspective (fov, ra, 0.25f, zfar);

  }
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
     glm::mat4 View(1.0f);
  if (not camara2){
      //camara normal

      View=glm::translate(View,glm::vec3(0.0f,0.f,-d));

      View= glm::rotate(View, glm::radians(factorAngleY),glm::vec3(1.f,0.f,0.f));
      View= glm::rotate(View, glm::radians(-factorAngleX),glm::vec3(0.f,1.f,0.f));
      View=glm::translate(View,glm::vec3(-vrp.x,-vrp.y,-vrp.z));
  }

  else{
      //camara 3a persona
      //com no fa falta fer angles de euler posem un lookAT
      View = glm::lookAt (glm::vec3(posMorty.x,0.8,posMorty.z), glm::vec3(posMorty.x + sin(glm::radians(angleMorty)),0.8,posMorty.z+cos(glm::radians(angleMorty))), up);

  }
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


void MyGLWidget::resizeGL (int w, int h) 
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
  ra = float(ample)/float(alt);
  projectTransform();
}

bool MyGLWidget::MortyPortalTest(float angleRick, glm::vec3 posMorty)
{
  glm::vec3 posPortal = glm::vec3(sin(angleRick),0.0,cos(angleRick))*glm::vec3(3) + glm::vec3(-2.5,0,0);
  return (glm::distance(posMorty,posPortal) <= 0.2);
}


void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
        posMorty = posMorty + glm::vec3(sin(glm::radians(angleMorty)),0.0,cos(glm::radians(angleMorty)))*glm::vec3(0.1);
        viewTransform();
      break;
    }
    case Qt::Key_Down: {
        posMorty = posMorty - glm::vec3(sin(glm::radians(angleMorty)),0.0,cos(glm::radians(angleMorty)))*glm::vec3(0.1);
        viewTransform();
         break;
    }
    case Qt::Key_Right:{
       angleMorty-=45;
        viewTransform();
       break;
  }
  case Qt::Key_Left:{
        angleMorty+=45;
        viewTransform();
        break;
  }

    case Qt::Key_Q:{
        girRick+=45;
        break;
    }
    case Qt::Key_E:{
        girRick-=45;
        break;
 }
  case Qt::Key_C:{
    if (not camara2){
        camara2=true;
    }
    else{
        camara2=false;
    }
    projectTransform();
    viewTransform();

     break;
  }
  case Qt::Key_R:{
    iniCamera();
    camara2=false;
    projectTransform();
    viewTransform();
    break;
  }
    case Qt::Key_P:{
        if(int(girPortal)%360 ==int(girRick)%360 and turnport){
            turnport=false;

        }
        else{
            girPortal=girRick;
            portx=-2.5+3*sin(glm::radians(girRick));
            portz=3*cos(glm::radians(girRick));
            turnport=true;
        }
        break;
      }

    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{

  makeCurrent();
  int deltaX = e->x() - xClick;
  int deltaY = e->y() - yClick;
  // Rotación horizontal
  factorAngleX += deltaX ;
  // Rotación vertical
  factorAngleY += deltaY ;

  // Actualizar la vista
  viewTransform();
  update();
  xClick = e->x();
  yClick = e->y();
 }



void MyGLWidget::creaBuffersTerra ()
{
  // VBO amb la posició dels vèrtexs
  glm::vec3 posTerra[4] = {
        glm::vec3(-7.5, 0.0, -5.0),
        glm::vec3(-7.5, 0.0,  5.0),
        glm::vec3( 7.5, 0.0, -5.0),
        glm::vec3( 7.5, 0.0,  5.0)
  }; 

  glm::vec3 c(0.8, 0.7, 1.0);
  glm::vec3 colTerra[4] = { c, c, c, c };

  // VAO
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posTerra), posTerra, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colTerra), colTerra, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray (0);
}

void MyGLWidget::calculaCapsaModel (Model &p, float &escala, float alcadaDesitjada, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = p.vertices()[0];
  miny = maxy = p.vertices()[1];
  minz = maxz = p.vertices()[2];
  for (unsigned int i = 3; i < p.vertices().size(); i+=3)
  {
    if (p.vertices()[i+0] < minx)
      minx = p.vertices()[i+0];
    if (p.vertices()[i+0] > maxx)
      maxx = p.vertices()[i+0];
    if (p.vertices()[i+1] < miny)
      miny = p.vertices()[i+1];
    if (p.vertices()[i+1] > maxy)
      maxy = p.vertices()[i+1];
    if (p.vertices()[i+2] < minz)
      minz = p.vertices()[i+2];
    if (p.vertices()[i+2] > maxz)
      maxz = p.vertices()[i+2];
  }
  escala = alcadaDesitjada/(maxy-miny);
  centreBase[0] = (minx+maxx)/2.0; centreBase[1] = miny; centreBase[2] = (minz+maxz)/2.0;
}

void MyGLWidget::creaBuffersModels ()
{
  // Càrrega dels models
  models[RICK].load("./models/Rick.obj");
  models[MORTY].load("./models/Morty.obj");
  models[PORTAL].load("./models/portal.obj");

  // Creació de VAOs i VBOs per pintar els models
  glGenVertexArrays(3, &VAO_models[0]);
  
  float alcadaDesitjada[3] = {2,1,3};
  
  for (int i = 0; i < 3; i++)
  {	
	  // Calculem la capsa contenidora del model
	  calculaCapsaModel (models[i], escalaModels[i], alcadaDesitjada[i], centreBaseModels[i]);
  
	  glBindVertexArray(VAO_models[i]);

	  GLuint VBO[2];
	  glGenBuffers(2, VBO);

	  // geometria
	  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
		   models[i].VBO_vertices(), GL_STATIC_DRAW);
	  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(vertexLoc);

	  // color
	  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
		   models[i].VBO_matdiff(), GL_STATIC_DRAW);
	  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(colorLoc);  
  }
  
  glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Identificador per als  atributs
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  morty2Loc=glGetUniformLocation(program->programId(), "pinta2");
   // Identificadors dels uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc  = glGetUniformLocation (program->programId(), "Proj");
  viewLoc  = glGetUniformLocation (program->programId(), "View");
}


