// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector

#include "Objeto3D.hpp"   // declaración de 'Objeto3D'

//------------------------------------------------------------------------

// ---------------------------------------------------------------------
// funciones auxiliares
Tupla3f calcularCentroCajaEnglobante(const std::vector<Tupla3f>& vertices);


// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{

   protected:
      // COMPLETAR: incluir aquí las variables y métodos privados que sean
      // necesarios para una malla indexada
      // ......
      unsigned num_ver;  // número de vértices (n v )
      unsigned num_tri; // número de triángulos (n t )
      std::vector<Tupla3f> ver ; // tabla de vértices (num_ver entradas)
      std::vector<Tupla3i> tri ; // tabla de triángulos (num_tri entradas)
      std::vector<Tupla3f> nor_ver; //tabla de vértices para las normales
      std::vector<Tupla3f> nor_caras; //tabla de vértices para las caras
      std::vector<Tupla3f> colores;

      std::vector<Tupla2f> texturas;

      GLuint id_vbo_ver ; // identificador del VBO con la tabla de vértices
      GLuint id_vbo_tri ; // identificador del VBO con la tabla de triángulos
      GLuint id_vbo_color_ver;
      GLuint id_vbo_normales;
      GLuint id_vbo_texturas;

      unsigned tam_ver ; // tamaño en bytes de la tabla de vértices
      unsigned tam_tri ; // tamaño en bytes de la tabla de triángulos

      //bool
      bool vbo_creado;
      bool usa_textura;
      bool normales_creadas;

      // calculo de las normales de esta malla
      virtual void calcular_normales();

      //
      void crearVBOs();
      // visualizar con 'draw elements', en modo inmediato
      void visualizarDE_MI( ContextoVis & cv );
      // visualizar con 'draw elements', en modo diferido (con VBOS)
      void visualizarDE_VBOs( ContextoVis & cv );

      void visualizarDE_PLANO( ContextoVis & cv );

   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd() ;
      // crea una malla vacía con un nombre concreto:
      MallaInd( const std::string & nombreIni );
      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv ) ;

      // pone los colores a los vertices
      void colorear(Tupla3f* color);

      // Recalcular el centro del objeto
      virtual void calcularCentroOC();
} ;
// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
   public:
      Cubo();
};
// ---------------------------------------------------------------------

class Tetraedro : public MallaInd
{
   public:
      Tetraedro();
};


#endif
