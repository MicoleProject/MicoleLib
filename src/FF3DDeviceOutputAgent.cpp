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
#include "FF3DDeviceOutputAgent.h"
#include "define.h"


#include "FF3DSphericalForceModel.h"
#include "FF3DPointForceModel.h"
#include "FF3DDiscForceModel.h"
#include "FF3DLinearForceModel.h"
#include "FF3DPlanarForceModel.h"
#include "FF3DPicobHGForceModel.h"
#include "FF3DPicobGForceModel.h"

/*#include <Shape.h>
#include <Sphere.h>
#include <Cone.h>
#include <Appearance.h>*/
#include <FrictionalSurface.h>


FF3DDeviceOutputAgent::FF3DDeviceOutputAgent()
: Child( new FF3DDeviceOutputAgentCollider ), MicoleAgent("FF3DDeviceOutputAgent","Output"), _fm(NULL), _transform(new Transform())
{
	static_cast<FF3DDeviceOutputAgentCollider *>(this->collider.get())->_parent = this;

	//add FF3DDeviceOutputAgent in scene
	Display * dis = static_cast<Display *>(ReachinMicoleApplication::getInstance()->getDisplay());
	
	
	_transform->children->add(this);
	dis->children->add( _transform );  

	/** dummy for test*/
	/*
#ifdef _DEBUG
	Shape * sh = new Shape;
	Cone * sph = new Cone;

	//FrictionalSurface * surf = new FrictionalSurface;
	sph->height->set(0.05);
	sph->bottom_radius->set(0.01);

	//surf->stiffness->set( 500 );

	Material * mat = new Material;
	mat->diffuse_color->set( RGB(0,1,0));

	Appearance * app = new Appearance;
	app->material->set(mat);
	//app->surface->set( surf );

	sh->appearance->set(app);
	sh->geometry->set(sph);

	_transform->children->add(sh);
#endif
	*/
	//en test

	bindMessage( "^OUT FF3D : point=\\((.*), (.*), (.*)\\);$", 
		BUS_CALLBACK_OF(FF3DDeviceOutputAgent, handlePointForceModel ));
	bindMessage( "^OUT FF3D : sphere=\\((.*), (.*), (.*), (.*)\\);$", 
		BUS_CALLBACK_OF(FF3DDeviceOutputAgent, handleSphericalForceModel ));
	bindMessage( "^OUT FF3D : line=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(FF3DDeviceOutputAgent, handleLinearForceModel ));
	bindMessage( "^OUT FF3D : plan=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(FF3DDeviceOutputAgent, handlePlanarForceModel ));
	bindMessage( "^OUT FF3D : disc=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\),(.*)\\);$", 
		BUS_CALLBACK_OF(FF3DDeviceOutputAgent, handleDiscForceModel ));
	bindMessage( "^OUT FF3D : none;$", 
		BUS_CALLBACK_OF(FF3DDeviceOutputAgent, handleNoForceModel ));
	bindMessage( "^OUT FF3D : picobhg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\); bumps=\\((.*)\\); bumplength=(.*); pauselength=(.*);$", 
		BUS_CALLBACK_OF(FF3DDeviceOutputAgent, handlePicobHgForceModel ));
	bindMessage( "^OUT FF3D : picobg=\\((.*), (.*), (.*)\\); bumps=\\((.*)\\); bumplength=(.*); pauselength=(.*);$", 
		BUS_CALLBACK_OF(FF3DDeviceOutputAgent, handlePicobGForceModel ));
	
	pb = new Playback();
	pb->SetPlaybackSpeed(100);
	pb->CreateControllersForOmni();

	// set the error threshold in mm
	pb->SetErrorThreshold(8);
	
	// avoid force wind up above 2N
	pb->SetIntegratorForceCap(2);

	// used in the Omni callback to set the initial state of the playback
	pb->LoadGestureFile("SavedTrajectory.txt");
	fopen_s(&log, "d3.txt", "a");
}

FF3DDeviceOutputAgent::~FF3DDeviceOutputAgent()
{
	delete _fm;
	delete pb;
//	delete _transform;
}

void FF3DDeviceOutputAgent::handlePointForceModel(MicoleBus *app, int argc, const char **argv)
{
	if (argc < 3)
		return;
	Vec3f position;
	try
	{
		position.x = atof(argv[0]);
		position.y = atof(argv[1]);
		position.z = atof(argv[2]);
		fprintf(log, "%f, %f, %f\n", position.x, position.y, position.z);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}

	_transform->translation->set(Vec3f(0,0,0));
	_transform->rotation->set( Rotation( Vec3f(0, 0, 0), 0 ) );

	setForceModel(new FF3DPointForceModel(pb, position));
}

void FF3DDeviceOutputAgent::handleLinearForceModel(MicoleBus *app, int argc, const char **argv)
{
		if (argc < 6)
		return;
	Vec3f pt1;
	Vec3f pt2;
	try
	{
		pt1.x = atof(argv[0]);
		pt1.y = atof(argv[1]);
		pt1.z = atof(argv[2]);

		//_transform->translation->set(pt1);
		pt2.x = atof(argv[3]);
		pt2.y = atof(argv[4]);
		pt2.z = atof(argv[5]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}

	setForceModel(new FF3DLinearForceModel(pt1,pt2));

	//we compute the euclidian norma
	Vec3f vec_line = pt1 - pt2;
	_transform->rotation->set( Rotation( Vec3f(vec_line.z, 0, -vec_line.x), acos( vec_line.y / vec_line.length() ) ) );
	_transform->translation->set( (pt1+pt2)/2 ); 
}

void FF3DDeviceOutputAgent::handlePlanarForceModel(MicoleBus *app, int argc, const char **argv)
{
	if (argc < 6)
		return;

	Vec3f pt1;
	Vec3f pt2;
	Vec3f pt3;
	try
	{
		pt1.x = atof(argv[0]);
		pt1.y = atof(argv[1]);
		pt1.z = atof(argv[2]);
		_transform->translation->set(pt1);

		pt2.x = atof(argv[3]);
		pt2.y = atof(argv[4]);
		pt2.z = atof(argv[5]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{ 
		return;
	}

	/*pt3.x = atof(argv[6]);
	pt3.y = atof(argv[7]);
	pt3.z = atof(argv[8]);*/

	Vec3f vec_line = pt1 - pt2;
	_transform->rotation->set( Rotation( Vec3f(vec_line.z, 0, -vec_line.x), acos( vec_line.y / vec_line.length() ) ) );
	_transform->translation->set( pt1 ); 

	setForceModel(new FF3DPlanarForceModel(pt1,pt2));
}

void FF3DDeviceOutputAgent::handleSphericalForceModel(MicoleBus *app, int argc, const char **argv)
{
	if (argc < 4)
		return;

	Vec3f position;
	mgFloat range;
	try
	{
		position.x = atof(argv[0]);
		position.y = atof(argv[1]);
		position.z = atof(argv[2]);
		range = atof(argv[3]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}

	_transform->translation->set(position);
	_transform->rotation->set( Rotation( Vec3f(0, 0, 0), 0 ) );

	//setForceModel(new FF3DPointForceModel(position));
	setForceModel( new FF3DSphericalForceModel(Vec3f(0,0,0),range) );
}

void FF3DDeviceOutputAgent::handleDiscForceModel(MicoleBus *app, int argc, const char **argv)
{
	if (argc < 7)
		return;

	Vec3f pt1;
	Vec3f pt2;
	Vec3f pt3;
	try
	{
		pt1.x = atof(argv[0]);
		pt1.y = atof(argv[1]);
		pt1.z = atof(argv[2]);
		_transform->translation->set(pt1);

		pt2.x = atof(argv[3]);
		pt2.y = atof(argv[4]);
		pt2.z = atof(argv[5]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}

	mgFloat range = atof(argv[6]);
	
	Vec3f vec_line = pt1 - pt2;
	_transform->rotation->set( Rotation( Vec3f(vec_line.z, 0, -vec_line.x), acos( vec_line.y / vec_line.length() ) ) );
	_transform->translation->set( pt1 ); 

	setForceModel(new FF3DDiscForceModel(pt1,pt2,range));
}

void FF3DDeviceOutputAgent::handleNoForceModel(MicoleBus *app, int argc, const char **argv)
{
	setForceModel(NULL);
}

void FF3DDeviceOutputAgent::handlePicobHgForceModel(MicoleBus *app, int argc, const char **argv)
{
	if (argc < 9)
		return;

	Vec3f pt1;
	Vec3f pt2;
	try
	{
		pt1.x = atof(argv[0]);
		pt1.y = atof(argv[1]);
		pt1.z = atof(argv[2]);

		//_transform->translation->set(pt1);
		pt2.x = atof(argv[3]);
		pt2.y = atof(argv[4]);
		pt2.z = atof(argv[5]);
	}
	catch (...)
	{ //can happen with truncated packet or bad transmission
	}

	//we compute the euclidian norma
	Vec3f vec_line = pt1 - pt2;
	_transform->rotation->set( Rotation( Vec3f(vec_line.z, 0, -vec_line.x), acos( vec_line.y / vec_line.length() ) ) );
	_transform->translation->set( (pt1+pt2)/2 ); 
	
	vector<Vec3f> bumpList;
	Picob::readPicobList(argv[6], bumpList);
	setForceModel(new FF3DPicobHGForceModel(pt1, pt2, bumpList, atof(argv[7]), atof(argv[8])));
}

void FF3DDeviceOutputAgent::handlePicobGForceModel(MicoleBus *app, int argc, const char **argv)
{
	if (argc < 6)
		return;

	Vec3f pt;
	try
	{
		pt.x = atof(argv[0]);
		pt.y = atof(argv[1]);
		pt.z = atof(argv[2]);
	}
	catch (...)
	{ //can happen with truncated packet or bad transmission
	}

	vector<Vec3f> bumpList;
	Picob::readPicobList(argv[3], bumpList);
	setForceModel(new FF3DPicobGForceModel(pt, bumpList, atof(argv[4]), atof(argv[5])));

	_transform->rotation->set( Rotation( Vec3f(0, 0, 0), 0 ) );
	_transform->translation->set(Vec3f(0,0,0));
}
