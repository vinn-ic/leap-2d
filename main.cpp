#include "raylib.h"
#include <iostream>

//arrumar o erro da camera!!!!!

using namespace std;

struct player{
    Rectangle rect;
    Vector2 velocity;
    bool isOnground;
};


int main(){
char key_direction = 'i';
const int screenwidth = 1600;
const int screenheight = 900;

InitWindow(1600, 900, "game-2d-teste");

player player = {{100, screenheight - 60, 50,50},{0,0}, true};
const float gravidade = 400.0f;
const float jumpforce = -350.0f;
const float movespeed = 200.0f;
Vector2 playerposition = {player.rect.x, player.rect.y};


Rectangle ground = {0, screenheight - 15, screenwidth, 10};

Camera2D camera = { 0 };
camera.target = playerposition;
camera.offset = (Vector2){screenwidth/2.0f, 840};
camera.rotation = 0.0f;          // Sem rotação
camera.zoom = 1.0f;

SetTargetFPS(60);
while (!WindowShouldClose()){
    //main loop
float dt = GetFrameTime();// tudo que for usar a velocidade e etc usar o *dt 


if(!player.isOnground){
    //se o player estiver pulando
    player.velocity.y += gravidade*dt;
    cout << "dont ground" << "\n";
}
else if(player.isOnground){
    // se o player estiver no chão
    cout << "player ground!" << "\n";
}

if(IsKeyDown(KEY_D)){
    //mover para direita
    key_direction = 'D';
    player.rect.x += movespeed*dt;
}
if(IsKeyDown(KEY_A)){
    //mover para a esquerda
    key_direction = 'A';
    player.rect.x -= movespeed*dt;
}
if(IsKeyDown(KEY_SPACE) && player.isOnground){
    //pulo
    cout << "space on" << "\n";
    player.velocity.y = jumpforce;
    player.isOnground = false;
}
player.rect.y += player.velocity.y * dt; // pasando as info do pulo para o rect.y para ter a vizualização e animação!

if(IsKeyPressed(KEY_Q)){

    float desh = 150.0f;
    switch (key_direction){
        case 'A':
            if(IsKeyDown(KEY_W)){
                player.rect.y -= desh/2;
                player.isOnground = false;
            }
            player.rect.x -= desh;
        break;
        case 'D':
            if(IsKeyDown(KEY_W)){
                player.rect.y -= desh/2;
                player.isOnground = false;
            }
            player.rect.x += desh;
        break;
        case 'i':
            cout << "inital" << "\n";
        break;
        
    }
}

if(CheckCollisionRecs(player.rect, ground)){
    player.rect.y = ground.y - player.rect.height;
    player.velocity.y = 0;
    player.isOnground = true;
}

playerposition = {player.rect.x, 840};
camera.target = playerposition;


BeginDrawing();
ClearBackground(RAYWHITE);

BeginMode2D(camera);
DrawRectangle(150,50,150,150, RED);
DrawRectangleRec(ground, GREEN);
DrawRectangleRec(player.rect, BLUE);

EndDrawing();
}

CloseWindow();
return 0;
}
//player.isOnGround = true/false, se esta no chão no momento,