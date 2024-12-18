#include "spritesheet.h"

struct spritesheet *spritesheet_create(const char *filename,
                                       int num_rows,
                                       int num_columns,
                                       int num_sprites,
                                       SDL_Renderer *renderer)
{
    struct spritesheet *spritesheet;
    spritesheet = (struct spritesheet *)malloc(sizeof(struct spritesheet));
    spritesheet->num_rows = num_rows;
    spritesheet->num_columns = num_columns;
    spritesheet->num_sprites = num_sprites;
    spritesheet->scale = 1.0;
    spritesheet->texture = NULL;
    spritesheet->renderer = renderer;
    SDL_Surface *loaded_surface = IMG_Load(filename);
    if (loaded_surface == NULL)
    {
        printf("Unable to load image %s: %s\n",
               filename, IMG_GetError());
    }
    else
    {
        spritesheet->texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        if (spritesheet->texture == NULL)
        {
            printf("Unable to create texture from %s: %s\n",
                   filename, SDL_GetError());
        }
        spritesheet->sprite_width = loaded_surface->w / num_columns;
        spritesheet->sprite_height = loaded_surface->h / num_rows;
        SDL_FreeSurface(loaded_surface);
    }
    return spritesheet;
}

void spritesheet_delete(struct spritesheet *spritesheet)
{
    SDL_DestroyTexture(spritesheet->texture);
    free(spritesheet);
}

void spritesheet_render(struct spritesheet *spritesheet,
                        int x, int y, int alpha, int sprite)
{
    int srcx = spritesheet->sprite_width * (sprite % spritesheet->num_columns);
    int srcy = spritesheet->sprite_height * (sprite / spritesheet->num_columns);
    SDL_Rect srcrect = {srcx, srcy, spritesheet->sprite_width, spritesheet->sprite_height};
    SDL_Rect dstrect = {x, y, (int)(spritesheet->scale * spritesheet->sprite_width),
                        (int)(spritesheet->scale * spritesheet->sprite_height)};
    SDL_SetTextureAlphaMod(spritesheet->texture, alpha);
    SDL_RenderCopy(spritesheet->renderer, spritesheet->texture, &srcrect, &dstrect);
}
