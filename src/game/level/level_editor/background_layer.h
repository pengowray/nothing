#ifndef BACKGROUND_LAYER_H_
#define BACKGROUND_LAYER_H_

#include "color_picker.h"

typedef struct {
    ColorPicker color_picker;
    Color prev_color;
} BackgroundLayer;

BackgroundLayer create_background_layer(Color color);
int background_layer_read_from_line_stream(BackgroundLayer *layer,
                                           LineStream *line_stream);

static inline
LayerPtr background_layer_as_layer(BackgroundLayer *layer)
{
    return (LayerPtr) {
        .ptr = layer,
        .type = LAYER_BACKGROUND
    };
}

int background_layer_render(BackgroundLayer *layer,
                            Camera *camera,
                            int active);
int background_layer_event(BackgroundLayer *layer,
                           const SDL_Event *event,
                           const Camera *camera,
                           UndoHistory *undo_history);
int background_layer_dump_stream(BackgroundLayer *layer,
                                 FILE *stream);

#endif  // BACKGROUND_LAYER_H_
