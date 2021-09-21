/*
//  main.c
//  SDL events: Moving zozor with arrows keys and with the mouse
//
//  Created by Sebastian Morera on 2021-08-30.
 
Concepts important du SDL:
1) Gestion de fênetre
2) Gestion de rendu (permettre de dessiner dans notre fênetre
3) Créer/Charger des textures (Texture (SDL2) est l'équivalent de surface (SDL1.2)
*/

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

void SDL_ExitWithDestruction(SDL_Renderer *renderer, SDL_Window *window, const char *message);
void SDL_ExitWithError(const char *message);
void PAUSE(void);


int main()
{
    char *title = "Tutorial";
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    SDL_Surface *zozor = NULL;
    SDL_Texture *textureZozor = NULL;
    SDL_Rect positionZozor;
    
    positionZozor.x = (WINDOW_WIDTH - positionZozor.w) / 2;
    positionZozor.y = (WINDOW_HEIGHT - positionZozor.h) / 2;

    // Initialisation SDL + affichage fênetre avec une image + une icone de fênetre
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Lancement SDL, chargement de tous les systemes
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        SDL_ExitWithError("Initialisation de la SDL");
    
    // Création fenêtre + rendu
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) != 0)
        SDL_ExitWithError("Impossible de creer la fenetre et le rendu");

    SDL_SetWindowTitle(window, title);  // Titre de la fênetre
    SDL_SetWindowResizable(window, SDL_TRUE);   // Active ou désactive le resize de la fênetre
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Select the color for drawing. It is set to red here.
    SDL_RenderClear(renderer); // Clear the entire screen to our selected color.
    
    // Création de l'icone pour la fênetre
    SDL_Surface *icone = IMG_Load("sdl_icone.bmp");
    SDL_SetWindowIcon(window, icone);
    SDL_FreeSurface(icone);
   
    /* Chargement et blittage de Zozor sur la scène */
    zozor = IMG_Load("zozor.bmp");
    if (zozor == NULL)
        SDL_ExitWithDestruction(renderer, window, "Impossible de charger image");
    
    SDL_SetColorKey(zozor, SDL_TRUE, SDL_MapRGB(zozor->format, 0, 0, 255));
    SDL_SetSurfaceAlphaMod(zozor, 128); //Transperence de notre image
    textureZozor = SDL_CreateTextureFromSurface(renderer, zozor);
    SDL_FreeSurface(zozor);
    
    if (textureZozor == NULL)
        SDL_ExitWithDestruction(renderer, window, "Impossible de creer la texture");
    
    if(SDL_QueryTexture(textureZozor, NULL, NULL, &positionZozor.w, &positionZozor.h) != 0)
        SDL_ExitWithDestruction(renderer, window, "Impossible de charger la texture");
    
    if(SDL_RenderCopy(renderer, textureZozor, NULL, &positionZozor) != 0)
        SDL_ExitWithDestruction(renderer, window, "Impossible d afficher la texture");
    
    SDL_RenderPresent(renderer); // Afficher l'image de fond
    
    // Events (ce que le programme doit faire)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SDL_ShowCursor(SDL_DISABLE);
    SDL_Event event;
    int continuer = 1;
    
    while(continuer)
        {
            SDL_WaitEvent(&event);
            switch (event.type)
            {
                case SDL_QUIT:
                    continuer = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_UP: // Flèche haut
                            positionZozor.y--;
                            break;
                        case SDLK_DOWN: // Flèche bas
                            positionZozor.y++;
                            break;
                        case SDLK_RIGHT: // Flèche droite
                            positionZozor.x++;
                            break;
                        case SDLK_LEFT: // Flèche gauche
                            positionZozor.x--;
                            break;
                        case SDLK_ESCAPE: /* Appui sur la touche Echap, on arrête le programme */
                            continuer = 0;
                            break;
                        case SDLK_b: /* Appui sur la touche Echap, on arrête le programme */
                            printf("vous avez clique sur b \n");
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                     if (event.button.button == SDL_BUTTON_LEFT) 
                     {
                         positionZozor.x = event.button.x;
                         positionZozor.y = event.button.y;
                     }
                     break;
                case SDL_MOUSEBUTTONUP:
                     if (event.button.button == SDL_BUTTON_RIGHT) /* On arrête le programme si on a fait un clic droit */
                         continuer = 0;
                     break;
                case SDL_MOUSEMOTION:
                    positionZozor.x = event.motion.x;
                    positionZozor.y = event.motion.y;
                    break;
                default:
                    break;
            }
            
            /* On efface l'écran */
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer); // Clear the entire screen to our selected color.
    
            /* On place Zozor à sa nouvelle position */
            if(SDL_RenderCopy(renderer, textureZozor, NULL, &positionZozor) != 0)
                SDL_ExitWithDestruction(renderer, window, "Impossible d afficher la texture");
    
            /* On met à jour l'affichage */
            SDL_RenderPresent(renderer); // Afficher l'image de fond
        }

    // Destruction de tous les élements crées
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

void SDL_ExitWithDestruction(SDL_Renderer *renderer, SDL_Window *window, const char *message)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_ExitWithError(message);
}

void SDL_ExitWithError(const char *message)
{
    SDL_Log("Erreur: %s > %s \n", message ,SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void PAUSE()
{
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}
