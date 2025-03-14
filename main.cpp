#include "raylib.h"
#include <iostream>
#include <vector>
#include <limits>  
#include <string> 

using namespace std;


const int screenwidth = 1350;
const int screenheight = 850;
const float gravidade = 550.0f;
const float jumpforce = 300.0f;
const float dash_distance = 150.0f;
const float movespeed = 6000.0f;
const float horizontaldrag = 400.0f;//força do efeito de parar gradativamente!


//fazer uma maneira de apagar as estruturas passadas!!!!, e criar novas se passar de fase
struct Player{
    Rectangle rect;
    Vector2 velocity;
    bool isOnground;
    bool isdoblejump; // se o player não deu o pulo doble jump // ver se o nome esta certo!
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
    bool loby;
};

struct BoolGronds{
    bool plataformaS1;
    bool plataformaS2;
    bool plataformaS3;
};


//variaveis globais!
GameState gamestate = {1,0,100,3};
vector<Rectangle> grounds;
Player player;
Enemy enemy;
Enemy enemy2;
BoolGronds boolgronds = {true, true, true};
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
    if(IsKeyPressed(KEY_SPACE) && player.velocity.y > -100 && player.velocity.y < 150 && !player.isdoblejump){
        player.velocity.y -= jumpforce;
        player.isdoblejump = true;
    }
    //desh
    if(IsKeyPressed(KEY_Q) && gamestate.dashCount > 0){
        if(IsKeyDown(KEY_A)){
            player.rect.x -= dash_distance;
            gamestate.dashCount--;
        }if(IsKeyDown(KEY_D)){
            player.rect.x += dash_distance;
            gamestate.dashCount--;
        }
    }
    if(IsKeyPressed(KEY_CAPS_LOCK)){
        gamestate.loby = false;
    }
}


void physics(float dt){
    if(gamestate.loby){

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
            player.isdoblejump = false;
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
    if(CheckCollisionRecs(player.rect, enemy2.rect)){
        gamestate.deaths++;
        gamestate.dashCount = 3;
        player.rect.x = gamestate.respawn;
        player.rect.y = 440;
    }
}

void Render(Camera2D &camera){
  
}

 
int main(){
    InitWindow(screenwidth, screenheight, "game-2d");
    SetTargetFPS(60);


    Camera2D camera = {0};
    camera.offset = {screenwidth/2.0f, screenheight/2.0f};
    camera.zoom = 1.0f;
    
    grounds = {
        {0, 500, 850, 20,},
        {1090, 500, 350, 20},
        {1740, 500, 300, 20}, // Goal
    };
    player = {{0, 400, 50, 60}, {0, 0}, true, false};
    enemy = {{1100, 440, 50, 60}, true};
    enemy2 = {{10520, 440, 50, 60}};

    gamestate.respawn = 100;
    gamestate.dashCount = 3;

    int lastPhase = gamestate.currentPhase; // Rastreamento da fase atual

    while(!WindowShouldClose()){
        cout << player.velocity.y << "\n";
        float dt = GetFrameTime(); 

        if (player.rect.x >= 1740 && player.rect.y == 440 && gamestate.currentPhase == 1) {
            gamestate.currentPhase = 2; // Atualiza para a fase 2
            gamestate.dashCount = 3;
        }

       if (gamestate.currentPhase != lastPhase) {
            lastPhase = gamestate.currentPhase;

            if (gamestate.currentPhase == 2) {
                // Lógica inicial da fase 2
                grounds.clear();
                grounds = {
                    {1740, 500, 300, 20},
                    {2340, 500, player.rect.width, 15},
                    {2700, 500, player.rect.width, 15},
                    {3000, 500, player.rect.width, 15},
                    {3500, 500, player.rect.width, 15},
                    {5200, 500, 300, 15}

                };
                enemy.rect.y = 9999;
                gamestate.respawn = 1760;

            }
        }


        if(gamestate.currentPhase == 2){
            // Atualiza o terreno dinamicamente
            if (player.rect.x >= grounds[1].x-60 && player.rect.x <= grounds[1].x+60 && player.rect.y >= 440) {
                //1
                grounds[1].y = 9999;
            }else{
                grounds[1].y = 500;
            }
            if (player.rect.x >= grounds[2].x-60 && player.rect.x <= grounds[2].x+60 && player.rect.y >= 440) {
                grounds[2].y = 9999;
            }else{
                grounds[2].y = 500;
            }
            if (player.rect.x >= grounds[3].x-60 && player.rect.x <= grounds[3].x+60 && player.rect.y >= 440) {
                grounds[3].y = 9999;
            }else{
                grounds[3].y = 500;
            }
            if (player.rect.x >= grounds[4].x-60 && player.rect.x <= grounds[4].x+60 && player.rect.y >= 440 && player.isOnground) {
                player.velocity.y -= 1650; // Faz o jogador pular
                player.isOnground = false;
            }else{
                grounds[4].y = 500;
            }
            if(player.rect.x >= 5200 && player.rect.x <= 5700 && player.rect.y == 440){
                //fase 3!!!
                gamestate.dashCount = 3; 
                gamestate.currentPhase = 3;
                grounds.clear();
                grounds = {
                    {5000, 500, 300, 15},
                    {5400, 500, 150, 20},
                    {6000, 500, 150, 20},
                    {6600, 500, 150, 20},
                    {7800, 500, 300, 15}//gool
                    };

            }
        };
        if(gamestate.currentPhase == 3){
            //logica fase3  
            gamestate.respawn = 5100;
            if(player.rect.x >= grounds[1].x && player.rect.x <= grounds[1].x+300 && player.rect.y == 440){
                if(boolgronds.plataformaS1) player.rect.x += 5;
                else if(!boolgronds.plataformaS1) player.rect.x -= 5; 
            }
            if(player.rect.x >= grounds[2].x && player.rect.x <= grounds[2].x+300 && player.rect.y == 440){
                if(boolgronds.plataformaS2) player.rect.x += 5;
                else if(!boolgronds.plataformaS2) player.rect.x -= 5;
            }
            if(player.rect.x >= grounds[3].x && player.rect.x <= grounds[3].x+300 && player.rect.y == 440){
                if(boolgronds.plataformaS3) player.rect.x += 5;
                else if(!boolgronds.plataformaS3) player.rect.x -= 5;
            }
            
            if(boolgronds.plataformaS1){
                grounds[1].x += 5;

                if(grounds[1].x >= 5925) boolgronds.plataformaS1 = false;
            }
            else if(!boolgronds.plataformaS1){
                grounds[1].x -= 5;
                if(grounds[1].x <= 5350) boolgronds.plataformaS1 = true;
            }

            if(boolgronds.plataformaS2){
                grounds[2].x += 5;

                if(grounds[2].x >= 6700) boolgronds.plataformaS2 = false;
            }
            else if(!boolgronds.plataformaS2){
                grounds[2].x -= 5;
                if(grounds[2].x <= 6075) boolgronds.plataformaS2 = true;
            }

            if(boolgronds.plataformaS3){
                grounds[3].x += 5;

                if(grounds[3].x >= 7500) boolgronds.plataformaS3 = false;
            }
            else if(!boolgronds.plataformaS3){
                grounds[3].x -= 5;
                if(grounds[3].x <= 6900) boolgronds.plataformaS3 = true;
            }

            if(player.rect.x >= 7650 && player.rect.x <= 7950 && player.rect.y == 440){
                boolgronds.plataformaS1 = true;
                boolgronds.plataformaS2 = true;
                boolgronds.plataformaS3 = true;
                //fase 4
                gamestate.dashCount = 4;
                gamestate.respawn = 7850;
                grounds.clear();
                grounds = {
                    {7800, 500, 300, 15},
                    {7900, 500, 60, 15},
                    {8100, 500, 60, 15},
                    {8600, 500, 60, 15},
                    {10000, 500, 600, 15},//enemy2
                    {10900, 500, 300, 15},//gool até o momento o final do game
                };
                gamestate.currentPhase = 4;
            }
        }
        if(gamestate.currentPhase == 4){
            //logica da fase4
            
            //config plat1
            if(boolgronds.plataformaS1){
                grounds[1].x += 5;
                
                if(grounds[1].x >= 8600) boolgronds.plataformaS1 = false;
            }else if(!boolgronds.plataformaS1) {
                grounds[1].x -= 5;
                
                if(grounds[1].x <= 8200) boolgronds.plataformaS1 = true;
            }

            if (player.rect.x >= grounds[1].x -60 && player.rect.x <= grounds[1].x+60 && player.rect.y >= 440) {
                grounds[1].y = 9999;
            }else{
                grounds[1].y = 500;
            }
            //config plat1 end

            //config plat2
            if(boolgronds.plataformaS2){
                grounds[2].x += 5; 

                if(grounds[2].x >= 9200) boolgronds.plataformaS2 = false;
            }else if(!boolgronds.plataformaS2){
                grounds[2].x -= 5;
                if(grounds[2].x <= 8800) boolgronds.plataformaS2 = true;
            }

            if (player.rect.x >= grounds[2].x -60 && player.rect.x <= grounds[2].x+60 && player.rect.y >= 440) {
                grounds[2].y = 9999;
            }else{
                grounds[2].y = 500;
            }
            // config plat2 end

            //config plat3
            if(boolgronds.plataformaS3){
                grounds[3].x += 5; 

                if(grounds[3].x >= 9900) boolgronds.plataformaS3 = false;
            }else if(!boolgronds.plataformaS3){
                grounds[3].x -= 5;
                if(grounds[3].x <= 9400) boolgronds.plataformaS3 = true;
            }

            if (player.rect.x >= grounds[3].x -60 && player.rect.x <= grounds[3].x+60 && player.rect.y >= 440) {
                grounds[3].y = 9999;
            }else{
                grounds[3].y = 500;
            }
            // config plat3 end
            
            //config enemy2
            if(player.rect.x >= grounds[4].x && player.rect.x <= grounds[4].x+600){
                if(enemy2.rect.x >= player.rect.x && enemy2.rect.x > grounds[4].x){
                        enemy2.rect.x -= 4;
                    
                }
                else if(enemy2.rect.x <= player.rect.x && enemy2.rect.x < grounds[4].x + 550){
                    enemy2.rect.x += 4;
                }
            }
            //config enemy2 end
        }
        

        
        logicagame(dt);
        physics(dt);
        UpadateEnemy();


        camera.target = Vector2Lerp(camera.target, {player.rect.x, player.rect.y},0.1f);
        // render!@!!!!!!!!! start
        float porcento_game;

        porcento_game = player.rect.x / 10900;
        porcento_game = porcento_game*100;
        if(porcento_game <= 0){
            porcento_game = 0;
        }
    
    
        if(!gamestate.loby){
            BeginDrawing();
            ClearBackground(SKYBLUE);
            BeginMode2D(camera);
    
            DrawRectangle(camera.target.x-400, camera.target.y-300, 800,600,RAYWHITE);
    
            DrawText("leap-2d",camera.target.x-60,camera.target.y-250,40,BLACK);
            DrawText("by-vinin4:20", camera.target.x-195, camera.target.y-180,20,BLACK);
    
            DrawText(TextFormat("mortes: %d",gamestate.deaths),camera.target.x-350, camera.target.y-100 ,20,BLACK);
            DrawText(TextFormat("%.2f%%",porcento_game),camera.target.x-350,camera.target.y-75,20,BLACK);
    
            DrawText("'Q'para dash", camera.target.x-380, camera.target.y+220,20,BLACK);
            DrawText("aperte 'ENTER' para começar! 'CAPS-LOCK' para pausar", camera.target.x-380, camera.target.y+250 ,20,BLACK);
    
    
            if(IsKeyPressed(KEY_ENTER)){
                gamestate.loby = true;
            }
    
            EndMode2D();
            EndDrawing();
            
            
        }else if(gamestate.loby){
    
            BeginDrawing();
            ClearBackground(SKYBLUE);
            
    
            BeginMode2D(camera);
    
            DrawCircle(camera.target.x-400,camera.target.y-300,80,YELLOW);
    
            for(const auto &ground : grounds){
                DrawRectangleRec(ground, GREEN);
            }
            DrawRectangleRec(player.rect, BLUE);
            DrawRectangleRec(enemy.rect, RED);
            DrawRectangleRec(enemy2.rect, RED);
    
    
            EndMode2D();
    
            DrawText(TextFormat("mortes: %d", gamestate.deaths), 10, 10,20, RED);
            DrawText(TextFormat("dashes: %d",gamestate.dashCount), 10,40,20,RED);
            DrawText(TextFormat("%.2f%%",porcento_game), 10, 70, 20, RED);
    
    
            if (player.rect.x >= 1510 && player.rect.x <= 1990 && player.rect.y == 440) {
                DrawText("fase 2!",650,240,45,RED);//
            }
            if (player.rect.x >= 4990 && player.rect.x <= 5250 && player.rect.y == 440) {
                DrawText("fase 3!",650,240,45,RED);//
            }
            if(player.rect.x >= 7800 && player.rect.x <= 8100 && player.rect.y == 440){
                DrawText("fase 4!",650,240,45,RED);
            }
            if(player.rect.x >= 10900 && player.rect.x <= 10900+300 && player.rect.y == 440){
                DrawText("fase 5!",650,240,45,RED);
            }
            EndDrawing();
        }
        //render!!!@!!!!!!! end
      
    }
    CloseWindow();
    return 0;
}
    
// g++ main.cpp -o game-jump.exe -static -lraylib -lopengl32 -lgdi32 -lwinmm