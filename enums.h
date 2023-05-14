typedef enum {
    BIG_BALL,
    TINY_BALL,
    LONG_PADDLE,
    SHORT_PADDLE,
    MORE_SPEED,
    WALL
} POWER_TYPE;

typedef enum {
    PADDLE_MOVE_NONE,
    PADDLE_MOVE_UP,
    PADDLE_MOVE_DOWN
} paddle_movement;

typedef enum {
    STANDBY, //0
    ACTIVE, //1
    APPLIED, //2
    DEAD //3
} POWER_STATES;