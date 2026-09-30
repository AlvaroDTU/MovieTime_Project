#include "pch.h"
#include "Sistema.h"
#include "RegistroUsuario.h"   // <- nuevo

int main() {
    RegistroUsuario::menuPrevio();   // <- nuevo: 1. Registrar / 2. Ir al login
    Sistema app;
    app.iniciar();
    return 0;
}