#pragma once

struct Animation
{
	// Data fields for one specific animation clip
	int index;	// The row index in the sprite sheet where this animation starts
	int frames; // The total number of frames in this animation clip
	int speed;  // The delay (in ms) between each frame

	//Constructor to initialize all members (Fixes C26495 warnings)
	Animation() : index(0), frames(0), speed(0) {}

	// Parameterized constructor
	Animation(int i, int f, int s) : index(i), frames(f), speed(s)
	{
	}
};