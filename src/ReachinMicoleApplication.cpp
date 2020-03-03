/*
 * This file is part of Micole Architecture
 *
 * Copyright (C) 2007 Micole partners
 *
 * Micole Architecture is free software: you can redistribute it 
 * and/or modify it under the terms of the GNU Lesser General 
 * Public License as published by the Free Software Foundation, 
 * either version 3 of the License, or (at your option) any 
 * later version.
 *
 * Micole Architecture is distributed in the hope that it will be 
 * useful, * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU Lesser General Public License for more 
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Micole Architecture.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "stdafx.h"
#include "ReachinMicoleApplication.h"

/*#include <Shape.h>
#include <Sphere.h>
#include <Appearance.h>*/
#include <FrictionalSurface.h>

using namespace Reachin;

ReachinMicoleApplication * ReachinMicoleApplication::_riInstance = NULL;

ReachinMicoleApplication::ReachinMicoleApplication()
: _display(new Display())
{
	//ref(_display);
}

ReachinMicoleApplication::~ReachinMicoleApplication()
{
	
}

void ReachinMicoleApplication::startScene() 
{
	try {
		//AutoRef<Display> dis(new Display); //create AutoRef for Display
		/*
		Shape * sh = new Shape;

		Sphere * sph = new Sphere;

		FrictionalSurface * surf = new FrictionalSurface;

		sph->radius->set(0.05);

		surf->stiffness->set( 500 );

		Material * mat = new Material;
		mat->diffuse_color->set( RGB(0,1,0));

		Appearance * app = new Appearance;
		app->material->set(mat);
		app->surface->set( surf );

		sh->appearance->set(app);
		sh->geometry->set(sph);

		_display->children->add(sh);
		*/
		Scene::startScene();
	}
	catch (Error::QuitAPI) 
	{
		sendBusMessage("Quit");
		Scene::stopScenes();
	}
	catch (Error::ErrorBase &e) 
	{
		cerr << e << endl;
		sendBusMessage("Quit");
		Scene::stopScenes();
	}
}
