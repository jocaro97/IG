// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include <matrices-tr.hpp>
#include "MallaRevol.hpp"

using namespace std ;

//*************************************************************************
// Funciones auxiliares

vector<Tupla3f> rotarPerfil(const Matriz4f& m,
                            const std::vector<Tupla3f>& perfil)
{
  vector<Tupla3f> rotado;
  for(auto vert : perfil)
    rotado.push_back(m * vert);
  return rotado;
}


// *****************************************************************************
MallaRevol::MallaRevol(const std::string & nombre):MallaInd(nombre)
{

}

MallaRevol::MallaRevol( const std::string & nombre_arch,
                        const unsigned nperfiles,
                        const bool     crear_tapas,
                        const bool     cerrar_malla,
                        const bool     crear_texturas)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // ...........................
   // ...........................
   vector<float> vertices;
   vector<Tupla3f> aux;

   ply::read_vertices(nombre_arch.c_str(), vertices);

   for(int i = 0; i+2 < vertices.size(); i+=3)
   {
     Tupla3f a(vertices[i], vertices[i+1], vertices[i+2]);
     aux.push_back(a);
   }

   nper = nperfiles;
   nvp = vertices.size()/3;
   num_ver = nper * nvp;

   crearMallaRevol(aux, nperfiles, crear_tapas, cerrar_malla, crear_texturas);
   // calcular la tabla de normales
  // calcular_normales();


}

void MallaRevol::crearMallaRevol( const std::vector<Tupla3f> & perfil_original,  // vértices del perfil original
                      const unsigned nperfiles,                      // número de perfiles
                      const bool crear_tapas,                        // true para crear tapas
                      const bool cerrar_malla,                       // true para cerrar la malla
                      const bool crear_texturas )
{

  unsigned i, j;
    vector<float> d;
    float alpha = cerrar_malla ? 360.0 / nper : 360.0 / (nper -1);
    Matriz4f m = MAT_Rotacion(alpha, 0, 1, 0);
    vector<Tupla3f> aux = perfil_original;
    ver.insert(ver.end(), aux.begin(), aux.end());

    // Añadir vértices
    for (i = 0; i < nper - 1; i++) {
      aux = rotarPerfil(m, aux);
      ver.insert(ver.end(), aux.begin(), aux.end());

      // Añadir caras
      for (j = 0; j < nvp - 1; j++) {
        int k1 = i * nvp + j;
        int k2 = i * nvp + j + 1;
        int k3 = (i+1) * nvp + j + 1;
        int k4 = (i+1) * nvp + j;

        tri.push_back({k4, k3, k2});
        tri.push_back({k4, k2, k1});
      }
    }

    if (cerrar_malla) {
      // Añadir caras (i = nper - 1)
      for (j = 0; j < nvp - 1; j++) {
        int k1 = i * nvp + j;
        int k2 = i * nvp + j + 1;
        int k3 = j + 1; // ((i+1) % nper) * nvp + j + 1
        int k4 = j; // ((i+1) % nper) * nvp + j

        tri.push_back({k4, k3, k2});
        tri.push_back({k4, k2, k1});
      }
    }

    // Coordenadas de textura
    if (crear_texturas && !cerrar_malla) {
      d.push_back(0.0);
      for (unsigned i = 1; i < nvp; i++)
        d.push_back(d[i-1] + sqrt((perfil_original[i] - perfil_original[i-1]).lengthSq()));

      for (unsigned i = 0; i < nper; i++)
        for (unsigned j = 0; j < nvp; j++)
          texturas.push_back({(float) i / (nper - 1), 1 - (float) d[j] / d[nvp - 1]});
    }

// Crear tapas
  if (crear_tapas) {

    // Cara inferior
    auto primer_ver = ver[0];
    if (primer_ver(X) != 0) {
      ver.push_back({0.0, primer_ver(Y), 0.0});
      num_ver++;
      texturas.push_back({0.0, 0.0}); // TODO: ??

      for (i = 0; i + nvp < num_ver - 1; i += nvp)
        tri.push_back({i, num_ver - 1, i + nvp});

      if (cerrar_malla) // Última cara (i = (nper - 1) * nvp)
        tri.push_back({i, num_ver - 1, 0});
    }

    // Cara superior
    auto ult_ver = ver[num_ver - 2];
    if (ult_ver(X) != 0) {
      ver.push_back({0.0, ult_ver(Y), 0.0});
      num_ver++;
      texturas.push_back({0.0, 0.0}); // TODO: ??

      for (i = nvp - 1; i + nvp < num_ver; i += nvp)
        tri.push_back({i, i + nvp, num_ver - 1});

      if (cerrar_malla) // Última cara (i = nper * nvp - 1)
        tri.push_back({i, nvp - 1, num_ver - 1});
    }
}

    colorear(nullptr);
/*
    if(crear_texturas && !cerrar_malla)
      crearTablaTexturas();
      */

}

void MallaRevol::crearTablaTexturas(){
    std::vector<float> d;

    d.push_back(0.0);

    for(unsigned i = 0; i < nvp -1; ++i)
      d.push_back(d[i] + sqrt((ver[i+1] - ver[i]).lengthSq()));

    for(unsigned i = 0; i < nper; ++i)
      for(unsigned j = 0; j < nvp; ++j)
        texturas.push_back(Tupla2f{(float) i / (float) (nper -1) , 1 - d[j]/d[nvp - 1]});
}

Cilindro::Cilindro(int n, int nperfiles, bool crear_tapas, bool cerrar_malla):MallaRevol("Cilindro revolucionado")
{
  std::vector<Tupla3f> vertices;
  for(int i=0; i < n; ++i)
    vertices.push_back(Tupla3f(0.5, i*1.0/n, 0.0));


  nper = nperfiles;
  nvp = n;
  num_ver = nper * nvp;

  crearMallaRevol(vertices, nperfiles, crear_tapas, cerrar_malla );
}

Esfera::Esfera(int n, int nperfiles, bool crear_tapas, bool cerrar_malla):MallaRevol("Esfera revolucionada")
{
    float y ; //arreglar la esfera que no llega a tener la tapa de arriba igual que la de abajo
    std::vector<Tupla3f> vertices;
    for(int i=0; i <  n; ++i)
    {
      y = i*2.0/(n-1) -1;
      vertices.push_back(Tupla3f(sqrt(1-(y*y)), y , 0.0));

    }

    nper = nperfiles;
    nvp = n;
    num_ver = nper * nvp;

    crearMallaRevol(vertices, nperfiles, crear_tapas, cerrar_malla );

}

Cono::Cono(int n, int nperfiles, bool crear_tapas, bool cerrar_malla):MallaRevol("Cono revolucionado")
{
  float y;
  std::vector<Tupla3f> vertices;

  for(int i=0; i<n; ++i)
  {
    y = 1.0*i/(n-1);
    vertices.push_back(Tupla3f(-y + 1, y, 0.0));
  }

  nper = nperfiles;
  nvp = n;
  num_ver = nper * nvp;

  crearMallaRevol(vertices, nperfiles, crear_tapas, cerrar_malla);
}

Huevo::Huevo(int n, int nperfiles, bool crear_tapas, bool cerrar_malla):MallaRevol("Huevo revolucionado")
{
  float y ;
  std::vector<Tupla3f> vertices;
  for(int i=0; i <= n; ++i)
  {
    y = i*4.0/n;
    vertices.push_back(Tupla3f(sqrt(2.0* sqrt(y*y*y) - y*y), y , 0.0));
  }

  nper = nperfiles;
  nvp = n+1;
  num_ver = nper * nvp;

  crearMallaRevol(vertices, nperfiles, crear_tapas, cerrar_malla );
}

Sombrero::Sombrero(int n, int nperfiles, bool crear_tapas, bool cerrar_malla):MallaRevol("Sombrero revolucionado")
{
  std::vector<Tupla3f> vertices;

  for(int i=0; i<n; ++i)
  {
    if(i< n/4)
      vertices.push_back(Tupla3f(1.0, (float) i/n, 0.0));
    else
      vertices.push_back(Tupla3f(0.5, (float) i/n, 0.0));
  }

  nper = nperfiles;
  nvp = n;
  num_ver = nper * nvp;

  crearMallaRevol(vertices, nperfiles, crear_tapas, cerrar_malla );
}

// *****************************************************************************
