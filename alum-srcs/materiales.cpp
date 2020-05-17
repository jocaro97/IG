// *********************************************************************
// **
// ** Gestión de materiales y texturas (implementación)
// **
// ** Copyright (C) 2014 Carlos Ureña
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


#include "matrices-tr.hpp"
#include "materiales.hpp"

using namespace std ;

const bool trazam = false ;

//*********************************************************************

PilaMateriales::PilaMateriales()
{
   actual = nullptr ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarMaterial( Material * material )
{
   if ( material != actual )
   {
      actual = material ;
      if ( actual != nullptr )
         actual->activar();
   }
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarActual()
{
   if ( actual != nullptr )
      actual->activar() ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::push(  )
{
   pila.push_back( actual );
}
// -----------------------------------------------------------------------------

void PilaMateriales::pop(  )
{
   if ( pila.size() == 0 )
   {
      cout << "error: intento de hacer 'pop' de un material en una pila de materiales vacía." << endl << flush ;
      exit(1);
   }

   Material * anterior = pila[pila.size()-1] ;
   pila.pop_back();
   activarMaterial( anterior );  // cambia 'actual'
}

//**********************************************************************

Textura::Textura( const std::string & nombreArchivoJPG )
{
   // COMPLETAR: práctica 4: inicializar todas las variables
   // .....
   enviada = false;
   modo_gen_ct = mgct_desactivada;  // coord automáticas desactivadas por defecto
   imagen = new jpg::Imagen(nombreArchivoJPG);

}

// ---------------------------------------------------------------------

//----------------------------------------------------------------------

void Textura::enviar()
{
   // COMPLETAR: práctica 4: enviar la imagen de textura a la GPU
   // .......
   glGenTextures(1, &ident_textura);
   glBindTexture(GL_TEXTURE_2D, ident_textura);

   //tema 3 diapositiva 157
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imagen->tamX(), imagen->tamY(), GL_RGB, GL_UNSIGNED_BYTE, imagen->leerPixels());

   enviada = true;

}

//----------------------------------------------------------------------

Textura::~Textura( )
{
   using namespace std ;
   cout << "destruyendo textura...imagen ==" << imagen << endl ;
   if ( imagen != NULL )
      delete imagen ;

   imagen = NULL ;
   cout << "hecho (no hecho!)" << endl << flush ;
}

//----------------------------------------------------------------------
// por ahora, se asume la unidad de texturas #0

void Textura::activar(  )
{
   // COMPLETAR: práctica 4: enviar la textura a la GPU (solo la primera vez) y activarla
   // .......
   // Activar funcionalidad de openGL para texturas
   glEnable(GL_TEXTURE_2D);

   // Enviar a la GPU la primera vez
   if (!enviada)
     enviar();
   else
     glBindTexture(GL_TEXTURE_2D, ident_textura);

     //tema3 161 y 162
     // Generación automática de coordenadas
    if (modo_gen_ct == mgct_desactivada) {
      glDisable( GL_TEXTURE_GEN_S );
      glDisable( GL_TEXTURE_GEN_T );
    } else {
      glEnable(GL_TEXTURE_GEN_S);
      glEnable(GL_TEXTURE_GEN_T);

      if (modo_gen_ct == mgct_coords_objeto) {
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        glTexGenfv( GL_S, GL_OBJECT_PLANE, coefs_s );
        glTexGenfv( GL_T, GL_OBJECT_PLANE, coefs_t );
      }else {
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

        glTexGenfv( GL_S, GL_EYE_PLANE, coefs_s ) ;
        glTexGenfv( GL_T, GL_EYE_PLANE, coefs_t ) ;
      }
    }

}

TexturaXY::TexturaXY( const std::string & nom )
  : Textura(nom)
{
  modo_gen_ct = mgct_coords_ojo;  // por ejemplo

  coefs_s[0] = 1.0;
  coefs_s[1] = coefs_s[2] = coefs_s[3] = 0.0;

  coefs_t[1] = 1.0;
  coefs_t[0] = coefs_t[2] = coefs_t[3] = 0.0;
}
// *********************************************************************

Material::Material()
{
   iluminacion = false ;
   tex = NULL ;
   coloresCero() ;
}
// ---------------------------------------------------------------------

Material::Material( const std::string & nombreArchivoJPG )
{
   iluminacion    = false ;
   tex            = new Textura( nombreArchivoJPG ) ;

   coloresCero();

   del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   del.difusa    = VectorRGB( 0.5, 0.5, 0.5, 1.0 );
   del.especular = VectorRGB( 1.0, 1.0, 1.0, 1.0 );

   tra.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   tra.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   tra.difusa    = VectorRGB( 0.2, 0.2, 0.2, 1.0 );
   tra.especular = VectorRGB( 0.2, 0.2, 0.2, 1.0 );

}

// ---------------------------------------------------------------------
// crea un material usando textura y coeficientes: ka,kd,ks y exponente
// (la textura puede ser NULL, la ilum. queda activada)

Material::Material( Textura * text, float ka, float kd, float ks, float exp )
:  Material()
{
   // COMPLETAR: práctica 4: inicializar material usando text,ka,kd,ks,exp
   // .....
   iluminacion = true;
   tex = text;

   color =
   del.emision =
   tra.emision = {0.0, 0.0, 0.0, 1.0};

   del.ambiente =
   tra.ambiente = {ka, ka, ka, 1.0};

   del.difusa =
   tra.difusa = {kd, kd, kd, 1.0};

   del.especular =
   tra.especular = {ks, ks, ks, 1.0};

   del.exp_brillo =
   tra.exp_brillo = exp;

   ponerNombre("material con textura e iluminación") ;

 }

// ---------------------------------------------------------------------
// crea un material con un color único para las componentes ambiental y difusa
// en el lugar de textura (textura == NULL)
Material::Material( const Tupla3f & colorAmbDif, float ka, float kd, float ks, float exp )
{
   // COMPLETAR: práctica 4: inicializar material usando colorAmbDif,ka,kd,ks,exp
   // .....
   color = {colorAmbDif(R), colorAmbDif(G), colorAmbDif(B), 1.0};

   iluminacion = true;
   tex = NULL;

   del.emision =
   tra.emision = {0.0, 0.0, 0.0, 1.0};

   del.ambiente =
   del.difusa =
   tra.ambiente =
   tra.difusa = color;

   del.especular =
   tra.especular = {ks, ks, ks, 1.0};

   del.exp_brillo =
   tra.exp_brillo = exp;

   ponerNombre("material color plano, ilum.") ;
}
// ---------------------------------------------------------------------

Material::Material( const float r, const float g, const float b )
{
   // COMPLETAR: práctica 4: inicializar material usando un color plano sin iluminación
   // .....
   iluminacion = false;
   tex = nullptr;

   color = {r, g, b, 1.0};

}

//----------------------------------------------------------------------

void Material::coloresCero()
{
   const VectorRGB ceroRGBopaco(0.0,0.0,0.0,1.0);

   color         =

   del.emision   =
   del.ambiente  =
   del.difusa    =
   del.especular =

   tra.emision   =
   tra.ambiente  =
   tra.difusa    =
   tra.especular = ceroRGBopaco ;

   del.exp_brillo =
   tra.exp_brillo = 1.0 ;
}
//----------------------------------------------------------------------

Material::~Material()
{
   if ( tex != nullptr )
   {  delete tex ;
      tex = nullptr ;
   }
}
//----------------------------------------------------------------------

void Material::ponerNombre( const std::string & nuevo_nombre )
{
   nombre_mat = nuevo_nombre ;
}
//----------------------------------------------------------------------

std::string Material::nombre() const
{
   return nombre_mat ;
}
//----------------------------------------------------------------------

void Material::activar(  )
{
   // COMPLETAR: práctica 4: activar un material
   // .....
   // Activar textura
  if (tex == NULL)
    glDisable(GL_TEXTURE_2D);
  else
    tex->activar();

  if (iluminacion) {

    glEnable(GL_LIGHTING);

    // Cara delantera
    glMaterialfv(GL_FRONT, GL_EMISSION, del.emision); // M_E
    glMaterialfv(GL_FRONT, GL_AMBIENT, del.ambiente); // M_A
    glMaterialfv(GL_FRONT, GL_DIFFUSE, del.difusa); // M_D
    glMaterialfv(GL_FRONT, GL_SPECULAR, del.especular); // M_S
    glMaterialf(GL_FRONT, GL_SHININESS, del.exp_brillo); // e

    // Cara trasera
    glMaterialfv(GL_BACK, GL_EMISSION, tra.emision); // M_E
    glMaterialfv(GL_BACK, GL_AMBIENT, tra.ambiente); // M_A
    glMaterialfv(GL_BACK, GL_DIFFUSE, tra.difusa); // M_D
    glMaterialfv(GL_BACK, GL_SPECULAR, tra.especular); // M_S
    glMaterialf(GL_BACK, GL_SHININESS, tra.exp_brillo); // e

    //permite que la textura brille o algo asi
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

  } else {
    glDisable(GL_LIGHTING);
    glColor4fv(color);
  }



}

//**********************************************************************

FuenteLuz::FuenteLuz( const VectorRGB & p_color )
{
   //CError();

   if ( trazam )
      cout << "creando fuente de luz." <<  endl << flush ;

   // inicializar parámetros de la fuente de luz

   col_ambiente  = p_color ;
   col_difuso    = p_color ;
   col_especular = p_color ;

   ind_fuente = -1 ; // la marca como no activable hasta que no se le asigne indice

   //CError();
}

//----------------------------------------------------------------------

void FuenteLuz::activar()
{
   // COMPLETAR: práctica 4: activar una fuente de luz (en GL_LIGHT0 + ind_fuente)
   // .....

}

//----------------------------------------------------------------------

bool FuenteDireccional::gestionarEventoTeclaEspecial( int key )
{
   bool actualizar = true ;
   const float incr = 3.0f ;

   switch( key )
   {
      case GLFW_KEY_RIGHT:
         longi = longi+incr ;
         break ;
      case GLFW_KEY_LEFT:
         longi = longi-incr ;
         break ;
      case GLFW_KEY_UP:
         lati = std::min( lati+incr, 90.0f) ;
         break ;
      case GLFW_KEY_DOWN:
         lati = std::max( lati-incr, -90.0f ) ;
         break ;
      case GLFW_KEY_HOME:
         lati  = lati_ini ;
         longi = longi_ini ;
         break ;
      default :
         actualizar = false ;
         cout << "tecla no usable para la fuente de luz." << endl << flush ;
   }

   //if ( actualizar )
   //   cout << "fuente de luz cambiada: longi == " << longi << ", lati == " << lati << endl << flush ;
   return actualizar ;
}

FuenteDireccional::FuenteDireccional(float a_ini, float b_ini, const VectorRGB & p_color):longi_ini(a_ini), lati_ini(b_ini), longi(a_ini), lati(b_ini), FuenteLuz(p_color){}

void FuenteDireccional::activar(){

  if(ind_fuente >= 0){
    // Activar fuente
    glEnable(GL_LIGHT0+ind_fuente);

    // Configurar colores
    glLightfv(GL_LIGHT0+ind_fuente, GL_AMBIENT, col_ambiente);
    glLightfv(GL_LIGHT0+ind_fuente, GL_DIFFUSE, col_difuso);
    glLightfv(GL_LIGHT0+ind_fuente, GL_SPECULAR, col_especular);

    Tupla4f z = {0.0, 0.0, 1.0, 0.0};

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix() ;

    // (4) hacer M = Identidad
    glLoadIdentity();
    // (3) rotación longi (α) grados en torno a eje Y
    glRotatef( longi, 0.0, 1.0, 0.0 );
    // (2) rotación lati (β) grados en torno al eje X-
    glRotatef( lati, -1.0, 0.0, 0.0 );
    // (1) hacer l_i : = ( 0, 0, 1 ) (paralela eje Z+)
    glLightfv( GL_LIGHT0+ind_fuente, GL_POSITION, z);

    glPopMatrix();
  }


}

void FuenteDireccional::variarAngulo(unsigned angulo, float incremento){

  if (angulo == 0)
   longi += incremento;
 else {
   if (incremento > 0)
     lati = std::min(lati + incremento, 90.0f);
   else
     lati = std::max(lati + incremento, -90.0f);
 }

}

FuentePosicional::FuentePosicional(const Tupla3f & posicion, const VectorRGB & p_color): FuenteLuz(p_color), posicion(posicion){}

void FuentePosicional::activar(){

  if(ind_fuente >= 0){
    // Activar fuente
    glEnable(GL_LIGHT0+ind_fuente);

    // Configurar colores
    glLightfv(GL_LIGHT0+ind_fuente, GL_AMBIENT, col_ambiente);
    glLightfv(GL_LIGHT0+ind_fuente, GL_DIFFUSE, col_difuso);
    glLightfv(GL_LIGHT0+ind_fuente, GL_SPECULAR, col_especular);

    Tupla4f pos_homogenea = {posicion(X), posicion(Y), posicion(Z), 1.0};

    glLightfv( GL_LIGHT0+ind_fuente, GL_POSITION, pos_homogenea );
  }

}

//**********************************************************************
ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = 8 ;
}
//----------------------------------------------------------------------

void ColFuentesLuz::insertar( FuenteLuz * pf )  // inserta una nueva
{
   assert( pf != nullptr );

   pf->ind_fuente = vpf.size() ;
   vpf.push_back( pf ) ;
}
//----------------------------------------------------------------------
// activa una colección de fuentes de luz

void ColFuentesLuz::activar( unsigned id_prog )
{
   // COMPLETAR: práctica 4: activar una colección de fuentes de luz
   // .....
   glEnable(GL_LIGHTING);

  //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE); // true = observador local, false = inf

  for (unsigned i = 0; i < vpf.size(); i++)
    vpf[i]->activar();

  for (unsigned i = vpf.size(); i < max_num_fuentes; i++)
    glDisable(GL_LIGHT0+i);

}
//----------------------------------------------------------------------
FuenteLuz * ColFuentesLuz::ptrFuente( unsigned i )
{
   assert(i < vpf.size()) ;
   return vpf[i] ;
}
//----------------------------------------------------------------------
ColFuentesLuz::~ColFuentesLuz()
{
   for( unsigned i = 0 ; i < vpf.size() ; i++ )
   {
      assert( vpf[i] != NULL );
      delete vpf[i] ;
      vpf[i] = NULL ;
   }
}


ColeccionFuentesP4::ColeccionFuentesP4()
{
  const VectorRGB color1 = {0.4, 0.4, 0.4, 1.0};
  const VectorRGB color2 = {0.4, 0.4, 0.4, 1.0};

  insertar(new FuenteDireccional(-10.0, 30.0, color1));
  insertar(new FuentePosicional({0.0, 3.0, 3.0}, color2));
}

//------------------------------------------------------------------------

MaterialLata::MaterialLata()
  : Material(new Textura("../imgs/lata-coke.jpg"), 0.2, 1.0, 0.5, 1.0)
{

}

MaterialTapasLata::MaterialTapasLata()
  : Material(NULL, 0.2, 1.0, 0.5, 1.0)
{

}

MaterialPeonNegro::MaterialPeonNegro()
  : Material(NULL, 0.0, 0.05, 0.2, 1.0)
{

}

MaterialPeonBlanco::MaterialPeonBlanco()
  : Material(NULL, 0.8, 0.8, 1.0, 5.0)
{

}

MaterialPeonMadera::MaterialPeonMadera()
  : Material(new TexturaXY("../imgs/text-madera.jpg"), 0.2, 1.0, 0.4, 1.0)
{

}
