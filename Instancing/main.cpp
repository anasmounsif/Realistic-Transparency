#include <iostream>
#include <time.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include "SDL.h" 
#include "float.h"
#include "vector3D.h"
#include "point3D.h"
#include "color.h"
#include "ray.h"
#include "geometry.h"
#include "cylinder.h"
#include "sphere.h"
#include "disk.h"
#include "rectangle.h"
#include "triangle.h"
#include "instance.h"
#include "raster.h"
#include "object.h"
#include "camera.h"
#include "color.h"
#include "scene.h"
#include "material.h"
#include "mesh.h"

int const nx = 480;	// x  480
int const ny = 270;	// y  270
int const ns = 256;	// number of samples
int const nr = 1; // number ray per path

scene world;

using namespace std;
using namespace concurrency;


int fail = 0;

int init() {
	/* // Initialize SDL2. */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	/* Create the window where we will draw. */
	window = SDL_CreateWindow("Realistic Transparency", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nx, ny, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	/* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		SDL_DestroyWindow(window);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	return 0;
}

void close() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void add_model(char* path, material* material) {
	object* model = new mesh(path);
	instance* mesh = new instance(model, material);
	world.addObject(mesh);
}

void martini() {

	// ***************** Camera *********************************
	point3D lookfrom(-0.5f, 7.6f, 9.0f);
	point3D lookat(0.0f, 3.9f, 3.0f);
	vector3D up(0, 1, 0);
	world.setCamera(lookfrom, lookat, up, 45, nx, ny, ns, nr);
	// **********************************************************

	// ***************** Room ***********************************

	color color_intensity = color(9.0f, 9.0f, 9.0f);
	material* light = new diffuse_light(new constant_texture(color_intensity));
	material* white = new lambertian(new constant_texture(color(0.73, 0.73, 0.73)));

	// Usa profondità, larghezza e altezza per cambiare forma alla stanza.
	// Lungo z - Profondità
	float depth11 = 15.0f; // z positive
	float depth12 = 3.0f; // z negative, uguale di default
	// Lungo x - Larghezza
	float width11 = 15.0f; // x positive
	float width12 = width11; // x negative, uguale di default
	//lungo y - Altezza
	float height11 = 20.0f; // y positive
	float height12 = 0.0f; // y negative, uguale di default

	material* texture_backwall = new lambertian(new image_texture("../texture/martini/texture_back.JPG"));
	material* texture_floor = new lambertian(new image_texture("../texture/martini/texture_floor.jpg"));

	// Back wall
	object* rectangle_model = new rectangle(point3D(-width12, -height12, -depth12), point3D(width11, -height12, -depth12), point3D(width11, height11, -depth12), point3D(-width12, height11, -depth12));
	instance* rectangle_ptr = new instance(rectangle_model, texture_backwall);
	world.addObject(rectangle_ptr);

	// Front wall
	rectangle_model = new rectangle(point3D(width11, -height12, depth11), point3D(-width12, -height12, depth11), point3D(-width12, height11, depth11), point3D(width11, height11, depth11));
	rectangle_ptr = new instance(rectangle_model, white);
	world.addObject(rectangle_ptr);

	// Left wall
	rectangle_model = new rectangle(point3D(-width12, -height12, depth11), point3D(-width12, -height12, -depth12), point3D(-width12, height11, -depth12), point3D(-width12, height11, depth11));
	rectangle_ptr = new instance(rectangle_model, white);
	world.addObject(rectangle_ptr);

	// Right wall
	rectangle_model = new rectangle(point3D(width11, -height12, -depth12), point3D(width11, -height12, depth11), point3D(width11, height11, depth11), point3D(width11, height11, -depth12));
	rectangle_ptr = new instance(rectangle_model, white);
	world.addObject(rectangle_ptr);

	// Ceiling
	rectangle_model = new rectangle(point3D(width11, height11, -depth12), point3D(width11, height11, depth11), point3D(-width12, height11, depth11), point3D(-width12, height11, -depth12));
	rectangle_ptr = new instance(rectangle_model, white);
	world.addObject(rectangle_ptr);

	// Floor
	rectangle_model = new rectangle(point3D(width11, -height12, depth11), point3D(width11, -height12, -depth12), point3D(-width12, -height12, -depth12), point3D(-width12, -height12, depth11));
	rectangle_ptr = new instance(rectangle_model, texture_floor);
	world.addObject(rectangle_ptr);

	//********************************************************************

	//**************** Luci ******************************
	// Luce Sferica Right
	float offeset_ceiling = 5.0f;
	float height_lights = abs(height11 - offeset_ceiling);
	object* sphere_model = new sphere(point3D(0.0, 0.0, 0.0), 2.0f);
	instance* sphere_ptr = new instance(sphere_model, light);
	sphere_ptr->translate(4.5f, height_lights, 0.0f);
	world.addObject(sphere_ptr);

	// Luce Sferica Left
	sphere_model = new sphere(point3D(0.0, 0.0, 0.0), 2.0f);
	sphere_ptr = new instance(sphere_model, light);
	sphere_ptr->translate(-4.5f, height_lights, 0.0f);
	world.addObject(sphere_ptr);

	// Luce Sferica Top
	sphere_model = new sphere(point3D(0.0, 0.0, 0.0), 2.0f);
	sphere_ptr = new instance(sphere_model, light);
	sphere_ptr->translate(0.0f, height_lights, 0.0f);
	world.addObject(sphere_ptr);

	//// Luce Sferica Bottom
	sphere_model = new sphere(point3D(0.0, 0.0, 0.0), 2.0f);
	sphere_ptr = new instance(sphere_model, light);
	sphere_ptr->translate(0.0f, height_lights, 4.5f);
	world.addObject(sphere_ptr);

	//*****************************************************

	//***************** Bicchiere **************************************************

	material* red = new lambertian(new constant_texture(color(0.65, 0.05, 0.05)));
	material* green = new lambertian(new constant_texture(color(0.12, 0.45, 0.15)));
	material* blue = new lambertian(new constant_texture(color(0.12, 0.15, 0.45)));
	material* celeste = new lambertian(new constant_texture(color(0.6, 0.18, 1.0)));
	material* legno = new lambertian(new constant_texture(color(0.87, 0.72, 0.53)));

	float aria = 1.0f;
	float vetro = 1.5f;
	//float acqua = 1.333f;
	float alcool = 1.365f;

	color neutro(1.0f, 1.0f, 1.0f);
	color martini(0.73f, 0.71f, 0.52f);

	material* vetro_aria = new dielectric(vetro, aria, neutro, neutro);
	material* alcool_aria = new dielectric(alcool, aria, martini, neutro);
	material* alcool_vetro = new dielectric(alcool, vetro, neutro, martini);

	// Disco liquido superiore
	object* model3d_disco_sup = new mesh("../models/martini/contatto_liquido_aria.obj");
	instance* mesh_ptr_disco_sup = new instance(model3d_disco_sup, alcool_aria);
	world.addObject(mesh_ptr_disco_sup);

	// Parte superiore
	object* model3d_bicchiere_sup = new mesh("../models/martini/bicchiere_superiore.obj");
	instance* mesh_ptr_bicchiere_sup = new instance(model3d_bicchiere_sup, vetro_aria);
	world.addObject(mesh_ptr_bicchiere_sup);

	// Oliva superiore
	object* model3d_oliva_sup = new mesh("../models/martini/oliva_grande.obj");
	material* texture_oliva_sup = new lambertian(new image_texture("../texture/martini/texture_oliva.jpg"));
	instance* mesh_ptr_oliva_sup = new instance(model3d_oliva_sup, texture_oliva_sup);
	world.addObject(mesh_ptr_oliva_sup);

	// Oliva inferiore
	object* model3d_oliva_inf = new mesh("../models/martini/oliva_piccola.obj");
	material* texture_oliva_inf = new lambertian(new image_texture("../texture/martini/texture_oliva.jpg"));
	instance* mesh_ptr_oliva_inf = new instance(model3d_oliva_inf, texture_oliva_inf);
	world.addObject(mesh_ptr_oliva_inf);

	// Liquido
	object* model3d_liquido_sup = new mesh("../models/martini/liquido.obj");
	instance* mesh_ptr_liquido_sup = new instance(model3d_liquido_sup, alcool_vetro);
	world.addObject(mesh_ptr_liquido_sup);

	// Stuzzicadenti
	object* model3d_stuzzicandenti = new mesh("../models/martini/stuzzicadenti.obj");
	instance* mesh_ptr_stuzzicandenti = new instance(model3d_stuzzicandenti, legno);
	world.addObject(mesh_ptr_stuzzicandenti);

	// Bicchiere Inferiore
	object* model3d_bicchiere_inf = new mesh("../models/martini/bicchiere_inferiore_esterno.obj");
	instance* mesh_ptr_bicchiere_inf = new instance(model3d_bicchiere_inf, vetro_aria);
	world.addObject(mesh_ptr_bicchiere_inf);

	// Stelo
	object* model3d_stelo = new mesh("../models/martini/stelo.obj");
	instance* mesh_ptr_stelo = new instance(model3d_stelo, vetro_aria);
	world.addObject(mesh_ptr_stelo);

	//***************************************************************************

}

void cocktail_wine() {

	int scene = 1; // 1 for Wine Scene, else for Cocktail Scene

	/* ENVIRONMENTS ------ */

	// Colors
	color neutral(1.0f, 1.0f, 1.0f);
	color liquid(0.6f, 0.2f, 0.2f);
	color color_intensity(8.0f, 8.0f, 8.0f);
	color black(0.0f, 0.0f, 0.0f);
	color white(0.73, 0.73, 0.73);

	// Refractive Indices
	float air, glass, water, alcool, ice;
	air = 1.0f;
	glass = 1.5f;
	water = 1.333f;
	alcool = 1.365f;
	ice = 1.31f;

	// Materials
	material* lambertian_black = new lambertian(new constant_texture(black));
	material* dielettric_glass_air = new dielectric(glass, air, neutral, neutral);
	material* dielettric_glass_water = new dielectric(alcool, glass, neutral, liquid);
	material* dielettric_water_air = new dielectric(alcool, air, liquid, neutral);
	material* dielettric_ice_water = new dielectric(ice, alcool, neutral, liquid);
	material* texture = new lambertian(new image_texture("../texture/cocktail_wine/wall.jpg"));
	material* texture_floor = new lambertian(new image_texture("../texture/cocktail_wine/floor.jpg"));
	material* light = new diffuse_light(new constant_texture(color_intensity));
	material* white_material = new lambertian(new constant_texture(white));

	/* SCENE ------ */

	vector3D up(0, 1, 0);
	point3D lookfrom, lookat;

	switch (scene)
	{
		
	case 1:
		// Wine Scene
		lookfrom = point3D(0.0f, 2.0f, 7.2f);
		lookat = point3D(0.0f, 1.9f, 0.0f);
		add_model("../models/wine/GLASS.obj", dielettric_glass_air);
		add_model("../models/wine/CONTACT_liquid_air.obj", dielettric_water_air);
		add_model("../models/wine/LIQUID.obj", dielettric_glass_water);
		break;
		
	default:
		// Cocktail Scene
		lookfrom = point3D(0.0f, 4.0f, 7.2f);
		lookat = point3D(0.0f, 2.65f, 0.0f);
		add_model("../models/cocktail/GLASS_top.obj", dielettric_glass_air);
		add_model("../models/cocktail/GLASS_bottom.obj", dielettric_glass_air);
		add_model("../models/cocktail/GLASS_contact.obj", dielettric_water_air);
		add_model("../models/cocktail/GLASS_liquid.obj", dielettric_glass_water);
		add_model("../models/cocktail/ICE.obj", dielettric_ice_water);
		add_model("../models/cocktail/STRAW.obj", lambertian_black);
		break;
	}

	world.setCamera(lookfrom, lookat, up, 45, nx, ny, ns, nr);

	/* ROOM ------ */

	// Depth
	float depth11 = 30.0f; // z positive
	float depth12 = 5.0; // z negative
	// Width
	float width11 = 12.0f; // x positive
	float width12 = width11; // x negative
	// Height
	float height11 = 20.0f; // y positive
	float height12 = 0.0f; // y negative

	// Front wall
	object* rectangle_model = new rectangle(point3D(-width12, -height12, -depth12), point3D(width11, -height12, -depth12), point3D(width11, height11, -depth12), point3D(-width12, height11, -depth12));
	instance* rectangle_ptr = new instance(rectangle_model, texture);
	world.addObject(rectangle_ptr);

	// Back wall
	rectangle_model = new rectangle(point3D(width11, -height12, depth11), point3D(-width12, -height12, depth11), point3D(-width12, height11, depth11), point3D(width11, height11, depth11));
	rectangle_ptr = new instance(rectangle_model, texture);
	world.addObject(rectangle_ptr);

	// Left wall
	rectangle_model = new rectangle(point3D(-width12, -height12, depth11), point3D(-width12, -height12, -depth12), point3D(-width12, height11, -depth12), point3D(-width12, height11, depth11));
	rectangle_ptr = new instance(rectangle_model, texture);
	world.addObject(rectangle_ptr);

	// Right wall
	rectangle_model = new rectangle(point3D(width11, -height12, -depth12), point3D(width11, -height12, depth11), point3D(width11, height11, depth11), point3D(width11, height11, -depth12));
	rectangle_ptr = new instance(rectangle_model, texture);
	world.addObject(rectangle_ptr);

	// Ceiling
	rectangle_model = new rectangle(point3D(width11, height11, -depth12), point3D(width11, height11, depth11), point3D(-width12, height11, depth11), point3D(-width12, height11, -depth12));
	rectangle_ptr = new instance(rectangle_model, white_material);
	world.addObject(rectangle_ptr);

	// Floor
	rectangle_model = new rectangle(point3D(width11, -height12, depth11), point3D(width11, -height12, -depth12), point3D(-width12, -height12, -depth12), point3D(-width12, -height12, depth11));
	rectangle_ptr = new instance(rectangle_model, texture_floor);

	/* LIGHTS ------ */

	world.addObject(rectangle_ptr);

	// Depth
	depth11 = 3.5f; // z positive
	depth12 = depth11; // z negative
	// Width
	width11 = 2.0f; // x positive
	width12 = width11; // x negative
	// Height
	height11 = 8.0f; // y positive

	rectangle_model = new rectangle(point3D(width11, height11, -depth12), point3D(width11, height11, depth11), point3D(-width12, height11, depth11), point3D(-width12, height11, -depth12));
	rectangle_ptr = new instance(rectangle_model, light);
	rectangle_ptr->rotate_x(60.0f);
	rectangle_ptr->rotate_y(-60.0f);
	world.addObject(rectangle_ptr);

}

void shot() {

	// *** Colori
	color neutro(1.0f, 1.0f, 1.0f);
	color verde_vivace(0.0313725490196078f, 0.992156862745098f, 0.0470588235294118f);
	color celeste_vivace(0.1019607843137255f, 0.807843137254902f, 0.9647058823529412f);
	color viola_vivace(0.6470588235294118f, 0.1058823529411765f, 0.9607843137254902f);
	// ***


	/// *** Materiali
	float aria = 1.0f;
	float vetro = 1.5f;
	float acqua = 1.333f;
	float alcool = 1.365f;

	// Parte Superiore & Parte Inferiore
	material* vetro_aria = new dielectric(vetro, aria, neutro, neutro);

	// Disco liquido superiore
	material* acqua_aria = new dielectric(alcool, aria, celeste_vivace, neutro);

	// Liquido Superiore
	material* vetro_acqua1 = new dielectric(alcool, vetro, neutro, celeste_vivace);

	// Disco liquido-liquido superiore
	material* acqua_acqua_sup = new dielectric(alcool, alcool, celeste_vivace, verde_vivace);

	// Liquido Intemedio
	material* vetro_acqua2 = new dielectric(alcool, vetro, neutro, verde_vivace);

	// Disco liquido-liquido inferiore
	material* acqua_acqua_inf = new dielectric(alcool, alcool, viola_vivace, verde_vivace);

	// Liquido Inferiore
	material* vetro_acqua_inf = new dielectric(alcool, vetro, neutro, viola_vivace);
	/// ***


	/// *** Cicchetto
	// Parte superiore
	object* model3d_bicchiere_sup = new mesh("../models/shot/Bicchiere_superiore.obj");
	instance* mesh_ptr_bicchiere_sup = new instance(model3d_bicchiere_sup, vetro_aria);
	world.addObject(mesh_ptr_bicchiere_sup);

	// Disco liquido superiore
	object* model3d_disco_sup = new mesh("../models/shot/Liquido_aria.obj");
	instance* mesh_ptr_disco_sup = new instance(model3d_disco_sup, acqua_aria);
	world.addObject(mesh_ptr_disco_sup);

	// Liquido Superiore
	object* model3d_liquido_sup = new mesh("../models/shot/liquido_3_strati/Liquido_superiore.obj");
	instance* mesh_ptr_liquido_sup = new instance(model3d_liquido_sup, vetro_acqua1);
	world.addObject(mesh_ptr_liquido_sup);

	// Disco liquido-liquido superiore
	object* model3d_disco_liquido_liquido_sup = new mesh("../models/shot/liquido_3_strati/Liquido_Liquido_superiore.obj");
	instance* mesh_ptr_disco_liquido_liquido_sup = new instance(model3d_disco_liquido_liquido_sup, acqua_acqua_sup);
	world.addObject(mesh_ptr_disco_liquido_liquido_sup);

	// Liquido Intermedio
	object* model3d_liquido_intermedio = new mesh("../models/shot/liquido_3_strati/Liquido_intermedio.obj");
	instance* mesh_ptr_liquido_intermedio = new instance(model3d_liquido_intermedio, vetro_acqua2);
	world.addObject(mesh_ptr_liquido_intermedio);

	// Disco liquido-liquido inferiore
	object* model3d_disco_liquido_liquido_inf = new mesh("../models/shot/liquido_3_strati/Liquido_Liquido_inferiore.obj");
	instance* mesh_ptr_disco_liquido_liquido_inf = new instance(model3d_disco_liquido_liquido_inf, acqua_acqua_inf);
	world.addObject(mesh_ptr_disco_liquido_liquido_inf);

	// Liquido Inferiore
	object* model3d_liquido_inf = new mesh("../models/shot/liquido_3_strati/Liquido_inferiore.obj");
	instance* mesh_ptr_liquido_inf = new instance(model3d_liquido_inf, vetro_acqua_inf);
	world.addObject(mesh_ptr_liquido_inf);

	// Parte inferiore
	object* model3d_bicchiere_inf = new mesh("../models/shot/Bicchiere_inferiore.obj");
	instance* mesh_ptr_bicchiere_inf = new instance(model3d_bicchiere_inf, vetro_aria);
	world.addObject(mesh_ptr_bicchiere_inf);
	// ***
}

void room_shot() {

	// Camera
	point3D lookfrom(0.0f, 1.9f, 4.0f);

	point3D lookat(0.0f, 1.3f, 0.0f);
	vector3D up(0, 1, 0);
	world.setCamera(lookfrom, lookat, up, 45, nx, ny, ns, nr);


	/// Room
	color color_intensity = color(4.0f, 4.0f, 4.0f);
	material* light = new diffuse_light(new constant_texture(color_intensity));
	material* white = new lambertian(new constant_texture(color(0.73, 0.73, 0.73)));


	// Usa profondità, larghezza e altezza per cambiare forma alla stanza.
	// Lungo z - Profondità
	float depth11 = 20.0f; // z positive
	float depth12 = depth11; // z negative, uguale di default
	// Lungo x - Larghezza
	float width11 = 18.0f; // x positive
	float width12 = width11; // x negative, uguale di default
	//lungo y - Altezza
	float height11 = 11.2f; // y positive
	float height12 = height11; // y negative, uguale di default


	material* sfondo = new lambertian(new image_texture("../texture/shot/sfondo.jpg"));
	material* pavimento = new lambertian(new image_texture("../texture/shot/pavimento5.jpg"));

	// Back wall
	object* rectangle_model = new rectangle(point3D(-width12, -height12, -depth12), point3D(width11, -height12, -depth12), point3D(width11, height11, -depth12), point3D(-width12, height11, -depth12));
	instance* rectangle_ptr = new instance(rectangle_model, sfondo);
	world.addObject(rectangle_ptr);


	// Floor Texture
	float height = 0.0f; // lungo y
	float width = 11.5;

	rectangle_model = new rectangle(point3D(-width, height, width), point3D(width, height, width), point3D(width, height, -width), point3D(-width, height, -width));
	rectangle_ptr = new instance(rectangle_model, pavimento);
	world.addObject(rectangle_ptr);

	// Front wall
	rectangle_model = new rectangle(point3D(width11, -height12, depth11), point3D(-width12, -height12, depth11), point3D(-width12, height11, depth11), point3D(width11, height11, depth11));
	rectangle_ptr = new instance(rectangle_model, white);
	world.addObject(rectangle_ptr);

	// Left wall
	rectangle_model = new rectangle(point3D(-width12, -height12, depth11), point3D(-width12, -height12, -depth12), point3D(-width12, height11, -depth12), point3D(-width12, height11, depth11));
	rectangle_ptr = new instance(rectangle_model, white);
	world.addObject(rectangle_ptr);


	// Right wall
	rectangle_model = new rectangle(point3D(width11, -height12, -depth12), point3D(width11, -height12, depth11), point3D(width11, height11, depth11), point3D(width11, height11, -depth12));
	rectangle_ptr = new instance(rectangle_model, white);
	world.addObject(rectangle_ptr);

	// Ceiling
	rectangle_model = new rectangle(point3D(width11, height11, -depth12), point3D(width11, height11, depth11), point3D(-width12, height11, depth11), point3D(-width12, height11, -depth12));
	rectangle_ptr = new instance(rectangle_model, white);
	world.addObject(rectangle_ptr);

	// Floor
	rectangle_model = new rectangle(point3D(width11, -height12, depth11), point3D(width11, -height12, -depth12), point3D(-width12, -height12, -depth12), point3D(-width12, -height12, depth11));
	rectangle_ptr = new instance(rectangle_model, pavimento);
	world.addObject(rectangle_ptr);


	//// Luci
	// Luce Sferica Right
	float offeset_ceiling = 2.0f;
	float height_lights = abs(height11 - offeset_ceiling);
	object* sphere_model = new sphere(point3D(0.0, 0.0, 0.0), 2.0f);
	instance* sphere_ptr = new instance(sphere_model, light);
	sphere_ptr->translate(4.2f, height_lights, 0.0f);
	world.addObject(sphere_ptr);

	// Luce Sferica Left
	sphere_model = new sphere(point3D(0.0, 0.0, 0.0), 2.0f);
	sphere_ptr = new instance(sphere_model, light);
	sphere_ptr->translate(-4.2f, height_lights, 0.0f);
	world.addObject(sphere_ptr);

	// Luce Sferica Top sx
	sphere_model = new sphere(point3D(0.0, 0.0, 0.0), 2.0f);
	sphere_ptr = new instance(sphere_model, light);
	sphere_ptr->translate(-4.2f, height_lights, -6.5f);
	world.addObject(sphere_ptr);

	// Luce Sferica Bottom dx
	sphere_model = new sphere(point3D(0.0, 0.0, 0.0), 2.0f);
	sphere_ptr = new instance(sphere_model, light);
	sphere_ptr->translate(4.2f, height_lights, -6.5f);
	world.addObject(sphere_ptr);

	// Luce Sferica Bottom
	sphere_model = new sphere(point3D(0.0, 0.0, 0.0), 2.0f);
	sphere_ptr = new instance(sphere_model, light);
	sphere_ptr->translate(0.0f, height_lights, 4.2f);
	world.addObject(sphere_ptr);

}

void scene_shot() {
	shot();
	room_shot();
}

void lucano() {

	// *** Colori
	color neutro(1.0f, 1.0f, 1.0f);
	color amaro(0.3058823529411765f, 0.0f, 0.0f);
	// ***

	/// *** Materiali
	float aria = 1.0f;
	float vetro = 1.5f;
	float acqua = 1.333f;
	float alcool = 1.365f;
	float ghiaccio = 1.31;


	// Parte Superiore & Parte Inferiore
	material* vetro_aria = new dielectric(vetro, aria, neutro, neutro);

	// Disco liquido superiore
	material* acqua_aria = new dielectric(alcool, aria, amaro, neutro);

	// Liquido
	material* vetro_acqua = new dielectric(alcool, vetro, neutro, amaro);

	// Ghiaccio
	material* ghiaccio_alcool = new dielectric(ghiaccio, alcool, neutro, amaro);
	/// ***

	/// *** Amaro Lucano
	// Parte superiore
	object* model3d_bicchiere_sup = new mesh("../models/shot/Bicchiere_superiore.obj");
	instance* mesh_ptr_bicchiere_sup = new instance(model3d_bicchiere_sup, vetro_aria);
	world.addObject(mesh_ptr_bicchiere_sup);

	// Disco liquido superiore
	object* model3d_disco_sup = new mesh("../models/shot/Liquido_aria.obj");
	instance* mesh_ptr_disco_sup = new instance(model3d_disco_sup, acqua_aria);
	world.addObject(mesh_ptr_disco_sup);

	// Ghiaccio
	object* model3d_ghiaccio = new mesh("../models/shot/Ghiaccio.obj");
	instance* mesh_ptr_ghiaccio = new instance(model3d_ghiaccio, ghiaccio_alcool);
	world.addObject(mesh_ptr_ghiaccio);

	// Liquido Superiore
	object* model3d_liquido_sup = new mesh("../models/shot/Liquido_mod/Liquido.obj");
	instance* mesh_ptr_liquido_sup = new instance(model3d_liquido_sup, vetro_acqua);
	world.addObject(mesh_ptr_liquido_sup);

	// Parte inferiore
	object* model3d_bicchiere_inf = new mesh("../models/shot/Bicchiere_inferiore.obj");
	instance* mesh_ptr_bicchiere_inf = new instance(model3d_bicchiere_inf, vetro_aria);
	world.addObject(mesh_ptr_bicchiere_inf);
	/// ***
}

void room_lucano() {

	//Camera
	point3D lookfrom(0.0f, 1.7f, 4.0f);

	point3D lookat(0.0f, 1.1f, 0.0f);
	vector3D up(0, 1, 0);
	world.setCamera(lookfrom, lookat, up, 45, nx, ny, ns, nr);


	/// Stanza
	color color_intensity = color(4.0f, 4.0f, 4.0f);
	material* light = new diffuse_light(new constant_texture(color_intensity));
	material* white = new lambertian(new constant_texture(color(0.73, 0.73, 0.73)));

	// Usa profondità, larghezza e altezza per cambiare forma alla stanza.
	// Lungo z - Profondità
	float depth11 = 5.0f; // z positive
	float depth12 = 1.9; // z negative
	// Lungo x - Larghezza
	float width11 = 10.0f; // x positive
	float width12 = width11; // x negative
	//lungo y - Altezza
	float height11 = 10.0f; // y positive
	float height12 = height11; // y negative


	material* pavimento = new lambertian(new image_texture("../texture/lucano/pavimento_amaro9.jpg"));
	material* sfondo = new lambertian(new image_texture("../texture/lucano/parete_amaro6.jpg"));

	// Front wall
	object* rectangle_model = new rectangle(point3D(-width12, height11, depth11), point3D(width11, height11, depth11), point3D(width11, -height12, depth11), point3D(-width12, -height12, depth11));
	instance* rectangle_ptr = new instance(rectangle_model, sfondo);
	world.addObject(rectangle_ptr);

	// Right wall
	rectangle_model = new rectangle(point3D(width11, height11, depth11), point3D(width11, height11, -depth12), point3D(width11, -height12, -depth12), point3D(width11, -height12, depth11));
	rectangle_ptr = new instance(rectangle_model, sfondo);
	world.addObject(rectangle_ptr);

	// Left wall
	rectangle_model = new rectangle(point3D(-width12, height11, -depth12), point3D(-width12, height11, depth11), point3D(-width12, -height12, depth11), point3D(-width12, -height12, -depth12));
	rectangle_ptr = new instance(rectangle_model, sfondo);
	world.addObject(rectangle_ptr);

	// Back wall
	rectangle_model = new rectangle(point3D(width11, height11, -depth12), point3D(-width12, height11, -depth12), point3D(-width12, -height12, -depth12), point3D(width11, -height12, -depth12));
	rectangle_ptr = new instance(rectangle_model, sfondo);
	world.addObject(rectangle_ptr);

	// Floor
	rectangle_model = new rectangle(point3D(-width12, -height12, -depth12), point3D(-width12, -height12, depth11), point3D(width11, -height12, depth11), point3D(width11, -height12, -depth12));
	rectangle_ptr = new instance(rectangle_model, white);
	world.addObject(rectangle_ptr);

	// Ceiling
	rectangle_model = new rectangle(point3D(-width12, height11, depth11), point3D(-width12, height11, -depth12), point3D(width11, height11, -depth12), point3D(width11, height11, depth11));
	rectangle_ptr = new instance(rectangle_model, white);
	world.addObject(rectangle_ptr);

	// Floor Texture
	float height = 0.0f; // lungo y
	float width = 7.0;

	rectangle_model = new rectangle(point3D(-width, height, width - 3.0f), point3D(width, height, width - 3.0f), point3D(width, height, -width), point3D(-width, height, -width));
	rectangle_ptr = new instance(rectangle_model, pavimento);
	rectangle_ptr->translate(0.0f, 0.0f, 3.5f);
	world.addObject(rectangle_ptr);

	//// Luce Rettangolare

	// Lungo z - Profondità
	depth11 = 1.5f; // z positive
	depth12 = depth11; // z negative, uguale di default
	// Lungo x - Larghezza
	width11 = 8.5f; // x positive
	width12 = width11; // x negative, uguale di default
	//lungo y - Altezza
	height11 = 5.5f; // y positive

	color color_light(0.9058823529411765f, 0.792156862745098f, 0.5882352941176471f);
	float intensity = 4.5;

	rectangle_model = new rectangle(point3D(width11, height11, -depth12), point3D(width11, height11, depth11), point3D(-width12, height11, depth11), point3D(-width12, height11, -depth12));
	rectangle_ptr = new instance(rectangle_model, new diffuse_light(new constant_texture(intensity * color_light)));
	rectangle_ptr->rotate_x(20.0f);
	rectangle_ptr->rotate_z(-30.0f);
	world.addObject(rectangle_ptr);
}

void scene_lucano() {
	lucano();
	room_lucano();
}

int main(int argc, char* argv[])
{
	if (init() == 1) {
		cout << "App Error! " << std::endl;
		return 1;
	}

	time_t start, end;
	time(&start);


	world.parallel_render();
	

	time(&end);
	double dif = difftime(end, start);
	printf("Rendering time: %.2lf seconds.", dif);
	printf("False points: %d", fail);

	SDL_Event event;
	bool quit = false;

	/* Poll for events */
	while (SDL_PollEvent(&event) || (!quit)) {

		switch (event.type) {

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
				// cases for other keypresses

			case SDLK_s:
				saveScreenshotBMP("../render/screenshot_" + to_string(start) + ".bmp");
				break;
			}
		}
	}

	close();
	return 0;
}

