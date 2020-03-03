/* 

     The contents of this file are subject to the Mozilla Public License
	 Version 1.1 (the "License"); you may not use this file except in
     compliance with the License. You may obtain a copy of the License at
     http://www.mozilla.org/MPL/

     Software distributed under the License is distributed on an "AS IS"
     basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
     License for the specific language governing rights and limitations
     under the License.

     The Original Code is the PID trajectory playback library.

     The Initial Developers of the Original Code are Andrew Crossan (ac@dcs.gla.ac.uk) 
	 and John Williamson (jhw@dcs.gla.ac.uk) from University of Glasgow.
	 
     Portions created by Andrew Crossan and John Williamson are Copyright (C) 2006.
     All Rights Reserved.

     Contributor(s): ______________________________________.

*/

/* PID Trajectory Playback v1.1 -  A library for playing a trajectory on a force feedback device
 * To playback a trajectory on the haptic device using the controller  
 * 1. Initialise the controller using one of the CreateController functions
 * 2. Add a series of points for the trajectory using AddWaypoint
 * 3. Set the controller playing with StartPlayback
 * 4. At every servo loop update, call GetForce to get the controller force
 *    GetForce will return false when the playback is finished
 *
 * if the trajectory has been recorded at full sampling rate, SetPlaybackAtSampleRate(true)
 * can be used to playback at the approximate recorded trajectory speed
*/

#pragma once


#include "pid.h"
#include "micolelib.h"

#include "FF3DForceModel.h"
using namespace Reachin;


#define PLAY_AT_SAMPLE_RATE false
#define ABSOLUTE_POSITIONING false

#define BEAD_SPEED	100
#define ERROR_THRESHOLD	10

struct PtsList {
// double time;

   double xPos, yPos, zPos;
// double xVel, yVel, zVel; /* Added velocities jhw */
   PtsList *prev, *next;
};  

#define PID_PI			3.141592
#define PID_PI_BY_2		PID_PI / 2.0
#define _PLAY_MICOLE_RATE 700

enum Mode
{
	PID_IDLE						= 0,
	PID_STICK_TO_POINT				= 1,
	PID_MOVE_TO_STICK_POINT			= 2,
	PID_PLAY						= 3,
	PID_MOVE_TO_PLAY_POINT			= 4,
	PID_PAUSE						= 5,
	PID_CONSTRAIN					= 6,
	PID_MOVE_TO_CONSTRAIN_POINT		= 7,
};

class Playback
{
public:
	Playback();
	~Playback();
// Overrides
public:

	Vec3f _stick_point;
	// Initialize a new PID controller with specfic values
	void CreateControllers(double p, double i, double d, double f, double out_filter,
			 double gain, double sat_low, double sat_high, double dead_zone);
/*
 *	p: proportional control gain
 *	i: integral control gain
 *	d: derivative control gain
 *	f: input filter (for deriv. estimation)
 *	out_filter: output filtering
 *	gain: overall gain
 *	sat_low: minimum saturation point
 *	sat_high: maximum_saturation point
 */

	// Initialise with preset Phantom omni parameters
	// p = 0.3, i = 0.0005, d = 20.0, f = 0.09, out_filter = 0.09, gain = 0.2, sat_low = -3.0,
	// sat_high = 3.0, dead_zone = 0.0
	void CreateControllersForOmni();

	// Initialise PD controller with preset Phantom omni parameters
	// Using PD instead of PID will avoid force wind up when playback is resisted
	// p = 0.3, i = 0.0, d = 20.0, f = 0.09, out_filter = 0.09, gain = 0.2, sat_low = -3.0,
	// sat_high = 3.0, dead_zone = 0.0
	void CreatePDControllersForOmni();

	// Initialise with preset Phantom Premium 1.5parameters
	// p = 0.3, i = 0.00005, d = 30.0, f = 0.09, out_filter = 0.09, gain = 0.2, sat_low = -1.5,
	// sat_high = 1.5, dead_zone = 0.0
	void CreateControllersForPremium15();
	
	// Reset the's the state of the controller
	void ResetControllerState();

	// moves the current position in the playback to the start of the gesture
	void MoveToStartOfGesture();
	
	// gets the current mode of playback
	Mode GetMode();
	
	// sets the playback state to idle. ie stops playback
	void SetIdle();

	// toggles between pause and play. Pause will stop the 
	// gesture without returning to the start of the gesture
	void TogglePauseGesture();

	// Tries to control the users cursor to one specifc point
	// curPPos is the current device position, pt is the position to stick to
	void StickToPoint(double curPos[3], double pt[3]);
	
	// Sticks the cursor to the currentPosition specified
	// by curPos
	void StickToThisPoint(double curPos[3]);

	// sets the controller to playback one sample per update
	// if false, playback rate is controller through SetPlaybackSpeed
	// Note... this means that if the user resists moving, the gesture will
	// still continue
	void SetPlayAtSampleRate(bool pasr);

	// returns true if the controller is set to playback at
	// the sample rate
	bool GetPlayAtSampleRate();

	// call at every servo loop update
	// afeter the call, force will contain the force calculated by the controller
	bool GetForce(double pt[3], double force[3]);

	// Returns true only if not IDLE. In pPos, the current x, y, z playback position or
	// stick point position will be returned
	bool GetPlaybackPos(double pPos[3]);

	// called to set the trajectory to play. You MUST still call GetForce every 
	// servo loop update to get the force of th playback
	// either specify a trajectory explictly by passing in a list of points
	// or if no list is provided, the pre-built (with AddWaypoint) trajectory 
	// is used
	void StartPlayback(PtsList *pt, double curPos[3]); // playback waypoints pt
	void StartPlayback(double curPos[3]); // playback currently stored waypoints 


	// called to set the trajectory to constrain the user to. You MUST still call 
	// GetForce every servo loop update to get the force of th playback
	// the pre-built (with AddWaypoint) trajectory is used for the constrain path
	// curPos is the currentPosition of the device
	// if using absolute positioning, the controller will initially go into 
	// PID_MOVE_TO_CONSTRAIN_POINT mode
	void StartConstrain(double curPos[3]);

	// set the speed of playback in mm/update if not in playback_at_sample_rate mode 
	// default is 100
	void SetPlaybackSpeed(double v);

	// clears the current trajectory
	void ClearPath();

	// add a waypoint to the end of the current trajectory. This is used in conjunction
	// with StartPlayback(double curPos[3])... 
	// using StartPlayback(PtsList *pt, double curPos[3]) will overwrite any trajectory
	// built with AddWaypoint with the list of points specified
	void AddWaypoint(double pt[3]);

	// load a gesture from a file fName is the path a name of the file
	// returns true if the file is found and false otherwise
	// File Format
	// time1, pt1_x, pt1_y, pt1_z
	// time2, pt2_x, pt2_y, pt2_z
	// time3, pt2_x, pt2_y, pt2_z
	// .......
	bool LoadGestureFile(char *fName);

	// Saves the current gesture to a file fName is the path a name of the file
	// returns true if the file can be created and false otherwise
	// File Format
	// time1, pt1_x, pt1_y, pt1_z
	// time2, pt2_x, pt2_y, pt2_z
	// time3, pt2_x, pt2_y, pt2_z
	// .......
	bool SaveGestureFile(char *fName);

	// absPos == True to use absolute Positioning for gesture 
	// absPos == False to start gesture at the current postion
	void UseAbsolutePositioning(bool absPos);

	// The error threshold is the distance that the device has to get
	// to the sample position before the trajectory is moved on to the 
	// next point. This is measured in mm (default 10mm)
	void SetErrorThreshold(double e);
	double GetErrorThreshold();

	// cap the maximum force from the integrator to avoid wind up
	// for the CreateControllersForOmni and the CreateControllersForPremium15,
	// this is automatically set to the controller max force cap
	void SetIntegratorForceCap(double maxforce);

	void stickToPointAndGetForce(Vec3f pos, Vec3f dest, Vec3f& force);


private:
	
	control_state *control_x, *control_y, *control_z; // Pos controllers
	PtsList *head, *cur, *end;
	PtsList *headToPt, *curToPt, *endToPt;
	double startPos[3], stickPoint[3], stickBeadPos[3], beadPos[3], beadSpeed;
	double startTime, prevTime;
	bool playAtSampleRate;
	bool absolutePositioning;
	double err_thresh;

	Mode mode;

	// helper functions for stick to Point	
	void CreateGestureToPoint(double curPos[3], double thePoint[3]);
	void FreeGestureToPoint();
	void MoveToPoint(double curPos[3], double pt[3], Mode m);

	// helper functions for constrain
	bool DoConstrain(double pPos[3], double force[3]);

	//Set the windup limit
	void SetIntegratorCap(double icap);


	// distance from cur along the pathway that the bead has moved in constrain
	// mode. This is used to ensure that the user can only move forwards along
	// the path therefore removing the tricky direction decision for sharp corners
	double curConstrainDist;

	//debug
	double a;
	double distFromPrev;
};
