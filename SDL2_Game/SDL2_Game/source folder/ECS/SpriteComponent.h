#pragma once
// --- MODIFICATION LOG (Sprite Component) ---
// 1. Decoupled Time: The 'update(float dt)' signature was already correct (accepts dt).
// 2. Code Quality: Added member initializer list to fix C26495 uninitialized member warnings.
// 3. ECS Cleanup: Added forward declaration for TransformComponent.

#include "Components.h"
#include "SDL.h"
#include "../Header files/TextureManager.h"
#include "Animation.h"
#include <map>

// Forward declaration to resolve dependency chain cleanly.
class TransformComponent;

class SpriteComponent : public Component
{
private:
	TransformComponent* transform; // Pointer to the owner's position and scale data.
	SDL_Texture* texture; // The image data loaded onto the GPU.
	SDL_Rect srcRect, destRect; // Source (sprite sheet crop) and Destination (screen position) rectangles. 

	bool animated; // Is this sprite currently playing an animation?
	int frames;    // Total number of frames in the current animation clip.
	int speed;     // Time delay (in ms) between frames for current animation.

public:
	// Public members
	int animIndex; // The row index on the sprite sheet for the current animation.

	// Map of all available animations (Name -> Animation struct).
	std::map<const char*, Animation> animations;

	// Used to flip the sprite horizontally for directional movement.
	SDL_RendererFlip spriteFlip;

	// Default constructor .
	SpriteComponent();

	// Constructor to initialize all members and set the texture. 
	SpriteComponent(const char* path)
		: transform(nullptr), texture(nullptr), animated(false), frames(0), speed(100), animIndex(0), spriteFlip(SDL_FLIP_NONE)
	{
		setTex(path);
	}

	// Parameterized constructor for animated sprites.
	SpriteComponent(const char* path, bool isAnimated)
		: transform(nullptr), texture(nullptr), animated(isAnimated), frames(0), speed(100), animIndex(0), spriteFlip(SDL_FLIP_NONE)
	{
		animated = isAnimated; // Setting the flag

		// Animations
		Animation idle = Animation(0, 4, 200); // Row 0, 4 frames, 200ms per frame
		Animation run = Animation(1, 6, 200);  // Row 1, 6 frames, 200ms per frame
		//Animation shoot = Animation(2, 6, 200);  // Row 2, 6 frames, 200ms per frame

		// Store clips in the map for playback
		animations.emplace("Idle", idle);
		animations.emplace("Run", run);
		//animations.emplace("Shoot", run);

		Play("Idle"); // Start the default animation

		setTex(path); // Load the main sprite sheet
	}

	// Destructor: Cleans up the GPU memory used by the texture.
	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}

	// Utility function to load a texture from a file path.
	void setTex(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
	}

	void init() override
	{
		// Grab a reference to the TransformComponent data from the owner Entity.
		transform = &entity->getComponent<TransformComponent>();

		// Initialize the source rectangle to the first frame (0, 0)
		srcRect.x = srcRect.y = 0;
		// Set source width/height based on the TransformComponent defaults
		srcRect.w = transform->width;
		srcRect.h = transform->height;

	}

	void update(float dt) override
	{
		if (animated)
		{
			// Animation Frame Calculation
			// (Time Elapsed / Frame Speed) % Total Frames
			// This cycles the frame count based on time, making the animation framerate-independent.
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		// Set the Y coordinate of the source rect to the current animation's row.
		srcRect.y = animIndex * transform->height;

		// Destination Calculation
		// Use the Entity's position from the TransformComponent.
		destRect.x = static_cast<int>(transform->position.x);
		destRect.y = static_cast<int>(transform->position.y);

		// Use the TransformComponent's scale and size.
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		// tell the TextureManager to draw the current frame onto the screen.
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	// Sets the current animation state using the name from the 'animations' map.
	void Play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};