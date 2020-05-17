// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaPLY'
// **
// *********************************************************************


#include <cassert>
#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include "MallaPLY.hpp"

using namespace std ;

// *****************************************************************************

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre(string("malla leída del archivo '") + nombre_arch + "'" );
   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................
   vector<float> vertices;
   vector<int> caras;

   ply::read(nombre_arch.c_str(), vertices, caras);

   for(int i = 0; i+2 < vertices.size(); i+=3)
   {
     Tupla3f a(vertices[i], vertices[i+1], vertices[i+2]);
     ver.push_back(a);
   }

   for(int i = 0; i+2 < caras.size(); i+=3)
   {
     Tupla3i a(caras[i], caras[i+1], caras[i+2]);
     tri.push_back(a);
   }

   num_ver = vertices.size() / 3;
   num_tri = caras.size() / 3;
   // calcular la tabla de normales
   calcular_normales();

   colorear(nullptr);

}

// *****************************************************************************
