// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include <math.h>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'

#define DRAW_ELEMENTS 1

using namespace std;
// *****************************************************************************
// funciones auxiliares
GLuint VBO_Crear( GLuint tipo, GLuint tamanio, GLvoid * puntero)
{
  assert( tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER );
  GLuint id_vbo ; // resultado: identificador de VBO
  glGenBuffers( 1, & id_vbo ); // crear nuevo VBO, obtener identificador
  glBindBuffer( tipo, id_vbo ); // activar el VBO usando su identificador
  glBufferData( tipo, tamanio, puntero, GL_STATIC_DRAW ); // transferencia RAM –> GPU
  glBindBuffer( tipo, 0 ); // desactivación del VBO (activar 0)

  return id_vbo ;
}

Tupla3f calcularCentroCajaEnglobante(const std::vector<Tupla3f>& vertices)
{
  assert(vertices.size() > 0);

  Tupla3f maximo = vertices[0],
          minimo = vertices[0]; // puntos diagonales opuestos

  for (auto ver : vertices) {
    maximo = {max(ver(X), maximo(X)), max(ver(Y), maximo(Y)), max(ver(Z), maximo(Z))};
    minimo = {min(ver(X), minimo(X)), min(ver(Y), minimo(Y)), min(ver(Z), minimo(Z))};
  }

  return (maximo + minimo) / 2.0;
}


// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre("malla indexada, anónima");

}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
   id_vbo_tri = id_vbo_ver = id_vbo_normales = id_vbo_texturas = id_vbo_color_ver = 0;
   num_ver = num_tri = 0;
   tam_tri = tam_ver = 0;
   vbo_creado = false;
   normales_creadas = usa_textura = false;

}
// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcular_normales()
{
   // COMPLETAR: en la práctica 2: calculo de las normales de la malla
   // .......

   assert(!ver.empty());

   for(int i = 0; i < ver.size(); i++){
    nor_ver.push_back({0,0,0});
  }

  for( auto cara: tri){
    Tupla3f p = ver[cara[0]],
      q = ver[cara[1]],
      r = ver[cara[2]];

      Tupla3f a = q - p;
      Tupla3f b = r - p;

      Tupla3f normal = a.cross(b);

      if(normal(X)>0.0 || normal(Y)>0.0 || normal(Z)>0.0)
        normal = normal.normalized();

    nor_caras.push_back(normal);

    for( int j = 0 ; j < 3; j++){
      nor_ver[cara[j]] = (nor_ver[cara[j]] + normal);

      if(nor_ver[cara[j]](X) > 0.0 || nor_ver[cara[j]](Y) > 0.0 || nor_ver[cara[j]](Z)>0.0)
        nor_ver[cara[j]] = nor_ver[cara[j]].normalized();
    }

  }

  normales_creadas = true;

}

void MallaInd::colorear(Tupla3f* color)
{
  colores.clear();

    for (unsigned i = 0; i < num_ver; i++)
      if(color == nullptr)
        colores.push_back({(float) (i+1) / num_ver, 0.1, 0.1});
      else
        colores.push_back(*color);

}

//------
void MallaInd::crearVBOs()
{
  //FALTAN PONER COSAS CON EL INDICE
  // crear VBO conteniendo la tabla de vértices
  tam_ver = sizeof(float)*3L*num_ver;
  id_vbo_ver = VBO_Crear( GL_ARRAY_BUFFER, tam_ver, ver.data() );


  // crear VBO con la tabla de triángulos (índices de vértices)
  tam_tri = sizeof(unsigned)*3L*tri.size();
  id_vbo_tri = VBO_Crear( GL_ELEMENT_ARRAY_BUFFER, tam_tri, tri.data() );

  if (colores.size() > 0)
    id_vbo_color_ver = VBO_Crear(GL_ARRAY_BUFFER, tam_ver, colores.data());

  if (nor_ver.size() > 0)
    id_vbo_normales = VBO_Crear(GL_ARRAY_BUFFER, tam_ver, nor_ver.data());

  if (texturas.size() > 0)
    id_vbo_texturas = VBO_Crear(GL_ARRAY_BUFFER, sizeof(float) * 2L * num_ver, texturas.data());

  vbo_creado = true;
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
   // COMPLETAR: en la práctica 1: visualizar en modo inmediato (glDrawElements)
   // ...........

#if DRAW_ELEMENTS == 1

  if (usa_textura) {


      glEnableClientState( GL_NORMAL_ARRAY );
      glNormalPointer( GL_FLOAT, 0, nor_ver.data() );


    if (texturas.size() > 0) {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, 0, texturas.data());
    }

  }

   if (colores.size() > 0 && ! cv.modoSeleccionFBO) {
    glColorPointer( 3, GL_FLOAT, 0, colores.data() );
    glEnableClientState( GL_COLOR_ARRAY );
  }
   // habilitar uso de un array de vértices
   glEnableClientState( GL_VERTEX_ARRAY );
   // especificar puntero a tabla de coords. de vértices
   glVertexPointer( 3, GL_FLOAT, 0, ver.data() );

   // dibujar usando vértices indexados
   // params.: (1) tipo de primitivas (2) número de índices
   // (3) tipo de índices (4) puntero a tabla de triáng.
   glDrawElements( GL_TRIANGLES, 3* num_tri, GL_UNSIGNED_INT, tri.data() ) ;
   // deshabilitar el array de vértices
   glDisableClientState( GL_VERTEX_ARRAY );
   glDisableClientState( GL_COLOR_ARRAY );
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);


#else
   //Modo Begin-End
   glBegin(GL_TRIANGLES);


   for (unsigned i = 0; i < tri.size(); ++i) {

     for (unsigned j = 0; j < 3; j++) {

       unsigned iv = tri[i](j);

          if (usa_textura) {
          glNormal3fv(nor_ver[iv]);
          if(texturas.size() > 0 ) glTexCoord2fv(texturas[iv]);
        }

        else if (colores.size() > 0 && ! cv.modoSeleccionFBO)
          glColor3fv(colores[iv]);

       glVertex3fv(ver[iv]);
      }
    }

    glEnd();

#endif

}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
   // COMPLETAR: práctica 1: visualizar en modo diferido,
   //                        usando VBOs (Vertex Buffer Objects)
   // ..........
   if(!vbo_creado)
    crearVBOs();

    // Colores o normales
    if (usa_textura) {
      glBindBuffer(GL_ARRAY_BUFFER, id_vbo_normales);
      glNormalPointer(GL_FLOAT, 0, 0);
      glEnableClientState(GL_NORMAL_ARRAY);


      if (texturas.size() > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, id_vbo_texturas);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      }
    }

    if (colores.size() > 0) {
      glBindBuffer(GL_ARRAY_BUFFER, id_vbo_color_ver);
      glColorPointer(3, GL_FLOAT, 0, 0);
      glEnableClientState(GL_COLOR_ARRAY);
    }

   // especificar formato de los vértices en su VBO (y offset)
   glBindBuffer( GL_ARRAY_BUFFER, id_vbo_ver ); // act. VBO
   glVertexPointer( 3, GL_FLOAT, 0, 0 );
   // formato y offset (=0)
   glBindBuffer( GL_ARRAY_BUFFER, 0 );  // desact VBO.
   glEnableClientState( GL_VERTEX_ARRAY ); // act. uso VA
   // visualizar con glDrawElements (puntero a tabla == NULL)
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri );
   glDrawElements( GL_TRIANGLES, 3L* tri.size(), GL_UNSIGNED_INT, NULL ) ;
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

   // desactivar uso de array de vértices
   glDisableClientState( GL_VERTEX_ARRAY );
   glDisableClientState( GL_COLOR_ARRAY );
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);


}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
   // COMPLETAR: práctica 1: visualizar en modo inmediato o en modo diferido (VBOs),
   // (tener en cuenta el modo de visualización en 'cv' (alambre, sólido, etc..))
   //
   // .............

   // Texturas
   usa_textura = cv.modoVis == modoIluminacionPlano || cv.modoVis == modoIluminacionSuave;

   if (!usa_textura) {
     glDisable(GL_LIGHTING);
     glDisable(GL_TEXTURE_2D);
   }
   else if (!normales_creadas)
      calcular_normales();


      // Modo selección P5
         if (cv.modoSeleccionFBO) {
           glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
           glShadeModel(GL_FLAT);
           visualizarDE_MI(cv);
           return;
      }

      GLenum mode;
       switch(cv.modoVis) {
         case modoSolido:
           mode = GL_FILL;
           glShadeModel(GL_SMOOTH);
           break;
         case modoPuntos:
           mode = GL_POINT;
           break;
         case modoIluminacionPlano:
           mode = GL_FILL;
           glShadeModel(GL_FLAT);
           break;
         case modoIluminacionSuave:
           mode = GL_FILL;
           glShadeModel(GL_SMOOTH);
           break;
         case modoAlambre:  // modo por defecto
         default:
           mode = GL_LINE;
           glShadeModel(GL_SMOOTH);
       }

       glPolygonMode(GL_FRONT_AND_BACK, mode);

       // Visualizar
       if (cv.usarVBOs)
         visualizarDE_VBOs(cv);
       else if (cv.modoVis == modoIluminacionPlano)
         visualizarDE_PLANO(cv);
       else
        visualizarDE_MI(cv);

}

void MallaInd::visualizarDE_PLANO( ContextoVis & cv ){
  //Modo Begin-End
  glBegin(GL_TRIANGLES);


  for (unsigned i =0; i < tri.size(); ++i) {

    glNormal3fv(nor_caras[i]);
    for (unsigned j = 0; j < 3; j++) {

      if(colores.size() > 0)
         glColor3fv(colores[tri[i](j)]);

      if (texturas.size() > 0) glTexCoord2fv(texturas[tri[i](j)]);

      glVertex3fv(ver[tri[i](j)]);
     }
   }

   glEnd();
}

// -----------------------------------------------------------------------------

void MallaInd::calcularCentroOC() {
  if (!centro_calculado) {
    ponerCentroOC(calcularCentroCajaEnglobante(ver));
    centro_calculado = true;
  }
}

// *****************************************************************************

// *****************************************************************************

Cubo::Cubo()
:  MallaInd( "malla cubo" )
{
  num_ver = 8;
  num_tri = 12;

  Tupla3f a(1.0, 0.0, 0.0);
  Tupla3f b(0.0, 0.0, -1.0);
  Tupla3f c(-1.0, 0.0, 0.0);
  Tupla3f d(0.0, 0.0, 1.0);

  Tupla3f a1(1.0, sqrt(2), 0.0);
  Tupla3f b1(0.0, sqrt(2), -1.0);
  Tupla3f c1(-1.0, sqrt(2), 0.0);
  Tupla3f d1(0.0, sqrt(2), 1.0);

  ver.push_back(a);
  ver.push_back(b);
  ver.push_back(c);
  ver.push_back(d);

  ver.push_back(a1);
  ver.push_back(b1);
  ver.push_back(c1);
  ver.push_back(d1);

  Tupla3i t1(0,1,3);
  Tupla3i t2(1,2,3);

  Tupla3i t3(4,7,5);
  Tupla3i t4(6,5,7);

  Tupla3i t5(1,0,5);
  Tupla3i t6(5,0,4);

  Tupla3i t7(1,6,2);
  Tupla3i t8(5,6,1);

  Tupla3i t9(2,3,7);
  Tupla3i t10(6,2,7);

  Tupla3i t11(3,0,4);
  Tupla3i t12(4,7,3);


  tri.push_back(t1);
  tri.push_back(t2);
  tri.push_back(t3);
  tri.push_back(t4);
  tri.push_back(t5);
  tri.push_back(t6);
  tri.push_back(t7);
  tri.push_back(t8);
  tri.push_back(t9);
  tri.push_back(t10);
  tri.push_back(t11);
  tri.push_back(t12);

  colorear(nullptr);

}
// *****************************************************************************

Tetraedro::Tetraedro()
:  MallaInd( "malla tetraedro")
{
    num_ver = 4;
    num_tri = 4;

    Tupla3f a(1,0,-1/sqrt(2));
    Tupla3f b(-1,0,-1/sqrt(2));
    Tupla3f c(0,1,1/sqrt(2));
    Tupla3f d( 0,-1,1/sqrt(2));

    ver.push_back(a);
    ver.push_back(b);
    ver.push_back(c);
    ver.push_back(d);

    Tupla3i t1(0,1,2);
    Tupla3i t2(0,1,3);
    Tupla3i t3(1,2,3);
    Tupla3i t4(2,0,3);

    tri.push_back(t1);
    tri.push_back(t2);
    tri.push_back(t3);
    tri.push_back(t4);

    colorear(nullptr);
}
// *****************************************************************************
