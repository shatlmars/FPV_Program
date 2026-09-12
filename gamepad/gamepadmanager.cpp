#include "gamepadmanager.h"

#include <SDL2/SDL.h>

const int kDeadZone = 4000;

GamepadManager::GamepadManager(QObject* parent): QObject(parent), timer(new QTimer(this))
{
    SDL_SetHint(SDL_HINT_JOYSTICK_THREAD, "1");
    if(SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0){
        qWarning() << "SDL_INIT Failed" << SDL_GetError();
        return;
    }
    openController();
    timer->setInterval(16);
    connect(timer, &QTimer::timeout, this, &GamepadManager::poll);
    timer->start();
}

GamepadManager::~GamepadManager(){
    timer->stop();
    closeController();
    SDL_Quit();
}

void GamepadManager::openController(){
    closeController();

    const int n = SDL_NumJoysticks();
    for(int i = 0; i < n; ++i){
        if(!SDL_IsGameController(i)){
            continue;
        }


        controller = SDL_GameControllerOpen(i);
        if(!controller)
            continue;
        _isConnected = true;
        _name = QString::fromUtf8(SDL_GameControllerName(controller));
        emit connectedChanged();
        emit nameChanged();
        return;
    }

    if(_isConnected){
        _isConnected = false;
        _name.clear();
        emit connectedChanged();
        emit nameChanged();
    }
}

void GamepadManager::closeController(){
    if(!controller)
        return;
    SDL_GameControllerClose(controller);
    controller = nullptr;
}


double GamepadManager::normalizeAxis(int raw){
    if( raw > -kDeadZone && raw < kDeadZone){
        return 0.0;
    }

    double v = raw/32768.0;
    if (v > 1.0) return 1.0;
    if (v < -1.0) return -1.0;
    return v;
}

void GamepadManager::poll(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_CONTROLLERDEVICEADDED || event.type == SDL_CONTROLLERDEVICEREMOVED){
            openController();
        }
    }

    if(!controller){
        return;
    }



    const double lx = normalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX));
    const double ly = normalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY));
    const double rx = normalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX));
    const double ry = normalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY));

    if (lx != _leftX || ly != _leftY || rx != _rightX || ry != _rightY) {
        _leftX = lx;
        _leftY = ly;
        _rightX = rx;
        _rightY = ry;
        emit axesChanged();
    }
}















