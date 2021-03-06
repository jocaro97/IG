// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp"
#include "materiales.hpp"


using namespace std ;

// COMPLETAR: práctica 4: declaración de variables de la práctica 4 (static)
// ....
static constexpr float DELTA = 3.0;
static constexpr int numObjetos4 = 2;
static unsigned objetoActivo4 = 0;
static NodoGrafoEscena * objetos4[numObjetos4] = {nullptr, nullptr};
static ColeccionFuentesP4 * cf4 = nullptr;
static unsigned angulo4 = 0;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar(  )
{
   cout << "Creando objetos de la práctica 4 .... " << flush ;

   // COMPLETAR: práctica 4: inicializar objetos de la práctica 4
   // ....
   cf4 = new ColeccionFuentesP4();
   objetos4[0] = new Lata();
   objetos4[1] = new Peones();

   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 4 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P4_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
  bool res = false  ; // valor devuelto: es true solo cuando se ha procesado alguna tecla
     FuenteDireccional * fuente_dir;

     switch ( toupper( tecla ) )
     {
        case 'O' :
           objetoActivo4 = (objetoActivo4 + 1) % numObjetos4 ;
           cout << "práctica 4: nuevo objeto activo es " << objetoActivo4 ;
           if ( objetos4[objetoActivo4] != nullptr )
              cout << " (" << objetos4[objetoActivo4]->leerNombre() << ")." << endl ;
           else
              cout << " (objeto no creado)" << endl ;

           res = true ;
           break ;

        case 'G' :
           angulo4 = 1 - angulo4;
           cout << "práctica 4: ángulo actual cambiado a " << angulo4 << endl;

           res = true;
           break ;

        case '>' :
           fuente_dir = (FuenteDireccional *) cf4->ptrFuente(0);
           fuente_dir->variarAngulo(angulo4, DELTA);
           cout << "práctica 4: ángulo " << angulo4 << " cambiado a "
                << (angulo4 ? fuente_dir->lati : fuente_dir->longi) << endl;

           res = true;
           break ;

        case '<' :
           fuente_dir = (FuenteDireccional *) cf4->ptrFuente(0);
           fuente_dir->variarAngulo(angulo4, -DELTA);
           cout << "práctica 4: ángulo " << angulo4 << " cambiado a "
                << (angulo4 ? fuente_dir->lati : fuente_dir->longi) << endl;

           res = true;
           break ;

        default :
           break ;
     }

     return res ;

}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 4  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P4_DibujarObjetos( ContextoVis & cv )
{
   // COMPLETAR: práctica 4: visualizar objetos
   //     (requiere activar las fuentes de luz y luego dibujar el grafo de escena)
   // ....
   cf4->activar(0);

   if (objetos4[objetoActivo4] != nullptr)
      objetos4[objetoActivo4]->visualizarGL(cv);

}
