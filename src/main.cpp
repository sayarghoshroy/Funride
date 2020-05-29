#include "main.h"
#include "timer.h"
#include "ball.h"
#include "platform.h"
#include "player.h"
#include "stdio.h"
#include "coin_a.h"
#include "projectile_a.h"
#include "enemy_bar.h"
#include "boom.h"
#include "line.h"
#include "mag.h"
#include "water.h"
#include "vis.h"
#include "snow.h"
#include "circle.h"
#include "semi.h"
#include "thrust.h"
#include "bits/stdc++.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

int score = 0, twice = 1, level = 1, level_up = 2500, sword = 0, sword_max_time = 900, sword_time = 0;

double ring_centre_x = 20.0, ring_centre_y = 3.5; 


Ball ball1;
Ball ball2;
Platform platform;
Player player;
Coin_a coin1[32];
int invalid_coin[32];

Vis vis;
struct bounding_box_t vis_box;

struct bounding_box_t projectile_a[8];
Projectile_a first_projectiles[8];
int activate_projectile_a[8];
int invalid_projectile[8];

struct bounding_box_t boombox[8];
Boom booms[8];
int activate_boom[8];
int invalid_boom[8];

struct bounding_box_t enemy_bar_box[8];
Enemy_bar bar_enemies[8];
int invalid_bar[8];

pair < pair < double, double >, double > line_loc[32];
int invalid_line[32];

struct bounding_box_t mag_box[4];
Mag mags[4];
int mag_state[4]; // 0 : Asleep, 1 : Awake, 2 : Extinct

struct bounding_box_t water_box[128];
Water water[128];
int water_valid[128]; // 0 : Exists, 1 : Used
int water_gen = 0; // Number of Balloons generated

Circle line_ends[32][2];

struct bounding_box_t snow_box;
Snow snow;
int snow_exists = 0;
int timer = 0;

struct bounding_box_t coina[32];

int move_left =0, move_right = 0, upthrust = 0;
double gravity = 0.004;

struct bounding_box_t b1;
struct bounding_box_t b2;

struct bounding_box_t p;
struct bounding_box_t sayar;

int temp = 0;

int not_done = 1;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    //glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    glm::vec3 eye ( player.position[0] - 0.25, 4, 1 );

    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target ( player.position[0] - 0.25, 4, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up ( 0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    //ball1.draw(VP);

    Semi semi_a = Semi(ring_centre_x, ring_centre_y, 3.0, COLOR_GREEN);
    Semi semi_b = Semi(ring_centre_x, ring_centre_y, 2.98, COLOR_BACKGROUND);
    semi_a.draw(VP);
    semi_b.draw(VP);

    platform.draw(VP);
    if(sword == 0)
    	player.draw(VP);
    else
    {
    	Player temp = Player(player.position[0], player.position[1], COLOR_PINK);
    	temp.draw(VP);
    }
    if(upthrust == 1)
    {
    	Thrust thrust = Thrust(player.position[0] + 0.25, player.position[1], COLOR_FIRE);
    	thrust.draw(VP);
    }
    //if(temp == 0)
    //	coin1.draw(VP);

    for(int i = 0; i < 20; ++i)
    {
    	if(invalid_coin[i] == 0)
    	{
    		Coin_a temp;
    		if( i % 2 == 0)
    			temp = Coin_a(coina[i].x, coina[i].y, COLOR_GREY);
    		else
    			temp = Coin_a(coina[i].x, coina[i].y, COLOR_GOLD);
    		temp.draw(VP);
    	}
    }

    for(int i = 0; i < 2; ++i)
    {
    	if(invalid_projectile[i] == 0 && activate_projectile_a[i] == 1)
    	{
    		first_projectiles[i].draw(VP);
    	}
    }

    for(int i = 0; i < 2; ++i)
    {
    	if(invalid_bar[i] == 0)
    	{
    		bar_enemies[i].draw(VP);
    		Circle left = Circle(bar_enemies[i].position[0], bar_enemies[i].position[1] + 0.5, 0.5, COLOR_PURPLE);
    		Circle right = Circle(bar_enemies[i].position[0] + 5, bar_enemies[i].position[1] + 0.5, 0.5, COLOR_PURPLE);
    		left.draw(VP);
    		right.draw(VP);
    	}
    }

    for(int i = 0; i < 2; ++i)
    {
    	if(invalid_boom[i] == 0 && activate_boom[i] == 1)
    	{
    		booms[i].draw(VP);
    	}
    }

    for(int i = 0; i < 20; ++i)
    {
    	if(invalid_line[i] == 0)
    	{
    		Line temp_draw;
    		temp_draw = Line(line_loc[i].first.first, line_loc[i].first.second, line_loc[i].second, COLOR_FIRE);
    		temp_draw.draw(VP);
    		line_ends[i][0].draw(VP);
    		line_ends[i][1].draw(VP);
    	}

    }

    for(int i = 0; i < 4; ++i)
    {
    	if(mag_state[i] == 1)
    	{
    		mags[i].draw(VP);
    	}
    }

    for(int i = 0; i < water_gen; ++i)
    {
    	if(water_valid[i] == 0)
    	{
    		water[i].draw(VP);
    	}
    }

    vis.draw(VP);

    if(snow_exists)
    	snow.draw(VP);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE && water_gen < 100)
    {
        double x_coord = player.position[0] + 0.5;
		double y_coord = player.position[1] + 0.125;
		water_box[water_gen].x = x_coord;
		water_box[water_gen].y = y_coord;
		water_box[water_gen].height = 0.25;
		water_box[water_gen].width = 0.25;

		Water temp;
		temp = Water( (float)x_coord, (float)y_coord, COLOR_BLUE);
		water[water_gen++] = temp;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if(yoffset == 1.0)
		screen_zoom += 0.01;
	else if(yoffset == -1.0)
		screen_zoom -= 0.01;
	reset_screen();
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    int balloon = glfwGetKey(window, GLFW_KEY_UP);
    if(balloon)
    	glfwSetKeyCallback(window, key_callback);

    glfwSetScrollCallback(window, scroll_callback);

    move_left =0;
    move_right = 0;
    upthrust = 0;
    if (left)
    {
        move_left = 1;
    }

    else if (right)
    {
    	move_right = 1;
    }

    if (up)
    {
    	upthrust = 1;
    }
}

int in_ring = 0;


void tick_elements() {
	++timer;
	timer %= 150;

	if(sword == 1)
	{
		++sword_time;
		if(sword_time > sword_max_time)
			sword = 0;
	}

	int lat;
	if(move_left == 1)
		lat = 1;
	else if(move_right == 1)
		lat = 2;
	else
		lat = 0;

	double dist_sqr = (player.position[0] + 0.25 - ring_centre_x) * (player.position[0] + 0.25 - ring_centre_x) + (player.position[1] - ring_centre_y) * (player.position[1] - ring_centre_y);
    if(dist_sqr < 3.0 * 3.0 && dist_sqr > 2.8 * 2.8 && in_ring == 0 && player.position[1] > ring_centre_y)
    {
    	in_ring = 1;
    	printf("You have entered the protective ring.\n");
    }

    if(in_ring == 1)
    {
    	player.tick(lat, 0, 0);
    	if(player.position[0] + 0.25 < ring_centre_x - 2.99 || player.position[0] + 0.25 > ring_centre_x + 2.99)
    	{
    		in_ring = 0;
    		printf("You have left the protective ring.\n");
    	}

    	if(in_ring == 1)
    	{
	    	double x_bar = player.position[0] + 0.25 - ring_centre_x;
	    	double y_bar = sqrt( 2.99 * 2.99 - x_bar * x_bar );
	    	player.position[1] = ring_centre_y + y_bar;
	    }
    }

    else
    	player.tick(lat, upthrust, gravity);

    if( level == 2 )
    {
    	player.position[0] += 0.04;
    	sayar.x += 0.04;
    }

    if(vis.position[1] == player.position[1])
    	vis.tick(0);
    else
    	vis.tick((player.position[1] - vis.position[1]) / abs(vis.position[1] - player.position[1]));
    vis_box.y = vis.position[1];

    if(timer == 0)
    {
    	snow_exists = 1;
    	
    	snow_box.x = vis_box.x - 0.25;
    	snow_box.y = vis_box.y + 0.125;
    	snow_box.height = 0.25;
    	snow_box.width = 0.25;
    	snow.set_position(snow_box.x, snow_box.y);
    }

    if(snow_exists)
    {
    	snow.tick(gravity);
    	snow_box.x = snow.position[0];
    	snow_box.y = snow.position[1];
    	if(snow_box.y < 2)
    		snow_exists = 0;
    }
    
    for(int j=0; j<2; ++j)
    {
    	if(invalid_projectile[j] == 1)
    		continue;
    	if(activate_projectile_a[j] == 0 && ((first_projectiles[j].position[0]) - (player.position[0])) < 5)
    		activate_projectile_a[j] = 1;
    	if(activate_projectile_a[j] == 0)
    		continue;
    	first_projectiles[j].tick(gravity/50);
    	projectile_a[j].x = first_projectiles[j].position[0];
    	projectile_a[j].y = first_projectiles[j].position[1]; 
    	if(first_projectiles[j].position[1] <= 2)
    		invalid_projectile[j] = 1;
    }

    for(int j=0; j<2; ++j)
    {
    	if(invalid_bar[j] == 1)
    		continue;
    	bar_enemies[j].tick();
    	enemy_bar_box[j].x = bar_enemies[j].position[0];
    	enemy_bar_box[j].y = bar_enemies[j].position[1];
    }

    for(int j=0; j<2; ++j)
    {
    	if(invalid_boom[j] == 1)
    		continue;
    	if( activate_boom[j] == 0 && ((booms[j].position[0]) - (player.position[0])) < 5)
    		activate_boom[j] = 1;
    	if(activate_boom[j] == 0)
    		continue;
    	booms[j].tick();
    	boombox[j].x = booms[j].position[0];
    	boombox[j].y = booms[j].position[1]; 
    	if(booms[j].position[1] <= 2)
    		invalid_boom[j] = 1;
    }

    for(int j=0; j<4; ++j)
    {
    	if(in_ring == 1)
    		continue;
    	if(mag_state[j] == 2)
    		continue;
    	if(mag_state[j] == 0 && ((mags[j].position[0]) - (player.position[0])) < 5)
    		mag_state[j] = 1;
    	if(mag_state[j] == 0)
    		continue;
    	double mag_x, mag_y, play_x, play_y;
    	mag_x = mags[j].position[0];
    	mag_y = mags[j].position[1];
    	play_x = player.position[0];
    	play_y = player.position[1];

    	double theta;

    	if(mag_x != play_x)
    	{
	    	theta = atan(abs((mag_y - play_y) / (mag_x - play_x)));
	    	
	    	if(mag_y > play_y && mag_x > play_x) //Quad 1
	    		theta = theta;

	    	else if(mag_y > play_y && mag_x < play_x) //Quad 2
	    		theta = 3.14 - theta;

	    	else if(mag_y < play_y && mag_x < play_x) //Quad 3
	    		theta = -3.14 + theta;

	    	else //Quad 4
	    		theta = -theta;
	    }
	    else
	    {
	    	if(mag_y > play_y)
	    		theta = 3.14 / 2;
	    	else
	    		theta = -3.14 / 2;
	    }

    	double magnet_attract = 0.2;

    	player.position[0] += (magnet_attract * cos(theta)); 
    	player.position[1] += (magnet_attract * sin(theta));

    	play_x = player.position[0];
    	play_y = player.position[1];

    	if( (play_x - mag_x) * (play_x - mag_x) + (play_y - mag_y) * (play_y - mag_y) < 0.707)
    		mag_state[j] = 2;
    }

    for(int i = 0; i < water_gen; ++i)
    {
    	if(water_valid[i] == 1)
    		continue;
    	water[i].tick(gravity);
    	water_box[i].x = water[i].position[0];
    	water_box[i].y = water[i].position[1];
    	if(water_box[i].y <= 2)
    		water_valid[i] = 1;
    }

}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    float ball1_init_height = 0.5;
    float ball2_init_height = -3;

    
    ball1       = Ball(1, ball1_init_height, COLOR_RED);

    b1.x = 1;
    b1.y = ball1_init_height;
    b1.height = 2;
    b1.width = 2;

    ball2       = Ball(1, ball2_init_height, COLOR_GREEN);

    b2.x = -10;
    b2.y = ball2_init_height;
    b2.height = 2;
    b2.width = 200;

    platform = Platform(COLOR_GREEN);

    p.x = 0;
    p.y = 0;
    p.height = 2;
    p.width = 200;

    player = Player(1.0, 2.0, COLOR_KNIGHT);

    sayar.x = 1;
    sayar.y = 2;
    sayar.height = 0.5;
    sayar.width = 0.5;

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1200;
    int height = 1200;

	for(int i = 0; i < 20; ++i)
	{
		double x_coord = (rand() / (RAND_MAX + 1.0)) * 5 + 5*i;
		double y_coord = (rand() / (RAND_MAX + 1.0)) * (7 - 3) + 3;

		coina[i].x = x_coord;
	    coina[i].y = y_coord;
	    coina[i].height = 0.15;
	    coina[i].width = 0.15;

	    //coin1[i] = Coin_a(x_coord,y_coord,COLOR_GREEN);
	}

    window = initGLFW(width, height);

    initGL (window, width, height);

    printf("Level 1\n");

    vis = Vis(COLOR_BLACK);

    vis_box.x = 95;
    vis_box.y = 2;
    vis_box.height = 0.5;
    vis_box.width = 0.5;

    snow = Snow(vis_box.x - 0.25, vis_box.y + 0.125, COLOR_BLUE);

    snow_box.x = vis_box.x - 0.25;
    snow_box.y = vis_box.y + 0.125;
    snow_box.height = 0.25;
    snow_box.width = 0.25;

    //snow_exists = 1;

    for(int j = 0; j < 2; ++j)
	{
		double x_coord = (rand() / (RAND_MAX + 1.0)) * 5 + 25 * (j+1);
		projectile_a[j].x = x_coord;
		projectile_a[j].y = 7.5;
		projectile_a[j].height = 0.25;
		projectile_a[j].width = 0.25;

		Projectile_a temp;
		temp = Projectile_a( (float)x_coord, 7.5f, COLOR_OLIVE);
		first_projectiles[j] = temp;
	}

	for(int j = 0; j < 2; ++j)
	{
		double x_coord = (rand() / (RAND_MAX + 1.0)) * 5 + 30 * (j+1);
		enemy_bar_box[j].x = x_coord;
		enemy_bar_box[j].y = 4.0;
		enemy_bar_box[j].height = 1.0;
		enemy_bar_box[j].width = 5.0;

		Enemy_bar temp;
		temp = Enemy_bar( (float)x_coord, 4.0f, COLOR_FIRE);
		bar_enemies[j] = temp;
	}

	for(int j = 0; j < 2; ++j)
	{
		double x_coord = (rand() / (RAND_MAX + 1.0)) * 5 + 25 * (j+1);
		boombox[j].x = x_coord;
		boombox[j].y = 7.5;
		boombox[j].height = 0.25;
		boombox[j].width = 0.25;

		Boom temp;
		temp = Boom( (float)x_coord, 7.5f, COLOR_WOOD);
		booms[j] = temp;
	}

	for(int j = 0; j < 20; ++j)
	{
		double x_coord = (rand() / (RAND_MAX + 1.0)) * 5 + 5 * j;
		double y_coord = (rand() / (RAND_MAX + 1.0)) * 2 + 3;
		line_loc[j].first.first = x_coord;
		line_loc[j].first.second = y_coord;
		line_loc[j].second = (rand() % 180);
		double theta = line_loc[j].second * M_PI / 180.0;

		line_ends[j][0] = Circle(x_coord, y_coord, 0.1, COLOR_PURPLE);
		line_ends[j][1] = Circle(x_coord + 5.0 * cos(theta), y_coord + 5.0 * sin(theta), 0.1, COLOR_PURPLE);
	}

	for(int j = 0; j < 4; ++j)
	{
		double x_coord = (rand() / (RAND_MAX + 1.0)) * 5 + 5 * (2*j + 1) + 40;
		double y_coord = (rand() / (RAND_MAX + 1.0)) * 4 + 3;
		mag_box[j].x = x_coord;
		mag_box[j].y = y_coord;
		mag_box[j].height = 0.25;
		mag_box[j].width = 0.25;

		Mag temp;
		temp = Mag( (float)x_coord, (float)y_coord, COLOR_SKY);
		mags[j] = temp;
	}

	int gameover = 0;

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        if(player.position.x > 99 || gameover == 1)
        	break;


        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();

            tick_input(window);
        }

        // Poll for Keyboard and mouse events

        sayar.x = player.position[0];
        sayar.y = player.position[1];

        for(int i = 0; i < 20; ++i )
        {
	        if(invalid_coin[i] == 0 && player_coin_col(sayar, coina[i]))
	        {
	        	invalid_coin[i] = 1;
	        	score += (50 * twice * ((i % 2) + 1));
	        	if(i % 2 == 1)
	        		printf("You Got a Gold Coin. Score = %d\n", score);
	        	else
	        		printf("You Got a Silver Coin. Score = %d\n", score);
	        	if(score > level_up && level == 1)
	        	{
	        		++level;
	        		sword = 1;
	        		printf("Level 2\n");
	        	}
	        }
	    }


		for(int i = 0; i < 2; ++i )
        {
        	if(invalid_bar[i] == 1)
        		continue;
        	enemy_bar_box[i].x = bar_enemies[i].position[0];
        	enemy_bar_box[i].y = bar_enemies[i].position[1];
	        if(player_bar_col(sayar, enemy_bar_box[i]))
	        {
	        	invalid_bar[i] = 1;
	        	if(in_ring == 1)
	        	{
	        		printf("You touched a fire bar. The ring saved you.\n");
	        	}
	        	else if(sword == 1)
	        	{
	        		printf("You went through a fire bar.\n");
	        	}
	        	else
	        	{
		        	score -= 200;
		        	printf("You hit a fire bar. Score = %d\n", score);
		        }
	        }
	    }

	    for(int i = 0; i < 2; ++i )
        {
        	if(invalid_boom[i] == 1)
        		continue;
        	boombox[i].x = booms[i].position[0];
        	boombox[i].y = booms[i].position[1];
	        if(player_boom_col(sayar, boombox[i]))
	        {
	        	invalid_boom[i] = 1;
	        	if(in_ring == 1)
	        	{
	        		printf("A Boomerang hit you. The ring saved you.\n");
	        	}
	        	else if(sword == 1)
	        	{
	        		printf("A boomerang hit you.\n");
	        	}
	        	else
	        	{
		        	score -= 100;
		        	printf("A boomerang hit you. Score = %d\n", score);
		        }
	        }
	    }

	    for(int i = 0; i < 2; ++i )
        {
        	if(invalid_projectile[i] == 1 || activate_projectile_a[i] == 0)
        		continue;
        	projectile_a[i].x = first_projectiles[i].position[0];
        	projectile_a[i].y = first_projectiles[i].position[1];

	        if(player_proj_col(sayar, projectile_a[i]))
	        {
	        	invalid_projectile[i] = 1;
	        	twice *= 2;
	        	printf("You caught a Projectile. Earn %d times for every coin from now on.\n", twice);
	        }
	    }

	    vis_box.x = vis.position[0];
	    vis_box.y = vis.position[1];
	    if(player_vis_col(sayar, vis_box))
	    {
	    	gameover = 1;
	    	score -= 500;
	    	printf("You collided with Viserion. Game Over. Score = %d\n", score);
	    }

	    snow_box.x = snow.position[0];
	    snow_box.y = snow.position[1];
	    if(snow_exists && player_snow_col(sayar, snow_box))
	    {
	    	snow_exists = 0;
	    	if(in_ring == 1)
	        {
	        	printf("A Snowball hit you. The ring saved you.\n");
	        }
	        else if(sword == 1)
	        {
	        	printf("You deflected a Snowball.\n");
	        }
	        else
	        {
		    	score -= 200;
		    	printf("You were hit by a snowball. Score = %d\n", score);
		    }
	    }

	    for(int i = 0; i < 2; ++i )
        {
        	if(invalid_bar[i] == 1)
        		continue;
        	enemy_bar_box[i].x = bar_enemies[i].position[0];
        	enemy_bar_box[i].y = bar_enemies[i].position[1];
        	for(int j = 0; j < water_gen; ++j)
        	{
        		if(water_valid[j] == 1)
        			continue;
        		water_box[j].x = water[j].position[0];
        		water_box[j].y = water[j].position[1];
		        if(water_bar_col(water_box[j], enemy_bar_box[i]))
		        {
		        	invalid_bar[i] = 1;
		        	water_valid[j] = 1;
		        	score += 500;
		        	printf("You removed a fire bar. Score = %d\n", score);
		        	if(score > level_up && level == 1)
		        	{
		        		++level;
		        		sword = 1;
		        		printf("Level 2\n");
		        	}
		        }
		    }
	    }

	    for(int i = 0; i < 20; ++i )
        {
	        if(invalid_line[i] == 0 && player_line_col(sayar, line_loc[i]))
	        {
	        	invalid_line[i] = 1;
	        	if(in_ring == 1)
	        	{
	        		printf("You touched a fire line. The ring saved you.\n");
	        	}
	        	else if(sword == 1)
	        	{
	        		printf("You went through a fire line.\n");
	        	}
	        	else
	        	{
		        	score -= 50;;
		        	printf("You Hit a Fire Line. Score = %d\n", score);
		        }
	        }
	    }

	    for(int i = 0; i < 20; ++i )
        {
	        if(invalid_line[i] == 1)
	        	continue;

	        for(int j = 0; j < water_gen; ++j)
        	{
        		if(water_valid[j] == 1)
        			continue;
        		water_box[j].x = water[j].position[0];
        		water_box[j].y = water[j].position[1];
		        if(water_line_col(water_box[j], line_loc[i]))
		        {
		        	invalid_line[i] = 1;
		        	water_valid[j] = 1;
		        	score += 200;
		        	printf("You removed a fire line. Score = %d\n", score);
		        	if(score > level_up && level == 1)
		        	{
		        		++level;
		        		sword = 1;
		        		printf("Level 2\n");
		        	}
		        }
		    }
	    }

        glfwPollEvents();
    }

    printf("Score = %d\n", score);
    quit(window);
    
}

bool player_coin_col(bounding_box_t a, bounding_box_t p) {
    return (abs(a.x - p.x + 0.175) * 2 < (a.width + p.width)) &&
           (abs(a.y - p.y + 0.175) * 2 < (a.height + p.height));
}

bool player_bar_col(bounding_box_t a, bounding_box_t p) {
    return (abs(a.x - p.x - 2.25) * 2 < (a.width + p.width)) &&
           (abs(a.y - p.y - 0.25) * 2 < (a.height + p.height));
}

bool player_boom_col(bounding_box_t a, bounding_box_t p) {
    return (abs(a.x - p.x + 0.125) * 2 < (a.width + p.width)) &&
           (abs(a.y - p.y + 0.125) * 2 < (a.height + p.height));
}

bool player_proj_col(bounding_box_t a, bounding_box_t p) {
    return (abs(a.x - p.x + 0.125) * 2 < (a.width + p.width)) &&
           (abs(a.y - p.y + 0.125) * 2 < (a.height + p.height));
}

bool player_vis_col(bounding_box_t a, bounding_box_t p) {
    return (abs(a.x - p.x) * 2 < (a.width + p.width)) &&
           (abs(a.y - p.y) * 2 < (a.height + p.height));
}

bool player_snow_col(bounding_box_t a, bounding_box_t p) {
    return (abs(a.x - p.x + 0.125) * 2 < (a.width + p.width)) &&
           (abs(a.y - p.y + 0.125) * 2 < (a.height + p.height));
}

bool water_bar_col(bounding_box_t a, bounding_box_t p) {
    return (abs(a.x - p.x - 2.375) * 2 < (a.width + p.width)) &&
           (abs(a.y - p.y - 0.375) * 2 < (a.height + p.height));
}

bool player_line_col(bounding_box_t a, std::pair < std::pair < double, double >, double > line)
{
	double p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y;
	p0_x = line.first.first;
	p0_y = line.first.second;
	p1_x = p0_x + 5 * cos((double)line.second * M_PI / 180.0);
	p1_y = p0_y + 5 * sin((double)line.second * M_PI / 180.0);

	p2_x = a.x; 
	p2_y = a.y;
	p3_x = a.x + 0.5;
	p3_y = a.y;
	if(get_line_intersection(p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y))
		return 1;

	p2_x = a.x + 0.5; 
	p2_y = a.y;
	p3_x = a.x + 0.5;
	p3_y = a.y + 0.5;
	if(get_line_intersection(p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y))
		return 1;

	p2_x = a.x + 0.5; 
	p2_y = a.y + 0.5;
	p3_x = a.x;
	p3_y = a.y + 0.5;
	if(get_line_intersection(p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y))
		return 1;

	p2_x = a.x; 
	p2_y = a.y + 0.5;
	p3_x = a.x;
	p3_y = a.y;
	if(get_line_intersection(p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y))
		return 1;

	return 0;
}

bool water_line_col(bounding_box_t a, std::pair < std::pair < double, double >, double > line)
{
	double p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y;
	p0_x = line.first.first;
	p0_y = line.first.second;
	p1_x = p0_x + 5 * cos((double)line.second * M_PI / 180.0);
	p1_y = p0_y + 5 * sin((double)line.second * M_PI / 180.0);

	p2_x = a.x; 
	p2_y = a.y;
	p3_x = a.x + 0.25;
	p3_y = a.y;
	if(get_line_intersection(p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y))
		return 1;

	p2_x = a.x + 0.25; 
	p2_y = a.y;
	p3_x = a.x + 0.25;
	p3_y = a.y + 0.25;
	if(get_line_intersection(p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y))
		return 1;

	p2_x = a.x + 0.25; 
	p2_y = a.y + 0.25;
	p3_x = a.x;
	p3_y = a.y + 0.25;
	if(get_line_intersection(p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y))
		return 1;

	p2_x = a.x; 
	p2_y = a.y + 0.25;
	p3_x = a.x;
	p3_y = a.y;
	if(get_line_intersection(p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y))
		return 1;

	return 0;
}

int get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y)
{
	//Returns: 1 if intersect, 0 otherwise
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    if((-s2_x * s1_y + s1_x * s2_y) == 0)
    	s = -1;
    else
    	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);

    if((-s2_x * s1_y + s1_x * s2_y) == 0)
    	t = -1;
    else
    	t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
        return 1;

    return 0; // No collision
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
