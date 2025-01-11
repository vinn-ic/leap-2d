#include "raylib.h"
#include <iostream>
#include <vector>
#include <limits>   

using namespace std;

int deaths = 0;
int amount_desh = 3;
const int screenwidth = 1600;
const int screenheight = 900;
const float gravidade = 550.0f;
const float jumpforce = 300.0f;
const float dash_distance = 250.0f;
const float movespeed = 6000.0f;
const float horizontaldrag = 400.0f;//força do efeito de parar gradativamente!


//fazer uma maneira de apagar as estruturas passadas!!!!, e criar novas se passar de fase
struct Player{
    Rectangle rect;
    Vector2 velocity;
    bool isOnground;
};
struct Enemy{
    Rectangle rect;
    bool movingright;
};

struct GameState{
    int currentPhase;
    int deaths;
    int respawn;
    int dashCount;
};

//variaveis globais!
GameState gamestate = {1,0,100,3};
vector<Rectangle> grounds;
Player player;
Enemy enemy;
int posplayer = player.rect.x;


Vector2 Vector2Lerp(Vector2 start, Vector2 end, float alpha) {
    return (Vector2){
        start.x + alpha * (end.x - start.x),
        start.y + alpha * (end.y - start.y)
        };
}

void logicagame(float dt){
    if(IsKeyDown(KEY_D)){
        //mover para direita
        player.velocity.x += movespeed*dt;
        if(player.velocity.x >= 300) player.velocity.x = 300;
    }
    if(IsKeyDown(KEY_A)){
        //mover para a esquerda
        player.velocity.x -= movespeed*dt;
        if(player.velocity.x <= -300) player.velocity.x = -300;
    }
    if(IsKeyDown(KEY_SPACE) && player.isOnground){
        //pulo
        player.velocity.y -= jumpforce;
        player.isOnground = false;
    }

    //desh
    if(IsKeyPressed(KEY_Q) && gamestate.dashCount > 0){
        gamestate.dashCount--;
        player.rect.x += (IsKeyDown(KEY_D) ? dash_distance : -dash_distance);
    }
}


void physics(float dt){
    //tudo que for da gravidade e etc, é aqui!
    player.velocity.y += gravidade*dt;//gravidade do game

    // fazer o efeito de parar gradativamente
    if(player.velocity.x > 0){
        player.velocity.x -= horizontaldrag*dt*2;
        if(player.velocity.x < 0) player.velocity.x = 0;
        
    }
    else if(player.velocity.x < 0){
        player.velocity.x += horizontaldrag*dt*2;
        if(player.velocity.x > 0) player.velocity.x = 0;
    }

    player.rect.y += player.velocity.y*dt; // pasando as info do pulo para o rect.y para ter a vizualização e animação!
    player.rect.x += player.velocity.x*dt;

    for(const auto& ground : grounds){
        if(CheckCollisionRecs(player.rect, ground)){
            player.rect.y = ground.y - player.rect.height;
            player.velocity.y = 0;
            player.isOnground = true;
        }
    }

    if(player.rect.y > screenheight + 75){
        //morrer por queda
        gamestate.deaths++;
        gamestate.dashCount = 3;
        player.rect.x = gamestate.respawn;
        player.rect.y = 440;
    }
}

void UpadateEnemy(){
    if (enemy.movingright) { 
        enemy.rect.x += 5; 
        if (enemy.rect.x >= 1390) enemy.movingright = false; 
    } else{
        enemy.rect.x -= 5;
        if (enemy.rect.x <= 1090) enemy.movingright = true;
    } 

    if(CheckCollisionRecs(player.rect, enemy.rect)){
        gamestate.deaths++;
        gamestate.dashCount = 3;
        player.rect.x = gamestate.respawn;
        player.rect.y = 440;
    }
}

void Render(Camera2D &camera){
    BeginDrawing();
    ClearBackground(SKYBLUE);

    BeginMode2D(camera);
    for(const auto &ground : grounds){
        DrawRectangleRec(ground, GREEN);
    }
    DrawRectangleRec(player.rect, BLUE);
    DrawRectangleRec(enemy.rect, RED);

    EndMode2D();

    DrawText(TextFormat("mortes: %d", gamestate.deaths), 10, 10,20, RED);
    DrawText(TextFormat("dashes: %d",gamestate.dashCount), 10,40,20,RED);

    EndDrawing();
}

 
int main(){
    InitWindow(screenwidth, screenheight, "game-2d");
    SetTargetFPS(60);


    Camera2D camera = {0};
    camera.offset = {screenwidth/2.0f, screenheight/2.0f};
    camera.zoom = 1.0f;
    
    grounds = {
        {0, 500, 850, 20},
        {1090, 500, 350, 20},
        {1740, 500, 300, 20} // Goal
    };
    player = {{100, 440, 50, 60}, {0, 0}, true};
    enemy = {{1100, 440, 50, 60}, true};

    gamestate.respawn = 100;
    gamestate.dashCount = 3;

    int lastPhase = gamestate.currentPhase; // Rastreamento da fase atual

    while(!WindowShouldClose()){
        float dt = GetFrameTime(); 

        if (player.rect.x >= 1740 && player.rect.y == 440 && gamestate.currentPhase == 1) {
            gamestate.currentPhase = 2; // Atualiza para a fase 2
        }

       if (gamestate.currentPhase != lastPhase) {
            lastPhase = gamestate.currentPhase;

            if (gamestate.currentPhase == 2) {
                // Lógica inicial da fase 2
                grounds.clear();
                grounds.push_back({1740, 500, 300, 20});
                grounds.push_back({2340, 500, player.rect.width, 15});
                enemy.rect.y = 9999;
                gamestate.respawn = 1760;

            }
        }grounds.push_back({2340, 500, player.rect.width,15});


        if(gamestate.currentPhase == 2){
            // Atualiza o terreno dinamicamente
            if (player.rect.x >= 2290 && player.rect.x <= 2390 && player.rect.y == 440) {
                grounds.push_back({2700, 500, player.rect.width, 15});
                gamestate.respawn = 2340;
            }
            if (player.rect.x >= 2650 && player.rect.x <= 2750 && player.rect.y == 440) {
                grounds.clear();
                grounds.push_back({3000, 500, player.rect.width, 15});
            }
            if (player.rect.x >= 2950 && player.rect.x <= 3050 && player.rect.y == 440) {
                grounds.clear();
                grounds.push_back({3300, 500, player.rect.width, 15});
            }
            if (player.rect.x >= 3250 && player.rect.x <= 3350 && player.rect.y == 440) {
                grounds.clear();
                player.velocity.y -= 1500; // Faz o jogador pular
                grounds.push_back({6000, 500, 300, 15}); // Goal
            }
        };
        
            
        



        logicagame(dt);
        physics(dt);
        UpadateEnemy();

        cout << player.rect.y << "\n";

        camera.target = Vector2Lerp(camera.target, {player.rect.x, player.rect.y},0.1f);

        Render(camera);
      
    }
    CloseWindow();
    return 0;
}
    