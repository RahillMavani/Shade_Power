//INCLUDING ALL THE HEADERS.
#include "Header files/GameLoop.h"
#include "Header files/TextureManager.h"
#include "Header files/TileMap.h"
#include "ECS/Components.h"
#include "Header files/Vector2D.h"
#include "Header files/Collision.h"
#include "ECS/CollisionSystem.h" 
#include "ECS/GroupLabels.h"
#include "ECS/AnimationSystem.h" 
#include "ECS/ProjectileComponent.h"
#include "Header files/UILabel.h"
#include <ctime>   // Needed for seeding random numbers
#include <cstdlib> // Needed for rand()
#include <fstream> //for high score 


//pointer to the map
TileMap* tileMap;

//score and high score STUFF
UILabel* scoreLabel;
int score = 0;

//Ammo and reloading
int ammo = 10;
bool isReloading = false;

int highScore = 0;
UILabel* highScoreLabel;

//where player is facing
int playerDir = 1;

//next wave of enemies
int nextWaveCount = 2;

//HEALTH
UILabel* healthLabel;
int health = 5;
SDL_Texture* heartTexture = nullptr;
Uint32 lastDamageTime = 0; // Stores the time when last got hit

//GAME OVER
bool gameOver = false;
UILabel* gameOverLabel;

//MENU
bool isMenu = true;
UILabel* titleLabel;
UILabel* startLabel;

//Music
Mix_Music* bgm = nullptr;      // For Background Music
Mix_Chunk* sfxShoot = nullptr; // For Shooting


//setting it to null
SDL_Renderer* GameLoop::renderer = nullptr;

//ECS manager, holds all my entities.
Manager manager;
//initializes the event holder.
SDL_Event GameLoop::event;


//starts at 0
float GameLoop::delta_time = 0.0f;
Uint32 GameLoop::frame_start = 0;

//well this is where the entities are created.
auto& player(manager.addEntity());
auto& wall(manager.addEntity());

//constructor and deconstructor
GameLoop::GameLoop()
{
}
GameLoop::~GameLoop()
{
}

//initializing the window
void GameLoop::init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen)
{
	//do i need fullscreen?
	int flags = 0;
	if (fullScreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	// Initialize random seed based on current time
	srand(time(NULL));

	//return 0 if success
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << " SDL Initialised" << std::endl;

		//creates the window
		window = SDL_CreateWindow("SDL2", xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window initialised" << std::endl;
		}
		//creates the renderer
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			std::cout << "Renderer initialised" << std::endl;
		}
		//running is true if it starts otherwise false.
		isRunning = true;
	}
	else { isRunning = false; }

	//creates a new tilemap object
	tileMap = new TileMap();
	//Loads the tilemap
	TileMap::LoadMap("assets/tileMap/tileMap.map", 35, 35);

	// Add position, sprite, input, and collider to the 'player' entity.
	player.addComponent<TransformComponent>(100.0f, 100.0f, 32, 32, 2);
	player.addComponent<SpriteComponent>("assets/player/player_anims.png", true); //true for animates
	player.addComponent<KeyboardEvent>();
	player.addComponent<ColliderComponent>("player");
	//add into the player group.
	player.addGroup(groupPlayers);
	//enemy 
	spawnEnemy(500.0f, 500.0f);

	// Initialize the Font System
	if (TTF_Init() == -1) {
		std::cout << "Error: TTF_Init Failed" << std::endl;
	}

	// Initialize Mixer (44100Hz, Default Format, 2 Channels, 2048 Chunk Size)
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "Error: SDL_Mixer Init Failed: " << Mix_GetError() << std::endl;
	}

	// Loading Audio Files
	bgm = Mix_LoadMUS("assets/audio/music.mp3");       // Load Music which is mp3
	sfxShoot = Mix_LoadWAV("assets/audio/shoot.wav");  // Load SFX which is wav

	if (!bgm || !sfxShoot) {
		std::cout << "Warning: Failed to load one or more audio files!" << std::endl;
	}

	// Start Background Music Loop
	Mix_PlayMusic(bgm, -1);

	// the label and size and text and location damn
	scoreLabel = new UILabel(500, 30, "Score: 0", "assets/OpenSans-Regular.ttf", 28);

	//high score txt to write and read from
	std::ifstream loadFile("shade_save.dat", std::ios::binary);

	if (loadFile.is_open()) {
		loadFile.seekg(0, std::ios::end);
		if (loadFile.tellg() > 0)
		{
			loadFile.seekg(0, std::ios::beg);
			int encryptedScore = 0;
			loadFile.read(reinterpret_cast<char*>(&encryptedScore), sizeof(encryptedScore));

			char key = 'X';
			highScore = encryptedScore ^ key;
		}
		else
		{
			highScore = 0;
		}
		loadFile.close();
	}
	else
	{
		highScore = 0; // File doesn't exist yet, so start fresh
	}
	highScoreLabel = new UILabel(450, 320, "Best: " + std::to_string(highScore), "assets/OpenSans-Regular.ttf", 50);

	//Health label
	healthLabel = new UILabel(100, 30, "Health: 5", "assets/OpenSans-Regular.ttf", 28);
	heartTexture = TextureManager::LoadTexture("assets/heart.png");

	//GameOver screen
	gameOverLabel = new UILabel(230, 400, "GAME OVER - PRESS R TO RESTART", "assets/OpenSans-Regular.ttf", 40);

	// Big Title
	titleLabel = new UILabel(350, 200, "SHADE POWER", "assets/OpenSans-Regular.ttf", 60);

	// Smaller Instruction
	startLabel = new UILabel(370, 500, "Press ENTER to Start", "assets/OpenSans-Regular.ttf", 40);
}

//ENEMY FUNCTION
void GameLoop::spawnEnemy(float x, float y)
{
	//adding a new enemy
	auto& enemy(manager.addEntity());

	// adding transform component for later movement use
	auto& trans = enemy.addComponent<TransformComponent>(x, y, 32, 32, 2);
	//loading the png
	enemy.addComponent<SpriteComponent>("assets/enemy/demon.png", true);

	// velocity to 1 (for demonstartion)
	//trans.velocity.x = 1;
	
	//enemy tag for collider and add to group
	enemy.addComponent<ColliderComponent>("enemy");
	enemy.addGroup(groupEnemies);
}

//BULLET FUNCTION
void GameLoop::spawnProjectile(Vector2D pos, Vector2D vel) {
	auto& projectile(manager.addEntity());

	// giving x,y pos and textures
	auto& trans = projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	trans.velocity = vel; // movement
	//speed
	trans.speed = 1000;
	//p=texture location and animation
	projectile.addComponent<SpriteComponent>("assets/player/bullet1.png", false);
	//movement (right for now)
	projectile.addComponent<ProjectileComponent>(5000, 1, vel);
	//collider tag projectile
	projectile.addComponent<ColliderComponent>("projectile");
	//adding to group and map
	projectile.addGroup(groupColliders);
	projectile.addGroup(groupMap);
}

//Reset game function
void GameLoop::resetGame()
{
	// Reset Variables
	health = 5;
	score = 0;
	ammo = 10;        
	nextWaveCount = 2;

	isMenu = true;
	gameOver = false;

	// Update Labels
	scoreLabel->SetText("Score: 0");
	healthLabel->SetText("Health: 5");

	// Reset Player Position
	player.getComponent<TransformComponent>().position = Vector2D(100, 100);
	player.getComponent<TransformComponent>().velocity = Vector2D(0, 0);

	// Kill All Enemies
	auto& enemies = manager.getGroup(groupEnemies);
	for (auto& e : enemies) {
		e->destroy();
	}

	// Kill All Potions
	// search the Map Group for anything tagged "potion"
	auto& mapObjects = manager.getGroup(groupMap);
	for (auto& e : mapObjects) {
		if (e->hasComponent<ColliderComponent>()) {
			if (e->getComponent<ColliderComponent>().tag == "potion") {
				e->destroy();
			}
		}
	}

	// Clear Bullets/Blood
	auto& colliders = manager.getGroup(groupColliders);
	for (auto& c : colliders) {
		if (c->getComponent<ColliderComponent>().tag == "projectile") {
			c->destroy();
		}
	}

	// Applying the deletions
	manager.refresh();
}

void GameLoop::update()
{
	// Calculate Delta Time
	GameLoop::delta_time = (SDL_GetTicks() - GameLoop::frame_start) / 1000.0f;
	GameLoop::frame_start = SDL_GetTicks();


	// Clean up dead entities (bullets/enemies marked as destroyed)
	manager.refresh();

	if (isMenu) return; // If in menu, freeze the game world.

	//if gameover is true, freeze the game
	if (gameOver) return;

	// Check if all enemies are dead
	if (manager.getGroup(groupEnemies).empty()) {

		std::cout << "Wave Cleared! Spawning " << nextWaveCount << " enemies." << std::endl;

		for (int i = 0; i < nextWaveCount; i++) {
			// Generate random positions inside the map area
			float spawnX = (rand() % 900) + 100; // Random X between 100 and 1000
			float spawnY = (rand() % 700) + 100; // Random Y between 100 and 800

			spawnEnemy(spawnX, spawnY);
		}

		// plus 1 the count for the next wave
		nextWaveCount += 1;
	}

	// Save Old Positions
	TransformComponent& playerTransform = player.getComponent<TransformComponent>();
	Vector2D oldPosition = playerTransform.position;

	// Save enemy old positions too
	auto& enemyGroup = manager.getGroup(groupEnemies);
	std::vector<Vector2D> enemyOldPositions;
	for (auto& e : enemyGroup) {
		enemyOldPositions.push_back(e->getComponent<TransformComponent>().position);
	}

	//Checking where player is so enemy can move towards it
	Vector2D targetPos = player.getComponent<TransformComponent>().position;
	//Gettign every enemies positions and velocities
	for (auto& enemy : enemyGroup) {
		Vector2D enemyPos = enemy->getComponent<TransformComponent>().position;
		Vector2D newVelocity(0, 0);

		SpriteComponent& enemySprite = enemy->getComponent<SpriteComponent>();

		// X-Axis Logic
		if (enemyPos.x < targetPos.x) {
			newVelocity.x = 0.3; // Move Right
			enemySprite.spriteFlip = SDL_FLIP_NONE;
		}
		else if (enemyPos.x > targetPos.x) {
			newVelocity.x = -0.3; // Move Left
			enemySprite.spriteFlip = SDL_FLIP_HORIZONTAL;
		}

		// Y-Axis Logic
		if (enemyPos.y < targetPos.y) {
			newVelocity.y = 0.3; // Move Down
		}
		else if (enemyPos.y > targetPos.y) {
			newVelocity.y = -0.3; // Move Up
		}

		// Applying new velocity to enemy
		enemy->getComponent<TransformComponent>().velocity = newVelocity;
	}

	// Standard ECS Update (Movement, Animation)
	manager.update(GameLoop::delta_time);

	// Check velocity to see which way player is looking
	Vector2D velocity = player.getComponent<TransformComponent>().velocity;

	if (velocity.x > 0) {
		playerDir = 1; // Facing Right
	}
	else if (velocity.x < 0) {
		playerDir = -1; // Facing Left
	}

	// Update Systems
	//Animations
	AnimationSystem animationSystem(manager);
	animationSystem.update(GameLoop::delta_time);
	//Collision for player
	CollisionSystem collisionSystem(manager);
	collisionSystem.update(&player, oldPosition);

	// Enemy vs Wall collisions
	for (size_t i = 0; i < enemyGroup.size(); i++) {
		collisionSystem.update(enemyGroup[i], enemyOldPositions[i]);
	}

	// Projectile vs Wall collisions
	auto& colliders = manager.getGroup(groupColliders);
	for (auto& c : colliders) {
		// Get the tag once 
		std::string tag = c->getComponent<ColliderComponent>().tag;

		// Update physics for Projectiles AND Potions
		if (tag == "projectile" || tag == "potion") {
			collisionSystem.update(c, c->getComponent<TransformComponent>().position);
		}
	}

	// PLAYER VS POTION
	auto& mapObjects = manager.getGroup(groupMap);
	for (auto& c : mapObjects)
	{
		// Check if this map object is actually a potion
		if (c->hasComponent<ColliderComponent>() && c->getComponent<ColliderComponent>().tag == "potion")
		{
			// MANUAL PHYSICS UPDATE
			// Since it's not in the main collider loop
			c->getComponent<ColliderComponent>().collider.x = c->getComponent<TransformComponent>().position.x;
			c->getComponent<ColliderComponent>().collider.y = c->getComponent<TransformComponent>().position.y;

			// Check Collision with Player
			if (Collision::AABB(player.getComponent<ColliderComponent>(), c->getComponent<ColliderComponent>()))
			{
				health = 5; // Restore Health
				c->destroy(); // Pick up potion
				std::cout << "Health Restored!" << std::endl;
			}
		}
	}



	// Loop through all objects with colliders
	for (auto& bullet : colliders) {
		// Is this a projectile?
		if (bullet->hasComponent<ColliderComponent>() && bullet->getComponent<ColliderComponent>().tag == "projectile") {

			// Check it against every enemy
			for (auto& enemy : enemyGroup) {
				// Using existing Collision::AABB function
				if (Collision::AABB(bullet->getComponent<ColliderComponent>(), enemy->getComponent<ColliderComponent>())) 
				{

					Vector2D enemyPos = enemy->getComponent<TransformComponent>().position;

					if (rand() % 5 == 0) {
						spawnPotion(enemyPos.x, enemyPos.y);
					}

					spawnBlood(enemyPos.x, enemyPos.y);

					// hit
					std::cout << "Enemy Hit!" << std::endl;

					// Increase Score
					score += 10;
					scoreLabel->SetText("Score: " + std::to_string(score));



					// Destroy the bullet and the enemy
					bullet->destroy();
					enemy->destroy();
				}
			}
		}
	}

	// PLAYER VS ENEMY DAMAGE
	//Getting both the colliders
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;

	for (auto& enemy : enemyGroup) {
		SDL_Rect enemyCol = enemy->getComponent<ColliderComponent>().collider;

		//Condition to collide
		if (Collision::AABB(playerCol, enemyCol)) {

			// Only take damage if 2000ms (2 seconds) have passed since the last hit
			if (SDL_GetTicks() > lastDamageTime + 2000) {
				//Damage
				std::cout << "Player Hit!" << std::endl;
				health--;
				healthLabel->SetText("Health: " + std::to_string(health));

				// Resetting the timer so player can't get hit again immediately
				lastDamageTime = SDL_GetTicks();

				//Closes the window since player lost
				if (health <= 0) {
					std::cout << "GAME OVER!" << std::endl;
					gameOver = true;
				}
				if (score > highScore)
				{
					char key = 'X'; // The secret key
					highScore = score; // Update variable

					highScoreLabel->SetText("Best: " + std::to_string(highScore));

					std::cout << "New High Score!" << std::endl;

					// SAVE TO FILE
					std::ofstream saveFile("shade_save.dat", std::ios::binary);
					if (saveFile.is_open())
					{
						int encryptedScore = highScore ^ key; // Encrypt

						std::cout << "Real Score: " << highScore << " | Encrypted in File: " << encryptedScore << std::endl;

						saveFile.write(reinterpret_cast<char*>(&encryptedScore), sizeof(encryptedScore));
						saveFile.close();
					}
				}
			}
		}
	}
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));



void GameLoop::render()
{
	SDL_RenderClear(renderer);

	if (isMenu) 
	{
		// Only draw the menu text
		titleLabel->draw();
		startLabel->draw();
		highScoreLabel->draw();
	}
	else
	{
		//DRAW EVERYONE
		for (auto& t : tiles)
		{
			t->draw();
		}
		for (auto& p : players)
		{
			p->draw();
		}
		for (auto& e : enemies)
		{
			e->draw();
		}

		//Draw the text
		scoreLabel->draw();
		//heart size, texture and position variables
		int heartSizeX = 32; // size x of heart
		int heartSizeY = 28; // size y of heart
		int startX = 432; // X position 
		int startY = 70;  // Y position

		//for every health, draw one (++ stuff)
		for (int i = 0; i < health; i++)
		{
			// Source Rectangle
			SDL_Rect srcRect = { 0, 0, 32, 32 };

			// Destination Rectangle
			// multiplying 'i' by 50 to space them out
			SDL_Rect destRect = { startX + (50 * i), startY, heartSizeX, heartSizeY };

			// Draw it
			TextureManager::Draw(heartTexture, srcRect, destRect, SDL_FLIP_NONE);
		}

		// Draw Ammo Bar
		int bulletX = 500;
		int bulletY = 900;

		for (int i = 0; i < ammo; i++)
		{
			// Draw a small yellow rectangle for each bullet
			SDL_Rect bulletRect = { bulletX + (15 * i), bulletY, 10, 20 };

			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
			SDL_RenderFillRect(renderer, &bulletRect);

			// Reset color to black (background) for next loop
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		}
		
		//Gameover screen
		if (gameOver)
		{
			gameOverLabel->draw();
		}

		// draw everything in collider group
		auto& collidables = manager.getGroup(groupColliders);
		for (auto& c : collidables) {
			// only draw projectile cuz everything else is already drawn
			if (c->getComponent<ColliderComponent>().tag == "projectile") {
				c->draw();
			}
		}
	}
	

	SDL_RenderPresent(renderer);
}

//Spawn potion function 
void GameLoop::spawnPotion(float x, float y)
{
	auto& potion(manager.addEntity()); //adding as an entity
	potion.addComponent<TransformComponent>(x, y, 32, 32, 1); //Adding transform
	potion.addComponent<SpriteComponent>("assets/heart.png", false); // Reusing heart image
	potion.addComponent<ColliderComponent>("potion"); // Tag 
	potion.addGroup(groupMap);
}


void GameLoop::handleEvents()
{
	//If the user clicks on X, close the game.
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;

	case SDL_KEYDOWN:

		if (gameOver)
		{
			if (event.key.keysym.sym == SDLK_r) {
				resetGame();
				std::cout << "Game Restarted!" << std::endl;
			}
			return; // Ignore other keys while on Game Over screen
		}

		if (isMenu)
		{
			// If in menu, only listen for Enter key
			if (event.key.keysym.sym == SDLK_RETURN) {
				isMenu = false; // Start the game!
				std::cout << "Game Started!" << std::endl;
			}
			return; // Ignore other keys while in menu
		}
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			// Get Player Position
			Vector2D playerPos = player.getComponent<TransformComponent>().position;

			// Calculate Bullet Start Position
			Vector2D bulletPos;

			// Y-Axis
			bulletPos.y = playerPos.y + 15;

			// X-Axis
			if (playerDir == 1) {
				bulletPos.x = playerPos.x + 65;
			}
			else {
				bulletPos.x = playerPos.x - 32;
			}

			if (ammo > 0)
			{
				ammo--;

				// Spawn the projectile
				spawnProjectile(bulletPos, Vector2D(playerDir, 0));
				Mix_PlayChannel(-1, sfxShoot, 0);
				std::cout << "Projectile Spawned!" << std::endl;
			}
			else
			{
				std::cout << "Out of Ammo! Press R to reload" << std::endl;
			}
			
		}
		if (event.key.keysym.sym == SDLK_r)
		{
			ammo = 10; // Refill
			std::cout << "Reloaded!" << std::endl;
		}
		break;

	default:
		break;
	}
}
//when enemy dies, spawn a blood png and make it explode 
void GameLoop::spawnBlood(float x, float y)
{
	for (int i = 0; i < 10; i++)
	{
		auto& blood(manager.addEntity());

		// add Transform
		auto& trans = blood.addComponent<TransformComponent>(x, y, 8, 8, 1);

		// sprite
		blood.addComponent<SpriteComponent>("assets/blood.png", false);

		// random Velocity (Explosion effect)
		float randomX = ((rand() % 20) - 10) / 5.0f;
		float randomY = ((rand() % 20) - 10) / 5.0f;
		Vector2D velocity(randomX, randomY);

		// velocity so it can move to different directions
		trans.velocity = velocity;
		trans.speed = 200; 

		// projectile Logic (Range, Speed, Velocity)
		// // reusing projectile component to achieve the splash effect.
		blood.addComponent<ProjectileComponent>(500, 2, velocity);

		// Group
		blood.addGroup(groupMap);
	}
}

//renders the tile (wall)
void GameLoop::addTile(int id, int x, int y)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 32, 32, id);

	//if the ID is 1, add collider in it.
	if (id == 1)
	{
		tile.addComponent<ColliderComponent>("wall");
		tile.addGroup(groupColliders);
	}
	//add it to the map group.
	tile.addGroup(groupMap);
}


//Destroys and cleans the memory.
void GameLoop::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	//delete audios
	Mix_FreeMusic(bgm);       // Free Music
	Mix_FreeChunk(sfxShoot);  // Free SFX
	Mix_Quit();               // Close Audio System
	//Delete text
	delete scoreLabel;
	delete healthLabel;
	delete gameOverLabel;
	delete titleLabel;
	delete startLabel;

	TTF_Quit();

	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}