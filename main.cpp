#include "raylib.h"
#include <iostream>
#include <vector>
#include <limits>   

//fazer uma maneira de apagar as estruturas passadas!!!!, e criar novas se passar de fase

int currentphase = 1;

void updatephase1();
void updatephase2();

using namespace std;
//ver depois isso como funciona!!!!
Vector2 Vector2Lerp(Vector2 start, Vector2 end, float alpha) {
    return (Vector2){
        start.x + alpha * (end.x - start.x),
        start.y + alpha * (end.y - start.y)
        };
}

struct player{
    Rectangle rect;
    Vector2 velocity;
    bool isOnground;
};
struct enemy{
    Rectangle rect;
};

vector<Rectangle> grounds = {
    //  posX, posY, width, height
        {}//cada linha é um chão, jeito mais facil que eu vi para ver isso!
    };

int deaths = 0;
int amount_desh = 3;

char key_direction = 'i';
const int screenwidth = 1600;
const int screenheight = 900;


//width, height = largura, altura
int main(){
    if(currentphase == 1){
        updatephase1();
    }
    else if(currentphase == 2){
        updatephase2();
    }

}
    //array para adicionar todos os chão ao mesmo tempo!


void updatephase1(){ //fase 1

   

    grounds.push_back({0,880,850,15});
    grounds.push_back({1090, 885, 350, 15});
    grounds.push_back({1740,885, 300, 15}); // gool
    InitWindow(screenwidth, screenheight, "game-2d-teste");

    enemy enemy = {1100, 825, 50,60};

    player player = {{100, screenheight - 60, 50,60},{0,0}, true};
    const float gravidade = 550.0f;
    const float jumpforce = -300.0f;
    const float movespeed = 6000.0f;
    const float horizontaldrag = 400.0f;//força do efeito de parar gradativamente!
    Vector2 playerposition = {player.rect.x, player.rect.y};



    // config da camera 2d
    Camera2D camera = { 0 };
    camera.target = playerposition;
    camera.offset = (Vector2){screenwidth/2.0f, 840};
    camera.rotation = 0.0f;          // Sem rotação
    camera.zoom = 1.0f;

    bool enemy_cheek = false;

    SetTargetFPS(60);
    while (!WindowShouldClose()){
        //main loop
    float dt = GetFrameTime();// tudo que for usar a velocidade e etc usar o *dt 



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
    //movimentação do inimigo
    if (!enemy_cheek) { 
        enemy.rect.x += 5; 
        if (enemy.rect.x >= 1340) { // Marca como verdadeiro quando ultrapassa ou atinge o limite
            enemy_cheek = true; 
        }
    }
    else if (enemy_cheek) {
        enemy.rect.x -= 5;
        if (enemy.rect.x <= 1100) { // Marca como falso quando ultrapassa ou atinge o limite
            enemy_cheek = false;
        }
    } 


    else if(player.isOnground){
        // se o player estiver no chão
        cout << "player ground!" << "\n";
    }

    if(IsKeyDown(KEY_D)){
        //mover para direita
        key_direction = 'D';
        player.velocity.x += movespeed*dt;
        if(player.velocity.x >= 300) player.velocity.x = 300;
    }
    if(IsKeyDown(KEY_A)){
        //mover para a esquerda
        key_direction = 'A';
        player.velocity.x -= movespeed*dt;
        if(player.velocity.x <= -300) player.velocity.x = -300;
    }
    if(IsKeyDown(KEY_SPACE) && player.isOnground){
        //pulo
        cout << "space on" << "\n";
        player.velocity.y = jumpforce;
        player.isOnground = false;
    }
    cout << enemy.rect.x << "\n";

    //desh
    if(IsKeyPressed(KEY_Q)){
        if(amount_desh > 0){
        amount_desh -= 1;
        float desh = 250.0f;
        switch (key_direction){
            case 'A':
                if(IsKeyDown(KEY_W)){
                    player.velocity.y = jumpforce;
                    player.isOnground = false;
                }
                    player.rect.x -= desh;
                
            break;
            case 'D':
                if(IsKeyDown(KEY_W)){
                    player.velocity.y = jumpforce;
                    player.isOnground = false;

                }
                player.rect.x += desh;
            break;
            case 'i':
                cout << "inital" << "\n";
            break;
            
        }
    }
    }

    for(const auto& ground : grounds){
        if(CheckCollisionRecs(player.rect, ground)){
            player.rect.y = ground.y - player.rect.height;
            player.velocity.y = 0;
            player.isOnground = true;
        }
    }
    if(player.rect.x >= 1740){
        currentphase = 2;
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }


    player.rect.y += player.velocity.y * dt; // pasando as info do pulo para o rect.y para ter a vizualização e animação!
    player.rect.x += player.velocity.x*dt;

    if(player.rect.y > screenheight + 75){
        //morrer por queda
        BeginDrawing();
        ClearBackground(RED);
        EndDrawing();
        deaths += 1;
        amount_desh = 3;
        player.rect.x = 100;
        player.rect.y =  screenheight - 60;
    }
    //morrer pelo inimigo
    if(CheckCollisionRecs(player.rect, enemy.rect)){
        BeginDrawing();
        ClearBackground(RED);
        EndDrawing();
        deaths += 1;
        amount_desh = 3;
        player.rect.x = 100;
        player.rect.y =  screenheight - 60;
    }


    float margin = 100.0f;
    Vector2 cameraTarget = camera.target;

    playerposition = {player.rect.x, 840};
    if (player.rect.x < camera.target.x - margin) cameraTarget.x = player.rect.x + margin;
    if (player.rect.x > camera.target.x + margin) cameraTarget.x = player.rect.x - margin;

    camera.target = Vector2Lerp(camera.target, cameraTarget, 0.1f);


    BeginDrawing();
    ClearBackground(SKYBLUE);

    BeginMode2D(camera);
    //(TextFormat("PosX: %03.0f", position)
    DrawText(TextFormat("desh: %d", amount_desh),camera.target.x - screenwidth/2+10, camera.target.y - 90,40, RED);
    DrawText(TextFormat("mortes: %d", deaths), camera.target.x - screenwidth/2+10, camera.target.y , 40, RED);

    DrawCircle(150,50, 80, YELLOW);
    for(const auto& ground : grounds){
        DrawRectangleRec(ground, GREEN);
    }
    DrawRectangleRec(player.rect, BLUE);
    DrawRectangleRec(enemy.rect, RED);
    DrawText("aparte Q para dar um desh",0,400,25,BLACK);
    DrawText("você tem 3 desh por fase!",0, 450,25,BLACK);

    EndDrawing();
    }

    CloseWindow();
}

void updatephase2(){
    grounds.push_back({0,580,850,15});
}
//player.isOnGround = true/false, se esta no chão no momento,