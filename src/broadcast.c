#include <stdlib.h>
#include <string.h>

#include "system/stacktrace.h"
#include "system/nth_alloc.h"
#include "broadcast.h"
#include "ebisp/interpreter.h"
#include "ebisp/expr.h"
#include "ebisp/scope.h"
#include "game.h"

struct EvalResult
unknown_target(Gc *gc, const char *source, const char *target)
{
    return eval_failure(
        list(gc, "qqq", "unknown-target", source, target));
}

static struct EvalResult
send(void *param, Gc *gc, struct Scope *scope, struct Expr args)
{
    trace_assert(param);
    trace_assert(gc);
    trace_assert(scope);

    Broadcast *broadcast = (Broadcast*) param;

    struct Expr path = void_expr();
    struct EvalResult result = match_list(gc, "e", args, &path);
    if (result.is_error) {
        return result;
    }

    return broadcast_send(broadcast, gc, scope, path);
}

struct Broadcast
{
    Game *game;
};

Broadcast *create_broadcast(Game *game)
{
    trace_assert(game);

    Broadcast *broadcast = nth_calloc(1, sizeof(Broadcast));
    if (broadcast == NULL) {
        return NULL;
    }

    broadcast->game = game;

    return broadcast;
}

void destroy_broadcast(Broadcast *broadcast)
{
    trace_assert(broadcast);
    free(broadcast);
}

struct EvalResult
broadcast_send(Broadcast *broadcast,
               Gc *gc,
               struct Scope *scope,
               struct Expr path)
{
    trace_assert(broadcast);
    trace_assert(gc);
    trace_assert(scope);

    const char *target = NULL;
    struct Expr rest = void_expr();
    struct EvalResult res = match_list(gc, "q*", path, &target, &rest);
    if (res.is_error) {
        return res;
    }

    if (strcmp(target, "game") == 0) {
        return game_send(broadcast->game, gc, scope, rest);
    }

    return unknown_target(gc, "game", target);
}

void broadcast_load_library(Broadcast *broadcast,
                            Gc *gc,
                            struct Scope *scope)
{
    trace_assert(gc);
    trace_assert(scope);
    trace_assert(broadcast);

    set_scope_value(gc, scope, SYMBOL(gc, "send"), NATIVE(gc, send, broadcast));
}
