/*

Original demo (below) modified to demonstrate Phya with hinged rods fixed to the world.


--
Bullet Continuous Collision Detection and Physics Library
Ragdoll Demo
Copyright (c) 2007 Starbreeze Studios

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Originally Written by: Marten Svanfeldt
ReWritten by: Francisco Le�n
*/

#include "btBulletDynamicsCommon.h"
#include "GlutStuff.h"
#include "GL_ShapeDrawer.h"

#include "LinearMath/btIDebugDraw.h"

#include "GLDebugDrawer.h"
#include "GenericJointDemo.h"

GLDebugDrawer debugDrawer;

void GenericJointDemo::initPhysics()
{
	setTexturing(true);
	setShadows(true);

	// Setup the basic world

	btDefaultCollisionConfiguration * collision_config = new btDefaultCollisionConfiguration();

	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collision_config);

	btVector3 worldAabbMin(-10000,-10000,-10000);
	btVector3 worldAabbMax(10000,10000,10000);
	btBroadphaseInterface* overlappingPairCache = new btAxisSweep3 (worldAabbMin, worldAabbMax);

	btConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,constraintSolver,collision_config);

	m_dynamicsWorld->setGravity(btVector3(0,-10,0));

	m_dynamicsWorld->setDebugDrawer(&debugDrawer);

	//m_dynamicsWorld->getSolverInfo().m_restingContactRestitutionThreshold = 0.f; //   m_singleAxisRollingFrictionThreshold = 0.f;//faster but lower quality

	// Setup a big ground box
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(200.),btScalar(0.),btScalar(200.))); //! 0 thickness
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0,-8,0));
		m_ground = localCreateRigidBody(btScalar(0.),groundTransform,groundShape);
	}

	Swing* swing1 = new Swing( m_dynamicsWorld, btVector3(0,9,0), 4.0f, m_ground );
	Swing* swing2 = new Swing( m_dynamicsWorld, btVector3(4,9,0), 4.0f, m_ground );

	clientResetScene();
}

/*
void GenericJointDemo::spawnRagdoll(bool random)
{
	//RagDoll* ragDoll = new RagDoll (m_dynamicsWorld, btVector3 (0,0,10),5.f);
	//m_ragdolls.push_back(ragDoll);
	Swing* swing = new Swing( m_dynamicsWorld, btVector3(0,0,10), 5.0f );
}
*/
void GenericJointDemo::clientMoveAndDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//simple dynamics world doesn't handle fixed-time-stepping
	float ms = getDeltaTimeMicroseconds();

	float minFPS = 1000000.f/60.f;
	if (ms > minFPS)
		ms = minFPS;

	if (m_dynamicsWorld)
	{
		m_dynamicsWorld->stepSimulation(ms / 1000000.f);
		//optional but useful: debug drawing
		m_dynamicsWorld->debugDrawWorld();
	}

	renderme();

	glFlush();

	swapBuffers();
}

void GenericJointDemo::displayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_dynamicsWorld)
		m_dynamicsWorld->debugDrawWorld();

	renderme();

	glFlush();
	swapBuffers();
}