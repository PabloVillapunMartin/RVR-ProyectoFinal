#pragma once

#include <vector>
#include "System.h"

class PlayerSystem: public System {
public:
    /// <summary>
	/// Constructora por defecto
	/// </summary>
	PlayerSystem();

    /// <summary>
	/// Inicializa el vector de jugadores
	/// </summary>
    virtual void init();

    /// <summary>
	/// Procesa mensajes locales
	/// </summary>
	virtual void recieve(const msg::Message& msg);

    /// <summary>
	/// Modifica la posicion de un plauer en funcion de los parámetros del mensaje
	/// </summary>
    void updatePlayerClient(msg::MoveMessage const& ms);

    std::vector<Entity*> players;
};

