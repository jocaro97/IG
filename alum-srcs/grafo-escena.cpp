// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "aux.hpp"
#include "matrices-tr.hpp"
#include "shaders.hpp"
#include "grafo-escena.hpp"
#include "MallaRevol.hpp"

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
   // COMPLETAR: práctica 3: recorrer las entradas y visualizar el nodo
   // ........
   bool modo_iluminacion = cv.modoVis == modoIluminacionPlano || cv.modoVis == modoIluminacionSuave;

  cv.pilaMateriales.push();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  for (unsigned i = 0; i < entradas.size(); i++) {
    if (entradas[i].tipo == TipoEntNGE::objeto) {
      if (cv.modoSeleccionFBO && identificador >= 0) {
        FijarColorIdent(identificador);
      }
      entradas[i].objeto->visualizarGL(cv);
    }

    else if (entradas[i].tipo == TipoEntNGE::transformacion) {
      glMatrixMode(GL_MODELVIEW);
      glMultMatrixf(*(entradas[i].matriz));
    }

    else if (entradas[i].tipo == TipoEntNGE::material && modo_iluminacion) {
      cv.pilaMateriales.activarMaterial(entradas[i].material);
    }
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  cv.pilaMateriales.pop();

}
// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{
   // COMPLETAR: práctica 3: inicializar un nodo vacío (sin entradas)
   // ........

}
// -----------------------------------------------------------------------------

void NodoGrafoEscena::fijarColorNodo( const Tupla3f & nuevo_color )
{
   // COMPLETAR: práctica 3: asignarle un color plano al nodo, distinto del padre
   // ........

}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   // COMPLETAR: práctica 3: agregar la entrada al nodo, devolver índice de la entrada
   // ........
   entradas.push_back(entrada);
   return entradas.size() - 1;
}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // COMPLETAR: práctica 3: devolver puntero la matriz en ese índice
   //   (debe de dar error y abortar si no hay una matriz en esa entrada)
   // ........
   if(entradas[indice].tipo == TipoEntNGE::transformacion)
    if(entradas[indice].matriz != 0)
      return entradas[indice].matriz;

    exit(1);

}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........
   Matriz4f mm = MAT_Ident();
  vector<Tupla3f> centros_hijos;

  for (auto &entrada : entradas) {
    if (entrada.tipo == TipoEntNGE::objeto) {
      entrada.objeto->calcularCentroOC();
      centros_hijos.push_back(mm * entrada.objeto->leerCentroOC());
    }
    else if (entrada.tipo == TipoEntNGE::transformacion)
      mm = mm * (*entrada.matriz);
  }

  ponerCentroOC(calcularCentroCajaEnglobante(centros_hijos));
  centro_calculado = true;

  //PREGUNTAR

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int         ident_busc, // identificador a buscar
   const Matriz4f &  mmodelado,  // matriz de modelado
   Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
   Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // ........
   assert( 0 < ident_busc );

  if (identificador > 0 && !centro_calculado)  // no calcula los centros de cosas no seleccionables
    calcularCentroOC();

  if (identificador == ident_busc) {
    centro_wc = mmodelado * leerCentroOC();
    *objeto = this;
    return true;
  }

  else {
    bool found = false;
    Matriz4f nueva_mmodelado = mmodelado;
    for (unsigned i = 0; i < entradas.size() && !found; i++) {
      if (entradas[i].tipo == TipoEntNGE::transformacion)
        nueva_mmodelado = nueva_mmodelado * (*entradas[i].matriz);
      else if (entradas[i].tipo == TipoEntNGE::objeto)
        found = entradas[i].objeto->buscarObjeto(ident_busc, nueva_mmodelado, objeto, centro_wc);
    }
    return found;
}
  //PREGUNTAR
}

// *****************************************************************************
// Nodo del grafo de escena, con una lista añadida de parámetros
// *****************************************************************************


// -----------------------------------------------------------------------------
// devuelve el numero de grados de libertad
int NodoGrafoEscenaParam::numParametros()
{
   // COMPLETAR: práctica 3: indicar cuantos parámetros hay
   // ........
   return parametros.size();

}
// -----------------------------------------------------------------------------

// devuelve un puntero al i-ésimo grado de libertad
Parametro * NodoGrafoEscenaParam::leerPtrParametro( unsigned i )
{
   // COMPLETAR: práctica 3: devolver puntero al i-ésimo parámetro
   // ........
   return &parametros[i];

}
// -----------------------------------------------------------------------------

void NodoGrafoEscenaParam::siguienteCuadro()
{
   // COMPLETAR: práctica 3: actualizar todos los parámetros al siguiente cuadro
   // ........
   for( auto &p: parametros)
    p.siguiente_cuadro();


}

 Potato::Potato()
 {
   agregar(new Cuerpo);
   auto sombrero = new SombreroMov();
   agregar(sombrero);
   auto brazoizq = new Brazo();
   agregar(brazoizq);

   auto ojoizq = new Ojo();
   auto orejaizq = new Oreja();

   auto brazodch = new Brazo();

   agregar(ojoizq);
   agregar(orejaizq);

   auto ojodch = new Ojo();
   auto orejadch = new Oreja();

   agregar(MAT_Escalado(-1.0,1.0,1.0));
   agregar(ojodch);

   agregar(MAT_Escalado(1.0,1.0,-1.0));
   agregar(brazodch);
   agregar(orejadch);

   Parametro p11("rotación del brazo izquierdo", brazoizq->articulacionBrazo(),
               [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
               true, 90, 45, 1);

   Parametro p21("rotación del antebrazo izquierdo", brazoizq->articulacionAnteBrazo(),
                [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
                true, 0, 90, 1);

  Parametro p12("rotación del brazo derecho", brazodch->articulacionBrazo(),
                [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
                true, 90, 45, 1);

  Parametro p22("rotación del antebrazo derecho", brazodch->articulacionAnteBrazo(),
               [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
               true, 0, 90, 1);

    Parametro p3("Balanceo del sombrero", sombrero->balanceoSombrero(),
                 [=](float v) {return MAT_Rotacion(v,0,0,1);},
                 true, 0, 27, 1);

    Parametro p4("Rotacion del sombrero", sombrero->rotacionSombrero(),
                 [=](float v) {return MAT_Rotacion(v,0,1,0);},
                 false, 0, 360, 1);

    Parametro p51("Rotacion del ojo izquierdo", ojoizq->movCircularOjo(),
                [=](float v) {return MAT_Traslacion(v,1,v);},
                true, 0, 2, 1);

    Parametro p52("Rotacion del ojo derecho", ojodch->movCircularOjo(),
                [=](float v) {return MAT_Traslacion(v,1,v);},
                true, 0, 2, 1);

    Parametro p61("Rotacion de la oreja izquierda", orejaizq->movOreja(),
                  [=](float v) {return MAT_Rotacion(v,0,1,0);},
                  true, 0, 45, 1);

    Parametro p62("Rotacion de la oreja derecha", orejadch->movOreja(),
                  [=](float v) {return MAT_Rotacion(v,0,1,0);},
                  true, 0, 45, 1);


  parametros.push_back(p11);
  parametros.push_back(p12);
  parametros.push_back(p21);
  parametros.push_back(p22);
  parametros.push_back(p3);
  parametros.push_back(p4);
  parametros.push_back(p51);
  parametros.push_back(p52);
  parametros.push_back(p61);
  parametros.push_back(p62);

 }

 Cuerpo::Cuerpo()
 {
   ponerNombre("cuerpo del huevo");

   agregar(MAT_Rotacion(180, 1, 0, 0));
   agregar( MAT_Traslacion(0, -4, 0));
   auto h = new Huevo(20,50, true, true);

   Tupla3f t = {0.435294118,0.309803922,0.156862745};
   h -> colorear(&t);

   agregar(h);
 }

 Brazo::Brazo()
 {
   auto c = new Cilindro(20,50,true,true);
   auto e = new Esfera(50,50, true, true);
   Tupla3f t = {0.349019608,0.207843137,0.121568627};

   c -> colorear(&t);
   e -> colorear(&t);

   ponerNombre("Brazo del huevo");


   agregar(MAT_Traslacion(-1.3, 2.0, 0.0));


   indicebrazo = agregar(MAT_Rotacion(90,0,0,1));

   agregar(MAT_Escalado(0.25,1.0,0.25));


   agregar(c); //brazo

   agregar(MAT_Traslacion(0.0,1.0,0.0));
   agregar(MAT_Escalado(0.5, 0.10, 0.5));
   agregar(e);

   agregar(MAT_Traslacion(0.0, 0.5,0.0));

   indiceantebrazo = agregar(MAT_Ident());

   agregar(MAT_Escalado(2.0,10.0,2.0));



   agregar(c);  // antebrazo

 }

 Matriz4f* Brazo::articulacionBrazo()
 {
   return leerPtrMatriz(indicebrazo);
 }

 Matriz4f* Brazo::articulacionAnteBrazo()
 {
   return leerPtrMatriz(indiceantebrazo);
 }

 SombreroMov::SombreroMov()
 {
   ponerNombre("Sombrero del huevo");

    agregar(MAT_Traslacion(0.0, 4.0, 0.0));

    balanceo = agregar(MAT_Ident());

    Matriz4f m2 = MAT_Ident();
    rotacion = agregar(m2);

    agregar(new Sombrero(20,50, true, true));

 }

 Matriz4f* SombreroMov::balanceoSombrero()
 {
   return leerPtrMatriz(balanceo);
 }

 Matriz4f* SombreroMov::rotacionSombrero()
 {
   return leerPtrMatriz(rotacion);
 }

 Ojo::Ojo()
 {
   ponerNombre("Ojo del potato");

   auto c = new Cilindro(20,50, true, false);
   Tupla3f t1(1.0,1.0,1.0);
   c -> colorear(&t1);

   auto e = new Esfera(20,50,true,true);
   Tupla3f t2(0.0,0.0,0.0);
   e -> colorear(&t2);

   agregar(MAT_Traslacion(-0.2, 2.1, 1.3));
   agregar(MAT_Rotacion(90,1,0,0));

   agregar(MAT_Escalado(0.25, 0.05, 0.25));

   agregar(c);

   agregar(MAT_Escalado(0.1, 1.0, 0.1));

   movcircular = agregar(MAT_Ident());

   agregar(e);

 }

 Matriz4f* Ojo::movCircularOjo()
 {
   return leerPtrMatriz(movcircular);
 }

 Oreja::Oreja()
 {
   ponerNombre("Oreja del potato");

   auto c = new Cilindro(20,50,true,true);
   Tupla3f t = {0.901960784,0.196078431,0.266666667};

   c -> colorear(&t);

   agregar(MAT_Traslacion(-1.3, 2.75, 0.0));

   movoreja = agregar(MAT_Ident());

   agregar(MAT_Rotacion(90,1,0,0));
   agregar(MAT_Escalado(0.35,0.05,1.0));

   agregar(c);

 }

 Matriz4f* Oreja::movOreja()
 {
   return leerPtrMatriz(movoreja);
 }

 Lata::Lata() {
   ponerNombre("lata coke");
   ponerIdentificador(1);

   agregar(MAT_Escalado(1.5, 1.5, 1.5));

   agregar(new MaterialTapasLata);
   auto tapa1 = new MallaRevol("../plys/lata-psup.ply", 30, false, false, false);
   tapa1->ponerIdentificador(-1);
   agregar(tapa1);

   agregar(new MaterialLata);
   auto cuerpo = new MallaRevol("../plys/lata-pcue.ply", 30, false, false, true);
   cuerpo->ponerIdentificador(-1);
   agregar(cuerpo);

   agregar(new MaterialTapasLata);
   auto tapa2 = new MallaRevol("../plys/lata-pinf.ply", 30, false, false, false);
   tapa2->ponerIdentificador(-1);
   agregar(tapa2);
 }

 Peones::PeonNegro::PeonNegro() {
   ponerNombre("peón negro");
   ponerIdentificador(2);

   agregar(new MaterialPeonNegro);
   agregar(new MallaRevol("../plys/peon.ply", 30, true, false, false));
 }

 Peones::PeonBlanco::PeonBlanco() {
   ponerNombre("peón blanco");
   ponerIdentificador(3);

   agregar(new MaterialPeonBlanco);
   agregar(new MallaRevol("../plys/peon.ply", 30, true, false, false));
 }

 Peones::PeonMadera::PeonMadera() {
   ponerNombre("peón madera");
   ponerIdentificador(4);

  agregar(new MaterialPeonMadera);
  agregar(new MallaRevol("../plys/peon.ply", 30, true, false, false));
 }

 Peones::Peones() {
   ponerNombre("peones");

   agregar(MAT_Escalado(0.5, 0.5, 0.5));
   agregar(new PeonNegro);
   agregar(MAT_Traslacion(2.0, 0.0, 1.0));
   agregar(new PeonBlanco);
   agregar(MAT_Traslacion(-4.0, 0.0, -2.0));
   agregar(new PeonMadera);
 }

 EscenaP5::EscenaP5(){
   ponerNombre("EscenaP5");

   agregar(new Lata);
   agregar(MAT_Traslacion(0.0, 0.0, 2.0));
   agregar(new Peones);

 }
