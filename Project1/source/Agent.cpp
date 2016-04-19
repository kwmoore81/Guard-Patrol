//--------
//	Brief: A class that can be moved around by using steering behaviours
//	Author: Elizabeth Rowlands
//	Updated by: Richard Stern
//	Date : 16/4/2015
//--------
#include "Agent.h"
#include "Input.h"

#define _USE_MATH_DEFINES
#include <math.h>


Agent::Agent()
{
	Init(0.0f, 0.0f);
}

Agent::Agent(float a_positionX, float a_positionY, float a_windowWidth, float a_windowHeight)
{
	Init(a_positionX, a_positionY);
	m_eColour = Green;
	
	//Patrol points
	m_pathSize = 4;
	m_path = new WayPoint[m_pathSize]();
	m_path[0].x = 200.0f;
	m_path[0].y = 200.0f;
	m_path[1].x = a_windowWidth - 200.0f;
	m_path[1].y = 200.0f;
	m_path[2].x = a_windowWidth - 200.0f;
	m_path[2].y = a_windowHeight - 200.0f;
	m_path[3].x = 200.0f;
	m_path[3].y = a_windowHeight - 200.0f;
	m_nCurrentDst = 0;

	//Sight radius
	m_nSightRange = 250;

	//Health
	m_currentHealth = 100;
	m_maxHealth = 100;
}

Agent::~Agent()
{
}

void Agent::Update(float a_deltaTime)
{
	// Update the velocity with the current acceleration
	m_movementInfo.m_velocityX += m_movementInfo.m_accelerationX;
	m_movementInfo.m_velocityY += m_movementInfo.m_accelerationY;

	// constrain the velocity
	if (m_movementInfo.m_velocityX > m_movementInfo.m_maxSpeed)
	{
		m_movementInfo.m_velocityX = m_movementInfo.m_maxSpeed;
	}
	else if (m_movementInfo.m_velocityX < -m_movementInfo.m_maxSpeed)
	{
		m_movementInfo.m_velocityX = -m_movementInfo.m_maxSpeed;
	}
	if (m_movementInfo.m_velocityY > m_movementInfo.m_maxSpeed)
	{
		m_movementInfo.m_velocityY = m_movementInfo.m_maxSpeed;
	}
	else if (m_movementInfo.m_velocityY < -m_movementInfo.m_maxSpeed)
	{
		m_movementInfo.m_velocityY = -m_movementInfo.m_maxSpeed;
	}
	
	// set rotation
	m_movementInfo.m_rotation = atan2(m_movementInfo.m_velocityY, m_movementInfo.m_velocityX) + (float)M_PI / 2.0f;

	// reset the acceleration for the next frame
	m_movementInfo.m_accelerationX = 0.0f;
	m_movementInfo.m_accelerationY = 0.0f;

	// add the velocity to the position
	m_movementInfo.m_positionX += m_movementInfo.m_velocityX;
	m_movementInfo.m_positionY += m_movementInfo.m_velocityY;

	CheckCollisions();
	switch (GetColour())
	{
	case Colour::Green: Patrol(); break;
	case Colour::Blue: Health(); break;
	case Colour::Red: Attack(); break;
	case Colour::Orange: Search(); break;
	}
	
}

void Agent::Wander(float radius, float jitter, float distance, float weight)
{
	auto &m = m_movementInfo;
	// determine some random angles for circle/jitter
	float cangle = rand();
	float jangle = rand();

	// create circle/jitter vectors

	float xCircle = cos(cangle) * radius;
	float yCircle = sin(cangle) * radius;
	
	float xJitter = cos(jangle) * jitter;
	float yJitter = sin(jangle) * jitter;
	//combine
	float xOffset = xCircle + xJitter;
	float yOffset = yCircle + yJitter;

	float offMag = sqrt(xOffset*xOffset + yOffset*yOffset);
	//normalize to size of circle
	xOffset *= radius / offMag;
	yOffset *= radius / offMag;

	// find normal of vector between, Mult by distance
	float velMag = sqrtf(m.m_velocityX*m.m_velocityX + m.m_velocityY*m.m_velocityY);
	float xPosition = (m.m_velocityX / velMag) * distance;
	float yPosition = (m.m_velocityY / velMag) * distance;

	// add our offset and circle position to the player's position to get our target
	float xTarget = xOffset + xPosition + m.m_positionX;
	float yTarget = yOffset + yPosition + m.m_positionY;

	Patrol();
}

void Agent::Attack() //Red
{
	int mouseX;
	int mouseY;
	Input::GetSingleton()->GetMouseXY(&mouseX, &mouseY);
	float fDist = GetDistanceFromTarget((float)mouseX, (float)mouseY);
	vec2 agentPos = { GetPositionX(), GetPositionY() };
	vec2 mousePos = { (float)mouseX, (float)mouseY };
	
	vec2 accel = (mousePos - agentPos).normal() * 5;
	m_movementInfo.m_accelerationX = accel.x;
	m_movementInfo.m_accelerationY = accel.y;

	if (fDist > m_nSightRange) SetColour(Colour::Orange);
	if (GetHealth() <= 50) SetColour(Colour::Blue);

}
void Agent::Patrol() //Green
{
	int mouseX;
	int mouseY;
	int j = 0;
	
	Input::GetSingleton()->GetMouseXY(&mouseX, &mouseY);
	float fDist = GetDistanceFromTarget((float)mouseX, (float)mouseY);
	
	if (GetDistanceFromTarget(m_path[m_nCurrentDst].x,
		m_path[m_nCurrentDst].y) < 15.f)
	{
		m_nCurrentDst++;
		m_nCurrentDst %= 4;
	}
	
		vec2 nodePos = { m_path[m_nCurrentDst].x, m_path[m_nCurrentDst].y };
		vec2 agentPos = { GetPositionX(), GetPositionY() };
		vec2 posDiff = nodePos - agentPos;
		vec2 accel = (nodePos - agentPos).normal() * 5;
		m_movementInfo.m_accelerationX = accel.x;
		m_movementInfo.m_accelerationY = accel.y;
		
	
	if (fDist < m_nSightRange) SetColour(Colour::Red);
	if (GetHealth() <= 50) SetColour(Colour::Blue);
}
void Agent::Search() //Orange
{
	searching = true;

	int mouseX, mouseY;
	Input::GetSingleton()->GetMouseXY(&mouseX, &mouseY);
	float fDist = GetDistanceFromTarget((float)mouseX, (float)mouseY);

	// Actuation
	Wander(180, 60, 60);

	// Transition
	if (searchTimer > 3)
	{
		searching = false;
		searchTimer = 0;
		SetColour(Colour::Green);
	}
	if (fDist < m_nSightRange)
	{
		searching = false;
		searchTimer = 0;
		SetColour(Colour::Red);
	}
	if (GetHealth() <= 50)
	{
		searching = false;
		searchTimer = 0;
		SetColour(Colour::Blue);
	}
}
void Agent::Health() //Blue
{
	
	vec2 agentPos = { GetPositionX(), GetPositionY() };
	vec2 healthPickUp = { m_pickupPos.x, m_pickupPos.y };
	vec2 accel = (healthPickUp - agentPos).normal() * 5;
	m_movementInfo.m_accelerationX = accel.x;
	m_movementInfo.m_accelerationY = accel.y;
	
	if (m_currentHealth == m_maxHealth)
	{
		SetColour(Colour::Green);
	}
	 
}

void Agent::CheckCollisions()
{
	//Check for collision with player and take health off if collided
	int mouseX;
	int mouseY;
	Input::GetSingleton()->GetMouseXY(&mouseX, &mouseY);
	
	float fDist = GetDistanceFromTarget((float)mouseX, (float)mouseY);
	if(fDist <= 20.0f)
	{
		if(!m_takingDamage)
		{
			m_currentHealth -= 15;
			if(m_currentHealth < 0)
				m_currentHealth = 0;
		
			//Make sure we only take damage once per collision
			m_takingDamage = true;
		}
	}
	else
		m_takingDamage = false;

	//Check for collision with health pickup
	fDist = GetDistanceFromTarget(m_pickupPos.x, m_pickupPos.y);
	if(fDist <= 20.0f)
	{
		m_currentHealth = m_maxHealth;
	}
}

void Agent::Init(float a_positionX, float a_positionY)
{
	m_movementInfo.m_positionX = a_positionX;
	m_movementInfo.m_positionY = a_positionY;
	m_movementInfo.m_velocityX = 0.0f;
	m_movementInfo.m_velocityY = 0.0f;
	m_movementInfo.m_accelerationX = 0.2f;
	m_movementInfo.m_accelerationY = 0.2f;
	m_movementInfo.m_maxSpeed = 4.0f;
	m_movementInfo.m_maxForce = 0.1f;
	m_movementInfo.m_rotation = 0.0f;
	m_movementInfo.m_rotationDampening = 0.15f;
}

float Agent::GetDistanceFromTarget(float targetX, float targetY)
{
	// Get the distance from the target position
	float distanceX = targetX - m_movementInfo.m_positionX;
	float distanceY = targetY - m_movementInfo.m_positionY;
	float magnitude = distanceX * distanceX + distanceY * distanceY;
	if(magnitude != 0.0f)
	{
		float oneOverSqrtMag = 1.0f / sqrt(magnitude);
		float dist = magnitude * oneOverSqrtMag;
		
		return dist;
	}
	return 0.0f;
}

void Agent::SetColour(Colour eColour)
{
	m_eColour = eColour;
}

Colour Agent::GetColour()
{
	return m_eColour;
}

void Agent::SetPickupPos(float x, float y)
{
	m_pickupPos.x = x;
	m_pickupPos.y = y;
}

WayPoint* Agent::GetPickupPos()
{
	return &m_pickupPos;
}

int Agent::GetHealth()
{
	return m_currentHealth;
}

int Agent::GetMaxHealth()
{
	return m_maxHealth;
}

