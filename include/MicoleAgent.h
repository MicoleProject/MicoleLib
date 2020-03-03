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

#ifndef MicoleAgent_H
#define MicoleAgent_H
#include <micolelib.h>

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "Binding.h"
#include "MicoleBus.h"

enum AgentState {ACTIVE, SUSPENDED, UNKNOWN, UNACTIVE };//Agent states
namespace AgentModuleConstants {

	//Message prefixes
	const static char * PHANTOM = "^Phantom:(.*)";
	const static char * SOUND = "^Sound:(.*)";
	const static char * OUTPUT = "^Output:(.*)";
	const static char * PHANTOM_INPUT = "^Phantom_input:(*.)";
	const static char * MAGELLAN_INPUT = "^Magellan_input:(.*)";
	const static char * KEYBOARD_INPUT = "^Keyboard_input:(.*)";

	const static char * QUIT = "^Quit$";
	const static char * PING = "^Ping$";
	const static char * SUSPEND = "^Suspend:(.*)";
	const static char * RESUME ="^Resume:(.*)";

	const static char * LOCALHOST = "127.0.0.1";
	const static char * MULTICAST = "127.255.255.255";

	
	//Direct message types. Types start from 100. 
	const static int REGISTER_REPLY = 100;
	const static int SUSPEND_REQUEST = 101;
}


/** @defgroup core Core
 *  Main core classes
 *  All core classes in "needed" by micoleLib. Please do not modify anything into it.
 *  @{
 */

/**
 * base agent for Micole Project
 * This agent provide communications with MicoleBus.
 * you ban bind message to catch messages and send messages on the bus
 * @see MicoleAgent::bindMessage
 * @see MicoleAgent::sendMessage
 */
class MICOLELIB_API MicoleAgent
{
	friend class SuspendMsgListener;
	friend class RegisterMsgListener;

	public:
		virtual ~MicoleAgent();
		string getAgentName();
		string getAgentType();
		void onApplicationConnected( MicoleBus *app );
		void onApplicationDisconnected( MicoleBus *app );

		//MicoleBus *app is the sender of the message.
		void handlePingMessage( MicoleBus *app, int argc, const char **argv);
		void handleQuitMessage( MicoleBus *app, int argc, const char **argv);
		/*	Suspends the MicoleAgent. Suspending means that its methods are unbound from
			the Micole bus. Sending Suspend message using sendMessage
		*/
		void handleSuspendMessage( MicoleBus *app, int argc, const char **argv);
		void handleResumeMessage( MicoleBus *app, int argc, const char **argv);

		void handleRegisterReply( MicoleBus *app, int argc, const char **argv);
		void handleStartMessage( MicoleBus *app, int argc, const char **argv);
		void handlePrintIDMessage( MicoleBus *app, int argc, const char **argv);
		//virtual void onDirectMessage( MicoleBus *app, int id, const char *arg );

		/** Sends a message using bus->SendMsg. The message goes to all agents. */
		int sendMessage( const char *message);

		/** Sends a message using bus->SendMsg. The message goes to all agents. */
		int sendMessage( const string message);

		/**
		 * Sends a message to the MicoleBus *app by calling app->SendMsg(const char *message)
		 *	The message goes only to agent specified by const char *to. The receiving agent must
		 *	have appropriate regexp bound to process the message.
		 */
		void sendMessage(const char *to, const char *message);

	protected:
		/**
		 * Default constructor
		 * @param name: MicoleAgent name
		 * @param type: string which can discriminate different agents ex: sender, input, output,...
		 * @param domain NOT NEEDED DEPRECATED (default:"127.255.255.255:2010"): agent listen to this adress and port
		 * @deprecated domain
		 */
		MicoleAgent(const string &name, const string &type, const char *domain = "127.255.255.255:2010");

		/**
		 * Binds a callback method to bus. When a message that satisfies
		 * condition given in *regexp is received, the call back method is called
		 * by the bus. Methods that are bound with this message are not called when
		 * the agent is SUSPENDED. Methods that are bound using bus->BindMsg() are
		 * called also when the agent's state is SUSPENDED. 
		 */
		int bindMessage(const char *regexp, MicoleCallback * cb);
		void unBindMessage(int messageId);
		void unBindMessage(MicoleBinding &binding);

		/**
		* Suspends the agent. Unbinds all callback methods that are bound using 
		* MicoleAgent::bindMessage()
		*/
		virtual void suspendAgent();

		/**
		* Resumes the agent. Re-binds all callback methods.
		*/
		virtual void resumeAgent();

		/**
		* Gives inheriting class a chance to save state when the agent
		* receives Quit-message
		*/
		virtual void stopAgent();

		/**
		* Obtains identifier from the RegistryAgent
		*/
		void obtainIdentifier();

		static MicoleBus* bus;
		//Ivy* bus;
		vector<MicoleBinding> bindings;
		AgentState state;
		int resumeId;

	private:
		string agentName;/**< the name of the agent*/
		string agentType;/**<the type of the agent. (not used yet) */
		//int identifier;//the agent identifier, received on start-up from agent network...(not used yet)
		string identifier;	
};

/** @} */ //end core

#endif
