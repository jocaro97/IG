// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaRevol : public MallaInd
{
  protected:

   unsigned
      nper , // numero de perfiles
      nvp  ; // numero de vertices por perfil

      // Método que crea las tablas vertices y triangulos
   void crearMallaRevol( const std::vector<Tupla3f> & perfil_original,  // vértices del perfil original
                         const unsigned nperfiles,                      // número de perfiles
                         const bool crear_tapas,                        // true para crear tapas
                         const bool cerrar_malla,                       // true para cerrar la malla
                         const bool crear_texturas = false
   ) ;

   public:
      // crea una malla de revolucion
      MallaRevol(const std::string & nombre);

      MallaRevol( const std::string & nombre_arch,
                  const unsigned nperfiles,
                  const bool     crear_tapas,
                  const bool     cerrar_malla,
                  const bool     crear_texturas = false) ;

      void crearTablaTexturas();

} ;

class Cilindro : public MallaRevol
{
  public:
    Cilindro(int n, int nperfiles, bool crear_tapas, bool cerrar_malla);
};

class Esfera : public MallaRevol
{
  public:
    Esfera(int n, int nperfiles, bool crear_tapas, bool cerrar_malla);
};

class Cono : public MallaRevol
{
public:
  Cono(int n, int nperfiles, bool crear_tapas, bool cerrar_malla);
};

class Huevo : public MallaRevol
{
  public:
    Huevo(int n, int nperfiles, bool crear_tapas, bool cerrar_malla);
};

class Sombrero : public MallaRevol
{
public:
  Sombrero(int n, int nperfiles, bool crear_tapas, bool cerrar_malla);
};


#endif
