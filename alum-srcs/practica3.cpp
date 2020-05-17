// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp"



using namespace std ;

// COMPLETAR: práctica 3: declarar variables de la práctica 3 (static)
// .................

static unsigned objetoActivo3 = 0 ; // objeto activo: malla ply (0), malla revol (1)
static constexpr int numObjetos3 = 1 ;

static NodoGrafoEscenaParam * objetos3[numObjetos3] = { nullptr };

static bool animacion = false;

static unsigned grado_activo = 0;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P3_Inicializar(  )
{
   cout << "Creando objetos de la práctica 3 .... " << flush ;
   // COMPLETAR: práctica 3: inicialización de objetos de la práctica 3
   // .................
   objetos3[0] = new Potato();

   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P3_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = false  ; // valor devuelto (true si se ha procesado tecla)

   switch ( toupper( tecla ) )
   {
      case 'O' :
         // COMPLETAR: práctica 3: activar siguiente objeto de la práctica
         //           (solo en el caso de que se incluyan varios objetos en la práctica)
         // ....
         objetoActivo3 = (objetoActivo3+1) % numObjetos3 ;
         cout << "práctica 3: nuevo objeto activo es: " << objetoActivo3 ;
         if ( objetos3[objetoActivo3] != nullptr )
            cout << " (" << objetos3[objetoActivo3]->leerNombre() << ")." << endl ;
         else
            cout << " (objeto no creado)" << endl ;
         res = true ;

         break ;

      case 'A' :
         // COMPLETAR: práctica 3: activar o desactivar animaciones
         // ....
         animacion = !animacion;

         if(animacion)
          FijarFuncDesocupado(FGE_Desocupado);

         if(animacion)
          std::cout << "Las animaciones están activas" << '\n';
        else
          std::cout << "Las animaciones están desactivadas" << '\n';

         res = true;

         break ;

      case 'G' :
         // COMPLETAR: práctica 3: activar siguiente parámetro
         // ....
         grado_activo = (grado_activo + 1) % (objetos3[objetoActivo3] -> numParametros());
         std::cout << "se cambió el grado de libertad activo por '"<< objetos3[objetoActivo3] -> leerPtrParametro(grado_activo) -> leer_descripcion()<<" ' " << '\n';

         res = true;
         break ;

      case 'R' :
         // COMPLETAR: práctica 3: reset de animaciones
         // ....
         animacion = false;

         for(unsigned i = 0; i < objetos3[objetoActivo3] -> numParametros(); ++i)
           objetos3[objetoActivo3] -> leerPtrParametro(i) -> reset();

         std::cout << "Animaciones desactivadas y todo reseteado." << '\n';

         res = true;

         break ;

      case '>' :
         // COMPLETAR: práctica 3: acelerar o incrementar parámetro
         // ....
         if(animacion){
           objetos3[objetoActivo3] -> leerPtrParametro(grado_activo) -> acelerar();
           std::cout << "animacion acelerada:'"<< objetos3[objetoActivo3] -> leerPtrParametro(grado_activo) -> leer_velocidad_actual() << "'" << '\n';
         }else{
           objetos3[objetoActivo3] -> leerPtrParametro(grado_activo) -> incrementar();
           std::cout << "valor incrementado: '"<< objetos3[objetoActivo3] -> leerPtrParametro(grado_activo) -> leer_valor_actual() << "'" << '\n';
         }

         res = true;

         break ;

      case '<' :
         // COMPLETAR: práctica 3: decelerar o decrementar parámetro
         // ....
         if(animacion){
           objetos3[objetoActivo3] -> leerPtrParametro(grado_activo) -> decelerar();
           std::cout << "animacion decelerada:'"<< objetos3[objetoActivo3] -> leerPtrParametro(grado_activo) -> leer_velocidad_actual() << "'" << '\n';
         }else{
           objetos3[objetoActivo3] -> leerPtrParametro(grado_activo) -> decrementar();
           std::cout << "valor decrementado: '"<< objetos3[objetoActivo3] -> leerPtrParametro(grado_activo) -> leer_valor_actual() << "'" << '\n';
         }

         res = true;

         break ;
      default :
         break ;

   }
   return res ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 3  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P3_DibujarObjetos( ContextoVis & cv )
{
   // COMPLETAR: práctica 3: visualizar el objeto de la práctica 3
   // ....
   objetos3[objetoActivo3] -> visualizarGL(cv);

}

//--------------------------------------------------------------------------

bool P3_FGE_Desocupado()
{
   // COMPLETAR: práctica 3: si las animaciones están activadas,
   // actualizar el objeto activo, forzar redibujado, devolver true.
   // si las animaciones están desactivadas, devolver false.
   // ....
   if (!animacion)
    return false;

  objetos3[objetoActivo3]->siguienteCuadro();

  redibujar_ventana = true;

  return true;

}
